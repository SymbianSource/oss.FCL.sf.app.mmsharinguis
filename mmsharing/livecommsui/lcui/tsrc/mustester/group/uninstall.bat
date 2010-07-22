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
move \epoc32\release\winscw\udeb\mcelclient.dll.orig \epoc32\release\winscw\udeb\mceclient.dll
move \epoc32\release\winscw\udeb\sipclient.dll.orig \epoc32\release\winscw\udeb\sipclient.dll
move \epoc32\release\winscw\udeb\sipprofilecli.dll.orig \epoc32\release\winscw\udeb\sipprofilecli.dll 

call abld reallyclean winscw udeb
call abld clean winscw udeb
del \epoc32\release\winscw\udeb\must.exe