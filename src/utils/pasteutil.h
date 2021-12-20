#ifndef PASTEUTIL_H
#define PASTEUTIL_H
#include <QString>
#include <QClipboard>
#include <QApplication>
#include "data/data.h"
#include "data/item.h"
#include "singleton.hpp"

#define VK_V 0x56


class PasteUtil
{
    SINGLETON(PasteUtil)
public:
    void paste(int);
    void pasteWithSep(const QString &, int msToSleep = 500);
    void pasteStr(const QString &, int msToSleep = 500);
    void pasteStr(const Item &, int msToSleep = 500);
    void pasteStr(QList<Item>::iterator &iter, int msToSleep = 500);

private:
    PasteUtil() {}
    Data &_data = Data::getData();
    QClipboard *_cb = QApplication::clipboard();

    QString connectWithSep(const QString &) const;
};

#endif // PASTEUTIL_H
