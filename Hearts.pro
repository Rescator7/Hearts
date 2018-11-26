#-------------------------------------------------
#
# Project created by QtCreator 2018-09-30T12:09:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hearts
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clabel.cpp \
    chearts.cpp \
    config.cpp \
    rules.cpp \
    credits.cpp \
    settings.cpp \
    cstats.cpp

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
    cstats.h

FORMS    += mainwindow.ui \
    rules.ui \
    credits.ui \
    settings.ui

unix:!macx: LIBS += -lallegro_audio -lallegro_acodec -lallegro_memfile -lallegro

RESOURCES += \
    resources.qrc
