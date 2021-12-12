#include "animatedstackedwidget.h"
#include <QPainter>


AnimatedStackedWidget::AnimatedStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
    , isAnimating(false)
    , firstRun(true)
{
    animation = new QPropertyAnimation(this, QByteArray());
    animation->setEasingCurve(QEasingCurve::Linear);
    connect(animation, &QPropertyAnimation::valueChanged, this, &AnimatedStackedWidget::valueChanged_slot);
    connect(animation, &QPropertyAnimation::finished, this, &AnimatedStackedWidget::animationFinished);
}

AnimatedStackedWidget::~AnimatedStackedWidget()
{
    delete animation;
}

void AnimatedStackedWidget::switchPage(Direction direction)
{
    if (isAnimating)
        return;

    isAnimating = true;
    switchDirection = direction;
    current = currentIndex();

    switch (direction) {
        case GOBACK : {
            nextIndex = count() - current - 1;
            animation->setStartValue(0);
            animation->setEndValue(currentWidget()->width());
            refreshPage(nextIndex);
            break;
        }
        case GOON : {
            nextIndex = (current+1) % count();
            animation->setStartValue(currentWidget()->width());
            animation->setEndValue(0);
            refreshPage(nextIndex);
            break;
        }
    }
    currentWidget()->hide();
    animation->start();
}

void AnimatedStackedWidget::refreshPage(int nextIndex)
{
    if (Q_UNLIKELY(firstRun)) {
        int widthNext = frameRect().width();
        int heightNext = frameRect().height();
        widget(nextIndex)->setGeometry(0, 0, widthNext, heightNext);
        firstRun = false;
    }
}

void AnimatedStackedWidget::valueChanged_slot(QVariant value)
{
    currentValue = value.toFloat();
    update();
}

void AnimatedStackedWidget::paintEvent(QPaintEvent *event)
{
    if (isAnimating) {
        QPainter paint(this);
        paintPrevious(paint, current);
        paintNext(paint, nextIndex);
    } else {
        QWidget::paintEvent(event);
    }
}

void AnimatedStackedWidget::paintPrevious(QPainter &paint, int current)
{
    QWidget *w = widget(current);
    QPixmap pixmap(w->size());
    w->render(&pixmap);

    switch (switchDirection) {
        case GOBACK: {
            paint.drawPixmap(currentValue, 0, pixmap);
            break;
        }
        case GOON: {
            paint.drawPixmap((currentValue - w->width()), 0, pixmap);
            break;
        }
    }
}

void AnimatedStackedWidget::paintNext(QPainter &paint, int nextIndex)
{
    QWidget *w = widget(nextIndex);
    QPixmap pixmap(w->size());
    w->render(&pixmap);

    switch (switchDirection) {
        case GOBACK: {
            paint.drawPixmap((currentValue - w->width()), 0, pixmap);
            break;
        }
        case GOON: {
            paint.drawPixmap(currentValue, 0, pixmap);
            break;
        }
    }
}

void AnimatedStackedWidget::animationFinished()
{
    isAnimating = false;
    setCurrentIndex(nextIndex);
}
