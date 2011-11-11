#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    a.setApplicationName("Pirateplayer");
    a.setApplicationVersion("0.2");
    MainWindow w;
    w.setWindowTitle(a.applicationName() + " v" + a.applicationVersion());
    w.show();

    return a.exec();
}
