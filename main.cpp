#include "home.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("lambol");
    a.setOrganizationDomain("lambol.pastk");
    a.setApplicationName("PasTk-Cpp");
    Home w;
    w.show();
    return a.exec();
}
