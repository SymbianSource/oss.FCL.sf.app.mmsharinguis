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


// USER
#include "musengtelephoneutils.h"
#include "musengaudioroutingobserver.h"
#include "musengmcesession.h"
#include "musengvolumechangeobserver.h"
#include "muslogger.h"

// SYSTEM
#include <centralrepository.h>
#include <telephonydomaincrkeys.h>
#include <e32property.h>
#include <telephonydomainpskeys.h>
#include <CPhCltCommandHandler.h> // for CPhCltCommandHandler


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTelephoneUtils* CMusEngTelephoneUtils::NewL()
    {
    CMusEngTelephoneUtils* self = new( ELeave ) CMusEngTelephoneUtils();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTelephoneUtils::~CMusEngTelephoneUtils()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::~CMusEngTelephoneUtils()" )
    
    Cancel();

    delete iPhoneCommandHandler;
    
    if( iTelephonyAudioRouting ) // If 2nd phase construction has succeeded 
        {
        CTelephonyAudioRouting::TAudioOutput currentMode =
                                            iTelephonyAudioRouting->Output();
        MUS_LOG1( "mus: [ENGINE] iAudioOutputAtStartup: %d", iAudioOutputAtStartup );
        MUS_LOG1( "mus: [ENGINE] currentMode: %d", currentMode );
        // When active call is dropped, audio output is set to ENotActive,
        // but in some cases Mush engine get deleted before OutputChanged()
        // notification comes. In that case we shouldn't touch output. 
        if( currentMode != iAudioOutputAtStartup && 
            currentMode != CTelephonyAudioRouting::ENotActive )
            {
            // As going down, let audiorouting api to show notification
            iTelephonyAudioRouting->SetShowNote( ETrue );
            TRAPD( err, DoSetOutputL( iAudioOutputAtStartup ) );
            MUS_LOG1( "mus: [ENGINE]    final route change completed: %d", err )
            err++;
            }
        }

    if ( iNotifier )
        {
        iNotifier->StopListening();
        delete iNotifier;
        }    

    delete iRepository;
    delete iTelephonyAudioRouting;
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::~CMusEngTelephoneUtils()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngTelephoneUtils::AudioRoutingCanBeChanged() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::AudioRoutingCanBeChanged" )
    
    TBool retValue = ( iTelephonyAudioRouting->Output() !=
                       CTelephonyAudioRouting::EWiredAudioAccessory &&
                       iTelephonyAudioRouting->Output() !=
                       CTelephonyAudioRouting::ETTY );
    
    MUS_LOG1( "mus: [ENGINE]  <- CMusEngTelephoneUtils::AudioRoutingCanBeChanged: %d",
              retValue )
              
    return retValue;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngTelephoneUtils::AudioOutputIsBT() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::AudioOutputIsBT" )
	    
    TBool retValue = ( iTelephonyAudioRouting->Output() ==
                       CTelephonyAudioRouting::EBTAudioAccessory );
	    
    MUS_LOG1( "mus: [ENGINE]  <- CMusEngTelephoneUtils::AudioOutputIsBT: %d",
               retValue )
	              
    return retValue;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::LoudspeakerL( TBool aEnable, TBool aShowDialog )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngTelephoneUtils::LoudspeakerL(%d)",
              aEnable )
    
    if ( aEnable )
        {
        if ( iTelephonyAudioRouting->Output() == 
             CTelephonyAudioRouting::EHandset || 
             iTelephonyAudioRouting->Output() == 
             CTelephonyAudioRouting::EBTAudioAccessory )  
            {
            // Disable note shown by audiorouting api as it causes
            // application going to background for a while. Instead, display
            // note by ourselves once setting output completes. This mechanism
            // is needed only for loudspeaker enabling as going to background
            // causes problems only at beginning of sharing session.
            if ( aShowDialog )
                {   
                iTelephonyAudioRouting->SetShowNote( EFalse );
                
                iShowDialog = aShowDialog;
                }
    
            DoSetOutputL( CTelephonyAudioRouting::ELoudspeaker );
            }
        }
    else
        {
        iTelephonyAudioRouting->SetShowNote( aShowDialog );
        
        if ( iAudioOutputAtStartup == CTelephonyAudioRouting::ELoudspeaker )
            {
            DoSetOutputL( CTelephonyAudioRouting::EHandset );
            }
        else
            {
            DoSetOutputL( iAudioOutputAtStartup );
            }
        }

    
                                
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::LoudspeakerL(...)" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngTelephoneUtils::IsLoudSpeakerEnabled() const
    {
    return ( iTelephonyAudioRouting->Output() ==
             CTelephonyAudioRouting::ELoudspeaker );
    }

