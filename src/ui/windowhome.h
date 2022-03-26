#ifndef WINDOWHOME_H
#define WINDOWHOME_H

#include "utils/windowx.h"
#include "ui/home.h"

#include <QObject>
#include <QCloseEvent>
#include <QMenu>

class WindowHome : public WindowX
{
    Q_OBJECT

public:
    WindowHome(QWidget *parent = nullptr);
    ~WindowHome();

private:
    void closeEvent(QCloseEvent *event);

private slots:
    void handleTrayActivated(QSystemTrayIcon::ActivationReason reason);
    inline void quitDirectly(){ directQuitFlag = true; close(); }
    void hideHome();

private:
    Home *home;
    QSettings *settings;
    bool directQuitFlag = false;
    bool firstHide = true;
    QSystemTrayIcon *tray;

    void initSysTray();
    void resetPos();
    void initHotKey();
    void awakeAndSleep();
};

#endif // WINDOWHOME_H
