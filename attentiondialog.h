#ifndef ATTENTIONDIALOG_H
#define ATTENTIONDIALOG_H

#include <QDialog>

namespace Ui {
class AttentionDialog;
}

class AttentionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AttentionDialog(QWidget *parent = nullptr);
    ~AttentionDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AttentionDialog *ui;
};

#endif // ATTENTIONDIALOG_H
