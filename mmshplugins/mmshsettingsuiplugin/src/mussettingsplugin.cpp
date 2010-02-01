/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MUSSettingsPlugin implementation.
*  Version     : %version: 34 % << Don't touch! Updated by Synergy at check-out.
*
*/



#include    "mussettingsplugin.h"
#include    "mussettingscontainer.h"
#include    "mussettingsmodel.h"
#include    "mussettingsplugin.hrh"
#include    "mussipprofilemodel.h"
#include    "muslogger.h"
#include    "musresourcefinderutil.h"
#include    "mussesseioninformationapi.h"
#include    <gscommon.hrh>
#include    <mussettingsplugin.mbg> // Icons
#include    <mussettingsuirsc.rsg> // GUI Resource
#include    <gsprivatepluginproviderids.h>
#include    <aknnotewrappers.h>
#include    <aknradiobuttonsettingpage.h>
#include    <aknpopupsettingpage.h>
#include    <akntextsettingpage.h>
#include    <aknViewAppUi.h>
#include    <AknGlobalNote.h>
#include    <featmgr.h>
#include    <StringLoader.h>
#include    <hlplch.h> // HlpLauncher
#include    <pathinfo.h>

#include    <e32property.h>

#include    <CAknMemorySelectionDialogMultiDrive.h>
#include    <AknCommonDialogsDynMem.h>

#include    <CAknMemorySelectionDialog.h>

// #include    <CAknMemorySelectionDialog.h>



// ======== MEMBER FUNCTIONS ========


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusSettingsPlugin::CMusSettingsPlugin()
    : iResources( *iCoeEnv )
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::CMusSettingsPlugin()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusSettingsPlugin::~CMusSettingsPlugin()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::~CMusSettingsPlugin()" )
    FeatureManager::UnInitializeLib();	
    if( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }
        
    CloseResourceFile();

    delete iModel;
    iModel = NULL;
	delete iHandler;
	iHandler = NULL;
    delete iDiskNotifyHandler;
    delete iCaption;
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::~CMusSettingsPlugin()" )
    }

// -----------------------------------------------------------------------------
// 
//
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ConstructL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::ConstructL()" )
    FeatureManager::InitializeLibL();
  
    iResourceFileOpen = false;
    OpenResourceFileL();

    MUS_LOG( "[MUSSET] Constructing the Base " )
    BaseConstructL( R_GS_VS_VIEW );
    iHandler = CMusSIPProfileModel::NewL();
    MUS_LOG( "[MUSSET]    CMusSettingsPlugin::ConstructL() 2" )
    iModel = CMusSettingsModel::NewL( *iHandler );
    iDiskNotifyHandler = CDiskNotifyHandler::NewL( *this, 
                                            iEikonEnv->FsSession() );
    User::LeaveIfError( iDiskNotifyHandler->NotifyDisk() ); // Subscribe disk notifications

    iCaption = StringLoader::LoadL( R_GS_VS_PLUGIN_CAPTION );
    CloseResourceFile();

    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::ConstructL()" )
    }

// -----------------------------------------------------------------------------
// 
//
// -----------------------------------------------------------------------------
//
CMusSettingsPlugin* CMusSettingsPlugin::NewL( TAny* /*aInitParams*/ )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::NewL()" )
    CMusSettingsPlugin* self = new( ELeave ) CMusSettingsPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::NewL()" )
    return self;
    }

// -----------------------------------------------------------------------------
// From class CAknView.
// Returns UID of *this* settings plugin.
// -----------------------------------------------------------------------------
//
TUid CMusSettingsPlugin::Id() const
    {
    MUS_LOG1( "[MUSSET] <- CMusSettingsPlugin::Id()( %d )",
              KGSVSSettingsPluginUID.iUid )
    return KGSVSSettingsPluginUID;
    }

