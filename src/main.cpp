#include "ui/home.h"
#include <QHotkey>
//#include <SingleApplication>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("lambol");
    a.setOrganizationDomain("lambol.pastk");
    a.setApplicationName("PasTk-Cpp");

    Home w;

    QHotkey hotkey(QKeySequence("Ctrl+/"), true, &a);
    qDebug() << "Is segistered:" << hotkey.isRegistered();
    w.setHotkey(&hotkey);

    w.show();
    return a.exec();
}
