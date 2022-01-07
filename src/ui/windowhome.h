#ifndef WINDOWHOME_H
#define WINDOWHOME_H

#include "utils/windowx.h"
#include "ui/home.h"
#include "utils/opacitywatcher.h"

#include <QObject>
#include <QCloseEvent>
#include <QMenu>

class WindowHome : public WindowX
{
    Q_OBJECT

public:
    WindowHome(QWidget *parent = nullptr);
    ~WindowHome();

/*protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
*/
private:
    void closeEvent(QCloseEvent *event);

private slots:
    void handleTrayActivated(QSystemTrayIcon::ActivationReason reason);
    inline void quitDirectly(){ directQuitFlag = true; close(); }
    void hideHome();

private:
    Home *home;
    OpacityWatcher *opacWatcher;
    QSettings *settings;
    bool directQuitFlag = false;
    bool firstHide = true;
    QSystemTrayIcon *tray;

    void initSysTray();
    void resetPos();
    void initHotKey();
};

#endif // WINDOWHOME_H
