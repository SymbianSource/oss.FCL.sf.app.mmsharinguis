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
* Description:   This class just checks whether videosharing sip profile exist
*
*/



#include "musavasipprofileavailability.h"
#include "musavaavailabilityobserver.h"
#include "musunittesting.h"
#include "musavaobserver.h"
#include "musavaavailability.h"
#include "musavasharedobject.h"
#include "muslogger.h"
#include "musavasip.h"
#include <e32base.h>



// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
CMusAvaSipprofileAvailability* CMusAvaSipprofileAvailability::NewL(
    MMusAvaAvailabilityObserver& aObserver)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipprofileAvailability::NewL " )
    CMusAvaSipprofileAvailability* self =
        CMusAvaSipprofileAvailability::NewLC(
            aObserver);
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipprofileAvailability::NewL " )
    return self;
    }


// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaSipprofileAvailability* CMusAvaSipprofileAvailability::NewLC(
    MMusAvaAvailabilityObserver& aObserver)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipprofileAvailability::NewLC " )
    CMusAvaSipprofileAvailability* self =
        new (ELeave) CMusAvaSipprofileAvailability(
            aObserver);
    CleanupStack::PushL (self);
    self->ConstructL();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipprofileAvailability::NewLC " )
    return self;
    }


// ---------------------------------------------------------------------------
//  Destructor.
// ---------------------------------------------------------------------------
//
CMusAvaSipprofileAvailability::~CMusAvaSipprofileAvailability()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipprofileAvailability::\
                                            ~CMusAvaSipprofileAvailability " )
    if( iSharedObj )iSharedObj->DeleteSingleton();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipprofileAvailability::\
                                            ~CMusAvaSipprofileAvailability " )
    
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMusAvaSipprofileAvailability::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipprofileAvailability::ConstructL " )
    iSharedObj = CMusAvaSharedObject::GetSingletonL();    
    SetProfileState();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipprofileAvailability::ConstructL " )
    }


// -------------------------------------------------------------------------
//  Constructor.
// -------------------------------------------------------------------------
//
CMusAvaSipprofileAvailability::CMusAvaSipprofileAvailability(
    MMusAvaAvailabilityObserver& aObserver)
    :CMusAvaAvailability( aObserver )
    {
    }


// -------------------------------------------------------------------------
// Executes for the master - salve pattern.
// -------------------------------------------------------------------------
//
void CMusAvaSipprofileAvailability::DoExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipprofileAvailability::DoExecuteL " )
    SetProfileState();
    SetState( iState );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipprofileAvailability::DoExecuteL " )
    }


// -------------------------------------------------------------------------
// Stop the execution.
// -------------------------------------------------------------------------
//
void CMusAvaSipprofileAvailability::Stop()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipprofileAvailability::Stop" )    
    SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipprofileAvailability::Stop" )
    }


// -------------------------------------------------------------------------
// CMusAvaSipprofileAvailability::Name
//
// -------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaSipprofileAvailability::Name()
    {
    return MMusAvaObserver::EMusAvaNameSipprofileExist;
    }

// -------------------------------------------------------------------------
//  CMusAvaSipprofileAvailability::IsProfileExist.
// -------------------------------------------------------------------------
//
TBool CMusAvaSipprofileAvailability::IsProfileExist()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipprofileAvailability::IsProfileExist ")    
    TBool bExist = EFalse ; 
    if ( iSharedObj->MusAvaSip().CreateSipProfile() )
        {
        TUint32 profileId;
        TInt err = iSharedObj->MusAvaSip().SipProfileId( profileId );
        if( err == KErrNotFound )
        	{
        	MUS_LOG( "mus: [MUSAVA] No Sip profile " )        
        	bExist = EFalse ;	
        	}
    	else
	        {
	        MUS_LOG( "mus: [MUSAVA] Fetch profile " )        
	        bExist = ETrue ;
	        }
        }
    MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaSipprofileAvailability::IsProfileExist\
    = %d",bExist) 
    return bExist;
    }


// -------------------------------------------------------------------------
// CMusAvaSipprofileAvailability::SetProfileState
//
// -------------------------------------------------------------------------
//
void CMusAvaSipprofileAvailability::SetProfileState()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipprofileAvailability::SetProfileState ")     
    if ( IsProfileExist() )
        {
        iState = MMusAvaObserver::EMusAvaStatusAvailable;        
        }
    else
        {
        iState = MMusAvaObserver::EMusAvaStatusNoSipProfile ;
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipprofileAvailability::SetProfileState ")     
    }
