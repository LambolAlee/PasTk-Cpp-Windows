#ifndef BLURWATCHER_H
#define BLURWATCHER_H

#include <QObject>
#include <QTimer>
#include <QEvent>
#include <QWidget>
#include <QPropertyAnimation>

class OpacityWatcher : public QObject
{
    Q_OBJECT
public:
    explicit OpacityWatcher(QObject *parent = nullptr);

    inline void stop() { setDisabled(true); }
    inline void start() { setDisabled(false); }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QTimer *timer;
    bool isAnimating = false;
    bool stopFilter = false;
    QPropertyAnimation *animator;

    bool isMouseGoBack(QWidget *win, QEvent *event);
    bool isMouseLeft(QWidget *win, QEvent *event);
    void startAnimating();

    void initAnimator(QObject *parent);
    void stopAnimating();

    inline void setDisabled(bool state) { stopFilter = state; }

};

#endif // BLURWATCHER_H
