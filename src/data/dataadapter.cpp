#include "dataadapter.h"
#include <QClipboard>

DataAdapter::DataAdapter(QListView *list)
    : list(list)
{

}

void DataAdapter::addData()
{
    int index = list->model()->rowCount();
    QModelIndexList i = list->selectionModel()->selectedRows();
    if (!i.isEmpty()) {
        index = i.at(0).row() + 1;
    }
    list->model()->insertRow(index);
}

void DataAdapter::removeData()
{
    auto s = list->selectionModel();
    auto i = s->selectedRows();
    if (i.isEmpty()) return;
    auto rend = i.crend();
    for (auto riter = i.rbegin(); riter != rend; ++riter)
        list->model()->removeRow(riter->row());
    emit dataRemovedSignal(s->selectedRows());
}

void DataAdapter::copyData()
{
    auto i = list->selectionModel()->selectedRows();
    if (!i.isEmpty()) {
        QString data = list->model()->data(i.at(0), Qt::UserRole).toString();
        _cb->setText(data);
    }
}
