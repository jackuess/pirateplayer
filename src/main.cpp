#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLibraryInfo>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    a.setApplicationName("Pirateplayer");
    a.setApplicationVersion("0.3.1");

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QStringList libPaths = QCoreApplication::libraryPaths();
    libPaths << QCoreApplication::applicationDirPath() + "/plugins";
    QCoreApplication::setLibraryPaths(libPaths);

    MainWindow w;
    w.setWindowTitle(a.applicationName() + " v" + a.applicationVersion());
    w.show();

    return a.exec();
}
