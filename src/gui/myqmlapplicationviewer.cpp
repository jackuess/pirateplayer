#include "myqmlapplicationviewer.h"

#include <QCloseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsObject>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

MyQmlApplicationViewer::MyQmlApplicationViewer(QWidget *parent) :
    QmlApplicationViewer(parent)
{
    setStyleSheet("background-color: #000; color: #0f0");
    engine()->setNetworkAccessManagerFactory(&namFactory);
    rootContext()->setContextObject(&ppContext);
}

void MyQmlApplicationViewer::addLibraryPath(const QString &lib) {
    engine()->addImportPath(lib);
}

void MyQmlApplicationViewer::closeEvent(QCloseEvent *event) {
    event->ignore();
    QDeclarativeView::keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Back, Qt::NoModifier));
}

void MyQmlApplicationViewer::showExpanded() {
    if (status() == Ready) {
        QmlApplicationViewer::showExpanded();
#ifndef Q_OS_ANDROID
        resize(320, 480);
#endif
    }
    else
        connect(this, SIGNAL(statusChanged(QDeclarativeView::Status)), SLOT(showOnLoaded()));
}

void MyQmlApplicationViewer::showOnLoaded() {
    if (status() == Ready) {
        disconnect(this, SLOT(showOnLoaded()));
        QmlApplicationViewer::showExpanded();
#ifndef Q_OS_ANDROID
        resize(320, 480);
#endif
    }
}

bool MyQmlApplicationViewer::open(const QString &url) {
    setOrientation(ScreenOrientationAuto);
    setSource(QUrl(url));
    showExpanded();
    return true;
}
