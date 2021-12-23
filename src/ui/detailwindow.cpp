#include "detailwindow.h"
#include "ui_detailwindow.h"
#include "data/itemdelegate.h"
#include <QPushButton>
#include <QClipboard>
#include <QScreen>
#include <QAction>
#include <QMenu>

DetailWindow::DetailWindow(DataModel *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DetailWindow),
    model(model)
{
    ui->setupUi(this);
    resetPos();
    setWindowModality(Qt::WindowModal);
    ui->previewList->setModel(model);
    ItemDelegate *d = new ItemDelegate(ItemDelegate::UserSizeHintRole::DetailItemSizeHint, ui->previewList);
    ui->previewList->setItemDelegate(d);

    connect(ui->quitButton, &QPushButton::clicked, this, &DetailWindow::close);
    connect(ui->previewList, &QListView::clicked, this, [=](const QModelIndex &index){ displayData(index); });
    connect(ui->detailText, &QTextEdit::copyAvailable, this, [this](bool yes){ this->textCanCopy = yes; });
    connect(ui->copyButton, &QPushButton::clicked, this, &DetailWindow::copySelected);
    connect(ui->commitButton, &QPushButton::clicked, this, &DetailWindow::commit);
    connect(ui->removeButton, &QPushButton::clicked, ui->previewList->adapter, &DataAdapter::removeData);
    connect(ui->previewList->adapter, &DataAdapter::dataRemovedSignal, this, &DetailWindow::updateText);
    connect(ui->addButton, &QPushButton::clicked, ui->previewList->adapter, &DataAdapter::addData);
    connect(this, &DetailWindow::windowGoFront, [=]{ displayData(); });
}

DetailWindow::~DetailWindow()
{
    delete ui;
}

void DetailWindow::closeEvent(QCloseEvent *event)
{
    model->clearUnusedNewItems();
    emit model->updateLcdNumber();
    emit windowGoBack();
    event->accept();
}

void DetailWindow::resetPos()
{
    QScreen *screen = QApplication::primaryScreen();
    QRect geometry = screen->availableGeometry();
    move((geometry.width()-width())/2, (geometry.height()-height())/2);
}

void DetailWindow::displayData(const QModelIndex &index)
{
    Item s = _data.getAt(index);
    if (s.isNewFish())
        s.clear();
    ui->detailText->setText(s.toString());
}

void DetailWindow::displayData()
{
    QModelIndexList i = ui->previewList->selectionModel()->selectedRows();
    if (i.isEmpty())
        ui->detailText->clear();
}

void DetailWindow::copySelected()
{
    if (textCanCopy) {
        ui->detailText->copy();
        ui->statusBar->showMessage("The selected text has been copied to clipboard successfully.", 1500);
    }
}

bool DetailWindow::commit()
{
    QModelIndexList i = ui->previewList->selectionModel()->selectedRows();
    if (i.isEmpty())
        return false;
    QString text = ui->detailText->toPlainText();
    if (text == _data.getAt(i.at(0)).toString())
        return false;
    QModelIndex index = i.at(0);
    model->setData(index, text);
    return true;
}

void DetailWindow::updateText(const QModelIndexList &l)
{
    if (l.isEmpty())
        ui->detailText->clear();
    else
        displayData(l.at(0));
}
