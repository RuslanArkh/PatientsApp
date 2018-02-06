#-------------------------------------------------
#
# Project created by QtCreator 2017-07-03T15:59:28
#
#-------------------------------------------------

CONFIG += c++14

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = patients_database
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    patient.cpp \
    photo.cpp \
    dbmanager.cpp \
    dbmanager_exceptions.cpp \
    patients_window.cpp \
    new_patient.cpp \
    imageview.cpp \
    view_patient.cpp \
    patients_model.cpp \
    patients_filter_proxe_model.cpp \
    delete_button_delegate.cpp \
    cremovepatientbutton.cpp

HEADERS += \
    patient.h \
    photo.h \
    dbmanager.h \
    dbmanager_exceptions.h \
    patients_window.h \
    new_patient.h \
    imageview.h \
    view_patient.h \
    patients_model.h \
    patients_filter_proxy_model.h \
    delete_button_delegate.h \
    cremovepatientbutton.h

FORMS += \
        mainwindow.ui \
    new_patient.ui \
    imageview.ui \
    view_patient.ui

RESOURCES += \
    resources.qrc