// -----------------------------------------------------------------------------
// Hides non-virtual member from base class CGSBaseView.
// Handles a change in client rectangle size.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::HandleClientRectChange()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::HandleClientRectChange()" )
    if ( iContainer && iContainer->iListBox )
        {
        iContainer->SetRect( ClientRect() );
        }
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::HandleClientRectChange()" )
    }

// -----------------------------------------------------------------------------
// From class CAknView.
// Called by framework when *this* control is to be activated/focused.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::DoActivateL(
    const TVwsViewId& aPrevViewId,
    TUid aCustomMessageId,
    const TDesC8& aCustomMessage )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::DoActivateL()" )
    OpenResourceFileL();
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::DoActivateL()" )
    }

// -----------------------------------------------------------------------------
// From class CAknView.
// Called by framework when *this* control is to be deactivated.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::DoDeactivate()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::DoDeactivate()" )
    CGSBaseView::DoDeactivate();
    CloseResourceFile();
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::DoDeactivate()" )
    }

// -----------------------------------------------------------------------------
// From class CAknView.
// Handles a user selected menu command.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::HandleCommandL( TInt aCommand )
    {
    MUS_LOG1( "[MUSSET] -> CMusSettingsPlugin::HandleCommandL()( %d )",
              aCommand )
    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
//        case EGSCmdAppChange:
            {
       		HandleListBoxSelectionL();
            break;
            }

        case EAknSoftkeyBack:
            {
            AppUi()->ActivateLocalViewL( iPrevViewId.iViewUid );
            break;
            }

        case EAknCmdHelp:
            {
            if( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), AppUi()->AppHelpContextL());
                }
            break;
            }

        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::HandleCommandL()" )
    }

// -----------------------------------------------------------------------------
// From class CGSPluginInterface.
// Gets caption text of *this* plugin.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::GetCaptionL( TDes& aCaption ) const
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::GetCaptionL()" )
    if ( iCaption && iCaption->Length() <= aCaption.MaxLength() )
		{
    	aCaption.Copy( *iCaption );
		}
    
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::GetCaptionL()" )
    }

// -----------------------------------------------------------------------------
// From class CGSPluginInterface.
// Returns provider category of *this* plugin.
// -----------------------------------------------------------------------------
//
TInt CMusSettingsPlugin::PluginProviderCategory() const
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::PluginProviderCategory()" )
    return KGSPluginProviderInternal;
    }

// -----------------------------------------------------------------------------
// From class MEikMenuObserver.
// Called by framework before creating menus
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::DynInitMenuPaneL( TInt aResourceId,
                                              CEikMenuPane* aMenuPane )
    {
    // Delete Help item if feature is not supported
    if( aResourceId == R_VS_MENU_ITEM_EXIT ) 
        {
        if( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            aMenuPane->DeleteMenuItem( EAknCmdHelp );
            }
        }
    }
    
// -----------------------------------------------------------------------------
// From MDiskNotifyHandlerCallback
// Called by framework When disk status changed
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::HandleNotifyDisk( TInt /*aError*/, 
                                           const TDiskEvent& /*aEvent*/ )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::HandleNotifyDisk()" )
    
    // Since the plugin is created immediately after opening GS but container
    // will be created only after opening the VS view, this function may be
    // called before the creation of container. In such a case we simply ignore
    // the notification.
    if ( Container() ) 
        {
        TRAP_IGNORE( 
                Container()->UpdateListBoxL( KGSSettIdRecordedVideoSaving ) )
        }
        
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::HandleNotifyDisk()" )
    }

// -----------------------------------------------------------------------------
// From class CGSBaseView.
// Called by GS framework to create a GS container for *this* plugin.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::NewContainerL()
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::NewContainerL()" )
    iContainer = new( ELeave ) CMusSettingsContainer( *iModel );
    }

