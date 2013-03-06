#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QAndroidStyle>
#endif

#include "gui/maingui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("pirateplayer");
    app.setApplicationVersion("0.5.0");
    app.setOrganizationName("pirateplay");
    app.setOrganizationDomain("pirateplay.se");

#ifdef Q_OS_ANDROID
    QAndroidStyle androidStyle;
    app.setPalette(androidStyle.standardPalette());
    app.setStyle(&androidStyle);
#endif

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    MainGui gui;
    gui.show();

    return app.exec();
}
