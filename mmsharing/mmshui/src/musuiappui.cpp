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



#include "musuiappui.h"
#include "musuilivesharingview.h"
#include "musuiclipsharingview.h"
#include "musuireceiveview.h"
#include "musuistatuspanehandler.h"
#include "musuistartcontroller.h"
#include "musuiactivequerydialog.h"
#include "musuidefinitions.h"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "musmanagercommon.h"
#include "musuiresourcehandler.h"
#include "musui.hrh"
#include "musuid.hrh"
#include "muslogger.h" // debug logging
#include <musui.rsg>
#include <musui.mbg>

#include <csxhelp/msh.hlp.hrh>
#include <avkon.hrh>
#include <AknsUtils.h>
#include <remconcoreapitarget.h>
#include <remconinterfaceselector.h>
#include <akntoolbar.h>
#include <AknVolumePopup.h>

using namespace MusSettingsKeys;

// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::ConstructL" );
    BaseConstructL( EAknEnableSkin | EAppOrientationAutomatic | EAknEnableMSK 
    		      | EAknSingleClickCompatible );
    
    iResourceHandler = CMusUiResourceHandler::NewL( *this );
    MultimediaSharing::TMusUseCase usecase = MusUiStartController::ReadUseCaseL();
    iResourceHandler->CheckInitialOrientationL(usecase);
    
    iForeground = ETrue;
    
    // View for Live Sharing:
    CMusUiLiveSharingView* liveSharingView =
                            new ( ELeave ) CMusUiLiveSharingView;
    CleanupStack::PushL( liveSharingView );
    liveSharingView->ConstructL();
    AddViewL( liveSharingView );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop( liveSharingView );

    // View for Clip Sharing:
    CMusUiClipSharingView* clipSharingView =
                            new ( ELeave ) CMusUiClipSharingView;
    CleanupStack::PushL( clipSharingView );
    clipSharingView->ConstructL();
    AddViewL( clipSharingView );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop( clipSharingView );

    // View for Receiving:
    CMusUiReceiveView* receiveView =  new ( ELeave ) CMusUiReceiveView;
    CleanupStack::PushL( receiveView );
    receiveView->ConstructL();
    AddViewL( receiveView );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop( receiveView );

    FindWindowGroupIdentifiersL();
    
    iStatusPaneHandler = CMusUiStatusPaneHandler::NewL( *iEikonEnv );
    // The application icon:
    iStatusPaneHandler->SetStatusPaneIconsL(
                            EMbmMusuiQgn_menu_mus_app_cxt,
                            EMbmMusuiQgn_menu_mus_app_cxt_mask );

    iStatusPaneHandler->GetVolumePopup()->SetObserver(this);
    
    AknsUtils::InitSkinSupportL();
    AknsUtils::SetAvkonSkinEnabledL( ETrue );

    // Check use case and set default view
    switch ( usecase )
        {
        case MultimediaSharing::EMusLiveVideo:
            ActivateLocalViewL( liveSharingView->Id() );
            break;
        case MultimediaSharing::EMusClipVideo:
            ActivateLocalViewL( clipSharingView->Id() );
            break;
        case MultimediaSharing::EMusReceive:
            ActivateLocalViewL( receiveView->Id() );
            break;
        default:
            User::Leave( KErrNotSupported );
            break;
        }

    iInterfaceSelector = CRemConInterfaceSelector::NewL();
    iCoreTarget = CRemConCoreApiTarget::NewL(*iInterfaceSelector, *this);
    iInterfaceSelector->OpenTargetL();
    
    // check if operator specific functionality is needed  
    iOperatorSpecificFunctionality = 
        ( MultimediaSharingSettings::OperatorVariantSettingL() == 
                                                    EOperatorSpecific );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::ConstructL" );
    }


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMusUiAppUi::~CMusUiAppUi()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::~CMusUiAppUi" );
    delete iResourceHandler;
    delete iConfirmationQuery;
    delete iStatusPaneHandler;
    delete iInterfaceSelector;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::~CMusUiAppUi" );
    }


