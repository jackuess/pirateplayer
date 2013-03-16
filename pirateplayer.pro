#-------------------------------------------------
#
# Project created by QtCreator 2013-02-20T21:08:55
#
#-------------------------------------------------

QT       += core gui declarative network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pirateplayer
TEMPLATE = app

CONFIG += mobility
MOBILITY = 


SOURCES += src/network/download.cpp \
        src/network/systemdownload.cpp \
        src/network/piratenetworkaccessmanager.cpp \
        src/network/piratenetworkreply.cpp \
        src/network/rtmpsession.cpp \
        src/network/downloadlistmodel.cpp \
        src/network/tidynetworkreply.cpp \
        src/network/tempplayer.cpp \
        src/gui/myqmlapplicationviewer.cpp \
        src/gui/pirateplayerdesktopviewer.cpp \
        src/gui/maingui.cpp \
        src/extra/archiveextractor.cpp \
        src/pirateplayercontext.cpp \
        src/main.cpp\
    src/mediaplayerinterface.cpp

HEADERS  += src/network/download.h \
        src/network/systemdownload.h \
        src/network/piratenetworkreply.h \
        src/network/piratenetworkaccessmanager.h \
        src/network/rtmpsession.h \
        src/network/downloadlistmodel.h \
        src/network/tidynetworkreply.h \
        src/network/tempplayer.h \
        src/gui/myqmlapplicationviewer.h \
        src/gui/pirateplayerdesktopviewer.h \
        src/gui/abstractqmlviewer.h \
        src/gui/maingui.h \
        src/extra/archiveextractor.h \
        src/pirateplayercontext.h \
    src/mediaplayerinterface.h

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/version.xml \
    android/res/values-fr/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-de/strings.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/values-id/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-es/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values-nb/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-nl/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QJavaInterface.java

RESOURCES += resources.qrc

win32 {
    INCLUDEPATH += "C:/Users/chucky/include"
    INCLUDEPATH += "C:/Users/chucky/src/tidy-html5/include"
    INCLUDEPATH += "C:/Program Files/libarchive/include"

    LIBS += -L"C:/Program Files/rtmpdump-git" -lws2_32
    LIBS += -L"C:/Users/chucky/src/tidy-html5/src/.libs"
    LIBS += -L"C:/Program Files/libarchive/lib"

    DESTDIR = build/target/
    OBJECTS_DIR = build/obj/
    MOC_DIR = build/moc/
    UI_DIR = build/ui/

    RC_FILE = pirateplayer.rc
}

android {
    LIBS += -L/home/chucky/src/tidy-html5-android/libs/armeabi
    INCLUDEPATH += /home/chucky/src/tidy-html5-android/include

    INCLUDEPATH += /home/chucky/src/librtmp-2.4-android-armeabi-v7a/include
    LIBS += -L/home/chucky/src/rtmpdump-master/librtmp

    LIBS += /home/chucky/src/libarchive-3.1.2/libarchive/libarchive.a
    INCLUDEPATH += /home/chucky/src/libarchive_include

    ffmpeg.files=/home/chucky/src/android-ffmpeg/ffmpeg/ffmpeg
    ffmpeg.path=/assets
    INSTALLS += ffmpeg
} else {
    LIBS += -larchive
}

mac {
    QT += opengl
    LIBS += -L/usr/lib -L/usr/local/lib
    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include
    ICON = pirateplayer.icns
}

unix {
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

LIBS += -lrtmp
LIBS += -ltidy

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
