/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Stub class of CMusAvaInterface to support unit testing.
*
*/


#include "cmusavainterfacestub.h"
#include "musavaavailabilitystub.h"
#include "mmusavasettingsstub.h"
#include "musavaavailability.h"
#include "musavaavailabilityobserverimp.h"

const MMusAvaObserver::TAvailabilityName KMinimumAvailability
    = MMusAvaObserver::EMusAvaBearerStatus;

const MMusAvaObserver::TAvailabilityName KMandatoryAvailability
    = MMusAvaObserver::EMusAvaNameRegistration;

const MMusAvaObserver::TAvailabilityName KExtensionAvailability
    = MMusAvaObserver::EMusAvaOptionHandler;
    
// ======== MEMBER FUNCTIONS ========


CMusAvaInterface2* CMusAvaInterface2::NewL()
	    {
	    CMusAvaInterface2* self = new( ELeave ) CMusAvaInterface2();
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop( self );
	    return self;
	    }


CMusAvaInterface2::~CMusAvaInterface2()
	    {
	    iAvailabilities.ResetAndDestroy();
	    iAvailabilities.Close();
	    delete iSettings;
	    delete iMusAvailObs;
	    }


CMusAvaInterface2::CMusAvaInterface2()
    {
    }


void CMusAvaInterface2::ConstructL()
    {
    iSettings = new(ELeave)CMusAvaSettings2();
    iSettings->ConstructL();
    // Create network availability stub
    iMusAvailObs = CMusAvaAvailabilityObserverImp::NewL();
    CMusAvaAvailabilityStub* stub = CMusAvaAvailabilityStub::NewLC( *iMusAvailObs, *iSettings );
    iAvailabilities.AppendL( stub );
    CleanupStack::Pop( stub );
    }


// ---------------------------------------------------------------------------
// Requests the implementation to provide availabilites.
// ---------------------------------------------------------------------------
//
void CMusAvaInterface2::Availability()
    {

    }


// ---------------------------------------------------------------------------
// Called by the ECOM framework when MusAvailability Plug-in is request
// retuns setting interface for the client.
// ---------------------------------------------------------------------------
//
MMusAvaSettings& CMusAvaInterface2::Settings()
    {
    return *iSettings;
    }


// ---------------------------------------------------------------------------
// Called by the ECOM framework when MusAvailability Plug-in is for
// setting Observer interface for the client.
// ---------------------------------------------------------------------------
//
void CMusAvaInterface2::SetObserver( MMusAvaObserver& aObserver )
    {
    iObserver = &aObserver;
    }


// ---------------------------------------------------------------------------
// Called by the ECOM framework when MusAvailability Plug-in is request
// to investigate availabilites for the client.
// ---------------------------------------------------------------------------
//
void CMusAvaInterface2::StartL()
    {
   
    }


// ---------------------------------------------------------------------------
// Called by the ECOM framework when MusAvailability Plug-in is request
// to stop investigate or monitoring availabilites for the client.
// ---------------------------------------------------------------------------
//
void CMusAvaInterface2::Stop()
    {
    }


// -----------------------------------------------------------------------------
//  tbd
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaInterface2::CurrentAvailability()
    {
    return iCurrentAvailability;
    }


// -----------------------------------------------------------------------------
//  tbd
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityStatus CMusAvaInterface2::AvailabilityStatus()
    {
    return iAvailabilityStatus;
    }


// -----------------------------------------------------------------------------
//  tbd
// -----------------------------------------------------------------------------
//
TBool CMusAvaInterface2::MinimumAvailability()
    {
    return iCurrentAvailability > KMinimumAvailability;
    }


// -----------------------------------------------------------------------------
//  tbd
// -----------------------------------------------------------------------------
//
TBool CMusAvaInterface2::MandatoryAvailability()
    {
    return iCurrentAvailability > KMandatoryAvailability;
    }


// -----------------------------------------------------------------------------
//  tbd
// -----------------------------------------------------------------------------
//
TBool CMusAvaInterface2::ExtensionAvailability()
    {
    return iCurrentAvailability > KExtensionAvailability;
    }


// -----------------------------------------------------------------------------
//  tbd
// -----------------------------------------------------------------------------
//
TBool CMusAvaInterface2::Available(
    MMusAvaObserver::TAvailabilityName aAvailability )
    {
    return iCurrentAvailability > aAvailability
           || ( iCurrentAvailability == aAvailability
           && MMusAvaObserver::EMusAvaStatusAvailable == iAvailabilityStatus );
    }


// -----------------------------------------------------------------------------
//  tbd
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityStatus CMusAvaInterface2::AvailabilityState(
    MMusAvaObserver::TAvailabilityName aAvailability )
    {
    for ( TInt i = 0; i < iAvailabilities.Count(); i++ )
        {
        if ( iAvailabilities[i]->Name() == aAvailability )
            {
            return iAvailabilities[i]->State();
            }
        }
    return MMusAvaObserver::EMusAvaStatusNotExecuted;
    }
    
void CMusAvaInterface2::AvailabilityChanged(
        TAvailabilityName aName,
        TAvailabilityStatus aStatus )
	{
		
	}

void CMusAvaInterface2::AvailabilityError(
        TAvailabilityName aName,
        TAvailabilityStatus aStatus )
	{
		
	}
	
void CMusAvaInterface2::SetSettingsObserver( MMusAvaSettingsObserver& aObserver )
    {
    }

// ======== GLOBAL FUNCTIONS ========


CImplementationInformation::CImplementationInformation()
    {

    }

CImplementationInformation::~CImplementationInformation()
    {

    }


CImplementationInformation* CImplementationInformation::NewL(
    TUid	/*aUid*/,
	TInt	/*aVersion*/,
	HBufC*  /*aName*/,
	HBufC8* /*aDataType*/,
	HBufC8* /*aOpaqueData*/,
	TDriveUnit /*aDrive*/,
	TBool /*aRomOnly*/,
	TBool /*aRomBased*/)
    {
    return new (ELeave) CImplementationInformation();
    }

void REComSession::DestroyedImplementation( TUid )
    {

    }

void REComSession::ListImplementationsL(
    TUid /*aInterfaceUid*/,
    RImplInfoPtrArray &aImplInfoArray )
    {
    TUid uid = { 0x10101010 };
    TDriveUnit drive;
    HBufC* name( NULL );
    HBufC8* type( NULL );

    CImplementationInformation* info = 
        CImplementationInformation::NewL( uid,
                                          1,
                                          name,
                                          type,
                                          type,
                                          drive,
                                          EFalse,
                                          EFalse );
    CleanupStack::PushL( info );
    
    aImplInfoArray.AppendL( info );

    CleanupStack::Pop( info );
    
    }


TAny* REComSession::CreateImplementationL(
    TUid /*aImplementationUid*/,
    TInt32 /*aDtorIDKey*/ )
    {
    return CMusAvaInterface2::NewL();
    }
    

    


