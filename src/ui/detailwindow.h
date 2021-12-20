#ifndef DETAILWINDOW_H
#define DETAILWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "data/data.h"
#include "data/datamodel.h"

namespace Ui {
class DetailWindow;
}

class DetailWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void windowGoBack();
    void windowGoFront();

public:
    explicit DetailWindow(DataModel *model, QWidget *parent = nullptr);
    ~DetailWindow();

    inline void showWindow() {
        model->refreshModel();
        emit windowGoFront();
        show();
    }

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::DetailWindow *ui;
    Data& _data = Data::getData();
    DataModel *model;
    bool textCanCopy = false;
    QModelIndexList selection;
    QMenu *context;

    void displayData(const QModelIndex &index);
    void displayData();
    void copySelected();
    bool commit();
    void resetPos();
    bool removeData();
    bool addData();
    void initContextMenu();
    void copyItem();
};

#endif // DETAILWINDOW_H
