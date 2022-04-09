#-------------------------------------------------
#
# Project created by QtCreator 2018-09-30T12:09:16
#
#-------------------------------------------------

# DEFINES options:
# ------------------------------------------------------------------------------------------------
# USE_LIBALLEGRO5
#   Turn on sounds. liballegro5 libraries must be installed to compile.
# DEBUG
#   Enable menu Debug including Cards History, Cards Played and Cheat mode.
# DEBUG_DATAGRAMS
#   provide online connection datagrans infos into the information channel. (For coders only)
# FULL_SCREEN
#   Enable application main windows resize, and full screen. (BETA)
# ONLINE_PLAY
#   Enable online play.
#
#   NOTE: If disabled by removing ONLINE_PLAY from DEFINES. The app will compile without online classes,
#         variables and functions associated with online play, BUT online icons will still be
#         included in the binary. As this option doesn't remove them from resources.qrc, also
#         that won't remove them in the help/credits (credits.ui). That must be done manually if required.
#         Online buttons, and such will still be present in the UI (mainwindow.ui), but hidden.
# ------------------------------------------------------------------------------------------------
# NOTE: require a rebuild all when modifing the defines options. (QTCREATOR)
#       or on the shell: make clean; qmake Hearts.pro; make
DEFINES += ONLINE_PLAY USE_LIBALLEGRO5 FULL_SCREEN DEBUG

equals(QMAKE_CXX, g++) { QMAKE_LFLAGS += -no-pie }

QT      += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

contains(DEFINES, ONLINE_PLAY) {
QT      += network

FORMS   += connect.ui \
     online.ui \
     ctable.ui \
     cgame.ui \

SOURCES += client.cpp \
    connect.cpp \
    ctable.cpp \
    online.cpp \
    cgame.cpp \

HEADERS +=  client.h \
    connect.h \
    ctable.h \
    online.h \
    cgame.h \
}

contains(DEFINES, DEBUG) {
SOURCES += debug.cpp \
    ccardsplayed.cpp \

HEADERS += debug.h \
    ccardsplayed.h \
}

contains(DEFINES, USE_LIBALLEGRO5) {
unix:!macx: LIBS += -lallegro_audio -lallegro_acodec -lallegro_memfile -lallegro
}

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
    cdeck.cpp \
    cstatistics.cpp \
    speed.cpp

HEADERS  += mainwindow.h \
    clabel.h \
    chearts.h \
    config.h \
    variants.h \
    rules.h \
    credits.h \
    settings.h \
    define.h \
    cdeck.h \
    cstatistics.h \
    speed.h

FORMS    += mainwindow.ui \
    rules.ui \
    credits.ui \
    settings.ui \
    cstatistics.ui \
    speed.ui

RESOURCES += \
    resources.qrc
