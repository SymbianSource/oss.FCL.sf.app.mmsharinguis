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

CONFIG += hb
QT += testlib xml
#CONFIG  += qtestlib

MUSAPP_DIR = ../..
MUS_DIR = ../../../..

DEPENDPATH += .

INCLUDEPATH += inc
INCLUDEPATH += stubs
INCLUDEPATH += $$MUSAPP_DIR/inc
INCLUDEPATH += $$MUS_DIR/tsrc/qtestutils/inc

# Input
HEADERS += inc/ut_lcapplication.h
HEADERS += $$MUSAPP_DIR/inc/lcapplication.h

SOURCES += src/main.cpp
SOURCES += src/ut_lcapplication.cpp
SOURCES += $$MUSAPP_DIR/src/lcapplication.cpp
SOURCES += $$MUS_DIR/tsrc/qtestutils/src/testresultxmlparser.cpp


symbian: {
    TARGET.UID2 = 0x100039CE
    TARGET.UID3 = 0xE5923aef
    TARGET.CAPABILITY = ALL -TCB
    TARGET.EPOCALLOWDLLDATA = 1
}
