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


CMusSettingsPlugin::CMusSettingsPlugin()
    : iResources( *iCoeEnv )
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::CMusSettingsPlugin()" )
    }


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

    iResources.Close();

    delete iModel;
    iModel = NULL;
	delete iHandler;
	iHandler = NULL;
    delete iDiskNotifyHandler;
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::~CMusSettingsPlugin()" )
    }


void CMusSettingsPlugin::ConstructL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::ConstructL()" )
    FeatureManager::InitializeLibL();

    HBufC* fileName = MusResourceFinderUtil::ResourcePathL(
        KVSSettingsResourceFileName );    
    TFileName fName(*fileName);
    delete fileName;
    MUS_LOG_TDESC( "[MUSSET] Resource FileName ",fName )
    iResources.OpenL(fName);        
    MUS_LOG( "[MUSSET] Constructing the Base " )
    BaseConstructL( R_GS_VS_VIEW );
    iHandler = CMusSIPProfileModel::NewL();
    MUS_LOG( "[MUSSET]    CMusSettingsPlugin::ConstructL() 2" )
    iModel = CMusSettingsModel::NewL( *iHandler );
    iDiskNotifyHandler = CDiskNotifyHandler::NewL( *this, 
                                            iEikonEnv->FsSession() );
    User::LeaveIfError( iDiskNotifyHandler->NotifyDisk() ); // Subscribe disk notifications
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::ConstructL()" )
    }


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


// ----------------------------------------------------------------------------
// From class CAknView.
// Returns UID of *this* settings plugin.
// ----------------------------------------------------------------------------
//
TUid CMusSettingsPlugin::Id() const
    {
    MUS_LOG1( "[MUSSET] <- CMusSettingsPlugin::Id()( %d )",
              KGSVSSettingsPluginUID.iUid )
    return KGSVSSettingsPluginUID;
    }


// ----------------------------------------------------------------------------
// Hides non-virtual member from base class CGSBaseView.
// Handles a change in client rectangle size.
// ----------------------------------------------------------------------------
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


// ----------------------------------------------------------------------------
// From class CAknView.
// Called by framework when *this* control is to be activated/focused.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::DoActivateL(
    const TVwsViewId& aPrevViewId,
    TUid aCustomMessageId,
    const TDesC8& aCustomMessage )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::DoActivateL()" )
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::DoActivateL()" )
    }


// ----------------------------------------------------------------------------
// From class CAknView.
// Called by framework when *this* control is to be deactivated.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::DoDeactivate()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::DoDeactivate()" )
    CGSBaseView::DoDeactivate();
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::DoDeactivate()" )
    }


// ----------------------------------------------------------------------------
// From class CAknView.
// Handles a user selected menu command.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::HandleCommandL( TInt aCommand )
    {
    MUS_LOG1( "[MUSSET] -> CMusSettingsPlugin::HandleCommandL()( %d )",
              aCommand )
    CMusSettingsContainer& container =
        *static_cast<CMusSettingsContainer*>( iContainer );
    const TInt currentItem = container.CurrentFeatureId();

    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
        case EGSCmdAppChange:
            {
        	if ( currentItem == KGSSettIdVSActivation
        		&& aCommand == EGSCmdAppChange )
        		{
        		if ( iModel->VSSettingsOperatorVariantL() ==
        			MusSettingsKeys::EOperatorSpecific )
        			{
        			ShowOperatorSpecificActivationSettingDialogL();
        			}
        		else
        			{
        			ShowVSSettingsActivationSettingDialogL();
        			}
        		}
        	else if ( currentItem == KGSSettIdRecordedVideoSaving
        		&& aCommand == EGSCmdAppChange )
        		{
        		ShowVSSettingsRecordedVideoSavingSettingDialogL();
        		}
        	else if ( KGSSettIdNote == currentItem
        		&& EGSCmdAppChange == aCommand )
        		{
        		ShowVSSettingsNoteSettingDialogL();
        		}
        	else
        		{
        		HandleListBoxSelectionL();
        		}
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


// ----------------------------------------------------------------------------
// From class CGSPluginInterface.
// Gets caption text of *this* plugin.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::GetCaptionL( TDes& aCaption ) const
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::GetCaptionL()" )
    HBufC* result = StringLoader::LoadL( R_GS_VS_PLUGIN_CAPTION );
    aCaption.Copy( *result );
    delete result;
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::GetCaptionL()" )
    }


// ----------------------------------------------------------------------------
// From class CGSPluginInterface.
// Returns provider category of *this* plugin.
// ----------------------------------------------------------------------------
//
TInt CMusSettingsPlugin::PluginProviderCategory() const
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::PluginProviderCategory()" )
    return KGSPluginProviderInternal;
    }


