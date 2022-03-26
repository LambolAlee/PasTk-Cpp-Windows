#include "buttonstatewatcher.h"
#include <QToolButton>
#include <qdebug.h>
#include <QEvent>

ButtonStateWatcher::ButtonStateWatcher(QObject *parent) : QObject(parent) {}

bool ButtonStateWatcher::eventFilter(QObject *obj, QEvent *event)
{
    QToolButton *tb = static_cast<QToolButton *>(obj);
    if (!icon_hover.isNull() && event->type() == QEvent::HoverEnter) {
        tb->setIcon(icon_hover);
        tb->setText("settings");
        return true;
    } else if (!icon.isNull() && event->type() == QEvent::HoverLeave) {
        tb->setIcon(icon);
        tb->setText("");
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void ButtonStateWatcher::setStateIcon(const QIcon &icon, const QIcon &icon_hover)
{
    this->icon = icon;
    this->icon_hover = icon_hover;
}
