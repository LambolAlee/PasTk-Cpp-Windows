#include "ui/home.h"
#include <QHotkey>
#include <SingleApplication>

int main(int argc, char *argv[])
{
    SingleApplication a(argc, argv);
    a.setOrganizationName("lambol");
    a.setOrganizationDomain("lambol.pastk");
    a.setApplicationName("PasTk-Cpp");

    Home w;

    QHotkey hotkey(QKeySequence("Ctrl+/"), true, &a);
    qDebug() << "Is segistered:" << hotkey.isRegistered();
    w.setHotkey(&hotkey);

    QObject::connect(&a, &SingleApplication::instanceStarted, [&w]{w.show();});

    w.show();
    return a.exec();
}
