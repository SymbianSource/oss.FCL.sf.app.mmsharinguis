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
TARGET = must
CONFIG += hb

# Input
HEADERS +=  lcapplication.h
HEADERS +=  mustestdirector.h
INCLUDEPATH +=  ..\..\..\..\..\inc\

SOURCES += main.cpp 
SOURCES += lcapplication.cpp
SOURCES += mustestdirector.cpp

symbian: {
    TARGET.UID2 = 0x1000008d
    TARGET.UID3 = 0x1028238D  
    TARGET.CAPABILITY = CAP_APPLICATION NetworkControl SwEvent Location MultimediaDD
    LIBS += -llivecommsui -lcentralrepository
}