// -----------------------------------------------------------------------------
// From class CGSBaseView.
// Handles users "middle click" aka MSK on selected feature.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::HandleListBoxSelectionL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::HandleListBoxSelectionL()" )
    CMusSettingsContainer& container =
        *static_cast<CMusSettingsContainer*>( iContainer );
    const TInt currentItem = container.CurrentFeatureId();
    MUS_LOG1( "[MUSSET]    Item selected: ", currentItem )

    switch ( currentItem )
        {
        case KGSSettIdVSActivation:
            {
            SwitchOnOffValueL( KGSSettIdVSActivation );
    		break;
    	    }
        case KGSSettIdSIPProfile:
            {
            ShowVSSettingsProfileSettingDialogL();
            break;
            }
        case KGSSettIdAutoRecord:
            {
            SwitchOnOffValueL( KGSSettIdAutoRecord );
        	break;
            }
        case KGSSettIdRecordedVideoSaving:
            {
        	ShowVSSettingsRecordedVideoSavingSettingDialogL();
        	break;
            }
        case KGSSettIdNote:
            {
        	SwitchOnOffValueL( KGSSettIdNote );
        	break;
            }
        default:
            {
            break;
            }
        }
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::HandleListBoxSelectionL()" )
    }

// -----------------------------------------------------------------------------
// From class CGSBaseView.
// Returns container class of *this* plugin. iContainer is always garanteed to
// be of type CMusSettingsContainer*.
// -----------------------------------------------------------------------------
//
CMusSettingsContainer* CMusSettingsPlugin::Container()
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::Container()" )
    return static_cast<CMusSettingsContainer*>( iContainer );
    }

// -----------------------------------------------------------------------------
// Shows SIP profile setting dialog (i.e. "use default profile" or "select
// profile from list"). If select profile from list is selected, a list of
// SIP profiles is provided for user to choose wanted SIP profile.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowVSSettingsProfileSettingDialogL()
    {
    MUS_LOG(
    "[MUSSET] -> CMusSettingsPlugin::ShowVSSettingsProfileSettingDialogL()" )
    TInt cenRepValue = MultimediaSharingSettings::SipProfileSettingL();
    TInt profileMode = CMusSettingsModel::KVsSipProfileDefault;
    if ( cenRepValue != CMusSettingsModel::KVsSipProfileDefault )
    	{
    	profileMode = CMusSettingsModel::KVsSipProfileSelect;
    	}
    TInt oldProfileMode( profileMode );

    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
        R_SIP_PROFILE_SETTING_PAGE_LBX);
    CleanupStack::PushL( items );
    items->Delete( CMusSettingsModel::KVsSipProfileSelectNone );

    CAknRadioButtonSettingPage* dlg = new ( ELeave ) 
        CAknRadioButtonSettingPage( R_VS_PROFILE_SETTING_PAGE,
                                    profileMode,
                                    items);

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        if ( profileMode == CMusSettingsModel::KVsSipProfileDefault )
        	{
        	if ( oldProfileMode != profileMode )
        		{
        		MultimediaSharingSettings::SetSipProfileSettingL( 
                                    CMusSettingsModel::KVsSipProfileDefault );
        		Container()->ShowNewProfileActiveAfterCallL();
        		Container()->UpdateListBoxL( KGSSettIdSIPProfile );
        		}
        	}
        else
        	{
            ShowVSSettingsSelectSipProfileDialogL();
			}
        }
    CleanupStack::PopAndDestroy( items );
    MUS_LOG(
    "[MUSSET] <- CMusSettingsPlugin::ShowVSSettingsProfileSettingDialogL()" )
    }

