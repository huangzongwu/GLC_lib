TARGET = example05
TEMPLATE = app
QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += warn_on

OBJECTS_DIR = ./Build
MOC_DIR = ./Build
UI_DIR = ./Build
RCC_DIR = ./Build

include(../examples.pri)

# Input
HEADERS += glwidget.h mainwindow.h
SOURCES += glwidget.cpp main.cpp mainwindow.cpp

include(../../../install.pri)

target.path = $${GLC_LIB_DIR}/examples
INSTALLS += target