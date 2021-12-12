#include "data.h"
#include <QApplication>

QVector<Item> Data::_data;

void Data::push(Item &str)
{
    _data.push_back(str);
}

void Data::clear()
{
    _data.clear();
}

void Data::remove(Item &str)
{
    _data.remove(_data.indexOf(str));
}

void Data::removeAt(int index)
{
    _data.removeAt(index);
}

void Data::display()
{
    for (const Item &i: _data)
        qDebug() << i.data() << Qt::endl;
}

Item& Data::getAt(int index)
{
    return _data[index];
}

Item& Data::getAt(const QModelIndex &index)
{
    return _data[index.row()];
}

bool Data::update(int old_index, const QVariant &new_str)
{
    _data[old_index].setData(new_str.toString());
    return true;
}

void Data::insert(int index, const Item &str)
{
    _data.insert(index, str);
}

int Data::indexOf(const Item &str)
{
    return _data.indexOf(str);
}

void Data::erase(QList<Item>::iterator start, QList<Item>::iterator end)
{
    _data.erase(start, end);
}
