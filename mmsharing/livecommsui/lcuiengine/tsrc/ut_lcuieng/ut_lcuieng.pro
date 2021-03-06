######################################################################
# Automatically generated by qmake (2.01a) Fri May 22 10:46:25 2009
######################################################################

TEMPLATE = app
TARGET = 

QT += testlib xml
CONFIG  += qtestlib

MUS_DIR = ../../../../..
LCUI_DIR = ../../../lcui
LCUIENGINE_DIR = ../..

DEPENDPATH += .

INCLUDEPATH += .
INCLUDEPATH += hbstubs
INCLUDEPATH += /epoc32/include
INCLUDEPATH += /epoc32/include/platform/app
INCLUDEPATH += /epoc32/include/ecom
INCLUDEPATH += inc
INCLUDEPATH += $$LCUIENGINE_DIR
INCLUDEPATH += $$LCUIENGINE_DIR/inc
INCLUDEPATH += $$LCUI_DIR/mmshlcuiengine/tsrc/lcstub/inc
INCLUDEPATH += $$LCUI_DIR/inc
INCLUDEPATH += $$LCUI_DIR/../inc
INCLUDEPATH += lcstub/inc
INCLUDEPATH += $$MUS_DIR/tsrc/qtestutils/inc
INCLUDEPATH += $$MUS_DIR/inc

CONFIG  += hb

DEFINES += QT_NO_DEBUG_OUTPUT

# Input
HEADERS += inc/ut_lcuiengine.h
HEADERS += $$LCUIENGINE_DIR/inc/lcuiengine.h

SOURCES += src/main.cpp 
SOURCES += src/ut_lcuiengine.cpp 
SOURCES += $$LCUIENGINE_DIR/src/lcuiengine.cpp
SOURCES += hbstubs/hbstubs.cpp
SOURCES += hbstubs/qthighway_stub.cpp
SOURCES += lcstub/src/lcsessionstub.cpp
SOURCES += lcstub/src/lcvideoplayerstub.cpp
SOURCES += lcstub/src/lcenginestub.cpp
SOURCES += $$MUS_DIR/tsrc/qtestutils/src/testrunner.cpp

symbian: {
    INCLUDEPATH += $$LCUIENGINE_DIR/inc
    HEADERS += inc/ut_lcactivitymanager.h
    HEADERS += $$LCUIENGINE_DIR/inc/lcuiengine_p.h
    HEADERS += $$LCUIENGINE_DIR/inc/lcactivitymanager.h
    SOURCES += src/ut_lcactivitymanager.cpp 
    SOURCES += $$LCUIENGINE_DIR/src/lcuiengine_p.cpp
    SOURCES += $$LCUIENGINE_DIR/src/lcactivitymanager.cpp
    SOURCES += lcstub/src/centralrepository_stub.cpp

    TARGET.UID2 = 0x100039CE
    TARGET.UID3 = 0xEfa329b2
    TARGET.CAPABILITY = ALL -TCB
    LIBS += -lecom -lflogger -lws32 -lbafl -lxqservice -lxqserviceutil -lcone -lxqsettingsmanager
    TARGET.EPOCALLOWDLLDATA = 1
}
