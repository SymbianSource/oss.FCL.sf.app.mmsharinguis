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

LCUI_DIR = ../../lcui
LCUIENGINE_DIR = ../../lcuiengine
LCUIENGINE_WIN32_DIR = $$LCUIENGINE_DIR/tsrc/win32

RESOURCES += $$LCUI_DIR/livecommsui.qrc

INCLUDEPATH += $$LCUI_DIR/inc
INCLUDEPATH += $$LCUIENGINE_WIN32_DIR
INCLUDEPATH += $$LCUIENGINE_DIR/inc

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

win32: { 
    INCLUDEPATH += $$LCUIENGINE_WIN32_DIR
    
    HEADERS += $$LCUIENGINE_WIN32_DIR/lcuiengine_p.h
    HEADERS += $$LCUIENGINE_WIN32_DIR/lcuienginesimulator.h
    HEADERS += $$LCUIENGINE_WIN32_DIR/lcconfiguration.h    
    SOURCES += $$LCUIENGINE_WIN32_DIR/lcuiengine_p.cpp
    SOURCES += $$LCUIENGINE_WIN32_DIR/lcuienginesimulator.cpp
    SOURCES += $$LCUIENGINE_WIN32_DIR/lcconfiguration.cpp    
    SOURCES += $$LCUIENGINE_WIN32_DIR/lcvideowidget.cpp
}
