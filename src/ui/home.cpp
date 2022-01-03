#include "home.h"
#include "ui_home.h"

#include "detailwindow.h"
#include "mergedialog.h"
#include "continuousdialog.h"
#include "selectiondialog.h"
#include "attentiondialog.h"
#include "widgets/animatedstackedwidget.h"

#include <QScreen>
#include <QActionGroup>
#include <QPainter>

Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _sep(new QString)
{
    ui->setupUi(this);
    resetPos();
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setAttribute(Qt::WA_Hover, true);
    setFixedSize(this->width(), this->height());
    setWindowIcon(QIcon(":/logo/PasTk_logo.ico"));
    settings = new QSettings("./conf.ini", QSettings::IniFormat);
    initStatusBar();
    createToolbar();
    initSysTray();
    model = new DataModel(this);
    ui->stackedWidget->setDuration(200);
    ui->startButton->setStyleSheet("#startButton{border-radius:8px; border:none; background:#d9ded9;}"
                                   "#startButton:hover{font-size:14px;}"
                                   "#startButton:pressed{background:#d7d7cb; font-size:13px;}");
    ui->stackedWidget->setStyleSheet("background:white;");
    ui->menuBar->setStyleSheet("background:#d2e4da;");
    ui->statusBar->setStyleSheet("background:white;");
    opacWatcher = new OpacityWatcher(this);
    installEventFilter(opacWatcher);
    connect(ui->startButton, &QPushButton::clicked, this, &Home::start);
    connect(model, &DataModel::updateLcdNumber, this, &Home::updateDigital);
}

Home::~Home()
{
    delete ui;
    delete settings;
    delete _sep;
}

void Home::initStatusBar()
{
    modeLabel = new QLabel(this);
    settingButton = new QPushButton(this);
    settingButton->setObjectName("settingButton");
    settingButton->setFocusPolicy(Qt::NoFocus);
    settingButton->setStyleSheet("#settingButton{background:transparent; border-style:none; padding:3px;}"
                                 "#settingButton:pressed{background:#e1e1e1;}");
    QIcon icon, icon1;
    icon.addFile(QString::fromUtf8(":/icons/setting-noHover.svg"), QSize(), QIcon::Normal, QIcon::Off);
    icon1.addFile(QString::fromUtf8(":/icons/setting.svg"), QSize(), QIcon::Normal, QIcon::Off);
    settingButton->setIcon(icon);
    b_obs = new ButtonStateWatcher(this);
    b_obs->setStateIcon(icon, icon1);
    settingButton->installEventFilter(b_obs);
    modeLabel->setStyleSheet("color:#646464;");
    ui->statusBar->addWidget(modeLabel);
    ui->statusBar->addPermanentWidget(settingButton);
}

void Home::createToolbar()
{
    QMenu *modeMenu = ui->menuBar->addMenu("Mode");
    QActionGroup *group = new QActionGroup(this);
    QAction *m = modeMenu->addAction("Merge Mode");
    m->setCheckable(true);
    m->setObjectName("merge");
    QAction *s = modeMenu->addAction("Segmented Mode");
    s->setCheckable(true);
    s->setObjectName("segmented");
    QAction *c = modeMenu->addAction("Continuous Mode");
    c->setCheckable(true);
    c->setObjectName("continuous");
    QAction *l = modeMenu->addAction("List-selection Mode");
    l->setCheckable(true);
    l->setObjectName("selection");
    group->addAction(m);
    group->addAction(s);
    group->addAction(c);
    group->addAction(l);
    connect(group, &QActionGroup::triggered, this, &Home::setMode);
    QString name = settings->value("mode", "merge").toString();
    QAction *action = modeMenu->findChild<QAction*>(name, Qt::FindDirectChildrenOnly);
    action->trigger();

    ui->menuBar->addAction("About", this, &Home::switchPage);
    ui->menuBar->addAction("Detail", this, &Home::callDetail);
    ui->menuBar->addAction("Reset", this, &Home::reset);
    ui->menuBar->addAction("Exit", this, &Home::close);
    obs = new ShiftClickObserver(this);
    ui->menuBar->installEventFilter(obs);
    connect(obs, &ShiftClickObserver::shiftClicked, this, &Home::quitDirectly);
}

