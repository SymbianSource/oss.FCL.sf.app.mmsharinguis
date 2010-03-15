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


// USER INCLUDES
#include "musengsession.h"
#include "musengsessionobserver.h"
#include "musengtelephoneutils.h"
#include "muslogger.h"

// SYSTEM INCLUDES

#include <e32cmn.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngSession::~CMusEngSession() // destruct - virtual, so no export
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngSession::~CMusEngSession()" )
    delete iTelephoneUtils;
    MUS_LOG( "mus: [ENGINE]  <- CMusEngSession::~CMusEngSession()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CMusEngSession::Rect() const
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngSession::Rect()" )
    return iRect;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::SetRectL( const TRect& aRect )
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngSession::SetRect()" )
    iRect = aRect;
    RectChangedL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngSession::AudioRoutingCanBeChanged()
    {
    return iTelephoneUtils->AudioRoutingCanBeChanged();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::EnableLoudspeakerL( TBool aEnable,
                                                  TBool aShowDialog )
    {
    TRAPD( error, iTelephoneUtils->LoudspeakerL( aEnable, aShowDialog ) )

    if ( error != KErrNone )
        {
        // This is writed for emulator use
        if ( error != KErrNotSupported )
            {
            MUS_LOG1( "mus: [ENGINE]     CMusEngSession::EnableLoudspeakerL() Leave: #%d",
                      error)
            User::Leave( error );
            }
        // Not leave in emulator if KErrNotSupported
        #if (!defined (__WINS__) && !defined(__WINSCW__))
        User::Leave( error );
        #endif
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngSession::IsLoudSpeakerEnabled() const
    {
    return iTelephoneUtils->IsLoudSpeakerEnabled();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngSession::VolumeL() const
    {
    return iTelephoneUtils->GetVolume();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::MuteMicL( TBool aMute )
    {
    iTelephoneUtils->MuteMicL( aMute );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngSession::IsMicMutedL()
    {
    return iTelephoneUtils->IsMicMutedL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::SetAudioRoutingObserver( 
                                    MMusEngAudioRoutingObserver* aObserver )
    {
    iTelephoneUtils->SetAudioRoutingObserver( aObserver );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::SetVolumeChangeObserver( 
                                   MMusEngVolumeChangeObserver* aObserver )
    {
    iVolumeObserver = aObserver;
    }


// -----------------------------------------------------------------------------
// Increases CS call volume level by one.
// Leaves if error occurs when accessing central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::VolumeUpL( )
    {
    TInt currentVolume = iTelephoneUtils->GetVolume();
    iTelephoneUtils->SetVolumeL( currentVolume  + 1 );
    }


// -----------------------------------------------------------------------------
// Decreases CS call volume level by one.
// Leaves if error occurs when accessing central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::VolumeDownL( )
    {
    TInt currentVolume = iTelephoneUtils->GetVolume();
    iTelephoneUtils->SetVolumeL( currentVolume - 1 );
    }

// -----------------------------------------------------------------------------
// Set CS call volume value
// Leaves if error occurs when accessing central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::SetVolumeL( TInt aVal )
    {
    iTelephoneUtils->SetVolumeL( aVal );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngSession::CMusEngSession( const TRect& aRect )
    :iRect( aRect )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngSession::ConstructL() // second-phase constructor
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngSession::ConstructL()" )
    iTelephoneUtils = CMusEngTelephoneUtils::NewL();
    iTelephoneUtils->SetVolumeChangeObserver( this );
    MUS_LOG( "mus: [ENGINE]  <- CMusEngSession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngSession::VolumeChanged( TInt aVolume, TBool aAudioRouteChanged )
    {
    if ( iVolumeObserver )         
        {
        iVolumeObserver->VolumeChanged( aVolume, aAudioRouteChanged );
        }
    }
