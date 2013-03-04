#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

#include "gui/maingui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("pirateplayer");
    app.setApplicationVersion("0.5.0");
    app.setOrganizationName("wrutschkow");
    app.setOrganizationDomain("pirateplay.se");

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    MainGui gui;
    gui.show();

    return app.exec();
}