// -----------------------------------------------------------------------------
// Provides user a list of SIP profiles to select from. If no SIP profiles
// exist an error note is displayed.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowVSSettingsSelectSipProfileDialogL()
    {
	// Get the array of the profile names, ownership changes
    CDesCArray* array = iModel->ListOfProfileNamesL();
	CleanupStack::PushL( array );

	if ( array->Count() < 1 )
		{
		ShowNoProfilesNotificationL();
		}
	else
		{
    	TInt selectedIndex = iModel->ProfileIndexByIdL(
                            MultimediaSharingSettings::SipProfileSettingL() );
        TInt currentIndex ( selectedIndex );

		if ( selectedIndex == KErrNotFound )
			{
			// first profile in the list
			selectedIndex = CMusSettingsModel::KVsSipProfileDefault;
			}

		// Create and display the pop-up list
		CAknRadioButtonSettingPage* defaultPopUp =
			new ( ELeave ) CAknRadioButtonSettingPage(
    			R_VS_SIP_PROFILE_LIST_VIEW_SELECT_SETTING_PAGE,
    			selectedIndex,
    			array );
		if ( defaultPopUp->ExecuteLD(
			CAknSettingPage::EUpdateWhenChanged ) )
			{
			if ( selectedIndex != currentIndex )
        		{
	        	// User has changed the selected profile, set new
    	    	// setting to persistent storage
    	    	TUint newValue = iModel->ProfileIdByIndex( selectedIndex );
	    	    MultimediaSharingSettings::SetSipProfileSettingL( newValue );
	    	    Container()->ShowNewProfileActiveAfterCallL();
				Container()->UpdateListBoxL( KGSSettIdSIPProfile );
    			}
			}
		}

	CleanupStack::PopAndDestroy( array );  // array
    }

// -----------------------------------------------------------------------------
// Provides a dialog for user to choose saving location for recorderded video.
// (locations are naturally phone memory or memory card).
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowVSSettingsRecordedVideoSavingSettingDialogL()
    {
    MUS_LOG(
    "[MUSSET] -> CMusSettingsPlugin::ShowVSSettingsRecordedVideoSavingSettingDialogL()" )
    TDriveUnit phoneMemUnit( 
                    TParsePtrC( PathInfo::PhoneMemoryRootPath() ).Drive() );
    TDriveUnit mmcUnit( TParsePtrC( PathInfo::MemoryCardRootPath() ).Drive() );
    TInt currentValue =	iModel->VSSettingsRecordedVideoSavingL();
    
    CAknMemorySelectionDialogMultiDrive* dlg = 
                                    iModel->MemorySelectionDialogLC();   

    TBool result( EFalse );
    TDriveNumber driveNumber((TDriveNumber)currentValue);    
    result = dlg->ExecuteL( driveNumber, NULL, NULL );

    if ( result != CAknCommonDialogsBase::TReturnKey(
                                        CAknCommonDialogsBase::ERightSoftkey) )
        {
        if ( /*( TInt )*/ driveNumber != currentValue )
            {
            MultimediaSharingSettings::SetVideoLocationSettingL( ( TInt ) driveNumber );
            }
        Container()->UpdateListBoxL( KGSSettIdRecordedVideoSaving );
        }

    CleanupStack::PopAndDestroy(dlg); 
    MUS_LOG(
    "[MUSSET] <- CMusSettingsPlugin::ShowVSSettingsRecordedVideoSavingSettingDialogL()" )
    }

