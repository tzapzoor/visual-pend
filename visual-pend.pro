#-------------------------------------------------
#
# Project created by QtCreator 2015-04-14T15:32:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VisualPend
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        Helper.cpp\
        DrawWidget.cpp \
    AboutDialog.cpp

HEADERS  += mainwindow.h\
            Helper.h\
            DrawWidget.h \
    AboutDialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

DISTFILES +=

RESOURCES += \
    media/media.qrc

RC_ICONS = media/icon.ico
