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
*  Version     : %version:  75.1.10 % << Don't touch! Updated by Synergy at check-out.
*
*/



#include "musuireceivecontroller.h"
#include "musuieventobserver.h"
#include "musuisharingobserver.h"
#include "musuireceiveobserver.h"
#include "musuicallbackservice.h"
#include "musuiactivetimer.h"
#include "musuidialogutil.h"
#include "musuicontactutil.h"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "musuiresourcehandler.h"
#include "musengreceivesession.h"
#include "musenguriparser.h"
#include "mussessionproperties.h"
#include "muslogger.h" // debug logging
#include "musuiappui.h"
#include "musuireceiveview.h"
#include "musuipropertywatch.h"
#include "mussesseioninformationapi.h"
#include "musui.hrh"
#include <musui.rsg>

#include <avkon.hrh>
#include <aknappui.h>           
#include <StringLoader.h>

using namespace MusSettingsKeys;
using namespace NMusSessionApi;

const TInt KMusMinDigitCountInValidTelNumber = 7;

/*
 * Private number.
*/
_LIT( KPrivateNumber, "" );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiReceiveController* CMusUiReceiveController::NewL(
                        MMusUiEventObserver& aEventObserver,
                        MMusUiSharingObserver& aSharingObserver,
                        MMusUiReceiveObserver& aReceiveObserver,
                        const TRect& aRect )
    {
    CMusUiReceiveController* self = new( ELeave ) CMusUiReceiveController( 
                                                        aEventObserver,
                                                        aSharingObserver, 
                                                        aReceiveObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiReceiveController::~CMusUiReceiveController()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::~CMusUiReceiveController" );
    delete iSession;
    delete iOriginator;
    delete iTimer;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::~CMusUiReceiveController" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiReceiveController::CMusUiReceiveController(
                                MMusUiEventObserver& aEventObserver,
                                MMusUiSharingObserver& aSharingObserver,
                                MMusUiReceiveObserver& aReceiveObserver )
    : CMusUiEventController( aEventObserver, aSharingObserver ), 
      iReceiveObserver( aReceiveObserver )
    {

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::ConstructL( const TRect& aRect )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::ConstructL" );

    // construct event listener
    CMusUiEventController::ConstructL();

    // construct an engine session
    iSession = CMusEngReceiveSession::NewL( aRect, *this, *this );
    iSession->SetAudioRoutingObserver( this );
        
    iTimer = CMusUiActiveTimer::NewL( this );
    iTimer->After( KMusReceiveTimeout );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::ConstructL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::ChangeFullScreenModeL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::ChangeFullScreenModeL" );

    iFullScreenSelected = !iFullScreenSelected;
    
    if ( !MUS_NO_TOOLBAR )
        {
        iSharingObserver.HighlightSelectedToolbarItem( EMusuiCmdToolbarFullScreen );
        }
        
    iReceiveObserver.SetFullScreenL( iFullScreenSelected );

    if ( ( iEventObserver.AppOrientation()!= CAknAppUiBase::EAppUiOrientationLandscape ) 
            && iFullScreenSelected )
        {
        if( !iEventObserver.OrientationCanBeChanged() ) 
            {
            MUS_LOG( "mus: [MUSUI ]    !OrientationCanBeChanged()" );
            iSession->SetOrientationL( CMusEngMceSession::ELandscape );
            }
        }
    else if ( ( iSession->OrientationL() != iOriginalMceOrientation ) 
                            && !iFullScreenSelected )
        {
        if( !iEventObserver.OrientationCanBeChanged() ) 
            {
            MUS_LOG( "mus: [MUSUI ]    !OrientationCanBeChanged()" );
            iSession->SetOrientationL( iOriginalMceOrientation );
            }
        }    

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::ChangeFullScreenModeL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::OfferToolbarEventL( TInt aCommand )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiReceiveController::OfferToolbarEventL [%d]",
                            aCommand );
    
    switch ( aCommand )
        {
        case EMusuiCmdToolbarFullScreen:
            {
            ChangeFullScreenModeL();
            break;
            }
        default:    // Not receive specific, let the base class handle
            {
            CMusUiEventController::OfferToolbarEventL( aCommand );
            break;
            }
            
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::OfferToolbarEventL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::HandleCommandL( TInt aCommand )
    {
    MUS_LOG1( "mus: [MUSUI ] ->  CMusUiReceiveController::HandleCommandL: %d",
                            aCommand );

    switch ( aCommand )
        {
        // Menu commands from view's HandleCommandL:
        case EMusuiCmdViewAudioNormal:
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiReceiveController::ConstructL:\
                                    EMusuiCmdViewAudioNormal" );
            if ( iSession->ContainsAudioL() )
                {
                iSession->UnmuteL();
                }
            break;
            }
        case EMusuiCmdViewAudioMute:
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiReceiveController::ConstructL:\
                                    EMusuiCmdViewAudioMute" );
            if ( iSession->ContainsAudioL() )
                {
                iSession->MuteL();
                }
            break;
            }
        case EMusuiCmdToolbarFullScreen:
            {
            OfferToolbarEventL( EMusuiCmdToolbarFullScreen );
            break;
            }            
        case EMusuiCmdViewFullScreen:
            {
            ChangeFullScreenModeL();
            break;
            }
        default:
            {
            // Try more general handling
            CMusUiEventController::HandleCommandL( aCommand );
            break;
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::HandleCommandL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::HandleIncomingSessionL( 
                                            const TDesC& aOriginator,
                                            const TDesC& aOriginatorIdentity )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::HandleIncomingSessionL" )
    
    delete iOriginator;
    iOriginator = NULL;
    TInt privacyStatus(0);
    
    
    MUS_LOG( "mus: [MUSUI ]  Check private number" )
    TRAPD( errorPrivacyStatus, privacyStatus = 
            CMusUiPropertyWatch::ReadIntPropertyL( NMusSessionApi::KPrivacyStatus ) );        
    if ( errorPrivacyStatus != KErrNone )
        {
       	MUS_LOG1( "mus: [MUSUI ]     ReadIntPropertyL leave code: %d", errorPrivacyStatus )
        HandleError( errorPrivacyStatus );
        }
        
    if ( privacyStatus == 1 )
      {
       MUS_LOG( "mus: [MUSUI ]  Call is from private number" )
       iOriginator = KPrivateNumber().AllocL();
      }
    else if( iOperatorSpecificFunctionality )
        {
        iOriginator = aOriginatorIdentity.AllocL();	
        }
    else
      {
      iOriginator = aOriginator.AllocL();
      }
         
    // Cancel the 10 seconds timer:
    iTimer->Cancel();
    
    // Display the query dialog:
    
    if ( privacyStatus == 1 )
        {
        iReceiveObserver.ShowInvitationQueryL( *iOriginator );
        }  
    else if ( iOperatorSpecificFunctionality && aOriginatorIdentity.Length() != 0 )
        {
        // We compare last seven digits, since that is the minimum amount
        // that can make up a valid telephone number.
        MUS_LOG_TDESC( "mus: [MUSUI ]       Right7 of MusTelNumberValue(): ",
                       MusTelNumberValue().Right( KMusMinDigitCountInValidTelNumber ) )
        MUS_LOG_TDESC( "mus: [MUSUI ]       Right7 of aOriginatorIdentity: ",
                       aOriginatorIdentity.Right( KMusMinDigitCountInValidTelNumber ) )
        MUS_LOG_TDESC( "mus: [MUSUI ]       MusContactName: ", MusContactName() )                            
            
        if  ( !( MusTelNumberValue().Length() >= 
              						KMusMinDigitCountInValidTelNumber && 
              aOriginatorIdentity.Length() >= 
              						KMusMinDigitCountInValidTelNumber &&
              MusTelNumberValue().Right( KMusMinDigitCountInValidTelNumber ) ==
              aOriginatorIdentity.Right( KMusMinDigitCountInValidTelNumber ) &&
              MusContactName().Length() > 0 ) )
            {
            // In some very rare scenario a valid session may come from MSISDN
        	// that is not the same as the one received from manager. To avoid
        	// showing of wrong MSISDN, we replace phone number with one 
        	// received from P-Asserted-Identity header. Since in this scenario 
        	// we cannot be sure about contact name either, we replace it with 
        	// phone number. Replacing of contact name must happen also when we 
        	// have no name for contact.
        	delete iTelNumber;
        	iTelNumber = NULL;
        	iTelNumber = aOriginatorIdentity.AllocL();
        	
        	delete iContactName;
        	iContactName = NULL;
        	iContactName = aOriginatorIdentity.AllocL();
            }
            
        iReceiveObserver.ShowInvitationQueryL( MusContactName() );
        
        }
    else
        {
       	if( MusUiContactUtil::CheckPbContactForVSAddressesL(
       	            *( iEventObserver.EikonEnv() ), *iOriginator, iContactId ) )
        	{
        	if ( MusContactName().Length() < 1 )
        	    { // originator is defined in Contacts but the name is empty
        	    iReceiveObserver.ShowInvitationQueryL( *iOriginator );
        	    }
        	else
        	    {
        	    iReceiveObserver.ShowInvitationQueryL( MusContactName() );
        	    }
       		}
       	else
       	    {
        	iReceiveObserver.ShowInvitationQueryL( *iOriginator );
        	}        
        }
   
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::HandleIncomingSessionL" )
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiReceiveController::ClipMutedL()
    {
    return iSession->IsMutedL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiReceiveController::ClipContainsAudioL()
    {
    if ( !ConnectionInitialized() )
        {
        return EFalse; // We cannot be sure yet
        }
        
    return iSession->ContainsAudioL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
CMusEngMceSession* CMusUiReceiveController::EngineSession()
    {
    return iSession; // CMusEngReceiveSession is CMusEngMceSession
    }
    

// -----------------------------------------------------------------------------
// Virtual function from CMusUiEventController
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::HandleAsyncEventL( TMusUiAsyncEvent aEventId )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::HandleAsyncEventL" );
    switch ( aEventId )
        {
        case EMusUiAsyncInviteAccepted:
            {
            ShowWaitDialogL();
            break;
            }
        default:
            {
            // Not receive specific, let the base class handle
            CMusUiEventController::HandleAsyncEventL( aEventId );
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::HandleAsyncEventL" );
    }


// -----------------------------------------------------------------------------
// Virtual function from CMusUiEventController
// -----------------------------------------------------------------------------
//
const TDesC& CMusUiReceiveController::TypedAddress() const
    {
    return *iOriginator;
    }
    

// -----------------------------------------------------------------------------
// Virtual function from CMusUiEventController
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::DeleteEngineSession()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::DeleteEngineSession" );
    delete iSession;
    iSession = NULL;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::DeleteEngineSession" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiReceiveController::FullScreenSelected()
    {
    return iFullScreenSelected;
    }


// -----------------------------------------------------------------------------
// From MMusEngReceiveSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::IncomingSessionPreNotification()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::IncomingSessionPreNotification" );
    iTimer->Cancel();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::IncomingSessionPreNotification" );
    }


// -----------------------------------------------------------------------------
// From MMusEngReceiveSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::IncomingSession( const TDesC& aOriginator, 
                                               const TDesC& aOriginatorIdentity )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::IncomingSession" );
    TRAPD( err, HandleIncomingSessionL( aOriginator, aOriginatorIdentity ) );
    if ( err != KErrNone )
        {
        iEventObserver.HandleError( err );
        }
    else
        {
        SetConnectionInitialized( ETrue );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::IncomingSession" );
    }


// -----------------------------------------------------------------------------
// From MMusEngReceiveSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::StreamBuffering()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::StreamBuffering" );
    if ( iStreamPaused )
        {
        MUS_LOG( "mus: [MUSUI ]  Stream was paused, not show buffering state" );
        }
    else
        {
        TRAPD( err, iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconWaitAnim ) );
        if ( err != KErrNone )
            {
            iEventObserver.HandleError( err );
            }        
        }

    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::StreamBuffering" );
    }


// -----------------------------------------------------------------------------
// From MMusEngSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::SessionEstablished()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::SessionEstablished" );
    
    // Common handling first
    CMusUiEventController::SessionEstablished();

    TRAPD( error, SessionEstablishedL() )

    if ( error != KErrNone )
        {
        iEventObserver.HandleError( error );
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::SessionEstablished" );
    }


// -----------------------------------------------------------------------------
// From MMusEngSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::SessionTerminated()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::SessionTerminated" );
    if ( !ConnectionEstablished() )
        {
        iReceiveObserver.DismissInvitationQuery( ETrue );
        TRAP_IGNORE( MusUiDialogUtil::ShowInformationDialogL(
                        R_MUS_LIVE_SHARING_VIEW_NOTE_INVITATION_CANCELLED ) )
        iEventObserver.HandleExit();
        }
    else
        {
        TRAP_IGNORE( iSharingObserver.DismissWaitDialogL() );
        
        TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
            
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::SessionTerminated" );
    }


// -----------------------------------------------------------------------------
// From MMusEngSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::SessionConnectionLost()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::SessionConnectionLost" );
    TRAPD( err,
           iSharingObserver.DismissWaitDialogL();
           MusUiDialogUtil::ShowGlobalErrorDialogL( 
                                R_MUS_LIVE_SHARING_VIEW_NOTE_CONNECTION_LOST ) );
    if ( err != KErrNone )
        {
        iEventObserver.HandleError( err );
        }
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );   

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::SessionConnectionLost" );
    }


