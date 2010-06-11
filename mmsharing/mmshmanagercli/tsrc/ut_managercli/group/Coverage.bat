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

set my_wins_udeb=%EPOCROOT%epoc32\release\winscw\udeb\
set my_output_file=Coverage.txt
set my_uncovered=UnCovered.txt
set my_dll=SipClientDiscoveryTest

prep /EXCALL /INC CSIPClientDiscoveryReceiver.cpp(0-0) /INC CSIPClientDiscoverySerializer.cpp(0-0) /INC CSipClientDiscovery.cpp(0-0) /INC CSipClientDiscoveryImpl.cpp(0-0) /INC SIPClientResolverServerStarter.cpp(0-0) /LV %my_wins_udeb%%my_dll%.dll

profile /i %my_wins_udeb%%my_dll% /o %my_wins_udeb%%my_dll% %my_wins_udeb%cppunit.exe %my_dll%.dll 2

prep /M %my_wins_udeb%%my_dll%

plist %my_wins_udeb%%my_dll% > %my_output_file%

grep -h -e ":    \.\|\.cpp" %my_output_file% | grep -vh "inline" > %my_uncovered%
