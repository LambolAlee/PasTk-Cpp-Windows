#include "titlebarofme.h"
#include "ui_titlebarofme.h"

#include <QStyle>

TitleBarOfMe::TitleBarOfMe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBarOfMe)
{
    ui->setupUi(this);
    setFixedHeight(24);
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("TitleBarOfMe{background:white;}");
}

TitleBarOfMe::~TitleBarOfMe()
{
    delete ui;
}

void TitleBarOfMe::on_miniButton_clicked()
{
    emit minimizeSig();
}

void TitleBarOfMe::setMaxmizedButtonVisible(bool state)
{
    ui->maxButton->setVisible(state);
}