// -----------------------------------------------------------------------------
// From MMusEngSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::SessionFailed()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::SessionFailed" );
    TRAPD( error, iSharingObserver.DismissWaitDialogL() );
    if ( error != KErrNone )
        {
        iEventObserver.HandleError( error );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::SessionFailed" );
    }


// -----------------------------------------------------------------------------
// From MMusEngSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::StreamIdle()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::StreamIdle" );
    iStreamPaused = ETrue;
    TRAPD( err, iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPause ) );
    if ( err != KErrNone )
        {
        iEventObserver.HandleError( err );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::StreamIdle" );
    }


// -----------------------------------------------------------------------------
// From MMusEngSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::StreamStreaming()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::StreamStreaming" );
    
    if ( ConnectionEstablished() )
        {
        TRAP_IGNORE( iSharingObserver.DismissWaitDialogL() )
        TRAPD( err, iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPlay ) )
        
        if ( err != KErrNone )
            {
            iEventObserver.HandleError( err );
            }
        iStreamPaused = EFalse;    
        }
   
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::StreamStreaming" );
    }


// -----------------------------------------------------------------------------
// From MMusEngSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::SessionTimeChanged(
                        const TTimeIntervalSeconds& aSeconds )
    {
    if ( aSeconds.Int() > -1 )
        {
        iSharingObserver.UpdateSessionTime( SessionTimeFormatted( aSeconds ) );
        }
    }


