#-------------------------------------------------
#
# Project created by QtCreator 2012-09-11T12:56:00
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml
QT       += declarative

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pirateplayer
TEMPLATE = app

ubuntu_12_04 {
  DEFINES += UBUNTU_12_04
  message("Ubuntu 12.04 enabled")
}

SOURCES += src/main.cpp\
    src/gui/mainwindow.cpp \
    src/gui/downloaddelegate.cpp \
    src/gui/savestreamdialog.cpp \
    src/gui/filepathedit.cpp \
    src/network/download.cpp \
    src/network/systemdownload.cpp \
    src/network/piratenetworkaccessmanager.cpp \
    src/network/piratenetworkreply.cpp \
    src/network/rtmpsession.cpp \
    src/models/downloadlistmodel.cpp \
    src/models/twitterfeedmodel.cpp \
    src/models/streamtablemodel.cpp \
    src/gui/downloadtablewidget.cpp \
    src/gui/messagelabel.cpp \
    src/gui/addon.cpp \
    src/extra/archiveextractor.cpp \
    src/network/tidynetworkreply.cpp

HEADERS  += src/gui/mainwindow.h \
    src/gui/downloaddelegate.h \
    src/gui/savestreamdialog.h \
    src/gui/filepathedit.h \
    src/network/abstractdownload.h \
    src/network/download.h \
    src/network/systemdownload.h \
    src/network/piratenetworkreply.h \
    src/network/piratenetworkaccessmanager.h \
    src/network/rtmpsession.h \
    src/models/downloadlistmodel.h \
    src/models/twitterfeedmodel.h \
    src/models/streamtablemodel.h \
    src/gui/downloadtablewidget.h \
    src/gui/messagelabel.h \
    src/gui/addon.h \
    src/extra/archiveextractor.h \
    src/network/tidynetworkreply.h

FORMS    += src/gui/mainwindow.ui

RESOURCES += resources.qrc

DESTDIR = build/target/
OBJECTS_DIR = build/obj/
MOC_DIR = build/moc/
UI_DIR = build/ui/

win32 {
    INCLUDEPATH += "C:/Users/chucky/include"
    INCLUDEPATH += "C:/Users/chucky/src/tidy-html5/include"
    INCLUDEPATH += "C:/Program Files/libarchive/include"

    LIBS += -L"C:/Program Files/rtmpdump-git" -lrtmp -lws2_32
    LIBS += -L"C:/Users/chucky/src/tidy-html5/src/.libs" -ltidy
    LIBS += -L"C:/Program Files/libarchive/lib" -larchive

    RC_FILE = pirateplayer.rc
}
unix {
    LIBS += -L/usr/lib/ -lrtmp -larchive -ltidy
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
    DEPENDPATH += /usr/local/include
    ICON = pirateplayer.icns
}
