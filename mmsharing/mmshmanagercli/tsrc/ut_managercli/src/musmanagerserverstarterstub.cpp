/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Stub implementation of MusManagerServerStarter
*
*/



#include "musmanagerserverstarter.h"
#include "musmanageripccommon.h"
#include "musunittesting.h"


TInt MusManagerServerStarter::Start ()
	{
    return KErrNone;
	}


TBool MusManagerServerStarter::Started()
	{
    return EFalse;
	}


TInt MusManagerServerStarter::CreateServerProcess( RSemaphore& /*aSemaphore*/ )
	{
    return KErrNone;
  	}