// -----------------------------------------------------------------------------
// In standard variant provides user a "Capability auditory note" setting
// dialog, and in operator variant provides user an "Alerts" setting dialog.
// Note that in both variants the different dialogs toggle the same setting.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowVSSettingsNoteSettingDialogL()
    {
    MUS_LOG(
    "[MUSSET] -> CMusSettingsPlugin::ShowVSSettingsNoteSettingDialogL()" )
    MusSettingsKeys::TAuditoryNotification currentValue =
                    MultimediaSharingSettings::AuditoryNotificationSettingL();
    TInt intCurrentValue = static_cast<TInt>( currentValue );

    CAknRadioButtonSettingPage* dlg;
    CDesCArrayFlat* items;

    if ( MultimediaSharingSettings::OperatorVariantSettingL() == 
         MusSettingsKeys::EStandard )
        {
        items = iCoeEnv->ReadDesC16ArrayResourceL(
            R_VS_AUDIO_SETTING_PAGE_LBX );
        CleanupStack::PushL( items );
        dlg = new ( ELeave ) CAknRadioButtonSettingPage(
            R_VS_AUDIO_SETTING_PAGE,
            intCurrentValue,
            items );
        }
    else
        {
        items = iCoeEnv->ReadDesC16ArrayResourceL(
            R_VS_NOTE_SETTING_PAGE_LBX );
        CleanupStack::PushL( items );
        dlg = new ( ELeave ) CAknRadioButtonSettingPage(
            R_VS_NOTE_SETTING_PAGE,
            intCurrentValue,
            items );
        }

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        currentValue =
                static_cast<MusSettingsKeys::TAuditoryNotification>( intCurrentValue );
        MultimediaSharingSettings::SetAuditoryNotificationSettingL(
                                                                currentValue );
        Container()->UpdateListBoxL( KGSSettIdNote );
        }

    CleanupStack::PopAndDestroy( items );
    MUS_LOG(
    "[MUSSET] <- CMusSettingsPlugin::ShowVSSettingsNoteSettingDialogL()" )
    }

// -----------------------------------------------------------------------------
// Shows a notifications that no SIP profiles exists.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowNoProfilesNotificationL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::ShowNoProfilesNotificationL()" )
    HBufC* infoTxt = StringLoader::LoadLC( R_QTN_MSH_SET_PROFILE_EMPTY );
    CAknInformationNote* note = new ( ELeave ) CAknInformationNote( ETrue );
    note->ExecuteLD( infoTxt->Des() );
    CleanupStack::PopAndDestroy( infoTxt );
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::ShowNoProfilesNotificationL()" )
    }

// -----------------------------------------------------------------------------
// Switches between two possible values from one to another (i.e. toggles a
// setting on/off). Toggled setting is passed in aValue parameter.
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::SwitchOnOffValueL( TInt aValue )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::SwitchOnOffValueL()" )
            
    switch( aValue )
	    {
	    case KGSSettIdVSActivation:
	        {
	        MusSettingsKeys::TOperatorVariant operatorVarValue =
                        MultimediaSharingSettings::OperatorVariantSettingL();
	            
	        if ( operatorVarValue == MusSettingsKeys::EOperatorSpecific )
                {
	        
                TInt aCallCount = 0;
                RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                              NMusSessionInformationApi::KMusCallCount,
                              aCallCount );
                MUS_LOG1( "CallCount ( %d )", aCallCount)
                if ( aCallCount == 0 )
                    {
                    TBool enabled = EFalse;
                    TRAPD( error, enabled = iHandler->ProfileEnabledL( ));
                    if ( error != KErrNone )
                        {
                        // Problems with re-reading profiles; use existing array
                        MUS_LOG1("Error returned( %d )", error )
                        }
                    else
                        {
                        if( enabled )
                            {
                            TRAPD( error, iHandler->EnableProfileL() );
                            if ( error != KErrNone )
                                {
                                // Problems with re-reading profiles; use existing array
                                iModel->SetActivationItem( ETrue );
                                }
                            iModel->SetActivationItem( EFalse );
                            }
                        else
                            {
                            TRAP_IGNORE( iHandler->DisableProfileL() );
                            iModel->SetActivationItem( ETrue );
                            }
                        }
                    }
                }
            else
                {
                if ( MusSettingsKeys::EAlwaysActive ==
                     MultimediaSharingSettings::ActivationSettingL() )
                    {
                    MUS_LOG( "[MUSSET]    Activation setting set off" )
                    //MusSettingsKeys::ENever = 2 can not be used anymore
                    //EActiveInHomeNetworks = 1 is used instead -> off
                    MultimediaSharingSettings::SetActivationSettingL(
                                    MusSettingsKeys::EActiveInHomeNetworks );
                    }
                else
                    {
                    MUS_LOG( "[MUSSET]    Activation setting set on" )
                    MultimediaSharingSettings::SetActivationSettingL(
                                        MusSettingsKeys::EAlwaysActive );
                    }
                }
            break;
            }
	    case KGSSettIdAutoRecord:
	        {
	        if ( MusSettingsKeys::EAutoRecordOff ==
                            MultimediaSharingSettings::AutoRecordSettingL() )
		    	{
                MultimediaSharingSettings::SetAutoRecordSettingL( 
                                            MusSettingsKeys::EAutoRecordOn );
		    	}
			else
				{
                MultimediaSharingSettings::SetAutoRecordSettingL(
                                            MusSettingsKeys::EAutoRecordOff );
				}
	        break;
	        }
		case KGSSettIdNote:
			{
		    if ( MusSettingsKeys::EAuditoryNotificationOn == 
		            MultimediaSharingSettings::AuditoryNotificationSettingL() )
		    	{
		    	MultimediaSharingSettings::SetAuditoryNotificationSettingL(
		    		MusSettingsKeys::EAuditoryNotificationOff );
		    	}
			else
				{
                MultimediaSharingSettings::SetAuditoryNotificationSettingL(
					MusSettingsKeys::EAuditoryNotificationOn );
				}
			break;
			}
			
	    default:
	    	{
	    	MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::SwitchOnOffValueL() - error unknown setting" )
	    	User::Leave( KErrArgument );
	    	}
	    }
	        
	Container()->UpdateListBoxL( aValue );
	        
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::SwitchOnOffValueL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowGlobalInformationDialogL( TInt aResourceId )
    {
    CAknGlobalNote* dlg = CAknGlobalNote::NewLC();
    HBufC* dlgPrompt = StringLoader::LoadLC( aResourceId );
    TRequestStatus status;
    dlg->ShowNoteL( status, EAknGlobalInformationNote, *dlgPrompt  );
    User::WaitForRequest( status );
    CleanupStack::PopAndDestroy( dlgPrompt );
    CleanupStack::PopAndDestroy( dlg );
    }

