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


#include "mcertpsource.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcemediastream.h"
#include "mcertpobserver.h"

#define KMceTIMERDISABLED 0


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceRtpSource::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSource* CMceRtpSource::NewL(
					   TUint aBufferLength,
					   TUint aBufferTreshold,
					   TUint32 aInactivityTimer,
					   HBufC8* aIdentity )
    {
    CMceRtpSource* self = NewLC( aBufferLength, aBufferTreshold,
    	 aInactivityTimer, aIdentity );
    CleanupStack::Pop( self );
    return self;   
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSource* CMceRtpSource::NewLC(
					   TUint aBufferLength,
					   TUint aBufferTreshold,
					   TUint32 aInactivityTimer,
					   HBufC8* /*aIdentity*/ )
    {
    CMceRtpSource* self = new (ELeave) CMceRtpSource();
    CleanupStack::PushL( self );
    self->ConstructL( aBufferLength, aBufferTreshold, 
                      aInactivityTimer, NULL );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceRtpSource::~CMceRtpSource
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSource::~CMceRtpSource()
    {
    delete iMediaBurstIndicator;
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::EnableL() 
    {
    CMceMediaSource::DoEnableL();
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::DisableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::DisableL() 
    {
    CMceMediaSource::DoDisableL();
    }


// -----------------------------------------------------------------------------
// CMceRtpSource::EnableInactivityTimerL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::EnableInactivityTimerL( TUint32 aInactivityTimer ) 
    {
    // Simulate MCE's check
    if ( iStreams[0] &&
         iStreams[0]->Session() &&
         iStreams[0]->Session()->State() != CMceSession::EIdle &&
         iStreams[0]->Session()->State() != CMceSession::EIncoming )
        {
        iInactivityTimer = aInactivityTimer;
        }
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::DisableInactivityTimer
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::DisableInactivityTimerL() 
    {
    iInactivityTimer = KMceTIMERDISABLED;
    }

// -----------------------------------------------------------------------------
// CMceRtpSource::UpdateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::UpdateL ( 
                                TUint aBufferLength,
								TUint aBufferTreshold,
								TUint32 aInactivityTimer,
								HBufC8* /*aIdentity*/ )
	{
    iInactivityTimer = aInactivityTimer;
    iBufferLength = aBufferLength;
    iBufferTreshold = aBufferTreshold;
	}

// -----------------------------------------------------------------------------
// CMceRtpSource::SendRRL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSource::SendRRL() 
    {
    __ASSERT_ALWAYS( iStreams[0] &&
                     iStreams[0]->Session() &&
                     iStreams[0]->Session()->State() != CMceSession::EIdle &&
                     iStreams[0]->Session()->State() != CMceSession::EIncoming,
                     User::Leave( KErrNotReady ) );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TUint32 CMceRtpSource::Ssrc() const
    {
    return 0;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C const RArray<TUint>& CMceRtpSource::Ssrcs() const
    {
    return iSsrcs;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceRtpSource::EnableSsrcL( TUint /*aSsrc*/ )
    {
    }

	
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//		
EXPORT_C void CMceRtpSource::DisableSsrcL( TUint /*aSsrc*/ )
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TBool CMceRtpSource::IsSsrcEnabledL( TUint /*aSsrc*/ )
    {
    return ETrue;
    }
		
	
// -----------------------------------------------------------------------------
// CMceRtpSource::CMceRtpSource
// -----------------------------------------------------------------------------
//
CMceRtpSource::CMceRtpSource()
   : CMceMediaSource()
    {
    iType = KMceRTPSource;
    }


// -----------------------------------------------------------------------------
// CMceRtpSource::ConstructL
// -----------------------------------------------------------------------------
//
void CMceRtpSource::ConstructL( 
                         TUint aBufferLength,
                         TUint aBufferTreshold,
                         TUint32 aInactivityTimer,
                         HBufC8* aMediaBurstIndicator )
    {
  
    iInactivityTimer = aInactivityTimer;
    iBufferLength = aBufferLength;
    iBufferTreshold = aBufferTreshold;
    iMediaBurstIndicator = aMediaBurstIndicator;
    
    }

