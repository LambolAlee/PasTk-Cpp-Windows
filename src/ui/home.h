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
#include <QPushButton>

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

    enum Direction {
        LEFT,
        RIGHT,
        TOP,
        FREE,
    };

    inline QSettings *getSettings() { return settings; }

signals:
    void windowClose();
    void windowCloseForced();
    void opacStopSig();
    void opacStartSig();
    void hideWindowSig();
    void showWindowSig();
    void startCopySig();

private slots:
    void start();
    void push();
    void updateDigital();
    void setMode(QAction *);
    void switchPage();
    inline void quitDirectly(){ directQuitFlag = true; close(); }

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
    QHotkey *hotkey;
    bool directQuitFlag = false;
    QMenu *trayMenu;
    QPushButton *settingButton;

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
    void continueToRun();
    inline void stopOpacAnimation() { emit opacStopSig(); }
    inline void startOpacAnimation() { emit opacStartSig(); }
    inline void showWindowHome() { emit showWindowSig(); }
    inline void hideWindowHome() { emit hideWindowSig(); }
};
#endif // HOME_H