// -----------------------------------------------------------------------------
// Returns locally cached the CS call volume level.
// -----------------------------------------------------------------------------
//
TInt CMusEngTelephoneUtils::GetVolume() const
    {
    return iCurrentVolume;
    }
        
// -----------------------------------------------------------------------------
// Gets the CS call volume level from central repository.
// Leaves if error occurs when accessing central repository.
// -----------------------------------------------------------------------------
//
TInt CMusEngTelephoneUtils::GetVolumeL() const
    {
    TInt currentVolume = 0;
    if ( IsLoudSpeakerEnabled() )
        {
        User::LeaveIfError( iRepository->Get( KTelIncallLoudspeakerVolume,
                                              currentVolume ) );
        }
    else
        {
        User::LeaveIfError( iRepository->Get( KTelIncallEarVolume,
                                              currentVolume ) );
        }

    return ValidateVolume( currentVolume );
    }


// -----------------------------------------------------------------------------
// Sets the CS call volume level.
// Leaves if error occurs when accessing central repository.
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::SetVolumeL( TInt aVolume )
    {
    TInt newVolume = ValidateVolume( aVolume );
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngTelephoneUtils::SetVolumeL(), %d", newVolume )
    if ( iCurrentVolume != newVolume )
        {
        if ( IsLoudSpeakerEnabled() )
            {
            User::LeaveIfError( iRepository->Set( KTelIncallLoudspeakerVolume,
                                                  newVolume ) );
            }
        else
            {
            User::LeaveIfError( iRepository->Set( KTelIncallEarVolume, 
                                                  newVolume ) );
            }
        iCurrentVolume = newVolume;
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::MuteMicL( TBool aMute )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngTelephoneUtils::MuteMicL(%d)", aMute )
    
    if ( IsActive() )
        {
        MUS_LOG( "mus: [ENGINE]     Cancel pending request" )
        Cancel();
        }
    
    iPhoneCommandHandler->MuteMicrophone( iStatus, aMute );
    SetActive();
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::MuteMicL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngTelephoneUtils::IsMicMutedL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::IsMicMutedL()" )
    
    TInt psVal;
    User::LeaveIfError( RProperty::Get( KPSUidTelMicrophoneMuteStatus,
                                        KTelMicrophoneMuteState,
                                        psVal ) );
                                        
    MUS_LOG1( "mus: [ENGINE]     Mute status in PS is (%d)", psVal )
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::IsMicMutedL()" )

    return ( psVal == EPSTelMicMuteOn );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::SetAudioRoutingObserver( 
                                    MMusEngAudioRoutingObserver* aObserver )
    {
    iAudioRoutingObserver = aObserver;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::SetVolumeChangeObserver( 
                                    MMusEngVolumeChangeObserver* aObserver )
    {
    iVolumeObserver = aObserver;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::RunL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::RunL()" )

    // NOP, since we really cannot do anything but log the result
    MUS_LOG1( "mus: [ENGINE]    Asynchronous call completed with code %d",
             iStatus.Int() )

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::RunL()" )
    }

// -------------------------------------------------------------------------
//  If RunL() leaves,It should be handled here.
// -------------------------------------------------------------------------
//
TInt CMusEngTelephoneUtils::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngTelephoneUtils::\
              RunError() return #%d", aError )
    
    // Nothing can be done here.
    aError = KErrNone;

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::RunError()" )
    return aError;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::DoCancel()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::DoCancel()" )
    
    if ( iPhoneCommandHandler )
        {
        iPhoneCommandHandler->CancelAsyncRequest( EPhCltCommandMuteMic );
        }
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::DoCancel()" )
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::AvailableOutputsChanged( 
                        CTelephonyAudioRouting& /*aTelephonyAudioRouting*/ )
    {
    // NOP
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::OutputChanged( 
                CTelephonyAudioRouting& aTelephonyAudioRouting )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::OutputChanged()" )

    // Some other application has changed audio output routing. We consider
    // this as a last will of a user and return to this state after sharing
    
    iAudioOutputAtStartup = aTelephonyAudioRouting.Output();
    
    MUS_LOG1( "mus: [ENGINE]     New audio routing is %d", iAudioOutputAtStartup )
    
    if ( iAudioRoutingObserver )
        {
        iAudioRoutingObserver->AudioRoutingChanged( EFalse );
        }
    
    UpdateCurrentVolume(ETrue);
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::OutputChanged()" )
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::SetOutputComplete( 
                    CTelephonyAudioRouting& /*aTelephonyAudioRouting*/,
                    TInt aError )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::SetOutputComplete()" )

    if ( aError == KErrNone )
        {
        if ( iAudioRoutingObserver )
            {
            // If audio routing api didn't shown note and show dialog mode is on,
            // we know that this completion is for such setoutput call for which
            // we need to show the note. Show note mode is turned off only in that
            // case.
            TBool dialogShownByUs( EFalse );
            TBool dialogShownByAudioRouting( EFalse );     
            aError = iTelephonyAudioRouting->GetShowNote( dialogShownByAudioRouting );
            if ( aError == KErrNone && !dialogShownByAudioRouting && iShowDialog )
                {
                dialogShownByUs = iShowDialog;
                iShowDialog = EFalse;
                }
        
            iAudioRoutingObserver->AudioRoutingChanged( dialogShownByUs );
            }

        UpdateCurrentVolume(ETrue);
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::SetOutputComplete()" )
    }
                            

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::UpdateCurrentVolume( TBool aAudioRouteChanged )
    {
    TInt volume(0);
    TRAPD(error, volume = GetVolumeL() );
    if( (KErrNone == error) && (iCurrentVolume != volume) )
         {
         iCurrentVolume = volume;
         if ( iVolumeObserver )
             {
             iVolumeObserver->VolumeChanged( volume, aAudioRouteChanged );
             }
         }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTelephoneUtils::CMusEngTelephoneUtils() 
	: CActive( CActive::EPriorityStandard )
    {
    iAudioOutputAtStartup = CTelephonyAudioRouting::ENotActive;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::ConstructL()" )

    // Volume control
    iRepository = CRepository::NewL( KCRUidInCallVolume );
    
    iNotifier = CCenRepNotifyHandler::NewL( *this, *iRepository );
    iNotifier->StartListeningL();
    
    // Audio routing control
    iTelephonyAudioRouting = CTelephonyAudioRouting::NewL( *this );

    iAudioOutputAtStartup = iTelephonyAudioRouting->Output();
    MUS_LOG1( "mus: [ENGINE] iAudioOutputAtStartup: %d", iAudioOutputAtStartup );
    // Phone
    MUS_LOG( "mus: [ENGINE]     Use static DLL" )
    iPhoneCommandHandler = CPhCltCommandHandler::NewL();
     
    iCurrentVolume = GetVolumeL();
    
    CActiveScheduler::Add( this );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::ConstructL()" )
    }


// -----------------------------------------------------------------------------
// Validates that requested volume level is valid (between 1-10) and if it is
// not, modifies requested volume level to be valid.  Returns validated volume.
// -----------------------------------------------------------------------------
//
TInt CMusEngTelephoneUtils::ValidateVolume( const TInt aVolume ) const
    {
    if ( aVolume < KMusEngMinVolume )
        {
        return KMusEngMinVolume;
        }
    else if ( aVolume > KMusEngMaxVolume )
        {
        return KMusEngMaxVolume;
        }
    else
        {
        // NOP, to keep PC-Lint happy
        }
        
    return aVolume;
    }


// -----------------------------------------------------------------------------
// Set output if setting is currently allowed.
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::DoSetOutputL( 
    CTelephonyAudioRouting::TAudioOutput aAudioOutput )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTelephoneUtils::DoSetOutputL()" )
    if ( iAudioRoutingObserver && !iAudioRoutingObserver->AudioRouteChangeAllowed() )
        {
        MUS_LOG( "mus: [ENGINE]     change not allowed!" )
        User::Leave( KErrAccessDenied );
        }
    iTelephonyAudioRouting->SetOutputL( aAudioOutput );
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTelephoneUtils::DoSetOutputL()" )
    }


// ---------------------------------------------------------------------------
// CMusEngTelephoneUtils::HandleNotifyGeneric
// ---------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::HandleNotifyGeneric( TUint32 aId )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngTelephoneUtils::HandleNotifyGeneric()" )
    TInt error = KErrArgument;
    TInt volume = 0;
    if ( KTelIncallEarVolume == aId )
        {
        error = iRepository->Get( KTelIncallEarVolume, volume );        
        MUS_LOG1( "mus: [ENGINE] EAR volume: %d", volume );
        }
    else if ( KTelIncallLoudspeakerVolume == aId )
        {
        error = iRepository->Get( KTelIncallLoudspeakerVolume, volume );        
        MUS_LOG1( "mus: [ENGINE] Loudspeakers volume: %d", volume );
        }
    
    volume = ValidateVolume(volume);
    if ( (KErrNone == error) && (iCurrentVolume != volume) )
        {
        MUS_LOG1( "mus: [ENGINE] volume changed: %d!, notifying UI...", volume )
        iCurrentVolume = volume;
        if( iVolumeObserver  )
            {
            iVolumeObserver->VolumeChanged( volume, EFalse );
            }
        }
    else
        {
        MUS_LOG( "mus: [ENGINE] volume hasn't changed!, do nothing..." )    
        }
        
    MUS_LOG1( "mus: [ENGINE] <- CMusEngTelephoneUtils::HandleNotifyGeneric(), error:%d",
            error );
    }