// -----------------------------------------------------------------------------
// From class CGSPluginInterface.
// Creates a new icon of desired type. Overrided to provide custom icons.
// Ownership of the created icon is transferred to the caller.
// -----------------------------------------------------------------------------
//
CGulIcon* CMusSettingsPlugin::CreateIconL( const TUid aIconType )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::CreateIconL()" )

    OpenResourceFileL();

	CGulIcon* icon;

    if( aIconType == KGSIconTypeLbxItem )
        {
        // Create a custom icon
	    TParse* fp = new( ELeave ) TParse();
	    CleanupStack::PushL( fp );
	    HBufC* fileName = MusResourceFinderUtil::AppResourcePathL(
	    		                        KGSVSSettingsPluginIconDirAndName );
	    CleanupStack::PushL(fileName);
	    fp->Set( *fileName , &KDC_BITMAP_DIR, NULL );
	    CleanupStack::PopAndDestroy( fileName );

        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropSetVideoSharing,
        fp->FullName(),
        EMbmMussettingspluginQgn_prop_set_video_sharing,
        EMbmMussettingspluginQgn_prop_set_video_sharing_mask );

        CleanupStack::PopAndDestroy( fp );
        }
    else
        {
        // Use default icon from base class CGSPluginInterface.
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::CreateIconL()" )

    return icon;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::OpenResourceFileL()
	{
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::OpenResourceFileL()" )
        
    if (!iResourceFileOpen) 
        {
        HBufC* fileName = MusResourceFinderUtil::ResourcePathL(
        KVSSettingsResourceFileName );    
        TFileName fName(*fileName);
        delete fileName;
        MUS_LOG_TDESC( "[MUSSET] Resource FileName ",fName )
        iResources.OpenL(fName);  
        iResourceFileOpen = true;
        } 
    
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::OpenResourceFileL()" )
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSettingsPlugin::CloseResourceFile()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::CloseResourceFile()" )
    iResources.Close();
    iResourceFileOpen = false;
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::CloseResourceFile()" )
    }

