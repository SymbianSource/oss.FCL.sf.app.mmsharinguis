#                                                                    
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).   
# All rights reserved.                                                
# This component and the accompanying materials are made available    
# under the terms of "Eclipse Public License v1.0"    
# which accompanies this distribution, and is available               
# at the URL "http://www.eclipse.org/legal/epl-v10.html".   
#                                                                     
# Initial Contributors:                                               
# Nokia Corporation - initial contribution.                           
#                                                                     
# Contributors:                                                       
#                                                                     
# Description:                                                        
#                                                                     
#

TEMPLATE = app
TARGET = 

QT += testlib xml
CONFIG += hb qtestlib dll

MUS_DIR = ../../../../..
LCUI_DIR = ../..
LCUIENGINE_DIR = ../../../lcuiengine

RESOURCES += $$LCUI_DIR/livecommsui.qrc

DEFINES += UNIT_TESTING
DEFINES += QT_NO_DEBUG_OUTPUT

DEPENDPATH += .
INCLUDEPATH = hbstubs $$INCLUDEPATH
INCLUDEPATH += .
INCLUDEPATH += $$LCUI_DIR/inc
INCLUDEPATH += $$MUS_DIR/tsrc/qtestutils/inc
INCLUDEPATH += $$MUS_DIR/mmsharinguis_plat/live_comms_plugin_api/inc
INCLUDEPATH += /epoc32/include/ecom
INCLUDEPATH += /epoc32/include/platform/app

# Input
HEADERS += inc/ut_lcviewmanager.h
HEADERS += $$LCUI_DIR/inc/lcviewmanager.h
HEADERS += $$LCUI_DIR/inc/lcviewmanager_p.h
HEADERS += inc/ut_lcuicomponentrepository.h
HEADERS += $$LCUI_DIR/inc/lcuicomponentrepository.h
HEADERS += inc/ut_lcview.h
HEADERS += $$LCUI_DIR/inc/lcview.h
HEADERS += inc/ut_lceffecthandler.h
HEADERS += $$LCUI_DIR/inc/lceffecthandler.h
HEADERS += inc/ut_lcvideowidget.h
HEADERS += $$LCUI_DIR/inc/lcvideowidget.h
HEADERS += $$LCUIENGINE_DIR/inc/lcuiengine.h
HEADERS += ./hbstubs/dialpad.h
HEADERS += ./hbstubs/dialpadvtkeyhandler.h
HEADERS += ./hbstubs/hblineedit.h

SOURCES += src/main.cpp
SOURCES += hbstubs/hbstubs.cpp
SOURCES += hbstubs/dialpad_stub.cpp
SOURCES += hbstubs/dialpadvtkeyhandler_stub.cpp
SOURCES += src/ut_lcviewmanager.cpp
SOURCES += $$LCUI_DIR/src/lcviewmanager.cpp
SOURCES += $$LCUI_DIR/src/lcviewmanager_p.cpp
SOURCES += src/ut_lcuicomponentrepository.cpp
SOURCES += $$LCUI_DIR/src/lcuicomponentrepository.cpp
SOURCES += src/ut_lcview.cpp
SOURCES += $$LCUI_DIR/src/lcview.cpp
SOURCES += src/ut_lceffecthandler.cpp
SOURCES += $$LCUI_DIR/src/lceffecthandler.cpp
SOURCES += src/ut_lcvideowidget.cpp
SOURCES += $$LCUI_DIR/src/lcvideowidget.cpp
SOURCES += $$LCUIENGINE_DIR/src/lcuiengine.cpp
SOURCES += $$MUS_DIR/tsrc/qtestutils/src/testrunner.cpp


symbian: {
    DEFINES += BUILD_SYMBIAN

    INCLUDEPATH += $$LCUIENGINE_DIR/inc

    SOURCES += lcuiengine_stub/symbian/stub_lcuiengine_p.cpp
    HEADERS += $$LCUIENGINE_DIR/inc/lcuiengine_p.h
    
    LIBS += -lecom -lflogger -lws32 -lbafl
    
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

win32: {
    INCLUDEPATH += $$LCUI_DIR/mmshlcuiengine/win32
    INCLUDEPATH += inc
    INCLUDEPATH += $$LCUIENGINE_DIR/tsrc/win32
    INCLUDEPATH += $$LCUIENGINE_DIR/inc

    SOURCES += lcuiengine_stub/win32/stub_lcuiengine_p.cpp
    HEADERS += $$LCUIENGINE_DIR/tsrc/win32/lcuiengine_p.h
}
