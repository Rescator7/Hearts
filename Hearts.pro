#-------------------------------------------------
#
# Project created by QtCreator 2018-09-30T12:09:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = Hearts
TEMPLATE = app
CONFIG += c++11

TRANSLATIONS = languages/translation_en.ts languages/translation_fr.ts languages/translation_ru.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    clabel.cpp \
    chearts.cpp \
    config.cpp \
    rules.cpp \
    credits.cpp \
    settings.cpp \
    debug.cpp \
    cdeck.cpp \
    client.cpp \
    connect.cpp \
    cgame.cpp \
    ctable.cpp \
    cstatistics.cpp \
    online.cpp \
    ccardsplayed.cpp

HEADERS  += mainwindow.h \
    clabel.h \
    chearts.h \
    config.h \
    variants.h \
    rules.h \
    credits.h \
    settings.h \
    define.h \
    debug.h \
    cdeck.h \
    client.h \
    connect.h \
    cgame.h \
    ctable.h \
    cstatistics.h \
    online.h \
    ccardsplayed.h

FORMS    += mainwindow.ui \
    rules.ui \
    credits.ui \
    settings.ui \
    connect.ui \
    cgame.ui \
    ctable.ui \
    cstatistics.ui \
    online.ui

unix:!macx: LIBS += -lallegro_audio -lallegro_acodec -lallegro_memfile -lallegro

RESOURCES += \
    resources.qrc

DISTFILES +=
