#-------------------------------------------------
#
# Project created by QtCreator 2018-10-10T16:50:09
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += network

TARGET = DiaThesis
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
        main.cpp \
        mainwindow.cpp \
    Scripts/Database/databasecontroller.cpp \
    workwindow.cpp \
    registrationwindow.cpp

HEADERS += \
        mainwindow.h \
    Scripts/Database/databasecontroller.h \
    workwindow.h \
    registrationwindow.h \
    Scripts/Databasetyps.h \
    Scripts/Exceptions/InvalidExceptions.h \
    ui_mainwindow.h \
    ui_registrationwindow.h \
    ui_workwindow.h \

FORMS += \
        mainwindow.ui \
    workwindow.ui \
    registrationwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Pictures/Logo_DiaThesis.png \
    Pictures/Logo_DiaThesis.png \
    TestDaten/Patient.json \
    TestDaten/measurment/datasingle.json \
    TestDaten/measurment/datenarray.json

