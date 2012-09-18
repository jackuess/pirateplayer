#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("pirateplayer");
    a.setApplicationVersion("0.4.2");
    a.setOrganizationName("wrutschkow");
    a.setOrganizationDomain("pirateplay.se");

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    MainWindow w;
    w.setWindowTitle("Pirateplayer v" + a.applicationVersion());
    w.show();
    
    return a.exec();
}
