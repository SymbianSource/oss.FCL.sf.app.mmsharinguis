/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


#include "musmanagerstub.h"
#include <musmanagercommon.h>

#include <e32std.h>

static TInt StartError = KErrNone;
static TInt Availability = KErrNone;
static MultimediaSharing::TMusUseCase useCase = MultimediaSharing::EMusContinue;

EXPORT_C void CMusManager::SetStartError( TInt aError )
    {
    StartError = aError;
    }

EXPORT_C void CMusManager::SetAvailability( TInt aAvailability )
    {
    Availability = aAvailability;    
    }

EXPORT_C void CMusManager::SetUseCase( MultimediaSharing::TMusUseCase aUseCase )
    {
    useCase = aUseCase;
    }
    
EXPORT_C MultimediaSharing::TMusUseCase CMusManager::UseCase()
    {
    return useCase;
    }
    
// -----------------------------------------------------------------------------
// ?description_if_needed
// -----------------------------------------------------------------------------
//
CMusManager::CMusManager()
    :iStartError(KErrNone),
    iExamineLeaveCode(KErrNone),
    iAvailability( ( MultimediaSharing::TMusAvailabilityStatus ) KErrGeneral)
    {
    
    
    
    }

// -----------------------------------------------------------------------------
// ?description_if_needed
// -----------------------------------------------------------------------------
//
void CMusManager::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// ?description_if_needed
// -----------------------------------------------------------------------------
//
EXPORT_C CMusManager* CMusManager::NewL()
    {
    CMusManager* self = CMusManager::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
EXPORT_C CMusManager* CMusManager::NewLC()
    {
    CMusManager* self = new( ELeave ) CMusManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// ?description_if_needed
// ---------------------------------------------------------------------------
//
CMusManager::~CMusManager()
    {
    }

EXPORT_C void CMusManager::ExamineAvailabilityL()
    {
    if( iExamineLeaveCode != KErrNone )
        {
        User::Leave( iExamineLeaveCode );
        }
    }

EXPORT_C MultimediaSharing::TMusAvailabilityStatus CMusManager::AvailabilityL()
    {
    return ( MultimediaSharing::TMusAvailabilityStatus ) Availability;
    }

EXPORT_C void CMusManager::HandleSipRequestL( TRequestType aChannelId )
    {
    iHandleSipRequestLCalledWithParam = aChannelId;
    }

EXPORT_C void CMusManager::StartApplicationL( MultimediaSharing::TMusUseCase aUseCase )
    {
    if( StartError != KErrNone )
        {
        User::Leave( StartError );
        }
    
    useCase = aUseCase;
    }

EXPORT_C void CMusManager::StopApplicationL()
    {
    }

EXPORT_C void CMusManager::MonitorAvailabilityL( 
                    TRequestStatus& aRequestStatus,
                    MultimediaSharing::TMusAvailabilityStatus& /*aAvailabilityStatus*/ )
    {
    iStatus = &aRequestStatus;
    }

EXPORT_C void CMusManager::CancelMonitoring()
	{
	RThread thread;
	
	thread.RequestComplete( iStatus, KErrNone );
	
	thread.Close();
	}


EXPORT_C void CMusManager::HandleCommandL( MultimediaSharing::TCommandType /*aCommandType*/ )
    {
    
    }
 
 
