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
    explicit MergeDialog(QWidget *parent = nullptr, bool segMode = false);
    ~MergeDialog();

signals:
    void connector(const QString &);

private slots:
    void sendConnector();

private:
    Ui::MergeDialog *ui;
    bool segMode;
    void showSegOption();
    void hideSegOption();
};

#endif // MERGEDIALOG_H
