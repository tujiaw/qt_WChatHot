#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T10:44:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webenginewidgets

TARGET = WChatHot
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    webengineview.cpp \
    json.cpp \
    articlelistdata.cpp \
    request.cpp \
    response.cpp \
    netmanager.cpp \
    tabwidget.cpp

HEADERS  += dialog.h \
    webengineview.h \
    json.h \
    articlelistdata.h \
    request.h \
    response.h \
    netmanager.h \
    tabwidget.h

FORMS    += dialog.ui

RESOURCES += \
    rc.qrc
