#-------------------------------------------------
#
# Project created by QtCreator 2018-12-31T12:34:37
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scribe
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    deviceinputdialog.cpp \
    highlighters/highlighter.cpp \
    highlighters/chighlighter.cpp \
    highlighters/cpphighlighter.cpp \
    highlighters/javahighlighter.cpp \
    highlighters/pythonhighlighter.cpp \
    main.cpp \
    mainwindow.cpp \
    finddialog.cpp \
    editor.cpp \
    memoryterminal.cpp \
    metricreporter.cpp \
    settings.cpp \
    utilityfunctions.cpp \
    searchhistory.cpp \
    gotodialog.cpp \
    tabbededitor.cpp \
    language.cpp

HEADERS += \
    deviceinputdialog.h \
    highlighters/highlighter.h \
    highlighters/chighlighter.h \
    highlighters/cpphighlighter.h \
    highlighters/javahighlighter.h \
    highlighters/pythonhighlighter.h \
    mainwindow.h \
    documentmetrics.h \
    finddialog.h \
    editor.h \
    linenumberarea.h \
    memoryterminal.h \
    metricreporter.h \
    settings.h \
    utilityfunctions.h \
    searchhistory.h \
    gotodialog.h \
    tabbededitor.h \
    language.h

FORMS += \
        deviceinputdialog.ui \
        mainwindow.ui \
        memoryterminal.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=

RC_FILE = texteditor.rc
