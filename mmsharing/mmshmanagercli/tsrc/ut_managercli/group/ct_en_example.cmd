rem
rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem

@echo off
set CODE_TEST_LIB=NTmvc_TargetLibMD.lib CtSymbianNativeMem.lib
set AMC_TARGET=x86-symbian-native

REM ########################################################
REM ### Default settings (performance analysis, statement coverage)
REM set CTDRIVER_ARGS=-CTsize_t=unsigned -CTtag-allocator -CTv -CTno-tag-includes

REM ########################################################
REM ### MCE Client
set CTDRIVER_ARGS=-CTsize_t=unsigned -CTtag-allocator -CTsymbian -CTno-tag-includes -CTtag-level=DC -CTonly-tag-files=mushmanager.cpp,mushmanagerclientsession.cpp,mushmanagerimpl.cpp,mushmanagerserverstarter.cpp

REM ########################################################
REM ### Display the new settings
echo CodeTest parameters set to:
set CODE_TEST_LIB
set CTDRIVER_ARGS

@echo on
