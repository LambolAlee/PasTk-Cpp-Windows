#ifndef SHIFTCLICKOBSERVER_H
#define SHIFTCLICKOBSERVER_H

#include <QObject>
#include <QEvent>
#include <QAction>

class ShiftClickObserver : public QObject
{
    Q_OBJECT
public:
    explicit ShiftClickObserver(QObject *parent = nullptr);

signals:
    void shiftClicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

};

#endif // SHIFTCLICKOBSERVER_H
