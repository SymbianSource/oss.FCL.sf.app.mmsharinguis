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
* Description:  Application's UI class.
*
*/



#include "musuiclipsharingcontroller.h"
#include "musuicallbackservice.h"
#include "musuieventobserver.h"
#include "musuisharingobserver.h"
#include "musuisendobserver.h"
#include "musuiclipsharingobserver.h"
#include "musuidialogutil.h"
#include "musengclipsession.h"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "musuiresourcehandler.h"
#include "musui.hrh"
#include "muslogger.h" // debug logging
#include "musuiactivetimer.h"
#include "musuigeneralview.h"

#include <musui.rsg>
#include <StringLoader.h>
#include <AknUtils.h>

#include <badesca.h>
#include <avkon.hrh>

// CONSTANTS
const TInt KMusTimeToCloseAfterClipEndedStandard  = 60 * 1000000; // 60 seconds
const TInt KMusTimeToCloseAfterClipEndedOpVariant =  1 * 1000000; //  1 second

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiClipSharingController* CMusUiClipSharingController::NewL(
                                MMusUiEventObserver& aEventObserver,
                                MMusUiSharingObserver& aSharingObserver,
        	                    MMusUiSendObserver& aSendObserver,
                                MMusUiClipSharingObserver& aClipObserver,
                                const TRect& aRect )
    {
    CMusUiClipSharingController* self =
            new( ELeave ) CMusUiClipSharingController( aEventObserver,
                                                       aSharingObserver,
                                                       aSendObserver,
                                                       aClipObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiClipSharingController::~CMusUiClipSharingController()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::~CMusUiClipSharingController" );
    delete iCloseTimer;
    DeleteEngineSession();
    delete iFileName;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::~CMusUiClipSharingController" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiClipSharingController::CMusUiClipSharingController(
                                MMusUiEventObserver& aEventObserver,
                                MMusUiSharingObserver& aSharingObserver,
                                MMusUiSendObserver& aSendObserver,
                                MMusUiClipSharingObserver& aClipObserver )
    :CMusUiSendController( aEventObserver, aSharingObserver, aSendObserver ),
    iClipObserver( aClipObserver )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::ConstructL( const TRect& aRect )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::ConstructL" );
    
    CMusUiSendController::ConstructL(); // Base class construction
                            
    iRect = aRect;  
    iSession = CMusEngClipSession::NewL( iRect,
                                         *this,
                                         *this,
                                         *this,
                                         iSipProfileId );
    iSession->SetAudioRoutingObserver( this );
    iSession->SetVolumeChangeObserver( this );
    
    if ( iSession->AudioRoutingCanBeChanged() && !iSession->IsAudioRoutingHeadset() )
        {
        iSession->EnableLoudspeakerL( ELoudspeakerEnabled, ETrue );
        }
    
    if ( iVideoCodec && iVideoCodec->Length() > 0 )
        {
        iSession->SetSupportedVideoCodecListL( *iVideoCodec );
        }
    
   	iCallbackService->AsyncEventL( EMusUiAsyncStartMediaGallery );
   	
   	iCloseTimer = CMusUiActiveTimer::NewL( this );
   	
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::HandleAsyncEventL( TMusUiAsyncEvent aEventId )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::HandleAsyncEventL" );
    switch ( aEventId )
        {
        case EMusUiAsyncStartMediaGallery:
            {
            iTranscode = EFalse;
            iTranscodingGoing = EFalse;
            
            if ( !iSession )
                {
                iSession = CMusEngClipSession::NewL( iRect, 
                                                     *this, 
                                                     *this, 
                                                     *this, 
                                                     iSipProfileId );
                iSession->SetAudioRoutingObserver( this );
                }
                        
            // Do fetching guarded. If registration completes during fetching,
            // there is no need to do anything.
            iFileFetchingOngoing = ETrue;
            iClipObserver.RunFetchDialogL();
            iFileFetchingOngoing = EFalse;
            
            TRAPD( error,
                   iSession->SetClipL( *iFileName ) )
            MUS_LOG_TDESC( "mus: [MUSUI ]     File name: ",
                           iFileName->Des() )
            if ( error == KErrNone)
                {
				if ( iSession->AudioRoutingCanBeChanged() )
                    {
                    iSession->EnableLoudspeakerL( ELoudspeakerEnabled, ETrue );
                    }
                iClipEnded = EFalse;                   
                StartInvitationL();	
                }
            else if ( error == KErrPermissionDenied )
                {
                MusUiDialogUtil::ShowInformationDialogL( 
                                        R_MUS_VIEW_DRM_INFO_SEND_FORBID );
                DeleteEngineSession();
                delete iFileName;
                iFileName = NULL;
 				iSession = CMusEngClipSession::NewL( iRect,
                                                     *this,
                                                     *this,
                                                     *this,
                                                     iSipProfileId );
                iSession->SetAudioRoutingObserver( this );
                if ( iSession->AudioRoutingCanBeChanged() )
                    {
                    iSession->EnableLoudspeakerL( ELoudspeakerEnabled, ETrue );
                    }
                iCallbackService->AsyncEventL( EMusUiAsyncStartMediaGallery );
                }
            else
                {
                HandleExitL();
                }
                
            break;
            }
        case EMusUiAsyncStartTranscoding:
            {
            // Parse Target Filename for the transcoding result
            // and start transcoding:
            //iSession->TranscodeL( VideoFileNameL() );
            // Show progress dialog:
            //iObserver->ShowTranscodingProgressDialogL();
            iTranscodingGoing = ETrue;
            break;
            }  
        
        default:
            {
            // Not clip specific, let the base class handle
            CMusUiSendController::HandleAsyncEventL( aEventId );
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::HandleAsyncEventL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::DeleteEngineSession()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::DeleteEngineSession" );
    
    CMusUiSendController::DeleteEngineSession();
    // Cancel transcoding
    if ( iSession && iTranscodingGoing )
        {
        MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingController::DeleteEngineSession:\
                            Canceling transcoding..." );
        DismissWaitDialog();
        TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( 
                                R_MUS_VIEW_NOTE_UNABLE_TO_CONVERT );
                     iSession->CancelTranscodeL() );
        }
    if ( iSession && iSession->IsAudioRoutingLoudSpeaker() )
	    {
	    iShowDialog = ETrue; 
	    }
    delete iSession;
    iSession = NULL;    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::DeleteEngineSession" );
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceOutSession* CMusUiClipSharingController::EngineOutSession()
    {
    return iSession; // Clip session is CMusEngMceOutSession
    }


// -----------------------------------------------------------------------------
// Handles status changes specific to sending sharing types.
// Calls base class version for status changes that are not sending specific. 
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::HandleChangedStatusL(
                        TMusAvailabilityStatus aStatus )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::HandleChangedStatusL" )
   
    switch( aStatus )
        {
        case EMultimediaSharingAvailable:
            {
            MUS_LOG( "mus: [MUSUI ]     EMultimediaSharingAvailable" )
            
            if ( iSipRegistrationPending )
                {
                iSipRegistrationPending = EFalse;
            
                // If fecth dialog is still open, there is no need to do anything
                if ( !iFileFetchingOngoing )
                	{
                	iSipRegistrationPending = EFalse;
                    // Set callback service, Call StartInvitationL when wait dialog dismissed
                	TRAPD( err, iSharingObserver.SetWaitDialogCallbackL( 
                	            &iWaitDialogCallback ) );
                    // Dismiss pending wait note
            	    iSharingObserver.DismissWaitDialogL();
            	    if ( err == KErrNotReady )
            	        {
            	        StartInvitationL();	
            	        }
                	}
                }
            break;
            }   
        default:
            {
            // Not clip specific, let the base class handle
            CMusUiSendController::HandleChangedStatusL( aStatus );
            break;
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::HandleChangedStatusL" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::InviteL( const TDesC& aRecipient )
    {
    MUS_LOG_TDESC( "mus: [MUSUI ]    -> CMusUiClipSharingController::InviteL:",
                            aRecipient );

    // Registration must not be pending in this phase
    __ASSERT_ALWAYS( !iSipRegistrationPending, 
                     iEventObserver.HandleError( KErrNotReady ) );
                     
    delete iAddress;
    iAddress = NULL;
    iAddress = aRecipient.AllocL();
    
    
    TBool inviteProceeding = DoInviteL();
    if ( !inviteProceeding )
        {
        MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::InviteL, invite failed" );
        return;
        }      
    if( iTranscode )
        {
        MUS_LOG( "mus: [MUSUI ]     Starting transcode..." );
        iTranscode = EFalse;
      
        // Start transcoding
        iTranscodingGoing = ETrue;
        
        SetFileNameL( VideoFileNameL() );
        TRAPD( err, iSession->TranscodeL( *iFileName ) );
        
        if ( err != KErrNone )
            {
            MUS_LOG1( "mus: [MUSUI ]     Transcoding failed: %d", err );
            iTranscodingGoing = EFalse;
            if ( iTranscodeDueUnknownRemoteCapas )
                {
                MUS_LOG( "mus: [MUSUI ]     Retry invite without transcoding" );
                // Still retry the clip without transcoding as other end might support current codec.
                iVideoToBeSaved = EFalse;
                inviteProceeding = DoInviteL();
                }
            else if ( err == KErrNotFound )  // No Video Editor Engine
                {
                MusUiDialogUtil::ShowGlobalErrorDialogL(
                    R_MUS_LIVE_SHARING_VIEW_NOTE_TRANSCODING_NOT_SUPPORTED );
                DeleteEngineSession();
                iVideoToBeSaved = EFalse;
                HandleExitL();
                inviteProceeding = EFalse;
                }
            else
                {
                // Unable to convert clip, if transc. leaves.
                // After note return back to the media gallery.
                TranscodingFailed();
                inviteProceeding = EFalse;
                }
            }
        else
            {
            // Show progress dialog:
            iClipObserver.ShowTranscodingProgressDialogL();
            inviteProceeding = EFalse;
            }
        }
    
    if ( inviteProceeding )
        {
        SetConnectionInitialized( ETrue );
        ShowInvitingWaitDialogL();
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::InviteL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiClipSharingController::DoInviteL()
    {
    __ASSERT_ALWAYS( iAddress, User::Leave( KErrNotReady ) );
    TRAPD( err, iSession->InviteL( *iAddress ) );
    MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingController::DoInviteL: After TRAPD" );
    
    // If address is in wrong format, Manual Address Entry Dialog
    // is displayed
    if ( err != KErrNone )
        {
        DismissWaitDialog();
        MusUiDialogUtil::ShowGlobalErrorDialogL(
                          R_MUS_LIVE_SHARING_VIEW_NOTE_INVITE_ERROR );
        if ( ( ++iTriedInvitations < 2 )  && ( err == KErrArgument ) )
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiLiveSharingController::DoInviteL: iTriedInvitations < 2" );
            iManualAddressTyped = ETrue;
            iResourceHandler->RequestKeypadL( ETrue );
            
            // If the address has to be queried again...:
            iTranscode = EFalse;
            
            MUS_LOG_TDESC( "mus: [MUSUI ]     CMusUiClipSharingController::DoInviteL: ",
                     iFileName->Des() )
                                               
            iSendObserver.ManualAddressEntryL( *iRemoteSipAddressProposal );
            }
        else
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiLiveSharingController::DoInviteL: ++iTriedInvitations > 1" );
            DeleteEngineSession();
            HandleExitL();
            }
        }
    return ( err == KErrNone );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::HandleSliderValueChangeL( TInt aNewLevel )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiClipSharingController::HandleSliderValueChangeL(%d)",
              aNewLevel )

    __ASSERT_ALWAYS( !ExitOccured(), User::Leave( KErrDied ) );
    __ASSERT_ALWAYS( iConnectionEstablished, User::Leave( KErrNotReady ) );
        
    iSession->SetPositionL( TTimeIntervalSeconds( aNewLevel ) );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::HandleSliderValueChangeL" )
    }



void CMusUiClipSharingController::OfferToolbarEventL( TInt aCommand )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiClipSharingController::OfferToolbarEventL [%d]",
                            aCommand );
                                                    
    switch ( aCommand )
        {
        case EMusuiCmdToolbarPause:
            {
            HandleCommandL( EMusuiCmdViewPause );
            iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarPause,
                                                    EMusuiCmdToolbarUnPause,
                                                    ETrue );
            break;
            }
        case EMusuiCmdToolbarUnPause:
            {
            HandleCommandL( EMusuiCmdViewContinue );
            iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarUnPause,
                                                    EMusuiCmdToolbarPause,
                                                    ETrue );
            break;
            }
        case EMusuiCmdToolbarFFRev:
            {
            iToolbarFFRevSelected = !iToolbarFFRevSelected; 
            iClipObserver.SetDurationValue( iSession->DurationL().Int() );
            if ( iToolbarFFRevSelected )
                {
                iWasPlayingBeforeWinding = iSession->IsPlayingL();
                PauseL();
                }
            else
                {
                StopWindingL();
                if ( iWasPlayingBeforeWinding )
                    {
                    PlayL();
                    }
                }
//            iClipObserver.SetDurationVisible();
            iSharingObserver.HighlightSelectedToolbarItem( EMusuiCmdToolbarFFRev );
            break;
            }
        default:    // Not clip specific, let the base class handle
            {
            CMusUiEventController::OfferToolbarEventL( aCommand );
            break;
            }    
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::OfferToolbarEventL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::HandleCommandL(TInt aCommand)
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::HandleCommandL" );
    
    __ASSERT_ALWAYS( !ExitOccured(), User::Leave( KErrDied ) );

    switch ( aCommand )
        {
        case EMusuiCmdViewPause:
            {
            PauseL();
            iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPause );
            break;
            }
        case EMusuiCmdViewContinue:
            {
            PlayL();
            iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPlay );
            break;
            }

        case EMusuiCmdViewReplay:
            {
            if( !MUS_NO_TOOLBAR )
                {
    			if (!AknLayoutUtils::PenEnabled())
    				{
    				iEventObserver.SetToolbarVisibility( ETrue );
            		}
    
                if ( ClipEnded() )
                    {
                    iEventObserver.SetToolbarVisibility( ETrue );
                    // Simulate press of play in toolbar
                    OfferToolbarEventL( EMusuiCmdToolbarUnPause );             
                    }
                }
            break;
            }

        case EMusuiCmdViewShareVideo:
            {
            PauseL();
            // TODO: Change to video sharing
            break;
            }
        case EMusuiCmdViewShareImage:
            {
            PauseL();
            // TODO: Change to image sharing
            break;
            }
        case EMusuiCmdViewShareLive:
            {
            // TODO: Change to live sharing
            break;
            }

        case EMusuiCmdViewAudioNormal:
            {
            if( !MUS_NO_TOOLBAR )
                {
    			if (!AknLayoutUtils::PenEnabled())
    				{
    				iEventObserver.SetToolbarVisibility( ETrue );
            		}
                }
            iSession->UnmuteL();
            break;
            }
        case EMusuiCmdViewAudioMute:
            {
			if (!AknLayoutUtils::PenEnabled())
				{
				iEventObserver.SetToolbarVisibility( ETrue );
        		}

            iSession->MuteL();
            break;
            }
        case EMusuiCmdViewFastForward:
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingController::HandleCommandL: EMusuiCmdViewFastForward" );
            if ( DoFastForwardingL( ETrue ) )
                {
                iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconForward );
                iClipObserver.SetPositionValueL( iSession->PositionL().Int() );
                iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarPause,
                		                                EMusuiCmdToolbarUnPause,
                                                        ETrue );
                //iClipObserver.SetDurationVisible();
                }
            MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingController::HandleCommandL: EMusuiCmdViewFastForward END" );
            break;
            }
        case EMusuiCmdViewFastReverse:
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingController::HandleCommandL: EMusuiCmdViewFastReverse" );
            if ( DoFastRewindingL( ETrue ) )
                {
                iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconRew );
                iClipObserver.SetPositionValueL( iSession->PositionL().Int() );
                iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarPause,
                		                                EMusuiCmdToolbarUnPause,
                                                        ETrue );
                //iClipObserver.SetDurationVisible();
                }
            MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingController::HandleCommandL: EMusuiCmdViewFastReverse END" );
            break;
            }
        case EMusuiCmdViewFastForwardEnd:
            {
            if ( DoFastForwardingL( EFalse ) )
                {
                iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPause );
                PauseL();
                iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarUnPause,
                		                                EMusuiCmdToolbarPause,
                                                        ETrue );
                }
            break;
            }
        case EMusuiCmdViewFastReverseEnd:
            {
            if ( DoFastRewindingL( EFalse ) )
                {
                iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPause );
                PauseL();
                iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarUnPause,
                		                                EMusuiCmdToolbarPause,
                                                        ETrue );
                }
            break;
            }
        default:
            {
            // Try more general handling
            CMusUiEventController::HandleCommandL( aCommand );
            break;
            }
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::HandleCommandL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::SetFileNameL( const TDesC& aFileName )
    {
    iFileName = aFileName.AllocL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiClipSharingController::ClipEnded()
    {
    return iClipEnded;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiClipSharingController::ClipMutedL()
    {
    return iSession->IsMutedL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiClipSharingController::ClipContainsAudioL()
    {
    if ( !iConnectionInitialized || !iSession )
        {
        return EFalse; // We cannot be sure yet
        }
        
    return iSession->ContainsAudioL();
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::CancelTranscodingL()
    {
    if ( iSession && iTranscodingGoing )
        {
        iTranscodingGoing = EFalse;
        iSession->CancelTranscodeL();
        }
    iVideoToBeSaved = EFalse;
    HandleExitL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::StopWindingL()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiClipSharingController::StopWindingL" );
    
    if ( DoFastForwardingL( EFalse ) || DoFastRewindingL( EFalse ) )
        {
        iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPause );
        }
        
    MUS_LOG( "mus: [MUSUI ] <- CMusUiClipSharingController::StopWindingL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiClipSharingController::ToolbarFFRevSelected()
    {
    return iToolbarFFRevSelected;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::TranscodingProgressedL( TInt aPercentage )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiClipSharingController::TranscodingProgressedL ( %d )", aPercentage );
    if ( aPercentage < KMusOneHundredPercent )
        {
        iTranscodingGoing = ETrue;
        iClipObserver.UpdateClipTranscodingPercentageL( aPercentage );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::TranscodingProgressedL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::TranscodingCompletedInitL()
    {
    MUS_LOG( 
        "mus: [MUSUI ]  -> CMusUiClipSharingController::TranscodingCompletedInitL" );
    if ( iTranscodingGoing )
        {
        iVideoToBeSaved = ETrue;
        iTranscodingGoing = EFalse;
        iClipObserver.CancelTranscodingDialogL();
        MusUiDialogUtil::ShowInformationDialogL( R_MUS_VIEW_NOTE_TRANSCODING_COMPLETE );
        }
    MUS_LOG( 
        "mus: [MUSUI ]  <- CMusUiClipSharingController::TranscodingCompletedInitL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::TranscodingCompletedFinalizeL()
    {
    MUS_LOG( 
        "mus: [MUSUI ]  -> CMusUiClipSharingController::TranscodingCompletedFinalizeL" );
    SetConnectionInitialized( ETrue );
    if ( !iTranscodingGoing )
        {
        ShowInvitingWaitDialogL();
        }
    MUS_LOG( 
        "mus: [MUSUI ]  <- CMusUiClipSharingController::TranscodingCompletedFinalizeL" );
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusUiClipSharingController::FormattedDurationString(
                            const TTimeIntervalSeconds& aDurationSeconds,
                            const TTimeIntervalSeconds& aPositionSeconds )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::FormattedDurationString" );
    _LIT( KDurationAndPositionSeparator, "/" );

    iDurationStringBuffer.SetLength( 0 );
    TLocale locale;
    _LIT( KZeroSymbol, "0" );

    TInt durationMinutes( 0 );
    TInt durationSeconds( aDurationSeconds.Int() );
    if ( aDurationSeconds.Int() > ( KMusOneMinuteInSeconds - 1 ) )
        {
        durationMinutes = (int)( aDurationSeconds.Int() / KMusOneMinuteInSeconds );
        durationSeconds = durationSeconds - ( durationMinutes * KMusOneMinuteInSeconds );
        }

    iDurationStringBuffer.AppendNum( durationMinutes );
    iDurationStringBuffer.Append( locale.TimeSeparator( KMusLocaleMinutesAndSecondsSeparator ) );
    if ( durationSeconds < KMusNumberValueTen )
        {
        iDurationStringBuffer.Append( KZeroSymbol );
        }
    iDurationStringBuffer.AppendNum( durationSeconds );
    iDurationStringBuffer.Append( KDurationAndPositionSeparator );

    TInt positionMinutes( 0 );
    TInt positionSeconds( aPositionSeconds.Int() );
    if ( aPositionSeconds.Int() > ( KMusOneMinuteInSeconds - 1 ) )
        {
        positionMinutes = (int)( aPositionSeconds.Int() / KMusOneMinuteInSeconds );
        positionSeconds = positionSeconds - ( positionMinutes * KMusOneMinuteInSeconds );
        }

    iDurationStringBuffer.AppendNum( positionMinutes );
    iDurationStringBuffer.Append( locale.TimeSeparator( KMusLocaleMinutesAndSecondsSeparator ) );
    if ( positionSeconds < KMusNumberValueTen )
        {
        iDurationStringBuffer.Append( KZeroSymbol );
        }
    iDurationStringBuffer.AppendNum( positionSeconds );


    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::FormattedDurationString" );
    return iDurationStringBuffer;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::EndOfClip()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::EndOfClip" );
    iClipEnded = ETrue;
    
        TRAP_IGNORE( iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconStop ) );
        if(!MUS_NO_TOOLBAR)
            {
            iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarPause,
                                                    EMusuiCmdToolbarUnPause,
                                                    ETrue );
            }
    iCloseTimer->Cancel();
    iCloseTimer->After( iOperatorSpecificFunctionality ?
                                   KMusTimeToCloseAfterClipEndedOpVariant : 
                                   KMusTimeToCloseAfterClipEndedStandard );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::EndOfClip" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::TranscodingNeeded(TBool aDueUnknownRemoteCapabilities)
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiClipSharingController::TranscodingNeeded, %d", 
               aDueUnknownRemoteCapabilities);
    iTranscode = ETrue;
    iTranscodeDueUnknownRemoteCapas = aDueUnknownRemoteCapabilities;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::TranscodingNeeded" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::TranscodingProgressed( TInt aPercentage )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiClipSharingController::TranscodingProgressed %d",
                            aPercentage );
    TRAP_IGNORE( TranscodingProgressedL( aPercentage ) )
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::TranscodingProgressed" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::TranscodingCompletedInit()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::TranscodingCompleted" );
    TRAPD( error, TranscodingCompletedInitL() );
    if ( error != KErrNone )
        {
        HandleError( error );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::TranscodingCompleted" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::TranscodingCompletedFinalize()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::TranscodingCompleted" );
    TRAPD( error, TranscodingCompletedFinalizeL() );
    if ( error != KErrNone )
        {
        HandleError( error );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::TranscodingCompleted" );
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::TranscodingFailed()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::TranscodingFailed" );
    iTranscodingGoing = EFalse;
    DismissWaitDialog();
    TRAP_IGNORE( iClipObserver.CancelTranscodingDialogL();
                 MusUiDialogUtil::ShowGlobalErrorModalDialogL( 
                                    R_MUS_VIEW_NOTE_UNABLE_TO_CONVERT ) );
    DeleteEngineSession();
    delete iFileName;
    iFileName = NULL;
    TRAPD( error,
           iCallbackService->AsyncEventL( EMusUiAsyncStartMediaGallery ) );
    if ( error != KErrNone )
        {
        HandleError( error );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::TranscodingFailed" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::SessionRecipientNotFound()
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiClipSharingController::SessionRecipientNotFound [%d]", iTriedInvitations );
    
    // If the address has to be queried again...:
    iTranscode = EFalse;    
    
    CMusUiSendController::SessionRecipientNotFound();
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::SessionRecipientNotFound" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::SessionTerminated()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::SessionTerminated" );
    DismissWaitDialog();
    if ( iTranscodingGoing )
        {
        TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( 
                                R_MUS_VIEW_NOTE_UNABLE_TO_CONVERT ) );
        }
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::SessionTerminated" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::SessionConnectionLost()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::SessionConnectionLost" );
    DismissWaitDialog();
    TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( 
                            R_MUS_LIVE_SHARING_VIEW_NOTE_CONNECTION_LOST ) );
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::SessionConnectionLost" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::SessionFailed()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::SessionFailed" );
    DismissWaitDialog();
    if ( iTranscodingGoing )
        {
        TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( 
                            R_MUS_VIEW_NOTE_UNABLE_TO_CONVERT ) );
        }
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::SessionFailed" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::StreamIdle()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::StreamIdle" );
    iClipObserver.InvalidVideoFrame( ETrue );
    TRAPD( error, StreamIdleL() );
    if ( error != KErrNone )
        {
        iEventObserver.HandleError( error );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::StreamIdle" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::StreamStreaming()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::StreamStreaming" );
    iClipObserver.InvalidVideoFrame( EFalse );
    TRAPD( error, StreamStreamingL() );
    if ( error != KErrNone )
        {
        iEventObserver.HandleError( error );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::StreamStreaming" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::SessionTimeChanged(
                        const TTimeIntervalSeconds& aSeconds )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::SessionTimeChanged" );
    TRAP_IGNORE( SessionTimeChangedL( aSeconds ) );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::SessionTimeChanged" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::InactivityTimeout()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::InactivityTimeout" );
    DismissWaitDialog();
    TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( 
                            R_MUS_LIVE_SHARING_VIEW_NOTE_SERVICE_N_A ) );
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::InactivityTimeout" );
    }


