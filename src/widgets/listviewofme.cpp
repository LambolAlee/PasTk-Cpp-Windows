#include "listviewofme.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QCursor>

ListViewOfMe::ListViewOfMe(QWidget *parent)
    :QListView(parent)
{
    adapter = new DataAdapter(this);
    initContextMenu();
}

ListViewOfMe::~ListViewOfMe()
{
    delete adapter;
}

void ListViewOfMe::initContextMenu()
{
    context = new QMenu(this);
    QAction *deleteAction = context->addAction("delete", adapter, &DataAdapter::removeData);
    context->addAction("add", adapter, &DataAdapter::addData);
    QAction *copyAction = context->addAction("copy", adapter, &DataAdapter::copyData);
    group = new QActionGroup(this);
    group->addAction(deleteAction);
    group->addAction(copyAction);
}

void ListViewOfMe::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event)
    QPoint p = QCursor::pos();
    QModelIndex i = indexAt(mapFromGlobal(p));
    if (i.row() == -1) {
        selectionModel()->reset();
        group->setDisabled(true);
    } else {
        group->setDisabled(false);
    }
    context->exec(p);
    QWidget::contextMenuEvent(event);
}