// -----------------------------------------------------------------------------
// From MMusEngSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::InactivityTimeout()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::InactivityTimeout" );
    TRAP_IGNORE( iSharingObserver.DismissWaitDialogL();
                 MusUiDialogUtil::ShowGlobalErrorDialogL( 
                            R_MUS_LIVE_SHARING_VIEW_NOTE_SERVICE_N_A ) );
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::InactivityTimeout" );
    }


// -----------------------------------------------------------------------------
// From MMusUiInvitationQueryObserver: Invitation accepted by the user
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::InvitationAcceptedL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::InvitationAcceptedL" );
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    // Enable loud speaker already before answering but disable it immediately
    // if accepting fails for some reason
    if ( iSession->AudioRoutingCanBeChanged() )
        {
        iSession->EnableLoudspeakerL( ELoudspeakerEnabled, ETrue );
        }
            
    TRAPD( err, iSession->AcceptInvitationL( ETrue ) );
    if ( err )
        {
        if ( iSession->AudioRoutingCanBeChanged() )
            {
            iSession->EnableLoudspeakerL( ELoudspeakerDisabled, ETrue );
            }
        User::Leave( err );
        }
          
    iCallbackService->AsyncEventL( EMusUiAsyncInviteAccepted );
    
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::InvitationAcceptedL" );
    }