// ----------------------------------------------------------------------------
// From class MEikMenuObserver.
// Called by framework before creating menus
// ----------------------------------------------------------------------------
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

    
// ----------------------------------------------------------------------------
// From MDiskNotifyHandlerCallback
// Called by framework When disk status changed
// ----------------------------------------------------------------------------
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


// ----------------------------------------------------------------------------
// From class CGSBaseView.
// Called by GS framework to create a GS container for *this* plugin.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::NewContainerL()
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::NewContainerL()" )
    iContainer = new( ELeave ) CMusSettingsContainer( *iModel );
    }


// ----------------------------------------------------------------------------
// From class CGSBaseView.
// Handles users "middle click" aka MSK on selected feature.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::HandleListBoxSelectionL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::HandleListBoxSelectionL()" )
    CMusSettingsContainer& container =
        *static_cast<CMusSettingsContainer*>( iContainer );
    const TInt currentItem = container.CurrentFeatureId();

    RDebug::Print( _L(
        "[CMusSettingsPlugin] Item selected: %d" ),
        currentItem );

	MusSettingsKeys::TOperatorVariant operatorVarValue =
		iModel->VSSettingsOperatorVariantL();

    switch ( currentItem )
        {
        case KGSSettIdVSActivation:
            {
        	if ( operatorVarValue == MusSettingsKeys::EOperatorSpecific )
    			{
    			SwitchOnOffValueL( KGSSettIdVSActivation );
    			container.UpdateListBoxL( KGSSettIdVSActivation );
				}
			else
				{
				ShowVSSettingsActivationSettingDialogL();
    			}
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
            container.UpdateListBoxL( KGSSettIdAutoRecord );
        	break;
            }

        case KGSSettIdRecordedVideoSaving:
            {

        	ShowVSSettingsRecordedVideoSavingSettingDialogL();

//        	SwitchOnOffValueL( KGSSettIdRecordedVideoSaving );
//        	container.UpdateListBoxL( KGSSettIdRecordedVideoSaving );

        	break;
            }

        case KGSSettIdNote:
            {
        	SwitchOnOffValueL( KGSSettIdNote );
        	container.UpdateListBoxL( KGSSettIdNote );
        	break;
            }

        default:
            {
            break;
            }
        }
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::HandleListBoxSelectionL()" )
    }


// ----------------------------------------------------------------------------
// From class CGSBaseView.
// Returns container class of *this* plugin. iContainer is always garanteed to
// be of type CMusSettingsContainer*.
// ----------------------------------------------------------------------------
//
CMusSettingsContainer* CMusSettingsPlugin::Container()
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsPlugin::Container()" )
    return static_cast<CMusSettingsContainer*>( iContainer );
    }


// ----------------------------------------------------------------------------
// Shows a dialog for user to modify VS activation setting. Note that this
// method has an alternative method for operator specific variant.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowVSSettingsActivationSettingDialogL()
    {
    MUS_LOG(
    "[MUSSET] -> CMusSettingsPlugin::ShowVSSettingsActivationSettingDialogL()"
     )

    MusSettingsKeys::TActivation currentValue =
    	iModel->VSSettingsActivationL();

    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
        R_ACTIVATION_SETTING_PAGE_LBX );

    CleanupStack::PushL( items );
    TInt intCurrentValue = static_cast<TInt>( currentValue );

    CAknRadioButtonSettingPage* dlg =
    	new ( ELeave ) CAknRadioButtonSettingPage(
        	R_ACTIVATION_SETTING_PAGE,
        	intCurrentValue,
        	items);

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        currentValue =
        	static_cast<MusSettingsKeys::TActivation>( intCurrentValue );

        iModel->SetVSSettingsActivationL( currentValue );
        Container()->UpdateListBoxL( KGSSettIdVSActivation );
        }

    CleanupStack::PopAndDestroy( items );
    MUS_LOG(
    "[MUSSET] <- CMusSettingsPlugin::ShowVSSettingsActivationSettingDialogL()" )
    }


