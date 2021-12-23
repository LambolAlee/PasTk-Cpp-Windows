#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QApplication>
#include "data/data.h"
#include "data/datamodel.h"
#include "detailwindow.h"
#include "utils/pasteutil.h"
#include "utils/shiftclickobserver.h"
#include <QSettings>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QHotkey>
#include <QToolButton>

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

    void setHotkey(QHotkey *hk);

private slots:
    void start();
    void push();
    void updateDigital();
    void setMode(QAction *);
    void switchPage();
    void hideAndShow();
    void handleTrayActivated(QSystemTrayIcon::ActivationReason);
    inline void quitDirectly(){ directQuitFlag = true; close(); }

private:
    void closeEvent(QCloseEvent *event) override;

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
    QString *_sep;
    QString old;
    QSystemTrayIcon *tray;
    QHotkey *hotkey;
    bool directQuitFlag = false;
    QMenu *trayMenu;
    bool firstHide = true;
    QToolButton *settingButton;
    ShiftClickObserver *obs;

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
    void initSysTray();
    void resetPos();
    void continueToRun();
    void hideHome();
};
#endif // HOME_H
