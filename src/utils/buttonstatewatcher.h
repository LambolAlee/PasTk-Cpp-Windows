#ifndef BUTTONSTATEWATCHER_H
#define BUTTONSTATEWATCHER_H

#include <QObject>
#include <QIcon>


class ButtonStateWatcher : public QObject
{
    Q_OBJECT
public:
    explicit ButtonStateWatcher(QObject *parent = nullptr);
    void setStateIcon(const QIcon &, const QIcon &);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QIcon icon;
    QIcon icon_hover;

};

#endif // BUTTONSTATEWATCHER_H