// ----------------------------------------------------------------------------
// Shows a dialog for user to modify VS activation setting. Note that this
// method is used only for operator specific variant. Alternative method for
// "standard" variant exists. Note that because standard variant contains 3
// different values and operator variant contains only 2 values (0,2) the value
// 2 (MusSettingsKeys::ENever) is converted to value 1
// (MusSettingsKeys::EActiveInHomeNetworks) in operator variant just before
// showing the dialog. After showing the dialog the conversion mentioned above
// is reversed. This temporarily conversion is done solely to use values 0 and
// 1 for direct mapping to items array.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowOperatorSpecificActivationSettingDialogL()
    {
    MUS_LOG(
    "[MUSSET] -> CMusSettingsPlugin::ShowOperatorSpecificActivationSettingDialogL()"
     )

    MusSettingsKeys::TActivation currentValue =
    	iModel->VSSettingsActivationL();

    if ( currentValue == MusSettingsKeys::ENever )
    	{
    	currentValue = MusSettingsKeys::EActiveInHomeNetworks;
    	}

    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
        R_OPERATOR_ACTIVATION_SETTING_PAGE_LBX);

    CleanupStack::PushL( items );
    TInt intCurrentValue = static_cast<TInt>(currentValue);

    CAknRadioButtonSettingPage* dlg =
    	new ( ELeave ) CAknRadioButtonSettingPage(
        	R_ACTIVATION_SETTING_PAGE,
        	intCurrentValue,
        	items );

    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        currentValue =
        	static_cast<MusSettingsKeys::TActivation>( intCurrentValue );
        if ( currentValue == MusSettingsKeys::EActiveInHomeNetworks )
        	{
        	currentValue = MusSettingsKeys::ENever;
        	}
        iModel->SetVSSettingsActivationL( currentValue );
        Container()->UpdateListBoxL( KGSSettIdVSActivation );
        }

    CleanupStack::PopAndDestroy( items );
    MUS_LOG(
    "[MUSSET] <- CMusSettingsPlugin::ShowOperatorSpecificActivationSettingDialogL()" )
    }


// ----------------------------------------------------------------------------
// Shows SIP profile setting dialog (i.e. "use default profile" or "select
// profile from list"). If select profile from list is selected, a list of
// SIP profiles is provided for user to choose wanted SIP profile.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowVSSettingsProfileSettingDialogL()
    {
    MUS_LOG(
    "[MUSSET] -> CMusSettingsPlugin::ShowVSSettingsProfileSettingDialogL()" )
    TInt cenRepValue = iModel->VSSettingsProfileL();
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
        		iModel->SetVSSettingsProfileL( 
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

// ----------------------------------------------------------------------------
// Provides user a list of SIP profiles to select from. If no SIP profiles
// exist an error note is displayed.
// ----------------------------------------------------------------------------
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
    	    iModel->VSSettingsProfileL() );
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
	    	    iModel->SetVSSettingsProfileL( newValue );
	    	    Container()->ShowNewProfileActiveAfterCallL();
				Container()->UpdateListBoxL( KGSSettIdSIPProfile );
    			}
			}
		}

	CleanupStack::PopAndDestroy( array );  // array
    }


// ----------------------------------------------------------------------------
// Provides a dialog for user to choose saving location for recorderded video.
// (locations are naturally phone memory or memory card).
// ----------------------------------------------------------------------------
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

    // Use ECFDDialogTypeSave to have double list box in the query
/*	CAknMemorySelectionDialog* dlg = CAknMemorySelectionDialog::NewL(
                                    ECFDDialogTypeSave,
                                    R_VS_RECORDED_VIDEO_SAVING_SETTING_PAGE,
                                    EFalse );
    CleanupStack::PushL( dlg );    
*/

    

    TBool result( EFalse );
    TDriveNumber driveNumber((TDriveNumber)currentValue);    
    result = dlg->ExecuteL( driveNumber, NULL, NULL );

    if ( result != CAknCommonDialogsBase::TReturnKey(
                                        CAknCommonDialogsBase::ERightSoftkey) )
        {
        if ( ( TInt ) driveNumber != currentValue )
            {
            iModel->SetVSSettingsRecordedVideoSavingL( ( TInt ) driveNumber );
            }
        Container()->UpdateListBoxL( KGSSettIdRecordedVideoSaving );
        }

/*  CAknMemorySelectionDialog::TMemory mem;
    if ( currentValue == ( TInt )mmcUnit )
        {
        mem = CAknMemorySelectionDialog::EMemoryCard;
        }
    else
        {
        mem = CAknMemorySelectionDialog::EPhoneMemory;
        }
                    
    TFileName ignore;
    TFileName path;

    if ( dlg->ExecuteL( mem, &path, &ignore ) )
        {
        if ( mem == CAknMemorySelectionDialog::EPhoneMemory 
        	&& currentValue != ( TInt ) phoneMemUnit )
        	{
        	iModel->SetVSSettingsRecordedVideoSavingL( ( TInt )phoneMemUnit );
        	Container()->UpdateListBoxL( KGSSettIdRecordedVideoSaving );
        	}	
        else if ( mem == CAknMemorySelectionDialog::EMemoryCard
        	&& currentValue != ( TInt )mmcUnit )
        	{
        	iModel->SetVSSettingsRecordedVideoSavingL( ( TInt )mmcUnit );
        	Container()->UpdateListBoxL( KGSSettIdRecordedVideoSaving );
        	}	
        }
*/

    CleanupStack::PopAndDestroy(dlg); 
    MUS_LOG(
    "[MUSSET] <- CMusSettingsPlugin::ShowVSSettingsRecordedVideoSavingSettingDialogL()" )
    }


