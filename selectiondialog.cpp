#include "selectiondialog.h"
#include "ui_selectiondialog.h"
#include "itemdelegate.h"
#include <windows.h>

SelectionDialog::SelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectionDialog),
    model(new DataModel(this))
{
    ui->setupUi(this);
    ui->listView->setModel(model);
    ItemDelegate *d = new ItemDelegate(ItemDelegate::UserSizeHintRole::SelectItemSizeHint, ui->listView);
    ui->listView->setItemDelegate(d);
    HWND m_hWnd = (HWND)winId();
    LONG exs = GetWindowLong(m_hWnd, GWL_EXSTYLE);
    SetWindowLong(m_hWnd, GWL_EXSTYLE, exs | WS_EX_NOACTIVATE | WS_EX_COMPOSITED);
    setWindowOpacity(0.8);
    connect(ui->listView, &QListView::clicked, this,
            [=](const QModelIndex &index){ paster.pasteStr(_data.getAt(index), 0); });
}

SelectionDialog::~SelectionDialog()
{
    delete ui;
    delete model;
}