// -----------------------------------------------------------------------------
// From MMusUiInvitationQueryObserver: Invitation rejected by the user
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::InvitationRejectedL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::InvitationRejectedL" );
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    iSession->AcceptInvitationL( EFalse );
    HandleExitL();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::InvitationRejectedL" );
    }


// -----------------------------------------------------------------------------
// From MMusUiActiveTimerObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::TimerComplete( CMusUiActiveTimer* /*aTimer*/ )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::TimerComplete" );
    iEventObserver.HandleExit();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::TimerComplete" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::ExitProcedureL( TBool aUserAcceptance )
    {
    
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::ExitProcedureL" );
    
    switch ( iShutdownState )
        {
        case EMusUiShutdownVideoSaved:
            {
            MUS_LOG( "mus: [MUSUI ]     ExitProcedureL: EMusuiShutdownVideoSaved" );
            
            // Check if our address is valid sip or tel uri. If not, there is no
            // reason to query user about permission to save it to contacts.
            
            TInt error = KErrNotFound;
            if ( iOriginator )
                {
                TMusEngUriParser parser( *iOriginator );
                TRAP( error, parser.ParseUriL() )                
                }

            // Save Address to Contacts query if contact found and
            // there is no address in contacts 
            // and if is the standard variant.
            if ( !iOperatorSpecificFunctionality && iContactId > 0 && 
                 !MusUiContactUtil::ShareViewFieldExistsL( 
                            *( iEventObserver.EikonEnv() ), iContactId ) &&
                 error == KErrNone )
                {
                iShutdownState = EMusUiShutdownContactSavingQueried;
                iEventObserver.SetToolbarVisibility( EFalse );
                SaveContactQueryL();
                }
            else
                {
                // Nothing receive specific, let the base class handle
                CMusUiEventController::ExitProcedureL( aUserAcceptance );
                }
            break;
            }

        default:
            {
            // Nothing receive specific, let the base class handle
            CMusUiEventController::ExitProcedureL( aUserAcceptance );
            break;
            }
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::ExitProcedureL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::ShowWaitDialogL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::ShowWaitDialogL" );
    
    delete iDialogPrompt;
    iDialogPrompt = NULL;
        
    iDialogPrompt = StringLoader::LoadL( 
                            R_MUS_RECEIVE_VIEW_WAITING_FOR_STREAM,
                            ( MusContactName().Length() > 0 ) ?
                                                    MusContactName() :
                                                    *iOriginator );
    
    if ( !iSharingObserver.ShowWaitDialogL( 
                            *iDialogPrompt, 
                            KMusInviteExpireTimeout,
                            R_MUS_LIVE_SHARING_VIEW_NOTE_TIMEOUT_EXPIRED ) )
        {
        HandleExitL();
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::ShowWaitDialogL" );
    }


// -----------------------------------------------------------------------------
// From MMusEngSessionObserver
// -----------------------------------------------------------------------------
//
void CMusUiReceiveController::SessionEstablishedL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveController::SessionEstablishedL" );

    iConnectionEstablished = ETrue;
    iSharingObserver.DismissWaitDialogL();

    if ( iCallbackService->CurrentEvent() == EMusUiAsyncInviteAccepted )
        {
        iCallbackService->Cancel();
        }
    
    // Orientation for video player
    iResourceHandler->RequestVideoPlayerL( ETrue );

    iOriginalMceOrientation = iSession->OrientationL();

    // set the mce orientation if different than videoplayer orientation
    CAknAppUiBase::TAppUiOrientation currentOrientation = 
        STATIC_CAST(CMusUiReceiveView&,iReceiveObserver).MusAppUi()->Orientation();

    if( (iOriginalMceOrientation == CMusEngMceSession::EPortrait && 
        currentOrientation== CAknAppUiBase::EAppUiOrientationPortrait ) ||
        (iOriginalMceOrientation == CMusEngMceSession::ELandscape && 
        currentOrientation== CAknAppUiBase::EAppUiOrientationLandscape ))
        {
        MUS_LOG( "mus: [MUSUI ]  -> MCE & Videoplayer orientation is same" );
        // Both MCE and Videoplayer has same orientation.
        }
    else
        {        
        if(currentOrientation == CAknAppUiBase::EAppUiOrientationPortrait)
            {
            MUS_LOG( "mus: [MUSUI ]  -> Setting MCE Orientation to Portrait" );
            iSession->SetOrientationL(CMusEngMceSession::EPortrait);    
            }
        else if(currentOrientation == CAknAppUiBase::EAppUiOrientationLandscape)
            {
            MUS_LOG( "mus: [MUSUI ]  -> Setting MCE Orientation to Landscape" );
            iSession->SetOrientationL(CMusEngMceSession::ELandscape); 
            }        
        else
            {
            MUS_LOG( "mus: [MUSUI ]  -> Undefined Orientation. " );
            }
        }       

    if ( iForeground )
        {
        EnableDisplayL( ETrue );
        iEventObserver.SetToolbarVisibility( ETrue );
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveController::SessionEstablishedL" );
    }


// End of file

