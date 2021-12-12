#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QApplication>
#include "data.h"
#include "detailwindow.h"
#include "pasteutil.h"
#include <QSettings>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Home : public QMainWindow
{
    Q_OBJECT

public:
    Home(QWidget *parent = nullptr);
    ~Home();

    enum Mode {
        MERGE,
        SEPERATE,
        CONTINUE,
        SELECT,
    };

private slots:
    void start();
    void push();
    void updateDigital();
    void setMode(QAction *);
    void switchPage();

private:
    Ui::MainWindow *ui;
    Data &_data = Data::getData();
    QClipboard *_cb = QApplication::clipboard();
    DetailWindow *detailW = nullptr;
    bool status = false;
    DataModel *model;
    QLabel *modeLabel;
    Mode _mode;
    PasteUtil &paster = PasteUtil::instance();
    QSettings *settings;

    void startWatch();
    void stopWatch();
    void callDetail();
    void reset();
    void clearData() {
        _data.clear();
        updateDigital();
    }
    void createToolbar();
    void initStatusBar();
    void resetPos();
    void continueToRun();
};
#endif // HOME_H
