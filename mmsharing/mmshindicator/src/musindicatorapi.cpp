/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MusIndicatorApi provides means to show notes and indicate
*                availability of videosharing to user.
*
*/



#include "musindicatorapi.h"
#include "musresourcefinderutil.h"
#include "muslogger.h"
#include "musindicatordsa.h"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "mussoundplayer.h"
#include "musresourceutil.h"
#include <musindicator.rsg>
#include <coreapplicationuisdomainpskeys.h>


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusIndicatorApi* CMusIndicatorApi::NewL( MMusIndicatorObserver& aObserver )
    {
    CMusIndicatorApi* self = CMusIndicatorApi::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusIndicatorApi* CMusIndicatorApi::NewLC( MMusIndicatorObserver& aObserver )
    {
    CMusIndicatorApi* self = new (ELeave) CMusIndicatorApi( &aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusIndicatorApi* CMusIndicatorApi::NewL()
    {
    CMusIndicatorApi* self = new (ELeave) CMusIndicatorApi( NULL );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusIndicatorApi::~CMusIndicatorApi()
    {
    MUS_LOG( "mus: [MUSIND ]  ->  MusIndicatorApi::~CMusIndicatorApi" );
    
    Cancel();
    delete iIndicatorWindow;
    delete iSoundPlayer;
    if ( !IsSubscriber() )
        {
        Indicator( EFalse );
        }
    
    iProperty.Close();

    MUS_LOG( "mus: [MUSIND ]  <-  MusIndicatorApi::~CMusIndicatorApi" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusIndicatorApi::IsSubscriber() const
    {
    return !iObserver;
    }


// -----------------------------------------------------------------------------
// Indicates VS availability to user.
// In operator specific variant this can include DSA note and an audio tone
// played with CMdaAudioPlayerUtility.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusIndicatorApi::IndicateAvailabilityL()
    {
    MUS_LOG( "mus: [MUSIND]  -> CMusIndicatorApi::IndicateAvailabilityL" )
    __ASSERT_ALWAYS( !IsSubscriber(), User::Leave( KErrArgument ) );
    
    Indicator( ETrue );

    if( MultimediaSharingSettings::AuditoryNotificationSettingL() ==
        MusSettingsKeys::EAuditoryNotificationOn )
        {
        MUS_LOG( "mus: [MUSIND]     Play tone" )
        PlayToneL();
        }

    MUS_LOG( "mus: [MUSIND]  <- CMusIndicatorApi::IndicateAvailabilityL" )
    }


// -----------------------------------------------------------------------------
// From CActive.
// Handles completion of an outstanding asynchronous request.
// -----------------------------------------------------------------------------
//
void CMusIndicatorApi::RunL()
    {
    MUS_LOG( "mus: [MUSIND]  <- CMusIndicatorApi::RunL" )
    
    if ( !IsSubscriber() )
        {
        MUS_LOG( "mus: [MUSIND ] : publisher" )
        StartLiveSharingL();
        }
    else
        {
        MUS_LOG( "mus: [MUSIND ] : subscriber" )
        ToggleIndicatorL();
        }

    MUS_LOG( "mus: [MUSIND]  -> CMusIndicatorApi::RunL" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusIndicatorApi::StartLiveSharingL()
    {
    MUS_LOG( "mus: [MUSIND]  -> CMusIndicatorApi::StartLiveSharingL" )

    if( iStatus.Int() != KErrNotFound ) // eq. Cancel -button
        {
        if ( iObserver )
            {
            iObserver->StartLiveSharingL();
            }
        }

    MUS_LOG( "mus: [MUSIND]  <- CMusIndicatorApi::StartLiveSharingL" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusIndicatorApi::ToggleIndicatorL()
    {
    MUS_LOG( "mus: [MUSIND]  -> CMusIndicatorApi::ToggleIndicatorL" )
    TInt val;
    

    
    User::LeaveIfError( RProperty::Get( KPSUidCoreApplicationUIs,
                                        KCoreAppUIsVideoSharingIndicator,
                                        val ) );
    iProperty.Subscribe( iStatus );
    SetActive();
    
    TBool on = ( val == ECoreAppUIsVideoSharingIndicatorOn );
    
    if ( on && !iIndicatorWindow 
         && MultimediaSharingSettings::OperatorVariantSettingL() ==
         MusSettingsKeys::EOperatorSpecific )
        {
        iIndicatorWindow = CMusIndicatorDsa::NewL();
        }
    else if ( !on )
        {
        delete iIndicatorWindow;
        iIndicatorWindow = NULL;
        }
    else
        {
        //NOP
        }
    
    MUS_LOG( "mus: [MUSIND]  <- CMusIndicatorApi::ToggleIndicatorL" )
    }


// -----------------------------------------------------------------------------
// From CActive.
// Cancels an outstanding asynchronous request.
// -----------------------------------------------------------------------------
//
void CMusIndicatorApi::DoCancel()
    {
    MUS_LOG( "mus: [MUSIND]  <- CMusIndicatorApi::DoCancel" )
    
    if ( IsSubscriber() )
        {
        iProperty.Cancel();
        }
    MUS_LOG( "mus: [MUSIND]  -> CMusIndicatorApi::DoCancel" )
    }


// -----------------------------------------------------------------------------
// From CActive.
// Handles a leave occurring in the request completion event handler RunL.
// -----------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CMusIndicatorApi::RunError( TInt aError )
#else
TInt CMusIndicatorApi::RunError( TInt )
#endif
    {
    MUS_LOG1( "mus: [MUSIND]  -> CMusIndicatorApi::RunError(%d)", aError )

    MUS_LOG( "mus: [MUSIND]  <- CMusIndicatorApi::RunError(%d)" )
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// From MMusSoundPlayerObserver.
// Indicates completion of sound playback.
// -----------------------------------------------------------------------------
//
#ifdef _DEBUG
void CMusIndicatorApi::PlaySoundComplete( TInt aError )
#else
void CMusIndicatorApi::PlaySoundComplete( TInt /* aError */ )
#endif
    {
    MUS_LOG1( "mus: [MUSIND ] -> MusIndicatorApi::PlaySoundComplete(%d)", aError )
    delete iSoundPlayer;
    iSoundPlayer = NULL;
    MUS_LOG( "mus: [MUSIND ] <- MusIndicatorApi::PlaySoundComplete" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusIndicatorApi::CMusIndicatorApi( MMusIndicatorObserver* aObserver )
    : CActive( EPriorityStandard ),
    iObserver( aObserver )
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusIndicatorApi::ConstructL()
    {
    MUS_LOG( "mus: [MUSIND ]  ->  MusIndicatorApi::ConstructL" )
    
    if ( IsSubscriber() )
        {
        MUS_LOG( "mus: [MUSIND ] :  subscriber (called by aiwprovider)" )

        User::LeaveIfError( iProperty.Attach( KPSUidCoreApplicationUIs,
                                              KCoreAppUIsVideoSharingIndicator ) );
        CActiveScheduler::Add( this );
        iProperty.Subscribe( iStatus );
        SetActive();
        }
    else
        {
        MUS_LOG( "mus: [MUSIND ] : publisher (called by manager)" )
        CActiveScheduler::Add( this );
        }
    MUS_LOG( "mus: [MUSIND ]  <-  MusIndicatorApi::ConstructL" )
    }


// -----------------------------------------------------------------------------
// Indicating availability of VS.
// -----------------------------------------------------------------------------
//
void CMusIndicatorApi::Indicator( TBool aShown )
	{
    MUS_LOG1( "mus: [MUSIND ]  <-  MusIndicatorApi::Indicator( %d)" , aShown )
    
	RProperty property;
    TInt error( KErrNone );

    error = property.Attach( KPSUidCoreApplicationUIs, KCoreAppUIsVideoSharingIndicator );
    if ( error == KErrNone )
        {
        if ( aShown )
            {
            MUS_LOG( "mus: [MUSIND ] : indicator ON" )
            
            property.Set( ECoreAppUIsVideoSharingIndicatorOn );
            }
        else
            {
            MUS_LOG( "mus: [MUSIND ] : indicator OFF" )
            property.Set( ECoreAppUIsVideoSharingIndicatorOff );
            }
        }
    property.Close();
    MUS_LOG( "mus: [MUSIND ]  <-  MusIndicatorApi::Indicator" )
    
	}

// -----------------------------------------------------------------------------
// Plays tone indicating availability of VS.
// -----------------------------------------------------------------------------
//
void CMusIndicatorApi::PlayToneL()
    {    
    MUS_LOG( "mus: [MUSIND ] -> MusIndicatorApi::PlayToneL" )
    delete iSoundPlayer;
    iSoundPlayer = NULL;
    iSoundPlayer = CMusSoundPlayer::NewL( *this );
    MUS_LOG( "mus: [MUSIND ]    MusIndicatorApi::PlayToneL - Play sound" )
    iSoundPlayer->PlayL( CMusSoundPlayer::EVsAvailableSound );
    MUS_LOG( "mus: [MUSIND ] <- MusIndicatorApi::PlayToneL" )
    }


