@echo off
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
@echo on

call del %1.xml

:FINAL
echo --because xml names are defined in main.cpp this -o is unnecessary at the moment
echo --\epoc32\release\winscw\udeb\%1.exe -xml -o c:\%1.xml
call \epoc32\release\winscw\udeb\%1.exe -xml -o c:\%1.xml
call copy \epoc32\winscw\c\%1.xml .
call type %1.xml