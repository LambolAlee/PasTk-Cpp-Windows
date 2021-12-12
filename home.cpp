#include "home.h"
#include "./ui_home.h"
#include "data.h"
#include "detailwindow.h"
#include "datamodel.h"

#include "mergedialog.h"
#include "continuousdialog.h"
#include "selectiondialog.h"
#include "attentiondialog.h"

#include <windows.h>
#include <QScreen>
#include <QActionGroup>


Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resetPos();
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowIcon(QIcon(":/resources/logo/PasTk_logo.ico"));
    settings = new QSettings("./conf.ini", QSettings::IniFormat);
    initStatusBar();
    createToolbar();
    model = new DataModel(this);
    ui->stackedWidget->setDuration(400);
    connect(ui->startButton, &QPushButton::clicked, this, &Home::start);
    connect(model, &DataModel::updateLcdNumber, this, &Home::updateDigital);
}

Home::~Home()
{
    delete ui;
    delete settings;
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
    qDebug() << action->text() << name << m->objectName();
    action->trigger();

    ui->menuBar->addAction("About", this, &Home::switchPage);
    ui->menuBar->addAction("Detail", this, &Home::callDetail);
    ui->menuBar->addAction("Reset", this, &Home::reset);
    ui->menuBar->addAction("Exit", QApplication::quit);
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
    if (settings->value("attention_remind", true).toBool()) {
        AttentionDialog dialog(this);
        dialog.exec();
    }
    hide();
    switch (_mode) {
        case MERGE: {
            MergeDialog md(this);
            connect(&md, &MergeDialog::connector, this, [=](const QString &sep) { paster.pasteWithSep(sep); });
            md.exec();
            break;
        }
        case SEPERATE: {
            MergeDialog md(this, true);
            connect(&md, &MergeDialog::connector, this, [=](const QString &sep){ paster.pasteWithSep(sep); });
            md.exec();
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
    _data.push(s);
}

void Home::updateDigital()
{
    int size = _data.size();
    int basic = size % 10;
    int tens = (size - basic) / 10;
    ui->baseLCD->display(basic);
    ui->tenLCD->display(tens);
}

void Home::startWatch()
{
    if (status) {
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
    if (!detailW) {
        qDebug() << "Create new instance!";
        detailW = new DetailWindow(model, this);
        connect(detailW, &DetailWindow::windowGoBack, this, &Home::startWatch);
    }
    stopWatch();
    detailW->showWindow();
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
