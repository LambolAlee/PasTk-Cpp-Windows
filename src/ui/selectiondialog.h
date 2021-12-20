#ifndef SELECTIONDIALOG_H
#define SELECTIONDIALOG_H

#include <QDialog>
#include "data/data.h"
#include "utils/pasteutil.h"
#include "data/datamodel.h"

namespace Ui {
class SelectionDialog;
}

class SelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectionDialog(QWidget *parent = nullptr);
    ~SelectionDialog();

private:
    Ui::SelectionDialog *ui;
    PasteUtil &paster = PasteUtil::instance();
    Data &_data = Data::getData();
    DataModel *model;
};

#endif // SELECTIONDIALOG_H
