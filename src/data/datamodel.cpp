#include "datamodel.h"
#include <algorithm>
#include <QSize>
#include <QToolTip>
#include <QIODevice>


DataModel::DataModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int DataModel::rowCount(const QModelIndex &parent) const
{
   return _data.size();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Item item = _data.getAt(index);

    if (role == Qt::DisplayRole) {
        return item.toString().simplified();
    } else if (role == Qt::ToolTipRole || role == Qt::UserRole) {
        if (item.isNewFish())
            return QVariant();
        else
            return item.data();
    }
    else
        return QVariant();
}

bool DataModel::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(index, position, position+rows-1);
    for (int p=0; p < rows; ++p) {
        Item newStr("**New item here**");
        newStr.setNewFish();
        _data.insert(position, newStr);
    }
    endInsertRows();
    return true;
}

void DataModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        Item &i = _data.getAt(index);
        if (i.isNewFish()) {
            removeNewFishFlag(i);
        }
        _data.update(index, value);
    } else {
        return false;
    }
    emit dataChanged(index, index);
    return true;
}

bool DataModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(index, position, position+rows-1);
    for (int row=0; row < rows; ++row) {
        Item &i = _data.getAt(position);
        if (i.isNewFish())
            removeNewFishFlag(i);
        _data.removeAt(position);
    }
    endRemoveRows();
    return true;
}

void DataModel::clearUnusedNewItems()
{
    auto iter = std::remove_if(_data.begin(), _data.end(), [](Item &item){ return item.isNewFish(); });
    _data.erase(iter, _data.end());
}

void DataModel::removeNewFishFlag(Item &i)
{
    i.setNewFish(false);
}

Qt::DropActions DataModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    Item &i = _data.getAt(index);
    if (i.isNewFish())
        return defaultFlags;

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList DataModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData *DataModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData;
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for (const QModelIndex &index : indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::UserRole).toString();
            stream << text;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

bool DataModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(action)
    Q_UNUSED(row)
    Q_UNUSED(parent)

    if (!data->hasFormat("application/vnd.text.list"))
        return false;
    if (column > 0)
        return false;
    return true;
}

bool DataModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (!canDropMimeData(data, action, row, column, parent))
        return false;
    if (action == Qt::IgnoreAction)
        return true;

    int beginRow;
    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = parent.row();
    else
        beginRow = rowCount();

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;

    while (!stream.atEnd()) {
        QString text;
        stream >> text;
        newItems << text;
        ++rows;
    }

    insertRows(beginRow, rows, QModelIndex());
    for (const QString &text : qAsConst(newItems)) {
        QModelIndex idx = index(beginRow, 0, QModelIndex());
        setData(idx, text);
        beginRow++;
    }

    return true;
}
