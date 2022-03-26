#include <QHotkey>
#include <SingleApplication>
#include <QFontDatabase>

#include "ui/windowhome.h"
#include "widgets/floatinformationlabel.h"


int main(int argc, char *argv[])
{
    SingleApplication a(argc, argv);
    a.setOrganizationName("lambol");
    a.setOrganizationDomain("lambol.pastk");
    a.setApplicationName("PasTk-Cpp");

    QFontDatabase::addApplicationFont(":/font/Pro Display tfb.ttf");

//    QHotkey hotkey(QKeySequence("Ctrl+Alt+Q"), true, &a);
//    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
//            qDebug() << "Hotkey Activated - the application will quit now";});

    WindowHome w;
//    FloatInformationLabel f;
//    f.show();
    w.show();

    return a.exec();
}