// ----------------------------------------------------------------------------
// In standard variant provides user a "Capability auditory note" setting
// dialog, and in operator variant provides user an "Alerts" setting dialog.
// Note that in both variants the different dialogs toggle the same setting.
// ----------------------------------------------------------------------------
//
void CMusSettingsPlugin::ShowVSSettingsNoteSettingDialogL()
    {
    MUS_LOG(
    "[MUSSET] -> CMusSettingsPlugin::ShowVSSettingsNoteSettingDialogL()" )
    MusSettingsKeys::TAuditoryNotification currentValue =
    	iModel->VSSettingsNoteL();
    TInt intCurrentValue = static_cast<TInt>( currentValue );

    CAknRadioButtonSettingPage* dlg;
    CDesCArrayFlat* items;

    if ( iModel->VSSettingsOperatorVariantL() == MusSettingsKeys::EStandard )
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
        	static_cast<MusSettingsKeys::TAuditoryNotification>
        	    ( intCurrentValue );
        iModel->SetVSSettingsNoteL( currentValue );
        Container()->UpdateListBoxL( KGSSettIdNote );
        }

    CleanupStack::PopAndDestroy( items );
    MUS_LOG(
    "[MUSSET] <- CMusSettingsPlugin::ShowVSSettingsNoteSettingDialogL()" )
    }


// ----------------------------------------------------------------------------
// Shows a notifications that no SIP profiles exists.
// ----------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------
// Switches between two possible values from one to another (i.e. toggles a
// setting on/off). Toggled setting is passed in aValue parameter.
// ---------------------------------------------------------------------------
//
void CMusSettingsPlugin::SwitchOnOffValueL( TInt aValue )
    {
    switch( aValue )
	    {
	    case KGSSettIdVSActivation:
	    	{
	    	if ( MusSettingsKeys::EAlwaysActive
	    		== iModel->VSSettingsActivationL() )
	    		{
	    		iModel->SetVSSettingsActivationL( MusSettingsKeys::ENever );
	    		}
	    	else
	    		{
	    		iModel->SetVSSettingsActivationL(
	    			MusSettingsKeys::EAlwaysActive );
	    		}
	    	break;
	    	}
	    case KGSSettIdAutoRecord:
	        {
	        if ( MusSettingsKeys::EAutoRecordOff ==
	                                iModel->VSSettingsAutoRecordL() )
		    	{
		    	iModel->SetVSSettingsAutoRecordL( 
		    	                    MusSettingsKeys::EAutoRecordOn );
		    	}
			else
				{
				iModel->SetVSSettingsAutoRecordL(
					                MusSettingsKeys::EAutoRecordOff );
				}
	        break;
	        }
	    case KGSSettIdRecordedVideoSaving:
	    	{
		    TDriveUnit phoneMemUnit( 
		            TParsePtrC( PathInfo::PhoneMemoryRootPath() ).Drive() );
            TDriveUnit mmcUnit( 
                    TParsePtrC( PathInfo::MemoryCardRootPath() ).Drive() );    
    
		    if ( ( TInt )phoneMemUnit 
		        == iModel->VSSettingsRecordedVideoSavingL() )
		    	{
		    	iModel->SetVSSettingsRecordedVideoSavingL( ( TInt )mmcUnit );
		    	}
			else
				{
				iModel->SetVSSettingsRecordedVideoSavingL( ( TInt )phoneMemUnit );
				}
			break;
	    	}
		case KGSSettIdNote:
			{
		    if ( MusSettingsKeys::EAuditoryNotificationOn
		    	== iModel->VSSettingsNoteL() )
		    	{
		    	iModel->SetVSSettingsNoteL(
		    		MusSettingsKeys::EAuditoryNotificationOff );
		    	}
			else
				{
				iModel->SetVSSettingsNoteL(
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

// ----------------------------------------------------------------------------
// From class CGSPluginInterface.
// Creates a new icon of desired type. Overrided to provide custom icons.
// Ownership of the created icon is transferred to the caller.
// ----------------------------------------------------------------------------
//
CGulIcon* CMusSettingsPlugin::CreateIconL( const TUid aIconType )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsPlugin::CreateIconL()" )

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

