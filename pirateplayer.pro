#-------------------------------------------------
#
# Project created by QtCreator 2011-09-27T00:05:15
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml
QT       += phonon

TARGET = pirateplayer
TEMPLATE = app


SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/piratenetworkaccessmanager.cpp \
    src/piratenetworkreply.cpp \
    src/downloadwidget.cpp \
    src/piratevideoplayer.cpp

HEADERS  += src/mainwindow.h \
    src/piratenetworkaccessmanager.h \
    src/piratenetworkreply.h \
    src/downloadwidget.h \
    src/piratevideoplayer.h

FORMS    += src/mainwindow.ui

RESOURCES += resources.qrc

DESTDIR = build/target/
OBJECTS_DIR = build/obj/
MOC_DIR = build/moc/
UI_DIR = build/ui/

win32 {
    INCLUDEPATH += /usr/include
    INCLUDEPATH += "C:/Users/chucky/include"
    LIBS += -L"C:/Program Files/rtmpdump-git" -lrtmp -lws2_32
}
unix {
    LIBS += -L/usr/lib/ -lrtmp
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include

    isEmpty(PREFIX):PREFIX = /usr/local
    target.path = $$PREFIX/bin

    INSTALLS += target
}
mac {
    #QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
    #QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.6.sdk
    QT += opengl
    LIBS = -L/usr/local/lib -lrtmp
    INCLUDEPATH = /usr/local/include
    DEPENDPATH = /usr/local/include
}






















