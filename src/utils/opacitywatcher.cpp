#include "opacitywatcher.h"

#include <QDebug>

OpacityWatcher::OpacityWatcher(QObject *parent) : QObject(parent)
  , timer(new QTimer(this))
{
    int delay = 3000;
    timer->setInterval(delay);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &OpacityWatcher::startAnimating);
    //timer->start();

    initAnimator(parent);
}

void OpacityWatcher::stopAnimating()
{
    if (isAnimating) {
        isAnimating = false;
        animator->stop();
    }
}

bool OpacityWatcher::eventFilter(QObject *obj, QEvent *event)
{
    if (stopFilter) return QObject::eventFilter(obj, event);
    //qDebug() << event->type();
    QWidget *win = static_cast<QWidget*>(obj);
    if (isMouseGoBack(win, event)) {
        stopAnimating();
        if (timer->isActive())
            timer->stop();
        if (win->windowOpacity() != 1)
            win->setWindowOpacity(1);
    } else if (isMouseLeft(win, event)) {
        timer->start();
    }

    return QObject::eventFilter(obj, event);
}

bool OpacityWatcher::isMouseGoBack(QWidget *win, QEvent *event)
{
    bool windowHovered = event->type() == QEvent::HoverEnter;
    return windowHovered;
}

bool OpacityWatcher::isMouseLeft(QWidget *win, QEvent *event)
{
    bool leftWindow = event->type() == QEvent::HoverLeave;
    return leftWindow;
}

void OpacityWatcher::startAnimating()
{
    if (isAnimating)
        return;
    isAnimating = true;
    animator->start();
}

void OpacityWatcher::initAnimator(QObject *parent)
{
    int interval = 500;
    animator = new QPropertyAnimation(parent, "windowOpacity", this);
    animator->setDuration(interval);
    animator->setEasingCurve(QEasingCurve::InCurve);
    animator->setStartValue(1.0);
    animator->setEndValue(0.5);
}
