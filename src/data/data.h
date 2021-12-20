#ifndef DATA_H
#define DATA_H

#include <QVector>
#include <QString>
#include <QModelIndex>
#include "item.h"

class Data
{
public:
    Data(const Data&)=delete;
    Data& operator=(const Data&)=delete;
    static Data& getData(){
        static Data instance;
        return instance;
    }

public:
    void push(Item &str);
    inline void push(const QString &str) {
        Item i(str);
        push(i);
    }
    void removeAt(int index);
    void clear();
    void remove(Item &str);
    void insert(int index, const Item &str);

    void display();
    int size() { return _data.size(); }
    Item& getAt(int index);
    Item& getAt(const QModelIndex &index);
    bool update(int old_index, const QVariant &new_str);
    inline bool update(const QModelIndex &index, const QVariant &new_str) {
        return update(index.row(), new_str);
    }
    int indexOf(const Item &str);
    QList<Item>::iterator begin() { return _data.begin(); }
    QList<Item>::iterator end() { return _data.end(); }
    void erase(QList<Item>::iterator start, QList<Item>::iterator end);

    inline bool isEmpty() { return _data.isEmpty(); }

private:
    static QVector<Item> _data;
    Data() { };
};

#endif // DATA_H