// -----------------------------------------------------------------------------
// Fetches a handle to status pane handler
// -----------------------------------------------------------------------------
//
CMusUiStatusPaneHandler* CMusUiAppUi::MusStatusPane()
    {
    return iStatusPaneHandler;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiAppUi::ExitValue()
    {
    return iExit;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::HandleWsEventL( const TWsEvent& aEvent, CCoeControl* aDestination )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::HandleWsEventL" );

    const TInt type = aEvent.Type();

    // Avoiding unknown failures
    if( !iView )
        {
        MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::HandleWsEventL, view is NULL" );
        return;
        }

    if( type == EEventKeyUp )
        {
        static_cast<CMusUiGeneralView*>
                ( iView )->HandleKeyUpEvent( iLastKeyCode );
        }

    if ( ( type == EEventKey ) && iView->Toolbar()->IsShown() && !MUS_NO_TOOLBAR )
        {
        const TKeyEvent* keyEvent = aEvent.Key();
        
        iLastKeyCode = keyEvent->iCode;        
        
        MUS_LOG1( "mus: [MUSUI ]     CMusUiAppUi::HandleWsEventL iCode: %d", 
                                keyEvent->iCode );
        CMusUiGeneralView* activatedView = 
                                    static_cast<CMusUiGeneralView*>( iView );
        activatedView->HandleToolbarCommandL( keyEvent->iCode );
        }
    
    if ( type == KAknFullOrPartialForegroundLost ||
         type == EEventFocusLost ||
         type == KAknFullOrPartialForegroundGained ||
         type == EEventFocusGained )
        {
        DoHandleForegroundEventL( type );
        }
         
    // All events are sent to base class.
    CAknAppUi::HandleWsEventL( aEvent, aDestination );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::HandleWsEventL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::HandleForegroundEventL( TBool aForeground )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::HandleForegroundEventL" );
    
    CAknViewAppUi::HandleForegroundEventL( aForeground );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::HandleForegroundEventL" );
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::MrccatoCommand( TRemConCoreApiOperationId aOperationId, 
                                  TRemConCoreApiButtonAction aButtonAct )
    {
    MUS_LOG2( "mus: [MUSUI ]  -> CMusUiAppUi::MrccatoCommand: %d, %d", 
              aOperationId,
              aButtonAct );
              
    switch ( aOperationId )
        {
        case ERemConCoreApiVolumeUp:  //  Volume Up
            {
            switch ( aButtonAct )
                {
                case ERemConCoreApiButtonClick:
                    {
                    // volume up clicked
                    CMusUiGeneralView* activatedView = 
                                    static_cast<CMusUiGeneralView*>( iView );
                                    if( MUS_NO_TOOLBAR )
                                        {
                                        TRAP_IGNORE( activatedView->HandleCommandL( 
                                                EMusuiCmdViewVolumeUp ) )                                        
                                        }
                                    else
                                        {
                                        TRAP_IGNORE( activatedView->HandleToolbarCommandL( 
                                                EMusuiCmdViewVolumeUp ) )
                                        }
                    break;
                    }
                default:
                    // Never hits this
                    break;
                }
                break;
            }
        case ERemConCoreApiVolumeDown:  //  Volume Down
            {
            switch ( aButtonAct )
                {
                case ERemConCoreApiButtonClick:
                    {
                    // volume down clicked
                    CMusUiGeneralView* activatedView = 
                                    static_cast<CMusUiGeneralView*>( iView );
                                    if( MUS_NO_TOOLBAR )
                                        {
                                        TRAP_IGNORE( activatedView->HandleCommandL( 
                                                    EMusuiCmdViewVolumeDown ) )
                                        }
                                    else
                                        {
                                        TRAP_IGNORE( activatedView->HandleToolbarCommandL( 
                                                    EMusuiCmdViewVolumeDown ) )                                        
                                        }
                    break;
                    }
                default:
                    // Never hits this
                    break;
                }
                break;
            }
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::MrccatoCommand" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::QueryAcceptedL( TBool aAccepted )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::QueryAccepted" );
    CMusUiGeneralView* activatedView = static_cast<CMusUiGeneralView*>( iView );
    // TODO: If next call leaves, it causes error handling to be done with
    // previously deleted query object. This leads to crash.
    activatedView->AsyncQueryDialogAcceptedL( aAccepted );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::QueryAccepted" );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CMusUiAppUi::HandleQueryError( TInt aError )
    {
    HandleError( aError ); // Forward to general error handler
    return KErrNone;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::ActivateLocalMusViewL( TUid aViewId )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::ActivateLocalMusViewL" );
    ActivateLocalViewL( aViewId );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::ActivateLocalMusViewL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::ActivateLocalMusViewL( TUid aViewId,
                                         TUid aCustomMessageId,
                                         const TDesC8& aCustomMessage )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::ActivateLocalMusViewL" );
    ActivateLocalViewL( aViewId, aCustomMessageId, aCustomMessage );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::ActivateLocalMusViewL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::ShowAsyncConfirmationQueryDialogL( const TDesC& aPrompt )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::ShowAsyncConfirmationQueryDialogL" );
    if ( !iConfirmationQuery )
        {
        iConfirmationQuery = CMusUiActiveQueryDialog::NewL( *this );
        }
    iConfirmationQuery->ShowL( aPrompt );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::ShowAsyncConfirmationQueryDialogL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::SetToolbarVisibility( TBool aVisible )
    {
    CMusUiGeneralView* activatedView = static_cast<CMusUiGeneralView*>( iView );
    if ( activatedView )
        {
        activatedView->SetToolbarVisibility( aVisible );
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::ShowNaviPaneIconL( TMusUiNaviMediaDecorator aIcon )
    {
    MusStatusPane()->ShowNaviPaneIconL( aIcon );
    }
   

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CEikonEnv* CMusUiAppUi::EikonEnv( ) const
    {
    return iEikonEnv;
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::TerminateCall()
    {
    // TODO: Remove hard-coded values!
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::TerminateCall" );
    RWsSession sess = iEikonEnv->WsSession();
    
    TApaTaskList taskList(sess);
    
    TApaTask phoneTask = taskList.FindApp(TUid::Uid(0x100058B3));
    MUS_LOG( "mus: [MUSUI ]     phoneTask" );
    TInt id = 0;
    if (phoneTask.Exists())
        id = phoneTask.WgId();
    MUS_LOG1( "PhoneTask ID: %d",id );    
    TWsEvent event;

    MUS_LOG( "mus: [MUSUI ]     CMusUiAppUi::TerminateCall: 1" );
    MUS_LOG( "mus: [MUSUI ]     CMusUiAppUi::EEventKeyDown" );
    event.SetType( EEventKeyDown );
    event.SetTimeNow();
    event.Key()->iCode = 0;
    event.Key()->iModifiers = EModifierNumLock;
    event.Key()->iRepeats = 0;
    event.Key()->iScanCode = EStdKeyNo;
    sess.SendEventToWindowGroup( id, event ); 

    MUS_LOG( "mus: [MUSUI ]     CMusUiAppUi::TerminateCall: 2" );
    MUS_LOG( "mus: [MUSUI ]     CMusUiAppUi::EEventKey" );
    event.SetType( EEventKey );
    event.SetTimeNow();
    event.Key()->iCode = EKeyNo;
    event.Key()->iModifiers = 32769;
    event.Key()->iRepeats = 0;
    event.Key()->iScanCode = EStdKeyNo;
    sess.SendEventToWindowGroup( id, event ); 

   	MUS_LOG( "mus: [MUSUI ]     CMusUiAppUi::TerminateCall: 3" );
    MUS_LOG( "mus: [MUSUI ]     CMusUiAppUi::EEventKeyUp" );
    event.SetType( EEventKeyUp );
    event.SetTimeNow();
    event.Key()->iCode = 0;
    event.Key()->iModifiers = 32769;
    event.Key()->iRepeats = 0;
    event.Key()->iScanCode = EStdKeyNo;
    sess.SendEventToWindowGroup( id, event );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::TerminateCall" );
    }


// -----------------------------------------------------------------------------
// Return current app orientation.
// -----------------------------------------------------------------------------
//
CAknAppUiBase::TAppUiOrientation CMusUiAppUi::AppOrientation() const
    {
    return ( ApplicationRect().Width() > ApplicationRect().Height() ) ?
            CAknAppUiBase::EAppUiOrientationLandscape :
            CAknAppUiBase::EAppUiOrientationPortrait;
    }


// -----------------------------------------------------------------------------
// Switch the active view's orientation, if it's in the foreground.
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::SwitchOrientationL( TAppUiOrientation aAppOrientation )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::SwitchOrientationL" );
    SetOrientationL( aAppOrientation );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::SwitchOrientationL" );
    }
        

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::HandleError( const TInt aReason )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::HandleError" );
    if ( aReason != KErrNone )
        {
        HandleExit();
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::HandleError" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::HandleExit()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::HandleExit" );
    if ( !iExit )
        {
        iExit = ETrue;
        PrepareToExit();

        RemoveView( KMusUidLiveSharingView );
        RemoveView( KMusUidClipSharingView );
        RemoveView( KMusUidReceivingView );

        User::Exit( KErrNone );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::HandleExit" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiAppUi::OrientationCanBeChanged() const
    {
    return CAknAppUiBase::OrientationCanBeChanged();
    }


// -----------------------------------------------------------------------------
// From base class CEikAppUi.  Handles a change to the application's
// resources which are shared across the environment.
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::HandleResourceChangeL( TInt aResourceChangeType )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::HandleResourceChangeL" );
    CAknAppUi::HandleResourceChangeL( aResourceChangeType );

    if ( aResourceChangeType == KEikDynamicLayoutVariantSwitch && iView )
        {
        MUS_LOG( "mus: [MUSUI ]  CMusUiAppUi::HandleResourceChangeL:\
                 aResourceChangeType == KEikDynamicLayoutVariantSwitch" );
        
        iView->Toolbar()->HandleResourceChange( aResourceChangeType );

        CMusUiGeneralView* activatedView =
                                static_cast<CMusUiGeneralView*>( iView );
        if ( !IsForeground() )
            {
            MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::HandleResourceChangeL" );
            return;
            }
        
        if ( activatedView )
            {
            activatedView->RefreshView();
            }
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::HandleResourceChangeL" );
    }


// -----------------------------------------------------------------------------
// takes care of key event handling
// -----------------------------------------------------------------------------
//
TKeyResponse CMusUiAppUi::HandleKeyEventL( const TKeyEvent& aKeyEvent,
                                           TEventCode /*aType*/ )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiAppUi::HandleKeyEventL: %d", 
                            aKeyEvent.iScanCode );
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiAppUi::HandleKeyEventL: %d", 
                            aKeyEvent.iCode );

    if ( aKeyEvent.iScanCode == EStdKeyDevice1 /*165*/ )   // Cancel in Options
        {
        SetToolbarVisibility( ETrue );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::HandleKeyEventL" );
    return EKeyWasNotConsumed;
    }


// -----------------------------------------------------------------------------
// takes care of command handling
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::HandleCommandL( TInt aCommand )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiAppUi::HandleCommandL [%d]", 
                            aCommand );
    switch ( aCommand )
        {
        case EMusuiGenCmdExit:
        case EAknSoftkeyExit:
        case EEikCmdExit:
            {
            CMusUiGeneralView* activatedView =
                                    static_cast<CMusUiGeneralView*>( iView );
            activatedView->HandleCommandL( EAknSoftkeyExit );

            break;
            }
        default:
            break;
        }
    MUS_LOG( "mus: [MUSUI ]  <-> CMusUiAppUi::HandleCommandL" );
    }

// -----------------------------------------------------------------------------
// CMusUiAppUi::GetVolumeFromUi()
// -----------------------------------------------------------------------------
//
TInt CMusUiAppUi::GetUiVolumeValue()
    {
    return iStatusPaneHandler->GetVolumeControlValue();
    }

// -----------------------------------------------------------------------------
// CMusUiAppUi::ResourceHandler()
// -----------------------------------------------------------------------------
//
CMusUiResourceHandler* CMusUiAppUi::ResourceHandler()
    {
    return iResourceHandler;
    }

// -----------------------------------------------------------------------------
// CMusUiAppUi::AppHelpContextL()
// -----------------------------------------------------------------------------
//
CArrayFix<TCoeHelpContext>* CMusUiAppUi::HelpContextL() const
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::HelpContextL" );
    CArrayFix<TCoeHelpContext>* contexts = 
            new (ELeave) CArrayFixFlat<TCoeHelpContext>(1);    
    CleanupStack::PushL(contexts);
    
    TCoeHelpContext help; 
    help.iMajor.iUid = KMusUiUid;
    
    if ( iView->Id() == KMusUidLiveSharingView )
        {
        help.iContext = KMSH_HLP_LIVE;
        }
    else if ( iView->Id() == KMusUidClipSharingView )
        {
        help.iContext = KMSH_HLP_CLIP;
        }
    else if ( iView->Id() == KMusUidReceivingView )
        {
        help.iContext = KMSH_HLP_RECEIVE;
        }

    contexts->AppendL(help);    
    CleanupStack::Pop(); // contexts

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::HelpContextL" );
    return contexts;
    }


// -----------------------------------------------------------------------------
// We might get PartialFocusLost also for some notes and
// overally items that are somehow overlapping screen instead of
// FocusLost. In those cases we should not put app to background.
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::DoHandleForegroundEventL( TInt aEventType )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiAppUi::DoHandleForegroundEventL, event:%d", 
              aEventType );
    
    __ASSERT_ALWAYS( aEventType == KAknFullOrPartialForegroundGained ||
                     aEventType == KAknFullOrPartialForegroundLost ||
                     aEventType == EEventFocusLost ||
                     aEventType == EEventFocusGained, User::Leave( KErrArgument ) );
    
    TBool handleEvent( EFalse );
    TBool foreground( 
            aEventType == KAknFullOrPartialForegroundGained ||
            aEventType == EEventFocusGained );

    const TInt windowGroupId =
        iCoeEnv->WsSession().GetFocusWindowGroup();

    MUS_LOG1( "mus: [MUSUI ]    focus wgid:%d", windowGroupId );
    
    if ( foreground )
        {
        iCoeEnv->RootWin().EnableFocusChangeEvents();
        handleEvent = ETrue;
        }
    else if ( ( windowGroupId != iThisApplicationWgId ) && 
              ( windowGroupId != iEikonServerWgId ) &&
              ( windowGroupId != iAknNfyServerWgId ) )
        {
        iCoeEnv->RootWin().DisableFocusChangeEvents();
        handleEvent = ETrue;
        }
    else
        {
        // NOP
        }
          
    if ( handleEvent && iView && foreground != iForeground )
        {
        iForeground = foreground;
        
        CMusUiGeneralView* activatedView = 
            static_cast<CMusUiGeneralView*>( iView );
            
        activatedView->DoHandleForegroundEventL( foreground );
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::DoHandlerForegroundEventL" );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::FindWindowGroupIdentifiersL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiAppUi::FindWindowGroupIdentifiersL" );
    
    // Name of the EIKON server window group.
    _LIT( KMusUiEikonServer, "EikonServer" );

    // Name of the AknCapServer window group.
    _LIT( KMusUiAknCapServer, "*akncapserver*" );

    // Name of the AknNotifierServer window group.
    _LIT( KMusUiAknNotifierServer, "*aknnfysrv*" );
    
    CEikonEnv& eikEnv = *( iEikonEnv );
    eikEnv.SetSystem( ETrue );
    eikEnv.WsSession().ComputeMode( RWsSession::EPriorityControlDisabled );

    iThisApplicationWgId = eikEnv.RootWin().Identifier();
    MUS_LOG1( "mus: [MUSUI ]    own wgid:%d", iThisApplicationWgId );
    iEikonServerWgId =
        eikEnv.WsSession().FindWindowGroupIdentifier( 0, KMusUiEikonServer );
    MUS_LOG1( "mus: [MUSUI ]    eikonserv wgid:%d", iEikonServerWgId );
    iAknCapServerWgId =
        eikEnv.WsSession().FindWindowGroupIdentifier( 0, KMusUiAknCapServer );
    MUS_LOG1( "mus: [MUSUI ]    akncapserv wgid:%d", iAknCapServerWgId );
    iAknNfyServerWgId =
        eikEnv.WsSession().FindWindowGroupIdentifier( 0,
            KMusUiAknNotifierServer );
    MUS_LOG1( "mus: [MUSUI ]    aknnotifserv wgid:%d", iAknNfyServerWgId );
    
    eikEnv.RootWin().EnableFocusChangeEvents();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiAppUi::FindWindowGroupIdentifiersL" );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusUiAppUi::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
    {
    CAknVolumePopup* popup = iStatusPaneHandler->GetVolumePopup();
    if ( popup && popup->IsVisible() && (popup == aControl) && 
         (aEventType == MCoeControlObserver::EEventStateChanged) )
        {
        MUS_LOG1( "mus: [MUSUI ]  CMusUiAppUi::HandleControlEventL(): volume[%d]",
                popup->Value() );
        CMusUiGeneralView* activatedView = 
                               static_cast<CMusUiGeneralView*>( iView );
        activatedView->HandleCommandL( EMusuiCmdViewVolumeChanged );
        }

    }
// end of file
