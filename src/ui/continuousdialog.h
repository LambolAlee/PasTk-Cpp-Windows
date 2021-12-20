#ifndef CONTINUOUSDIALOG_H
#define CONTINUOUSDIALOG_H

#include <QDialog>
#include "data/item.h"
#include "data/data.h"
#include "utils/pasteutil.h"

namespace Ui {
class ContinuousDialog;
}

class ContinuousDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContinuousDialog(QWidget *parent = nullptr);
    ~ContinuousDialog();

private slots:
    void yesClicked();

    void ignoreClicked();

private:
    Ui::ContinuousDialog *ui;
    PasteUtil &paster = PasteUtil::instance();
    QList<Item>::iterator iter = _data.begin();
    QList<Item>::iterator endIter = _data.end();
    Data &_data = Data::getData();

    void next();
    bool checkIfOver();
    void checkQuit();
};

#endif // CONTINUOUSDIALOG_H
