#include "windowhome.h"
#include "attentiondialog.h"
#include <SingleApplication>
#include <windows.h>

WindowHome::WindowHome(QWidget *parent)
    : WindowX(parent)
    , home(new Home) // why cannot use Home(this)
    , settings(home->getSettings())
{
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_Hover);
    setContentWidget(home);
    home->setParent(this);

    resetPos(); // some problem ???

    opacWatcher = new OpacityWatcher(this);
    installEventFilter(opacWatcher);

    initSysTray();
    initHotKey();

    connect(home, &Home::windowClose, this, &WindowHome::close);
    connect(home, &Home::windowCloseForced, this, &WindowHome::quitDirectly);
    connect(home, &Home::opacStartSig, this, [=]{ opacWatcher->start(); });
    connect(home, &Home::opacStopSig, this, [=]{ opacWatcher->stop(); });
    connect(title, &TitleBarOfMe::minimizeSig, this, &WindowHome::showMinimized);
}

WindowHome::~WindowHome()
{
    delete home;
}

void WindowHome::initHotKey()
{
    SingleApplication *a = static_cast<SingleApplication*>(SingleApplication::instance());
    connect(a, &SingleApplication::instanceStarted, this, &WindowHome::show);

    QHotkey *hotkeyForShowWindow = new QHotkey(QKeySequence("Ctrl+/"), true, this);
    connect(hotkeyForShowWindow, &QHotkey::activated, this, &WindowHome::hideHome);// wait to adapt

    QHotkey *hotkeyForQuickStart = new QHotkey(QKeySequence("Ctrl+\\"), true, this);
    connect(hotkeyForQuickStart, &QHotkey::activated, home, &Home::startCopySig);
}

void WindowHome::resetPos()
{
    QRect rtWorkArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rtWorkArea, 0);
    move((rtWorkArea.width()-width())/2, 0);
}

void WindowHome::closeEvent(QCloseEvent *event)
{
    if (directQuitFlag)
        event->accept();
    else if (settings->value("hide_to_system_tray", false).toBool()) {
        hide();
        event->ignore();
    } else {
        AttentionDialog dialog;
        dialog.setTextWithSetting("Do you want to hide to the system tray rather than quit directly", "hide_to_system_tray");
        switch (dialog.exec()) {
        case (QDialog::Accepted):
            hideHome();
            event->ignore();
            break;
        case (QDialog::Rejected):
            event->accept();
            break;
        }
    }
}

void WindowHome::hideHome()
{
    hide();
    if (Q_UNLIKELY(firstHide)) {
        firstHide = false;
        tray->showMessage("PasTk-Cpp", "I'm in your system tray~");
    }
}

void WindowHome::initSysTray()
{
    QMenu *trayMenu = new QMenu(this);
    QAction *showAction = trayMenu->addAction("Show", this, [=]{ if (!isVisible()) show(); });
    QAction *exitAction = trayMenu->addAction("Exit", this, &WindowHome::quitDirectly);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/browse.svg"), QSize(), QIcon::Normal, QIcon::Off);
    showAction->setIcon(icon);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/icons/sign-out.svg"), QSize(), QIcon::Normal, QIcon::Off);
    exitAction->setIcon(icon1);

    tray = new QSystemTrayIcon(QIcon(":/logo/PasTk_logo.ico"), this);
    tray->setToolTip("PasTk-Cpp");
    tray->setContextMenu(trayMenu);
    connect(tray, &QSystemTrayIcon::activated, this, &WindowHome::handleTrayActivated);
    tray->show();
}

void WindowHome::handleTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case (QSystemTrayIcon::DoubleClick):
        if (!isVisible())
            show();
        break;
    default:
        break;
    }
}
