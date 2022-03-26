#include "windowx.h"

#include <QColor>
#include <QDebug>
#include <QMouseEvent>


WindowX::WindowX(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint);
    initTitleBar();
}

void WindowX::setContentWidget(QWidget *w)
{
    vbox->addWidget(w);
}

void WindowX::initTitleBar()
{
    title = new TitleBarOfMe(this);
    title->installEventFilter(this);
    vbox = new QVBoxLayout(this);
    vbox->addWidget(title);
    vbox->setSpacing(0);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(vbox);
}

bool WindowX::eventFilter(QObject *obj, QEvent *event)
{
    static bool pressed = false;
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        startPos = e->globalPosition() - pos();
        pressed = true;
    } else if (pressed && event->type() == QEvent::MouseMove) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        QPoint distance = (e->globalPosition() - startPos).toPoint();
        move(distance);
    } else if (event->type() == QEvent::MouseButtonRelease) {
        pressed = false;
    }
    return QWidget::eventFilter(obj, event);
}
