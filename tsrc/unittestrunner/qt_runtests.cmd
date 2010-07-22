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

call del MON.sym
call del MON.dat

call qmake -platform symbian-abld
call sbs -c winscw_udeb reallyclean

echo -- Build sbs -c winscw_udeb--
call sbs -c winscw_udeb

echo -- Build and instrument for Code coverage --
echo -- ctcwrap -i f : This instruments the code for function coverage measurement 
call ctcwrap -i d -C "EXCLUDE+*/tsrc/*" -C "EXCLUDE+*/*/tsrc/*" -C "EXCLUDE+*/*/*/tsrc/*" -C "EXCLUDE+moc_*" sbs -c winscw_udeb

echo --qt_runall.cmd
call qt_runall.cmd

echo --ctcpost MON.sym MON.dat -p profile.txt
call ctcpost MON.sym MON.dat -p profile.txt

echo --you can give the output directory for the result
echo --e.q qt_runtests.cmd R:\qtresult
echo --ctc2html -i profile.txt -o %1

IF ["%1"] EQU [""] GOTO :USEDEFDIR

call ctc2html -i profile.txt -o %1
GOTO :ENDDIR

:USEDEFDIR
echo --default output directory used
call ctc2html -i profile.txt

:ENDDIR
echo -- Coverage calculated --

:END
echo -- Build Complete --


