#ifndef LISTVIEWOFME_H
#define LISTVIEWOFME_H

#include <QListView>
#include <QActionGroup>
#include "data/data.h"
#include "data/dataadapter.h"

class ListViewOfMe : public QListView
{
    Q_OBJECT
public:
    explicit ListViewOfMe(QWidget *parent = NULL);
    ~ListViewOfMe();

    DataAdapter *adapter;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    Data &_data = Data::getData();
    QMenu *context;
    QActionGroup *group;

    void initContextMenu();
};

#endif // LISTVIEWOFME_H
