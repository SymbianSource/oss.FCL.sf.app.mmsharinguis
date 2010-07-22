@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem
@echo off
echo Installing mustester application ...
echo Backup the original dlls ...
move \epoc32\release\winscw\udeb\mceclient.dll \epoc32\release\winscw\udeb\mcelclient.dll.orig
move \epoc32\release\winscw\udeb\sipclient.dll \epoc32\release\winscw\udeb\sipclient.dll.orig
move \epoc32\release\winscw\udeb\sipprofilecli.dll \epoc32\release\winscw\udeb\sipprofilecli.dll.orig
echo Building mustester ...
call cd ..\..\..\
call qmake mmshlcui.pro
call bldmake bldfiles
call abld reallyclean winscw udeb
call abld build winscw udeb
call cd tsrc\mustester\group
call bldmake bldfiles
call abld reallyclean winscw udeb
call abld build winscw udeb
echo Starting emulator...
echo Goto eshell and execute "must.exe -r usecase_no"
echo     usecase_no = MultimediaSharing::TMusUsecase defined in musmanagercommon.h
call \epoc32\release\winscw\udeb\epoc.exe
