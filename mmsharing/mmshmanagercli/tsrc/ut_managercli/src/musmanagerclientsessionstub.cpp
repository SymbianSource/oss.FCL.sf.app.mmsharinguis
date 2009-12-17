/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Stub implementation of CMusManagerServerClientSession
*
*/



#include "musmanagerclientsession.h"
#include "musmanagercommon.h"
#include "musmanageripccommon.h"
#include "musunittesting.h"
#include <e32base.h>


TRequestStatus* iRequestStatus;

// ======== MEMBER FUNCTIONS ========


RMusManagerClientSession::RMusManagerClientSession()
    : RSessionBase ()
    {
    }


void RMusManagerClientSession::ConnectL()
    {
    }


TVersion RMusManagerClientSession::Version(void) const
	{
    return TVersion( KMusManagerServerMajorVersionNumber,
                     KMusManagerServerMinorVersionNumber,
                     KMusManagerServerBuildVersionNumber);
	}


void RMusManagerClientSession::Disconnect()
    {
    }


void RMusManagerClientSession::InvestigateAvailabilityL()
    {
    }


MultimediaSharing::TMusAvailabilityStatus RMusManagerClientSession::QueryAvailabilityL()
    {
    return (MultimediaSharing::TMusAvailabilityStatus) KErrNone;
    }


void RMusManagerClientSession::StartL( MultimediaSharing::TMusUseCase /*aUseCase*/ )
    {
    }


void RMusManagerClientSession::StopL()
    {
    }


void RMusManagerClientSession::HandleSipRequestL( TInt /*aUid*/ )
    {
    }


void RMusManagerClientSession::MonitorAvailabilityL(
            TRequestStatus& aRequestStatus,
            TIpcArgs& /*aIpcArgs*/ )
    {
    iRequestStatus = &aRequestStatus;
    }


void RMusManagerClientSession::CancelMonitoringL()
    {
    User::RequestComplete( iRequestStatus, KErrCancel );
    }
    
void RMusManagerClientSession::HandleCommandL( MultimediaSharing::TCommandType /*aCommandType*/ )
    {
    }
    
void RMusManagerClientSession::StartObservingAvailabilityL()
    {
    }
