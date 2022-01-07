#ifndef WINDOWX_H
#define WINDOWX_H

#include <QObject>
#include <QVBoxLayout>
#include <QEvent>

#include "widgets/titlebarofme.h"

class WindowX : public QWidget
{
    Q_OBJECT
public:
    WindowX(QWidget *parent = nullptr);

    void setContentWidget(QWidget *w);

protected:
    TitleBarOfMe *title;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QPointF startPos;

private:
    QWidget *widget;
    QVBoxLayout *vbox;

    void initTitleBar();
};

#endif // WINDOWX_H
