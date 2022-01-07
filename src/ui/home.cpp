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
    setWindowFlag(Qt::Widget);

    setFixedSize(this->width(), 128);
    settings = new QSettings("./conf.ini", QSettings::IniFormat);

    initStatusBar();
    createToolbar();
    model = new DataModel(this);
    ui->stackedWidget->setDuration(200);
    ui->startButton->setStyleSheet("#startButton{border-radius:8px; border:none; background:#d2e4da;}"// #d9ded9
                                   "#startButton:hover{font-size:14px;}"
                                   "#startButton:pressed{background:#d7d7cb; font-size:13px;}");
    ui->stackedWidget->setStyleSheet("background:white;");
    ui->menuBar->setStyleSheet("QMenuBar{background:#d2e4da;}");
    ui->statusBar->setStyleSheet("background:white;");

    connect(ui->startButton, &QPushButton::clicked, this, &Home::start);
    connect(model, &DataModel::updateLcdNumber, this, &Home::updateDigital);
    connect(this, &Home::startCopySig, &Home::start);
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
    modeLabel->setStyleSheet("color:#646464;");
    ui->statusBar->addWidget(modeLabel);
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
    ui->menuBar->addAction("Exit", this, &Home::windowClose);
    obs = new ShiftClickObserver(this);
    ui->menuBar->installEventFilter(obs);
    connect(obs, &ShiftClickObserver::shiftClicked, this, &Home::windowCloseForced);
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
        stopOpacAnimation();
        ui->startButton->setText("Over");
        startWatch();
    } else {        // 关闭复制监听
        ui->startButton->setText("Start");
        stopWatch();
        continueToRun();
        clearData();
        startOpacAnimation();
    }
}

void Home::continueToRun()
{
    if (_data.isEmpty()) {
        ui->statusBar->showMessage("No data to paste...", 1500);
        return;
    }

    if (settings->value("attention_remind", true).toBool()) {
        AttentionDialog dialog(this);
        dialog.exec();
    }
    hideWindowHome();
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
    showWindowHome();
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
    stopOpacAnimation();
    if (!detailW) {
        //qDebug() << "Create new instance!";
        detailW = new DetailWindow(model, this);
        connect(detailW, &DetailWindow::windowGoBack, this, &Home::startWatch);
    }
    stopWatch();
    detailW->showWindow();
    startOpacAnimation();
}

void Home::reset()
{
    status = false;
    stopWatch();
    clearData();
    ui->startButton->setText("Start");
}
