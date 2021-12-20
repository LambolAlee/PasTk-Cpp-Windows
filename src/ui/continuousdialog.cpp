#include "continuousdialog.h"
#include "ui_continuousdialog.h"
#include <windows.h>

ContinuousDialog::ContinuousDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContinuousDialog)
{
    ui->setupUi(this);
    HWND m_hWnd = (HWND)winId();
    LONG exs = GetWindowLong(m_hWnd, GWL_EXSTYLE);
    SetWindowLong(m_hWnd, GWL_EXSTYLE, exs | WS_EX_NOACTIVATE | WS_EX_COMPOSITED);
    setWindowOpacity(0.8);
    connect(ui->yesButton, &QPushButton::clicked, this, &ContinuousDialog::yesClicked);
    connect(ui->ignoreButton, &QPushButton::clicked, this, &ContinuousDialog::ignoreClicked);
    ui->textEdit->setText((*iter).toString());
}

ContinuousDialog::~ContinuousDialog()
{
    delete ui;
}

void ContinuousDialog::yesClicked()
{
    paster.pasteStr(iter, 0);
    next();
}

void ContinuousDialog::next()
{
    ++iter;
    if (checkIfOver())
        return;
    ui->textEdit->setText((*iter).toString());
}

void ContinuousDialog::ignoreClicked()
{
    next();
}

bool ContinuousDialog::checkIfOver()
{
    if (iter == endIter) {
        ui->textEdit->setText("OVER!");
        disconnect(ui->yesButton, &QPushButton::clicked, this, 0);
        connect(ui->yesButton, &QPushButton::clicked, this, &ContinuousDialog::reject);
        disconnect(ui->ignoreButton, &QPushButton::clicked, this, 0);
        connect(ui->ignoreButton, &QPushButton::clicked, this, &ContinuousDialog::reject);
        return true;
    }
    return false;
}
