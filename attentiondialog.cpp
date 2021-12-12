#include "attentiondialog.h"
#include "ui_attentiondialog.h"
#include <QSettings>

AttentionDialog::AttentionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttentionDialog)
{
    ui->setupUi(this);
}

AttentionDialog::~AttentionDialog()
{
    delete ui;
}

void AttentionDialog::on_buttonBox_accepted()
{
    if (ui->remindOption->isChecked()) {
        QSettings settings("./conf.ini", QSettings::IniFormat);
        settings.setValue("attention_remind", false);
        settings.sync();
    }
}
