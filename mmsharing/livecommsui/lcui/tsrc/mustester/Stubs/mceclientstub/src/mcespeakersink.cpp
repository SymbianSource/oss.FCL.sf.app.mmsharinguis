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


#include "mcespeakersink.h"
#include "mcesession.h"
#include "mcemediastream.h"


// ============================ MEMBER FUNCTIONS ===============================

const TInt KMceMaxVolume = 10;

// -----------------------------------------------------------------------------
// CMceSpeakerSink::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSpeakerSink* CMceSpeakerSink::NewL()
    {
    CMceSpeakerSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSpeakerSink* CMceSpeakerSink::NewLC()
    {
    CMceSpeakerSink* self = new (ELeave) CMceSpeakerSink();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::~CMceSpeakerSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSpeakerSink::~CMceSpeakerSink()
    {
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSpeakerSink::EnableL() 
    {
    CMceMediaSink::DoEnableL();
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::Disable
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSpeakerSink::DisableL() 
    {
    CMceMediaSink::DoDisableL();
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::SetVolumeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSpeakerSink::SetVolumeL(TInt aVolume)
	{
	__ASSERT_ALWAYS( iStream &&
                     iStream->Session() &&
                     iStream->Session()->State() != CMceSession::EIdle &&
                     iStream->Session()->State() != CMceSession::EIncoming,
                     User::Leave( KErrNotReady ) );
                     	
	__ASSERT_ALWAYS( aVolume <= MaxVolumeL() &&
	                 aVolume > 0, 
	                 User::Leave( KErrArgument ) );
    iVolume = aVolume; 
	}

// -----------------------------------------------------------------------------
// CMceSpeakerSink::VolumeL
// -----------------------------------------------------------------------------
//		        
EXPORT_C TInt CMceSpeakerSink::VolumeL() const
	{
	__ASSERT_ALWAYS( iStream &&
                     iStream->Session() &&
                     iStream->Session()->State() != CMceSession::EIdle &&
                     iStream->Session()->State() != CMceSession::EIncoming,
                     User::Leave( KErrNotReady ) );
	
	return iVolume;
	}

// -----------------------------------------------------------------------------
// CMceSpeakerSink::MaxVolumeL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceSpeakerSink::MaxVolumeL() const
	{
	__ASSERT_ALWAYS( iStream &&
                     iStream->Session() &&
                     iStream->Session()->State() != CMceSession::EIdle &&
                     iStream->Session()->State() != CMceSession::EIncoming,
                     User::Leave( KErrNotReady ) );
                     	
	return iMaxVolume;
	}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSpeakerSink::SetRoutingL( TInt aRoute )
    {
    __ASSERT_ALWAYS( iStream &&
                     iStream->Session() &&
                     iStream->Session()->State() != CMceSession::EIdle &&
                     iStream->Session()->State() != CMceSession::EIncoming,
                     User::Leave( KErrNotReady ) );
                     
    iAudioRouting = aRoute;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceSpeakerSink::RoutingL() const
    {
    __ASSERT_ALWAYS( iStream &&
                     iStream->Session() &&
                     iStream->Session()->State() != CMceSession::EIdle &&
                     iStream->Session()->State() != CMceSession::EIncoming,
                     User::Leave( KErrNotReady ) );

    return iAudioRouting;
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::CMceSpeakerSink
// -----------------------------------------------------------------------------
//
CMceSpeakerSink::CMceSpeakerSink()
    {
    iType = KMceSpeakerSink;
    iMaxVolume = KMceMaxVolume;
    }

// -----------------------------------------------------------------------------
// CMceSpeakerSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSpeakerSink::ConstructL()
    {
    }
    

