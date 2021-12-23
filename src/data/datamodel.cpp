#include "datamodel.h"
#include <algorithm>
#include <QSize>
#include <QToolTip>


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
            return "";
        else
            return item.toString();
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
