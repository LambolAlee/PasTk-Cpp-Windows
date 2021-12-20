#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractListModel>
#include "data.h"

class DataModel : public QAbstractListModel
{
    Q_OBJECT

signals:
    void updateLcdNumber();

public:
    explicit DataModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    void refreshModel();

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void clearUnusedNewItems();

private:
    Data &_data = Data::getData();
    void removeNewFishFlag(Item &i);
};

#endif // DATAMODEL_H
