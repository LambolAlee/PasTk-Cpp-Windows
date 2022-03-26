#ifndef FLOATINFORMATIONLABEL_H
#define FLOATINFORMATIONLABEL_H

#include <QWidget>

namespace Ui {
class FloatInformationLabel;
}

class FloatInformationLabel : public QWidget
{
    Q_OBJECT

public:
    explicit FloatInformationLabel(QWidget *parent = nullptr);
    ~FloatInformationLabel();

private:
    Ui::FloatInformationLabel *ui;
};

#endif // FLOATINFORMATIONLABEL_H
