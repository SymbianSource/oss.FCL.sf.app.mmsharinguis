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


#include "musmanager.h"
#include "musmanagerimpl.h"

#include "musunittesting.h"
#include "muslogger.h"
#include "musavailabilityobserver.h"
#include <featmgr.h>

// ---------------------------------------------------------------------------
// Default Constructor for CMusManager
// CMusManager::CMusManager()
// ---------------------------------------------------------------------------
//
CMusManager::CMusManager()
    {
    }

// ---------------------------------------------------------------------------
// Two Phase Constructor
// CMusManager::ConstructL()
// ---------------------------------------------------------------------------
//
void CMusManager::ConstructL()
    {
	FeatureManager::InitializeLibL();
    TBool support = FeatureManager::FeatureSupported( KFeatureIdMultimediaSharing );
	FeatureManager::UnInitializeLib();
	if ( !support )
      {
      User::Leave( KErrNotSupported );
      }
    iImpl = CMusManagerImpl::NewL();
    }
    
   
// ---------------------------------------------------------------------------
// Two Phase Constructor
// CMusManager::ConstructL()
// ---------------------------------------------------------------------------
//
void CMusManager::ConstructL( MMusAvailabilityObserver* aObserver )
    {
	FeatureManager::InitializeLibL();
    TBool support = FeatureManager::FeatureSupported( KFeatureIdMultimediaSharing );
	FeatureManager::UnInitializeLib();
	if ( !support )
      {
      User::Leave( KErrNotSupported );
      }
    iImpl = CMusManagerImpl::NewL( aObserver );
    }    


// ---------------------------------------------------------------------------
// Two Phase Constructor
// CMusManager::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CMusManager* CMusManager::NewL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::NewL()" );
    CMusManager* self = CMusManager::NewLC();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::NewL()" );
    return self;
    }


// ---------------------------------------------------------------------------
// Two Phase Constructor
// CMusManager::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CMusManager* CMusManager::NewLC()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::NewLC()" );

    CMusManager* self = new( ELeave ) CMusManager;
    CleanupStack::PushL( self );
    self->ConstructL();

    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::NewLC()" );
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// Two Phase Constructor
// CMusManager::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CMusManager* CMusManager::NewL( MMusAvailabilityObserver* aObserver )
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::NewL()" );
    CMusManager* self = CMusManager::NewLC( aObserver );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::NewL()" );
    return self;
    }


// ---------------------------------------------------------------------------
// Two Phase Constructor
// CMusManager::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CMusManager* CMusManager::NewLC( MMusAvailabilityObserver* aObserver )
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::NewLC()" );

    CMusManager* self = new( ELeave ) CMusManager;
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );

    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::NewLC()" );
    return self;
    }    


// ---------------------------------------------------------------------------
// Destructor
// CMusManager::~CMusManager()
// ---------------------------------------------------------------------------
//
CMusManager::~CMusManager()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::~CMusManager()" );
    delete iImpl;
    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::~CMusManager()" );
    }

// ---------------------------------------------------------------------------
// CMusManager::ExamineAvailabilityL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMusManager::ExamineAvailabilityL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::ExamineAvailabilityL()" );
    iImpl->ExamineAvailabilityL();
    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::ExamineAvailabilityL()" );
    }

// ---------------------------------------------------------------------------
// CMusManager::AvailabilityL()
// ---------------------------------------------------------------------------
//
EXPORT_C MultimediaSharing::TMusAvailabilityStatus CMusManager::AvailabilityL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::AvailabilityL()" );
    MultimediaSharing::TMusAvailabilityStatus status = iImpl->AvailabilityL();
    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::AvailabilityL()" );
    return status;
    }
    
// ---------------------------------------------------------------------------
// CMusManager::HandleCommandL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMusManager::HandleCommandL( 
								MultimediaSharing::TCommandType aCommandType )
	{
	MUS_LOG( "mus: [MUSCLI]  -> CMusManager::HandleCommandL()" );
    iImpl->HandleCommandL( aCommandType );
    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::HandleCommandL()" );	
	}
// ---------------------------------------------------------------------------
// CMusManager::HandleSipRequestL( TRequestType aChannelId )
// ---------------------------------------------------------------------------
//
EXPORT_C void CMusManager::HandleSipRequestL( TRequestType aChannelId )
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::HandleSipRequestL()" );
    iImpl->HandleSipRequestL( ( TInt ) aChannelId );
    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::HandleSipRequestL()" );
    }

// ---------------------------------------------------------------------------
// CMusManager::StartApplicationL( MultimediaSharing::TMusUseCase aUseCase )
// ---------------------------------------------------------------------------
//
EXPORT_C void CMusManager::StartApplicationL( 
                                      MultimediaSharing::TMusUseCase aUseCase )
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::StartApplicationL()" );
    iImpl->StartApplicationL( aUseCase );
    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::StartApplicationL()" );
    }

// ---------------------------------------------------------------------------
// CMusManager::StopApplicationL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CMusManager::StopApplicationL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManager::StopApplicationL()" );
    iImpl->StopApplicationL();
    MUS_LOG( "mus: [MUSCLI]  <- CMusManager::StopApplicationL()" );
    }
