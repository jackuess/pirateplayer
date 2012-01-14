#-------------------------------------------------
#
# Project created by QtCreator 2011-09-27T00:05:15
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml
QT       += phonon
QT       += webkit

TARGET = pirateplayer
TEMPLATE = app


SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/piratenetworkaccessmanager.cpp \
    src/piratenetworkreply.cpp \
    src/downloadwidget.cpp \
    src/piratevideoplayer.cpp \
    src/rtmpsession.cpp

HEADERS  += src/mainwindow.h \
    src/piratenetworkaccessmanager.h \
    src/piratenetworkreply.h \
    src/downloadwidget.h \
    src/piratevideoplayer.h \
    src/rtmpsession.h

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
    RC_FILE = pirateplayer.rc
}
unix {
    LIBS += -L/usr/lib/ -lrtmp
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include

    isEmpty(PREFIX):PREFIX = /usr/local
    DATADIR = $$PREFIX/share

    target.path = $$PREFIX/bin

    INSTALLS += target \
        desktop \
        iconsvg \
        icon16 \
        icon32 \
        icon48 \
        icon64 \
        icon128 \
        icon256 \
        icon512

    desktop.path = $$DATADIR/applications
    desktop.files += pirateplayer.desktop

    iconsvg.path = $$DATADIR/icons/hicolor/scalable/apps
    iconsvg.files += data/pirateplayer.svg
    icon16.path = $$DATADIR/icons/hicolor/16x16/apps
    icon16.files += data/16x16/pirateplayer.png
    icon32.path = $$DATADIR/icons/hicolor/32x32/apps
    icon32.files += data/32x32/pirateplayer.png
    icon48.path = $$DATADIR/icons/hicolor/48x48/apps
    icon48.files += data/48x48/pirateplayer.png
    icon64.path = $$DATADIR/icons/hicolor/64x64/apps
    icon64.files += data/64x64/pirateplayer.png
    icon128.path = $$DATADIR/icons/hicolor/128x128/apps
    icon128.files += data/128x128/pirateplayer.png
    icon256.path = $$DATADIR/icons/hicolor/256x256/apps
    icon256.files += data/256x256/pirateplayer.png
    icon512.path = $$DATADIR/icons/hicolor/512x512/apps
    icon512.files += data/512x512/pirateplayer.png
}
mac {
    #QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
    #QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.6.sdk
    QT += opengl
    LIBS += -L/usr/lib -L/usr/local/lib -lrtmp
    INCLUDEPATH += /usr/local/include
    #LIBS += -L/usr/local/lib
    #INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include
    ICON = pirateplayer.icns
}






















