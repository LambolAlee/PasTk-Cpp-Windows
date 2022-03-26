#include "floatinformationlabel.h"
#include "ui_floatinformationlabel.h"

FloatInformationLabel::FloatInformationLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatInformationLabel)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->frame->setStyleSheet("#frame{border-radius:15px; background:rgba(238,232,213,0.8);}");
    ui->label->setStyleSheet("color:#20294d;");
}

FloatInformationLabel::~FloatInformationLabel()
{
    delete ui;
}
