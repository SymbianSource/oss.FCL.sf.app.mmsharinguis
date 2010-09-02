# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
# Description:

TEMPLATE = lib
TARGET = livecommsui
CONFIG += hb \
    dll \
    svg
    
QT += xml

DEFINES += LIVECOMMSUI_LIB
TRANSLATIONS = dialer.ts

LCUI_DIR = lcui
LCUIENGINE_DIR = lcuiengine
DOCML_DIR = lcui/resources/lcviews

DOCML += $$DOCML_DIR/lcidleview.docml
DOCML += $$DOCML_DIR/lcreceiveview.docml
DOCML += $$DOCML_DIR/lcreceiveonlyview.docml
DOCML += $$DOCML_DIR/lcsendview.docml
DOCML += $$DOCML_DIR/lctwowayview.docml
DOCML += $$DOCML_DIR/lcvtview.docml

RESOURCES += $$LCUI_DIR/livecommsui.qrc

INCLUDEPATH += $$LCUI_DIR/inc
INCLUDEPATH += $$LCUIENGINE_DIR/inc
INCLUDEPATH += ../inc

# Input
HEADERS += $$LCUI_DIR/inc/lcviewmanager.h     
HEADERS += $$LCUI_DIR/inc/lcviewmanager_p.h
HEADERS += $$LCUI_DIR/inc/lcview.h
HEADERS += $$LCUI_DIR/inc/lcuicomponentrepository.h
HEADERS += $$LCUI_DIR/inc/lceffecthandler.h
HEADERS += $$LCUI_DIR/inc/lcvideowidget.h
HEADERS += $$LCUIENGINE_DIR/inc/lcuiengine.h

SOURCES += $$LCUI_DIR/src/lcviewmanager.cpp    
SOURCES += $$LCUI_DIR/src/lcviewmanager_p.cpp
SOURCES += $$LCUI_DIR/src/lcview.cpp
SOURCES += $$LCUI_DIR/src/lcuicomponentrepository.cpp
SOURCES += $$LCUI_DIR/src/lceffecthandler.cpp
SOURCES += $$LCUIENGINE_DIR/src/lcuiengine.cpp

libFiles.sources = livecommsui.dll
DEPLOYMENT += libFiles

symbian: {

    INCLUDEPATH += /epoc32/include/ecom
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    
    HEADERS += $$LCUIENGINE_DIR/inc/lcuiengine_p.h
    HEADERS += $$LCUIENGINE_DIR/inc/lcactivitymanager.h
    
    SOURCES += $$LCUIENGINE_DIR/src/lcuiengine_p.cpp
    SOURCES += $$LCUIENGINE_DIR/src/lcactivitymanager.cpp
    SOURCES += $$LCUI_DIR/src/lcvideowidget.cpp
        
    TARGET.UID2 = 0x1000008d
    TARGET.UID3 = 0x2002434D
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.EPOCALLOWDLLDATA = 1
    
    libFiles.path = "!:/sys/bin"
    LIBS += -lecom \
        -lflogger \
        -lws32 \
        -lbafl \
        -lcone \
        -lcentralrepository \
        -lxqserviceutil \
        -ldialpad \
        -lxqsettingsmanager
    
    defFiles = "$${LITERAL_HASH}ifdef WINS" \
        "DEFFILE bwins/livecommsui.def" \
        "$${LITERAL_HASH}else" \
        "DEFFILE eabi/livecommsui.def" \
        "$${LITERAL_HASH}endif"
    MMP_RULES += defFiles
            
    :BLD_INF_RULES.prj_exports += "$$LCUI_DIR/inc/lcviewmanager.h /epoc32/include/lcviewmanager.h"
    :BLD_INF_RULES.prj_exports += "$$LCUI_DIR/inc/lcexport.h /epoc32/include/lcexport.h"    
}

