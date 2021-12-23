#include "shiftclickobserver.h"
#include <QDebug>
#include <QMouseEvent>
#include <QMenuBar>

ShiftClickObserver::ShiftClickObserver(QObject *parent) : QObject(parent)
{

}

bool ShiftClickObserver::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mevent = static_cast<QMouseEvent *>(event);
        if (mevent->modifiers() == Qt::ShiftModifier && mevent->button() == Qt::LeftButton) {
            QMenuBar *mb = static_cast<QMenuBar *>(obj);
            QAction *a = mb->actionAt(mb->mapFromGlobal(QCursor::pos()));
            if (a->text() == "Exit") {
                emit shiftClicked();
                return true;
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
