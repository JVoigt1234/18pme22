#-------------------------------------------------
#
# Project created by QtCreator 2018-10-10T16:50:09
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

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
    Scripts/Database/databasecontroller.cpp \
    Scripts/UI/mainwindow.cpp \
    Scripts/UI/qcustomplot.cpp \
    Scripts/UI/registrationwindow.cpp \
    Scripts/UI/userwindow.cpp \
    Scripts/UI/workwindow.cpp \
    Scripts/main.cpp


HEADERS += \
    Scripts/Database/databasecontroller.h \
    Scripts/Database/Databasetyps.h \
    Scripts/Exceptions/InvalidExceptions.h \
    Scripts/UI/mainwindow.h \
    Scripts/UI/qcustomplot.h \
    Scripts/UI/registrationwindow.h \
    Scripts/UI/userwindow.h \
    Scripts/UI/workwindow.h \
    Scripts/UI/ui_userwindow.h


FORMS += \
    Scripts/UI/mainwindow.ui \
    Scripts/UI/registrationwindow.ui \
    Scripts/UI/userwindow.ui \
    Scripts/UI/workwindow.ui

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

