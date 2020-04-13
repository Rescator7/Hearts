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
    cstats.cpp \
    debug.cpp \
    cdeck.cpp \
    client.cpp \
    connect.cpp \
    cgame.cpp \
    ctable.cpp \
    cstatistics.cpp

HEADERS  += mainwindow.h \
    clabel.h \
    chearts.h \
    cardspos.h \
    config.h \
    variants.h \
    rules.h \
    credits.h \
    settings.h \
    define.h \
    cstats.h \
    debug.h \
    cdeck.h \
    client.h \
    connect.h \
    cgame.h \
    ctable.h \
    cstatistics.h

FORMS    += mainwindow.ui \
    rules.ui \
    credits.ui \
    settings.ui \
    connect.ui \
    cgame.ui \
    ctable.ui \
    cstatistics.ui

unix:!macx: LIBS += -lallegro_audio -lallegro_acodec -lallegro_memfile -lallegro

RESOURCES += \
    resources.qrc

DISTFILES +=
