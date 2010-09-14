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
* Description:  
*
*/



#include "musuisendcontroller.h"
#include "musuieventobserver.h"
#include "musuisharingobserver.h"
#include "musuisendobserver.h"
#include "musuidialogutil.h"
#include "musuicallbackservice.h"
#include "musuifileutil.h"
#include "mussettings.h"
#include "musuipropertywatch.h"
#include "mussessionproperties.h"
#include "mussettingskeys.h"
#include "musuiresourcehandler.h"
#include "muslogger.h" // debug logging

#include "musengmceoutsession.h"
#include "musenguriparser.h"
#include <musui.rsg>

#include <StringLoader.h>
#include <pathinfo.h>
#include <CDirectoryLocalizer.h>


using namespace NMusSessionApi;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiSendController::~CMusUiSendController()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::~CMusUiSendController" );
    
    delete iAddress;
    delete iRemoteSipAddress;
    delete iRemoteSipAddressProposal;
    TRAP_IGNORE( DeleteFileL() );  
    delete iVideoFileName;
    delete iLocalizer;
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::~CMusUiSendController" );
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession* CMusUiSendController::EngineSession()
    {
    return EngineOutSession(); // Engine out session is CMusEngMceSession
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::DeleteEngineSession()
    {
    // Cancel invite
    if ( EngineOutSession() && !ConnectionEstablished() )
        {
        MUS_LOG( "mus: [MUSUI ]     CMusUiSendController::DeleteEngineSession:\
                            Canceling inviting..." );
        TRAP_IGNORE( EngineOutSession()->CancelInviteL() );
        }
    }     
        

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::HandlePropertyError( const TInt aReason )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiEventController::HandleError [%d]",
                            aReason );
    TRAP_IGNORE( DeleteVideoL() );
    
    CMusUiEventController::HandlePropertyError( aReason ); // For the base class
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::HandleError" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::HandleAsyncEventL( TMusUiAsyncEvent aEventId )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::HandleAsyncEventL" );
    switch ( aEventId )
        {
        case EMusUiAsyncInviteCancel:
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiSendController::\
                     HandleAsyncEventL: EMusUiAsyncInviteCancel" );
            HandleExitL();
            break;
            }
        case EMusUiAsyncManualAddressEntry:
            {
            iResourceHandler->RequestKeypadL( ETrue ); // Exits on fail                    
            iSendObserver.ManualAddressEntryL( *iRemoteSipAddressProposal );
            break;
            }
        case EMusUiAsyncStartInvitation:
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiSendController::\
                     HandleAsyncEventL: EMusUiAsyncStartInvitation" );
            InviteL();	            
            break;
            }
        default:
            {
            // Not send specific, let the base class handle
            CMusUiEventController::HandleAsyncEventL( aEventId );
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::HandleAsyncEventL" );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusUiSendController::TypedAddress() const
    {
    return *iAddress;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiSendController::ManualAddressTyped() const
    {
    return iManualAddressTyped;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::StartInvitationL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::StartInvitationL" );
    
    iCallbackService->AsyncEventL( EMusUiAsyncStartInvitation );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::StartInvitationL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::InviteL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::InviteL" );
    
    if ( !iSipRegistrationPending )
        {
        TRAPD( error, 
               iRemoteSipAddress = CMusUiPropertyWatch::ReadDescPropertyL(
                                                    KRemoteSipAddress ) );
        if ( error != KErrNone )
            {
            iRemoteSipAddress = KNullDesC().AllocL();
            }

        TRAP( error, iRemoteSipAddressProposal =
                                CMusUiPropertyWatch::ReadDescPropertyL(
                                                    KRemoteSipAddressProposal ) );
        if ( error != KErrNone )
            {
            iRemoteSipAddressProposal = KNullDesC().AllocL();
            }
        
        if ( iRemoteSipAddress->Length() > 0 )
            {
            if ( iRemoteSipAddress->Find( KMusCommaSymbol ) == KErrNotFound )
                {
                InviteL( *iRemoteSipAddress );
                }
            else
                {
                // Split the addresses using KMusCommaSymbol as a separator
                CDesCArray* addressArray =
                            new ( ELeave ) CDesCArrayFlat(
                                                    KMusSipAddressesMaxAmount );
                SplitL( *iRemoteSipAddress, KMusCommaSymbol, addressArray );
                // Show List Query Dialog
                TBuf<KMusSipAddressMaxLength> selectedAddress;
                if ( MusUiDialogUtil::SelectAddressDialogL( addressArray, 
                                                            selectedAddress ) )
                    {
                    // Invite with the chosen address                    
                    InviteL( selectedAddress );
                    }
                else
                    {
                    HandleExitL();
                    }
                }
            }
        else
            {
            iResourceHandler->RequestKeypadL( ETrue ); // Exits on failure
            iManualAddressTyped = ETrue;
            iSendObserver.ManualAddressEntryL( *iRemoteSipAddressProposal );
            }
        }
    else
        {
        // Show waiting dialog
        delete iDialogPrompt;
        iDialogPrompt = NULL;
        iDialogPrompt = StringLoader::LoadL(
                                R_MUS_REGISTRATION_PENDING_WAIT_NOTE_TXT );
                       
        if ( !iSharingObserver.ShowWaitDialogL( 
                                    *iDialogPrompt, 
                                    KMusInviteExpireTimeout, //Reuse 1 min timer
                                    R_MUS_LIVE_SHARING_VIEW_NOTE_NO_REG ) )
            {
            iEventObserver.HandleExit();
            }
            
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::InviteL" );
    }    


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::CancelInvitationL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::CancelInvitationL" );
    
    if ( EngineOutSession() && !ConnectionEstablished() )
        {
        TRAP_IGNORE( EngineOutSession()->CancelInviteL() );
        }
        
    HandleExitL();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::CancelInvitationL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::HandleForegroundEventL( TBool aForeground )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::HandleForegroundEventL" );
    
    CMusUiEventController::HandleForegroundEventL( aForeground );
    
    if ( ConnectionEstablished() &&  !ExitOccured() )
        {
        if ( aForeground )
            { 
            if ( iContinuePlayingWhenForeground )
                {
                MUS_LOG( "mus: [MUSUI ]     Resume playing" );
                PlayL();
                }
            }
        else
            {
            MUS_LOG( "mus: [MUSUI ]     Pause playing" );
            if ( EngineOutSession() )
                {
                iContinuePlayingWhenForeground = IsPlayingL(); 
                }
            PauseL();
            }
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::HandleForegroundEventL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::PlayL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::PlayL" );
    if ( EngineOutSession() )
        {
        EngineOutSession()->PlayL();
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::PlayL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::PauseL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::PauseL" );
    if ( EngineOutSession() )
        {
        EngineOutSession()->PauseL();
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::PauseL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiSendController::IsPlayingL()
    {
    TBool isPlaying( EFalse );
    
    if ( EngineOutSession() )
        {
        isPlaying = EngineOutSession()->IsPlayingL();
        }
    
    return isPlaying;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionEstablished()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionEstablished" );
    
    // Common handling first
    CMusUiEventController::SessionEstablished();
    
    iConnectionEstablished = ETrue;
    DismissWaitDialog();
    
    if ( iForeground )
        {
        MUS_LOG( "mus: [MUSUI ]     Enable display and start playing" );
        TRAP_IGNORE( PlayL(); 
                     EnableDisplayL( ETrue ); )
                     
        iEventObserver.SetToolbarVisibility( ETrue );
        
        }
    else
        {
        MUS_LOG( "mus: [MUSUI ]     In background, don't start playing yet" );
        iContinuePlayingWhenForeground = ETrue;
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionEstablished" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionRejected()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionRejected" );
    DismissWaitDialog();
    TRAP_IGNORE (
            MusUiDialogUtil::ShowInformationDialogL( 
                        R_MUS_LIVE_SHARING_VIEW_NOTE_CONNECTION_REJECTED ) );
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
       
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionRejected" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionBadRequest()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionBadRequest" );
    
    DismissWaitDialog();
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionBadRequest" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionUnauthorized()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionUnauthorized" );
    
    DismissWaitDialog();
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionUnauthorized" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionPaymentRequired()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionPaymentRequired" );
    
    DismissWaitDialog();
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionPaymentRequired" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionRecipientNotFound()
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiSendController::SessionRecipientNotFound [%d]",
                            iTriedInvitations );
    DismissWaitDialog();
    if ( ExitOccured() )
        {
        return;
        }
    if ( ++iTriedInvitations > 1 )
        {
        TRAP_IGNORE( MusUiDialogUtil::ShowInformationDialogL(
                                R_MUS_LIVE_SHARING_VIEW_NOTE_WRONG_ADDRESS ) );
        
        TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncInviteCancel ) );
           
        }
    else
        {
        TRAP_IGNORE( MusUiDialogUtil::ShowInformationDialogL(
                                R_MUS_LIVE_SHARING_VIEW_NOTE_WRONG_ADDRESS ) );
        iManualAddressTyped = ETrue;
        
        TRAP_IGNORE(
            iCallbackService->AsyncEventL( EMusUiAsyncManualAddressEntry ) );
            
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionRecipientNotFound" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionProxyAuthenticationRequired()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionProxyAuthenticationRequired" );
    
    DismissWaitDialog();
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionProxyAuthenticationRequired" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionRequestTimeOut()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionRequestTimeOut" );
    DismissWaitDialog();    
    TRAP_IGNORE( MusUiDialogUtil::ShowInformationDialogL( 
                            R_MUS_LIVE_SHARING_VIEW_NOTE_NO_RESPONSE ) );
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionRequestTimeOut" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionUnsupportedMediaType()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionUnsupportedMediaType" );
    DismissWaitDialog();
    if ( !ExitOccured() )
        {
        TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( 
                                    R_MUS_LIVE_SHARING_VIEW_NOTE_APP_N_A ) );
        }
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionUnsupportedMediaType" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionBusyHere()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionBusyHere" );
    DismissWaitDialog();
    TRAP_IGNORE( 
            MusUiDialogUtil::ShowInformationDialogL( 
                               R_MUS_LIVE_SHARING_VIEW_NOTE_RECIPIENT_N_A ) );
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionBusyHere" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionRequestCancelled()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionRequestCancelled" );
    DismissWaitDialog();
    TRAP_IGNORE( 
          MusUiDialogUtil::ShowInformationDialogL( 
                        R_MUS_LIVE_SHARING_VIEW_NOTE_INVITATION_CANCELLED ) );
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionRequestCancelled" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SessionTemporarilyNotAvailable()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SessionRequestCancelled" );
    DismissWaitDialog();
    if ( !ExitOccured() )
        {
        TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( 
                            R_MUS_LIVE_SHARING_VIEW_NOTE_SERVICE_N_A ) );
        }
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::SessionRequestCancelled" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::ConstructL" );
    
    CMusUiEventController::ConstructL(); // Base class construction
    iLocalizer = CDirectoryLocalizer::NewL();
     
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::ConstructL" );   
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiSendController::CMusUiSendController(
                                    MMusUiEventObserver& aEventObserver,
                                    MMusUiSharingObserver& aSharingObserver, 
	                                MMusUiSendObserver& aSendObserver )
	: CMusUiEventController( aEventObserver, aSharingObserver ),
	  iSendObserver( aSendObserver ),
	  iTriedInvitations( 0 ),
	  iContinuePlayingWhenForeground( ETrue ),
	  iWaitDialogCallback( *this )
    {
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::ExitProcedureL( TBool aUserAcceptance )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::ExitProcedureL" );
    
    DismissWaitDialog();
    iEventObserver.SetExitingFlag();
    switch ( iShutdownState )
        {
        case EMusUiShutdownStarted:
            {
            MUS_LOG( "mus: [MUSUI ]     ExitProcedureL: EMusuiShutdownStarted" );
        
            if ( iVideoToBeSaved && 
                 iVideoFileName &&
                 MusUiFileUtil::FileExists( *iVideoFileName ) )
                {
                iShutdownState = EMusUiShutdownVideoSaved;
                ShowVideoSavedDialogL();
                ExitProcedureL( ETrue ); // Accept saving
                }
            else
                {
                CMusUiEventController::ExitProcedureL( aUserAcceptance );
                }
            break;
            }
            
        case EMusUiShutdownVideoSaved:
            {
            MUS_LOG( "mus: [MUSUI ]     ExitProcedureL: EMusuiShutdownVideoSaved" );
            
            // Check if our address is valid sip or tel uri. If not, there is no
            // point to query user about permission to save it to contacts.
            
            TInt error = KErrNotFound;
            if ( iAddress )
                {
                TMusEngUriParser parser( *iAddress );
                TRAP( error, parser.ParseUriL() )                
                }
   
            // Save Address to Contacts query if manually entered
            // and if is the standard variant.
            // NOTE: KErrNotSupported means there is more than one contact id.
            if ( !iOperatorSpecificFunctionality && ManualAddressTyped() && 
                 iContactId != KErrNotSupported &&
                 error == KErrNone )
                {
                iShutdownState = EMusUiShutdownContactSavingQueried;
                SaveContactQueryL();
                }
            else
                {
                // Nothing send specific, let the base class handle
                CMusUiEventController::ExitProcedureL( aUserAcceptance );
                }
            break;
            }

        default:
            {
            // Nothing send specific, let the base class handle
            CMusUiEventController::ExitProcedureL( aUserAcceptance );
            break;
            }
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::ExitProcedureL" );
    }
    
    
// -----------------------------------------------------------------------------
// Handles status changes specific to sending sharing types.
// Calls base class version for status changes that are not sending specific. 
// -----------------------------------------------------------------------------
//
void CMusUiSendController::HandleChangedStatusL(
                        TMusAvailabilityStatus aStatus )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::HandleChangedStatusL" )
    switch( aStatus )
        {
        case ESipRegistrationPending:
            {
            MUS_LOG( "mus: [MUSUI ]     ESipRegistrationPending" );
            iSipRegistrationPending = ETrue;
            break;
            }
        case ESipOptionsNotSent:    // FALLTHROUGH
        case ESipOptionsSent:       // FALLTHROUGH
        	{
        	break;	
        	}
        case EManualActivation:     // FALLTHROUGH
        case EMultimediaSharingAvailable:
            {
            MUS_LOG( "mus: [MUSUI ]     EManualActivation or EMultimediaSharingAvailable" )
      
            if( iSipRegistrationPending )
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
            break;
            }

        default:
            {
            // Not sending specific, let the base class handle
            CMusUiEventController::HandleChangedStatusL( aStatus );
            break;
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::HandleChangedStatusL" )
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusUiSendController::VideoFileNameL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::VideoFileNameL" );
    
    TParse parse;
    TPtrC rootPath;
    
    SetVideoPathValueL();//sets video rootpath according to setting value.
   
    if ( parse.Set( iVideoFilePath,NULL,NULL) != KErrBadName )
        {
         
         if(parse.Drive().Compare( PathInfo::MemoryCardRootPath()))
            {
            TRAPD(error,MusUiFileUtil::VideoFilePathExistsL(iVideoFilePath));
            
            switch(error)
                {
                case KErrNotReady:
                case KErrCorrupt:
                case KErrLocked:
                    {
                    MusUiDialogUtil::ShowGlobalInformationDialogL(
                                           R_MUS_VIEW_NOTE_MEMORYCARD_PROBLEM);
                    
                    MUS_LOG( "mus: [MUSUI ]  -> Changing to Phone Memory " );
    		        rootPath.Set(PathInfo::PhoneMemoryRootPath());
    		        TParsePtrC pathPtr(rootPath);
    	            iVideoFilePath  = rootPath;
    	            iVideoFilePath  += PathInfo::VideosPath();
    	            // Make sure the new path exists
    	            TRAP_IGNORE( MusUiFileUtil::VideoFilePathExistsL(iVideoFilePath) )
                    break;
                    }
                case KErrAlreadyExists:
                case KErrNone:
                    {
                    MUS_LOG( "mus: [MUSUI ]  -> Using MMC " );
                    rootPath.Set(PathInfo::MemoryCardRootPath());
                    break;
                    }
                default:
                    {
                    delete iVideoFileName;
                    iVideoFileNamePlain = KNullDesC;
        	    	iVideoFileName = iVideoFileNamePlain.AllocL(); 
                    HandleError( error);
                    break;
                    }
                }
            }
        else//Phone memory is being used.
            {
            TRAPD(error,MusUiFileUtil::VideoFilePathExistsL(iVideoFilePath));
               
            switch(error)
                {
                case KErrAlreadyExists:
                case KErrNone:
                    {
                    MUS_LOG( "mus: [MUSUI ] -> Using Phone memory" );
                    rootPath.Set(PathInfo::PhoneMemoryRootPath());
                    break;
                    }
                default:
                    {
                    delete iVideoFileName;
                    iVideoFileNamePlain = KNullDesC;
        	    	iVideoFileName = iVideoFileNamePlain.AllocL(); 
                    HandleError( error);
                    break;
                    }
                }
             }
        
        }
        else
            {
            delete iVideoFileName;
            iVideoFileNamePlain = KNullDesC;
	    	iVideoFileName = iVideoFileNamePlain.AllocL(); 
            HandleError(KErrBadName);
            }
        
    TParsePtrC pathPtr(rootPath);
	iVideoFilePath  += pathPtr.NameAndExt();
	MusUiFileUtil::CreateVideoFileNameL(iVideoFilePath,
	                                    iVideoFileNamePlain);

    delete iVideoFileName;
    TFileName videoFileName;
    videoFileName += iVideoFilePath;
    videoFileName += iVideoFileNamePlain;
    videoFileName += KMusVideoFileNameExtension;
    iVideoFileName = videoFileName.AllocL();
   
    MUS_LOG_TDESC( "mus: [MUSUI ]    <- CMusUiSendController::VideoFileNameL:",
                            iVideoFileName->Des() );
    return *iVideoFileName;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::ShowInvitingWaitDialogL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::ShowWaitDialogL" );
    delete iDialogPrompt;
    iDialogPrompt = NULL;
    iDialogPrompt = StringLoader::LoadL(
                            R_MUS_LIVE_SHARING_VIEW_INVITING_WAIT_NOTE_TXT,
                            MusContactName().Length() > 0 ?
                            MusContactName() : iOperatorSpecificFunctionality ?
                            MusTelNumberValue() : *iAddress );
                   
    if ( !iSharingObserver.ShowWaitDialogL( 
                                *iDialogPrompt, 
                                KMusInviteExpireTimeout,
                                R_MUS_LIVE_SHARING_VIEW_NOTE_NO_RESPONSE ) )
        {
        CancelInvitationL();
        }
            
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::ShowWaitDialogL" );
    }
        
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::DismissWaitDialog()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::DismissWaitDialog" );
    TRAPD( err, iSharingObserver.DismissWaitDialogL(); );
    if ( err != KErrNone )
        {
        HandleError( err );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::DismissWaitDialog" );
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SplitL( const TDesC& aDes,
                                   const TDesC& aChar,
                                   CDesCArray* aArray )
    {
    TInt charPos( 0 );
    TPtrC ptr( aDes );
    TBool found = EFalse;
    while( ( charPos = ptr.Find( aChar ) ) > 0 )
        {
        found = ETrue;
        TPtrC str = ptr.Left( charPos );
        aArray->AppendL( str );
        ptr.Set( ptr.Right( ptr.Length() - str.Length() - 1 ) );
        }
    if ( found && ptr.Length() > 0 )
        {
        aArray->AppendL( ptr );
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::SetVideoPathValueL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::SetVideoPathValueL" );
    TInt drive = MultimediaSharingSettings::VideoLocationSettingL();                

    TFileName rootPath;
    User::LeaveIfError( PathInfo::GetRootPath( rootPath, drive ) );

	TParsePtrC pathPtr(rootPath);
	iVideoFilePath  = rootPath;
	iVideoFilePath  += PathInfo::VideosPath();
    MUS_LOG_TDESC( "mus: [MUSUI ]  <- CMusUiSendController::SetVideoPathValueL: ",
                   iVideoFilePath ); 
    iLocalizer->SetFullPath( iVideoFilePath );
    if ( iLocalizer->IsLocalized() )
        {
        iLocalizedVideoFilePath =iLocalizer->LocalizedName();
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::ShowVideoSavedDialogL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::ShowVideoSavedDialogL" );
    //MUS_LOG_TDESC( "mus: [MUSUI ]      ", iVideoFileNamePlain );
    //MUS_LOG_TDESC( "mus: [MUSUI ]      ", iVideoFilePath );
    
    delete iDialogPrompt;
    iDialogPrompt = NULL;
    
    TBuf<KMusFileNameMaxLength> buf;
    TFileName videoFileName;
    videoFileName += iVideoFileNamePlain;
    videoFileName += KMusVideoFileNameExtension;
    buf = videoFileName;
    
    CDesCArrayFlat* savingTextsArray = 
            new ( ELeave ) CDesCArrayFlat( KMusStringSubstArrayGranularity );
    CleanupStack::PushL( savingTextsArray );
    //"File %0U saved to folder %1U"
    //%0U = iVideoFileName
    //%1U = iVideoFilePath
    savingTextsArray->InsertL( 0, videoFileName );

    if ( MultimediaSharingSettings::OperatorVariantSettingL() ==
    	 MusSettingsKeys::EOperatorSpecific ) // For AT&T products
        {
    	HBufC* folder = StringLoader::LoadLC( R_MUS_VIDEO_SAVE_FOLDER_NAME );
    	savingTextsArray->InsertL( 1, *folder );
		CleanupStack::PopAndDestroy( folder );
        }
    else // Standard products
        {
	    if ( iLocalizer->IsLocalized() )
	        {
	        savingTextsArray->InsertL( 1, iLocalizedVideoFilePath );
	        }
	    else
	        {
	        savingTextsArray->InsertL( 1, iVideoFilePath );
	        }
	    }

    MUS_LOG_TDESC( "mus: [MUSUI ]      filename:", savingTextsArray->MdcaPoint( 0 ) );
    MUS_LOG_TDESC( "mus: [MUSUI ]      path:", savingTextsArray->MdcaPoint( 1 ) );

    iDialogPrompt = StringLoader::LoadL( R_MUS_VIEW_NOTE_VIDEO_SAVED,
                                         *savingTextsArray );        
    CleanupStack::Pop( savingTextsArray );
                                         
    MUS_LOG_TDESC( "mus: [MUSUI ]      ", iDialogPrompt->Des() );

    MusUiDialogUtil::ShowGlobalInformationDialogL( *iDialogPrompt );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::ShowVideoSavedDialogL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::DeleteVideoL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::DeleteVideoL" );
    if ( iVideoFileName )
        {
        MusUiFileUtil::DeleteFileL( *iVideoFileName );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::DeleteVideoL" );
    }   


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::DeleteFileL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendController::DeleteFileL" );
    if ( iVideoFileName && 
    		MusUiFileUtil::FileExists( *iVideoFileName ) && !iVideoToBeSaved ) 
		{
        DeleteVideoL();
		}
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendController::DeleteFileL" );
    }   



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiSendController::
TWaitDialogCallback::TWaitDialogCallback( CMusUiSendController& aController )
    : iController( aController )
    {       
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendController::
TWaitDialogCallback::DialogDismissedL( TInt /*aButtonId*/ )
    {
    iController.StartInvitationL();
    }
    
// End of file

