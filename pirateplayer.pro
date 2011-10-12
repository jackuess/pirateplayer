#-------------------------------------------------
#
# Project created by QtCreator 2011-09-27T00:05:15
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml

TARGET = pirateplayer
TEMPLATE = app


SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/piratenetworkaccessmanager.cpp \
    src/piratenetworkreply.cpp

HEADERS  += src/mainwindow.h \
    src/piratenetworkaccessmanager.h \
    src/piratenetworkreply.h

FORMS    += src/mainwindow.ui

DESTDIR = build/target/
OBJECTS_DIR = build/obj/
MOC_DIR = build/moc/
UI_DIR = build/ui/

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lrtmp
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lrtmp
win32 {
    INCLUDEPATH += /usr/include
    #LIBS += -L/usr/i486-mingw32/bin/
    LIBS += -L/home/chucky/src/rtmpdump-win-git/ -lrtmp
    QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
    #LIBS += -L/home/chucky/src/rtmpdump/librtmp/ -Wl,-Bstatic -lrtmp -lssl -lcrypto -lz -lwinmm -lws2_32 -Wl,-Bdynamic
    #CONFIG += static
    #QMAKE_LFLAGS += -static -static-libgcc
}
unix {
    LIBS += -L/usr/lib/ -Wl,-Bstatic -lrtmp -Wl,-Bdynamic
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include

    isEmpty(PREFIX):PREFIX = /usr/local
    target.path = $$PREFIX/bin

    #target.path = /usr/local/bin
    #target.files = pirateplayer
    INSTALLS += target
}

#Statiiiskt
#CONFIG += static
#QMAKE_LFLAGS += -static
#QMAKE_LIBDIR_QT += /opt/qt-static/lib


















