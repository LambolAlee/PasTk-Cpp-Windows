#ifndef MERGEDIALOG_H
#define MERGEDIALOG_H

#include <QDialog>

namespace Ui {
class MergeDialog;
}

class MergeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MergeDialog(QString *sep, QWidget *parent = nullptr, bool segMode = false);
    ~MergeDialog();

private slots:
    void sendConnector();

private:
    Ui::MergeDialog *ui;
    QString *_sep;
    bool segMode;
    void showSegOption();
    void hideSegOption();
};

#endif // MERGEDIALOG_H
