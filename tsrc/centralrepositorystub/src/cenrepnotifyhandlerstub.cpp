/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/


#include "cenrepnotifyhandler.h"
#include "centralrepository.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CCenRepNotifyHandler::CCenRepNotifyHandler(
     MCenRepNotifyHandlerCallback& aCallback, 
     CRepository& aSession)
     : iSession(aSession), iCallback(aCallback)
    {
    } 

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CCenRepNotifyHandler* CCenRepNotifyHandler::NewL(
     MCenRepNotifyHandlerCallback& aCallback, 
     CRepository& aSession)
    {
    CCenRepNotifyHandler* newInstance = NewLC(aCallback, aSession);
    CleanupStack::Pop();
    return newInstance;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CCenRepNotifyHandler* CCenRepNotifyHandler::NewLC(
     MCenRepNotifyHandlerCallback& aCallback, 
     CRepository& aSession)
    {
    CCenRepNotifyHandler* newInstance = new (ELeave) CCenRepNotifyHandler(aCallback, aSession);
    CleanupStack::PushL( newInstance );
    
    return newInstance;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CCenRepNotifyHandler::~CCenRepNotifyHandler()
    {
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CCenRepNotifyHandler::StartListeningL()
    {
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CCenRepNotifyHandler::StopListening()
    {
    
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void MCenRepNotifyHandlerCallback::HandleNotifyGeneric(TUint32 /*aId*/)
    {
    
    }