// -----------------------------------------------------------------------------
// Called when predefined time elapsed 
// after the clip had reached the end
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::TimerComplete( CMusUiActiveTimer* /*aTimer*/ )
    {
    MUS_LOG( "mus: [MUSUI ]  Close time elapsed from clip ending => \
             closing session" );
    TRAP_IGNORE( HandleExitL() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::StreamStreamingL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::StreamStreamingL" );
    
    if ( ConnectionEstablished() )
        {
        iClipEnded = EFalse;
        iCloseTimer->Cancel();    
        iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPlay );
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::StreamStreamingL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::StreamIdleL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::StreamIdleL" );
    
    if ( ConnectionEstablished() )
        {    
        iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPause );
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::StreamIdleL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::SessionTimeChangedL( const TTimeIntervalSeconds& aSeconds )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::SessionTimeChangedL" );
    if ( aSeconds.Int() > -1 )
        {
        TTimeIntervalSeconds clipDuration = iSession->DurationL();
        TTimeIntervalSeconds clipPosition = iSession->PositionL();
                
        iSharingObserver.UpdateSessionTime( SessionTimeFormatted( aSeconds ) );
        iClipObserver.UpdateClipPositionL( clipPosition.Int(),
                                          FormattedDurationString(  
                                                clipPosition, 
                                                clipDuration ) );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::SessionTimeChangedL" );
    }


// -----------------------------------------------------------------------------
// CMusUiClipSharingController::SessionEstablished()
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingController::SessionEstablished()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::SessionEstablished" );
    
    TRAPD( error, iResourceHandler->RequestVideoPlayerL( ETrue ) );
    if ( error != KErrNone )
        {
        MUS_LOG1( "mus: [MUSUI ]     RequestVideoPlayerL leave code: %d", error )
        }
    
    // Duration setting visible
    TRAP( error, iClipObserver.SetDurationValue( iSession->DurationL().Int() ) )
    if ( error != KErrNone )
        {
        HandleError( error );
        }
    iClipObserver.SetDurationVisible();

    // General handling
    CMusUiSendController::SessionEstablished(); // Base class handling    
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::SessionEstablished" );
    }


// -----------------------------------------------------------------------------
// CMusUiClipSharingController::DoFastForwardingL()
// -----------------------------------------------------------------------------
//
TBool CMusUiClipSharingController::DoFastForwardingL( TBool aUseWinding )
    {
    if( iFastForward ^ aUseWinding ) 
        {
        iSession->FastForwardL( aUseWinding );
        iFastForward = aUseWinding;
        return ETrue;
        }
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CMusUiClipSharingController::DoFastRewindingL()
// -----------------------------------------------------------------------------
//    
TBool CMusUiClipSharingController::DoFastRewindingL( TBool aUseWinding )
    {
    if( iFastRewind ^ aUseWinding ) 
        {
        iSession->FastRewindL( aUseWinding );
        iFastRewind = aUseWinding;
        return ETrue;
        }
    return EFalse;
    }
    

// -----------------------------------------------------------------------------
// CMusUiClipSharingController::RewindFromEndL()
// -----------------------------------------------------------------------------
//    
void CMusUiClipSharingController::RewindFromEndL()
	{
	MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingController::RewindFromEndL" );
    PauseL();
    iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPause );
    iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarPause,
                                            EMusuiCmdToolbarUnPause,
                                            ETrue );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingController::RewindFromEndL" );
	}
// End of file

