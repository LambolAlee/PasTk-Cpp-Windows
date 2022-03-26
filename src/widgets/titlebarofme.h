#ifndef TITLEBAROFME_H
#define TITLEBAROFME_H

#include <QWidget>
#include <QIcon>

namespace Ui {
class TitleBarOfMe;
}

class TitleBarOfMe : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBarOfMe(QWidget *parent = nullptr);
    ~TitleBarOfMe();

    void setMaxmizedButtonVisible(bool state);

signals:
    void minimizeSig();

private slots:
    void on_miniButton_clicked();

private:
    Ui::TitleBarOfMe *ui;
    QIcon maxIcon;
    QIcon restoreIcon;
};

#endif // TITLEBAROFME_H
