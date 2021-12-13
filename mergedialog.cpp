#include "mergedialog.h"
#include "ui_mergedialog.h"

MergeDialog::MergeDialog(QString *sep, QWidget *parent, bool segMode) :
    QDialog(parent),
    ui(new Ui::MergeDialog),
    segMode(segMode),
    _sep(sep)
{
    ui->setupUi(this);
    if (segMode)
        showSegOption();
    else
        hideSegOption();
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &MergeDialog::sendConnector);
}

MergeDialog::~MergeDialog()
{
    delete ui;
}

void MergeDialog::sendConnector()
{
    QString con;
    if (segMode) {
        QRadioButton *b = static_cast<QRadioButton *>(ui->buttonGroup->checkedButton());
        if (b->text() == "newline")
            con = '\n';
        else
            con = '\t';
    } else {
        con = ui->conjLineEdit->text();
    }
    _sep->clear();
    _sep->append(con);
}

void MergeDialog::showSegOption()
{
    ui->conjLineEdit->setDisabled(true);
    ui->newlineRadio->show();
    ui->tableRadio->show();
}

void MergeDialog::hideSegOption()
{
    ui->conjLineEdit->setDisabled(false);
    ui->newlineRadio->hide();
    ui->tableRadio->hide();
}