void Home::initSysTray()
{
    QMenu *trayMenu = new QMenu(this);
    QAction *showAction = trayMenu->addAction("Show", this, [=]{ if (!isVisible()) show(); });
    QAction *exitAction = trayMenu->addAction("Exit", this, &Home::quitDirectly);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icons/browse.svg"), QSize(), QIcon::Normal, QIcon::Off);
    showAction->setIcon(icon);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/icons/sign-out.svg"), QSize(), QIcon::Normal, QIcon::Off);
    exitAction->setIcon(icon1);

    tray = new QSystemTrayIcon(QIcon(":/logo/PasTk_logo.ico"), this);
    tray->setToolTip("PasTk-Cpp");
    tray->setContextMenu(trayMenu);
    connect(tray, &QSystemTrayIcon::activated, this, &Home::handleTrayActivated);
    tray->show();
}

void Home::handleTrayActivated(QSystemTrayIcon::ActivationReason reason)
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

void Home::setHotkey(QHotkey *hk)
{
    hotkey = hk;
    connect(hk, &QHotkey::activated, this, &Home::hideAndShow);
}

void Home::closeEvent(QCloseEvent *event)
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

void Home::switchPage()
{
    AnimatedStackedWidget::Direction direction = AnimatedStackedWidget::Direction::GOON;
    if (ui->stackedWidget->currentIndex() == 1)
        direction = AnimatedStackedWidget::Direction::GOBACK;
    ui->stackedWidget->switchPage(direction);
}

void Home::start()
{
    status = !status;
    if (status) {   // 启动复制监听
        ui->startButton->setText("Over");
        startWatch();
    } else {        // 关闭复制监听
        ui->startButton->setText("Start");
        stopWatch();
        continueToRun();
        clearData();
    }
}

void Home::continueToRun()
{
    if (_data.isEmpty()) {
        ui->statusBar->showMessage("No data to paste...", 1500);
        return;
    }

    opacWatcher->stop();
    if (settings->value("attention_remind", true).toBool()) {
        AttentionDialog dialog(this);
        dialog.exec();
    }
    hide();
    switch (_mode) {
        case MERGE: {
            MergeDialog md(_sep, this);
            if (md.exec() == QDialog::Accepted)
                paster.pasteWithSep(*_sep);
            break;
        }
        case SEPERATE: {
            MergeDialog md(_sep, this, true);
            if (md.exec() == QDialog::Accepted)
                paster.pasteWithSep(*_sep);
            break;
        }
        case CONTINUE: {
            ContinuousDialog cd(this);
            cd.exec();
            break;
        }
        case SELECT: {
            SelectionDialog sd;
            sd.exec();
            break;
        }
    }
    show();
    opacWatcher->start();
}

void Home::setMode(QAction *action)
{
    QString t = action->text();
    if (t == "Merge Mode")
        _mode = Mode::MERGE;
    else if (t == "Segmented Mode")
        _mode = Mode::SEPERATE;
    else if (t == "Continuous Mode")
        _mode = Mode::CONTINUE;
    else         //List-selection Mode
        _mode = Mode::SELECT;
    settings->setValue("mode", action->objectName());
    modeLabel->setText(t);
}

void Home::push()
{
    QString s = _cb->text();
    if (s == old || s.isEmpty())
        return;
    else
        old = s;
    _data.push(s);
}

void Home::updateDigital()
{
    int size = _data.size();
    int basic = size % 10;
    int tens = (size - basic) / 10;
    ui->baseLCD->setText(QString::number(basic));
    ui->tenLCD->setText(QString::number(tens));
}

void Home::startWatch()
{
    if (status) {
        old.clear();
        connect(_cb, &QClipboard::dataChanged, this, &Home::push);
        connect(_cb, &QClipboard::dataChanged, this, &Home::updateDigital);
    }
}

void Home::stopWatch()
{
    disconnect(_cb, &QClipboard::dataChanged, this, 0);
}

void Home::callDetail()
{
    opacWatcher->stop();
    if (!detailW) {
        //qDebug() << "Create new instance!";
        detailW = new DetailWindow(model, this);
        connect(detailW, &DetailWindow::windowGoBack, this, &Home::startWatch);
    }
    stopWatch();
    detailW->showWindow();
    opacWatcher->start();
}

void Home::reset()
{
    status = false;
    stopWatch();
    clearData();
    ui->startButton->setText("Start");
}

void Home::resetPos()
{
    QScreen *screen = QApplication::primaryScreen();
    QRect geometry = screen->availableGeometry();
    move((geometry.width()-width())/2, 0);
}

void Home::hideAndShow()
{
    if (isVisible())
        hideHome();
    else {
        if (windowOpacity() != 1.0)
            setWindowOpacity(1.0);
        show();
    }
}

void Home::hideHome()
{
    hide();
    if (Q_UNLIKELY(firstHide)) {
        firstHide = false;
        tray->showMessage("PasTk-Cpp", "I'm in your system tray~");
    }
}
