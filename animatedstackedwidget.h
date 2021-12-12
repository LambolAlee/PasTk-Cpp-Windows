#ifndef ANIMATEDSTACKEDWIDGET_H
#define ANIMATEDSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>

class AnimatedStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit AnimatedStackedWidget(QWidget *parent = NULL);
    ~AnimatedStackedWidget();

    enum Direction {
        GOON,
        GOBACK
    };

    void paintEvent(QPaintEvent *);
    inline void setDuration(int duration) { animation->setDuration(duration); };
    inline int getDuration() const { return animation->duration(); }

    void switchPage(Direction direction);

public slots:
    void valueChanged_slot(QVariant);
    void animationFinished();

private:
    void paintPrevious(QPainter &, int);
    void paintNext(QPainter &, int);
    void refreshPage(int nextIndex);

private:
    QPropertyAnimation *animation;
    bool isAnimating;
    bool firstRun;
    float currentValue;
    int widgetCount;
    int nextIndex;
    int current;
    Direction switchDirection;
};

#endif // ANIMATEDSTACKEDWIDGET_H
