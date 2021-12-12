#include "pasteutil.h"
#include <windows.h>


void PasteUtil::paste(int msToSleep)
{
    if (msToSleep > 0)
        QThread::msleep(msToSleep);
    keybd_event(VK_CONTROL, 0,0,0);
    keybd_event(VK_V, 0,0,0);
    keybd_event(VK_V, 0,2,0);
    keybd_event(VK_CONTROL, 0,2,0);
}

void PasteUtil::pasteWithSep(const QString &sep, int msToSleep)
{
    pasteStr(connectWithSep(sep), msToSleep);
}

QString PasteUtil::connectWithSep(const QString &sep) const
{
    QString s;
    QStringList sl;
    for (const Item &i: _data) {
        sl.append(i.toString());
    }
    s = sl.join(sep);
    return s;
}

void PasteUtil::pasteStr(const QString &s, int msToSleep)
{
    _cb->setText(s);
    paste(msToSleep);
}

void PasteUtil::pasteStr(const Item &item, int msToSleep)
{
    _cb->setText(item.toString());
    paste(msToSleep);
}

void PasteUtil::pasteStr(QList<Item>::iterator &iter, int msToSleep)
{
    _cb->setText((*iter).toString());
    paste(msToSleep);
}
