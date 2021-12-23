#ifndef DATAADAPTER_H
#define DATAADAPTER_H
#include <QObject>
#include <QListView>
#include <QApplication>

class DataAdapter : public QObject
{
    Q_OBJECT
public:
    DataAdapter(QListView *);

    void addData();
    void removeData();
    void copyData();

signals:
    void dataRemovedSignal(const QModelIndexList &);

private:
    QListView *list;
    QClipboard *_cb = QApplication::clipboard();
};

#endif // DATAADAPTER_H
