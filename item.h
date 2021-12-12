#ifndef ITEM_H
#define ITEM_H
#include <QVariant>

class Item
{
public:
    Item() {}
    Item(QString value): _value(value) {}

public:
    QVariant data() const { return _value; }
    inline void setData(const QString &value) { _value = value; }
    inline void setNewFish(const bool state = true) { newFish = state; }
    inline bool isNewFish() { return newFish; }
    inline QString toString() const { return _value.toString(); }
    inline void clear() { _value.clear(); }

    inline bool operator==(const Item &item) const { return _value == item.data(); }

private:
    QVariant _value;
    bool newFish = false;
};

#endif // ITEM_H
