#include "attentiondialog.h"
#include "ui_attentiondialog.h"
#include <QSettings>

AttentionDialog::AttentionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttentionDialog)
{
    ui->setupUi(this);
    setting_item = "attention_remind";
    //setAttribute(Qt::WA_QuitOnClose, true);
}

AttentionDialog::~AttentionDialog()
{
    delete ui;
}

void AttentionDialog::on_buttonBox_accepted()
{
    if (ui->remindOption->isChecked()) {
        QSettings settings("./conf.ini", QSettings::IniFormat);
        if (setting_item == "attention_remind")
            settings.setValue(setting_item, false);
        else if (setting_item == "hide_to_system_tray")
            settings.setValue(setting_item, true);
        settings.sync();
    }
}

void AttentionDialog::setTextWithSetting(const QString &text, const QString &name)
{
    ui->label->setText(text);
    setting_item = name;
}
