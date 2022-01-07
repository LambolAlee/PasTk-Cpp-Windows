#include "windowx.h"

#include <windows.h>

#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QDebug>
#include <QMouseEvent>


WindowX::WindowX(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint);
    HWND m_hWnd = (HWND)winId();
    LONG s = GetWindowLong(m_hWnd, GWL_STYLE);
    SetWindowLong(m_hWnd, GWL_STYLE, s | WS_SYSMENU | WS_MINIMIZEBOX);

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
    TitleBarOfMe *title = static_cast<TitleBarOfMe *>(obj);
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        //qDebug() << pos() << " " << e->pos() << " " << e->globalPosition();
        startPos = e->globalPosition() - pos();
    } else if (event->type() == QEvent::MouseMove) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        QPoint distance = (e->globalPosition() - startPos).toPoint();
        move(distance);
    }
    return QWidget::eventFilter(obj, event);
}
