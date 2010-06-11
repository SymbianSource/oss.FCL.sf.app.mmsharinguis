/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "apgcli.h"

RApaLsSession::RApaLsSession()
    {
    iConnected = EFalse;
    }


 void RApaLsSession::Close()
    {
    iConnected = EFalse;
    }


 TInt RApaLsSession::Connect()
    {
    iConnected = ETrue;        
    return KErrNone;
    }


 TVersion RApaLsSession::Version() const
    {
    return TVersion();

    }


 TInt RApaLsSession::GetAppInfo(TApaAppInfo& aInfo,TUid aAppUid) const
    {
    return KErrNone;

    }


 TInt RApaLsSession::StartApp(const CApaCommandLine& aCommandLine)
    {
    return KErrNone;

    }// This is only recommended for non document based apps


 TInt RApaLsSession::StartApp(const CApaCommandLine& aCommandLine,TThreadId& aThreadId)
    {
    return KErrNone;
    }


 TInt RApaLsSession::StartApp(const CApaCommandLine& aCommandLine,TThreadId& aThreadId,TRequestStatus* aRequestStatusForRendezvous)
    {
    return KErrNone;
    }
    
 TInt RApaLsSession::StartDocument( const TDesC& /*aFileName*/, TUid /*aAppUid*/, TThreadId& /*aThreadId*/, TLaunchType /*aLaunchType*/ )
    {
    return KErrNone;
    }
