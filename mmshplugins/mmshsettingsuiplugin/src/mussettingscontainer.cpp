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
* Description:  Container for MUSSettingsPlugin.
*  Version     : %version: be1sipx1#22 % << Don't touch! Updated by Synergy at check-out.
*
*/



#include "mussettingscontainer.h"
#include "mussettingsplugin.h"
#include "mussettingsmodel.h"
#include "gslistbox.h"
#include "mussettingsplugin.hrh"
#include <mussettingsuirsc.rsg>
#include <aknlists.h>
#include <akntitle.h>
#include <aknnotewrappers.h>
#include <csxhelp/msh.hlp.hrh>
#include <gsfwviewuids.h>
#include <StringLoader.h>
#include <sipmanagedprofile.h>
#include <ctsydomainpskeys.h>
#include <e32property.h>
#include <pathinfo.h>


#ifdef RD_MULTIPLE_DRIVE
#include    <CAknMemorySelectionDialogMultiDrive.h>

#endif

/** default SIP profile is selected */
const TInt KDefaultSipProfile = 0;

/** No SIP profile is selected */
const TInt KNoSipProfileSelected = -1;

// UID of Multimedia Sharing.
// This UID is used for accessing the help IDs controlled by MUSH help files
const TUid KUidMUSH = { 0x10282391 };

// ======== MEMBER FUNCTIONS ========


CMusSettingsContainer::CMusSettingsContainer( CMusSettingsModel& aModel ) :
	iModel( aModel )
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::CMusSettingsContainer()" )
    }


void CMusSettingsContainer::ConstructL( const TRect& aRect )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::ConstructL()" )

    iOperatorVariant = MultimediaSharingSettings::OperatorVariantSettingL();

    iListBox = new( ELeave ) CAknSettingStyleListBox;

    if ( iOperatorVariant == MusSettingsKeys::EStandard )
    	{
    	BaseConstructL( aRect, R_GS_VSSETTINGS_VIEW_TITLE, R_VS_LBX );
    	}
    else
    	{
    	BaseConstructL( aRect, R_GS_VSSETTINGS_VIEW_TITLE, R_VS_LBX_OPERATOR );
    	}

    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::ConstructL()" )
    }


CMusSettingsContainer::~CMusSettingsContainer()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::~CMusSettingsContainer()" )
    delete iActivationItems;
    delete iProfileItems;
    delete iAutoRecordItems;
    delete iNoteItems;
    delete iListboxItemArray;
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::~CMusSettingsContainer()" )
    }


// -----------------------------------------------------------------------------
// Constructs a listbox from a specified resource id.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::ConstructListBoxL( TInt aResLbxId )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::ConstructListBoxL()" )
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    iListboxItemArray = CGSListBoxItemTextArray::NewL(
        aResLbxId, *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    // OCC: removed "Automatic in home network" choice
    iActivationItems =
            iCoeEnv->ReadDesC16ArrayResourceL(
            R_OPERATOR_ACTIVATION_SETTING_PAGE_LBX );

    iAutoRecordItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                    R_VS_VIDEO_SAVING_SETTING_PAGE_LBX );

    iProfileItems = iCoeEnv->ReadDesC16ArrayResourceL(
    	R_SIP_PROFILE_SETTING_PAGE_LBX );

    iNoteItems = iCoeEnv->ReadDesC16ArrayResourceL(
							R_VS_AUDIO_SETTING_PAGE_LBX );
    
    CreateListBoxItemsL();

    // Operator variant mode needs some item should not be visible to user.
    // Hide those items.
    if ( iOperatorVariant == MusSettingsKeys::EOperatorSpecific )
    	{        
        // Hide VS Activation Settings. If needed in future it can be made
        // visible.
    	// 
        //HideItemsL( KGSSettIdVSActivation );
        // Hide SIP Profile choosing option.If needed in future it can be made
        // visible.
        HideItemsL( KGSSettIdSIPProfile );
        }    
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::ConstructListBoxL()" )
    }


// -----------------------------------------------------------------------------
// Populates listbox items.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::CreateListBoxItemsL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::CreateListBoxItemsL()" )
    if ( iOperatorVariant == MusSettingsKeys::EStandard )
    	{
    	MakeActivationItemL();
    	}
    else
    	{
    	MakeOperatorActivationItemL();
    	}
    MakeSIPProfileItemL();
    MakeAutoRecordItemL();
    MakeRecordedVideoSavingItemL();
    MakeNoteItemL();
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::CreateListBoxItemsL()" )
    }


// -----------------------------------------------------------------------------
// Updates a specified feature (item in listbox).
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::UpdateListBoxL( TInt aFeatureId )
    {
    MUS_LOG1( "[MUSSET] <- CMusSettingsContainer::UpdateListBoxL()( %d )",
              aFeatureId )
    switch ( aFeatureId )
        {
        case KGSSettIdVSActivation:
            if ( iOperatorVariant == MusSettingsKeys::EStandard )
            	{
            	MakeActivationItemL();
            	}
            else
            	{
            	MakeOperatorActivationItemL();
            	}
            break;
        case KGSSettIdSIPProfile:
            MakeSIPProfileItemL();
            break;
        case KGSSettIdAutoRecord:
            MakeAutoRecordItemL();
            break;
        case KGSSettIdRecordedVideoSaving:
            MakeRecordedVideoSavingItemL();
            break;
        case KGSSettIdNote:
            MakeNoteItemL();
            break;
        default:
            break;
        }

    iListBox->HandleItemAdditionL();
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::UpdateListBoxL()" )
    }


// -----------------------------------------------------------------------------
// Generic method to add an item to listbox and make it visible.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::AddItemL( TInt aId, const TPtrC aText )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::AddItemL()" )
    iListboxItemArray->SetDynamicTextL( aId, aText );
    iListboxItemArray->SetItemVisibilityL( aId,
    CGSListBoxItemTextArray::EVisible );
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::AddItemL()" )
    }


// -----------------------------------------------------------------------------
// Adds activation setting item to listbox.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::MakeActivationItemL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::MakeActivationItemL()" )

    MusSettingsKeys::TActivation activation = 
                                MultimediaSharingSettings::ActivationSettingL();
 
    if ( activation != MusSettingsKeys::EAlwaysActive )
        {
        // We make sure that if someone has written to CenRep value bigger
        // than 1 (which after OCC changes means 'Off') we treat it as 'Off'.
        activation = MusSettingsKeys::EActiveInHomeNetworks;
        }
    
    AddItemL( KGSSettIdVSActivation, ( *iActivationItems )[ activation ] );

    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::MakeActivationItemL()" )
    }


// -----------------------------------------------------------------------------
// Adds operator specific activation setting item to listbox.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::MakeOperatorActivationItemL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::MakeOperatorActivationItemL()" )
            
    //sync with real value, if op specifig
    iModel.InitializeProfileEnablerL();
    
    MUS_LOG1( "Activation item=%d (0=VS enabled, 1=VS disabled)", iModel.ActivationItem() )
    AddItemL( KGSSettIdVSActivation, ( *iActivationItems )[ iModel.ActivationItem() ] );

    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::MakeOperatorActivationItemL()" )
    }


// -----------------------------------------------------------------------------
// Adds SIP profile setting item to settings listbox. Item will contain
// indication of [no SIP profile selected], [using default SIP profile] or
// [name of SIP profile specified to be used]
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::MakeSIPProfileItemL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::MakeSIPProfileItemL()" )

    TInt profile = MultimediaSharingSettings::SipProfileSettingL();

    if ( profile != KDefaultSipProfile && profile != KNoSipProfileSelected )
    	{
        // profile id defined, get profile name
        HBufC* name = iModel.ProfileNameL( profile );

        if ( name )
            {
            CleanupStack::PushL( name );
            AddItemL( KGSSettIdSIPProfile, *name );
            CleanupStack::PopAndDestroy( name );
            }
    	else
            {
            // show profile value not selected
            AddItemL( KGSSettIdSIPProfile,
                    ( *iProfileItems )[ CMusSettingsModel::KVsSipProfileSelectNone ] );
                    }
            }
    else if ( profile == KNoSipProfileSelected )
    	{
    	AddItemL( KGSSettIdSIPProfile,
    		( *iProfileItems )[ CMusSettingsModel::KVsSipProfileSelectNone ] );
        }
    else
    	{
        AddItemL(
            KGSSettIdSIPProfile,
            ( *iProfileItems )[ CMusSettingsModel::KVsSipProfileDefault ] );
            }

    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::MakeSIPProfileItemL()" )
    }


// -----------------------------------------------------------------------------
// Adds autorecord item to listbox.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::MakeAutoRecordItemL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::MakeAutoRecordItemL()" )
    TInt autoRecordMode = MultimediaSharingSettings::AutoRecordSettingL();
    
    MUS_LOG1( "[MUSSET]    autoRecordMode: %d", autoRecordMode  )
    MUS_LOG1( "[MUSSET]    iAutoRecordItems.Count: %d", iAutoRecordItems->Count() )

    if ( autoRecordMode > iAutoRecordItems->Count() - 1 )
        {
        MUS_LOG2( "[MUSSET] autoRecordMode too big: %d, should be max %d => leave", 
                autoRecordMode,  iAutoRecordItems->Count() -1 );
        User::Leave( KErrOverflow );
        }

    AddItemL( KGSSettIdAutoRecord, ( *iAutoRecordItems )[ autoRecordMode ] );
    
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::MakeAutoRecordItemL()" )
    }


// -----------------------------------------------------------------------------
// Adds recorded video saving item to listbox.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::MakeRecordedVideoSavingItemL()
    {
    MUS_LOG(
    	"[MUSSET] -> CMusSettingsContainer::MakeRecordedVideoSavingItemL()" )

    TInt savingMode = iModel.VSSettingsRecordedVideoSavingL();

    CAknMemorySelectionDialogMultiDrive* dlg = 
                                        iModel.MemorySelectionDialogLC();
    TDriveNumber driveNumber = ( TDriveNumber )savingMode;
    TInt index = dlg->FindIndexByDrive( driveNumber );
    
    if ( index == KErrNotFound )
        {
        MUS_LOG( "[MUSSET]    Preferred drive doesn't exist anymore, " )
        MUS_LOG( "[MUSSET]    e.g. remote drive deleted, using system drive" )

        driveNumber = RFs::GetSystemDrive();
        index = dlg->FindIndexByDrive( driveNumber );
        MultimediaSharingSettings::SetVideoLocationSettingL( driveNumber );
        }

    TFileName item( KNullDesC );  
    dlg->GetItem( index, item );
    MUS_LOG1( "[MUSSET]     Current drive:%d", driveNumber )
    MUS_LOG_TDESC( "[MUSSET]     pref. mem: " , item ) 
    
    AddItemL( KGSSettIdRecordedVideoSaving, item );
    CleanupStack::PopAndDestroy( dlg );

    MUS_LOG(
    	"[MUSSET] <- CMusSettingsContainer::MakeRecordedVideoSavingItemL()" )
    }


// -----------------------------------------------------------------------------
// Adds note item to listbox. (Alerts setting). Present only with operator
// variant set.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::MakeNoteItemL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::MakeNoteItemL()" )

    MusSettingsKeys::TAuditoryNotification auditoryNotification = 
                    MultimediaSharingSettings::AuditoryNotificationSettingL();

    MUS_LOG1( "[MUSSET] -> CMusSettingsContainer::Mode:%d()", 
              auditoryNotification )

    // If unset, use and set default value
    if ( auditoryNotification < 0 || auditoryNotification > 1 )
        {
        auditoryNotification = MusSettingsKeys::EAuditoryNotificationOff;
        MultimediaSharingSettings::SetAuditoryNotificationSettingL(
                                    MusSettingsKeys::EAuditoryNotificationOff );
        }

    AddItemL( KGSSettIdNote, ( *iNoteItems )[ auditoryNotification ] );
	
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::MakeNoteItemL()" )
    }


// -----------------------------------------------------------------------------
// Gets help context.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::GetHelpContext()" )
    aContext.iMajor = KUidMUSH;
    aContext.iContext = KMSH_HLP_SETTINGS;
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::GetHelpContext()" )
    }


// -----------------------------------------------------------------------------
// Returns currently selected feature (listbox item).
// -----------------------------------------------------------------------------
//
TInt CMusSettingsContainer::CurrentFeatureId() const
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::CurrentFeatureId()" )
    return iListboxItemArray->CurrentFeature( );
    }


// -----------------------------------------------------------------------------
// If call is on, informs user with note that new profile is going to be active
// after current call.
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::ShowNewProfileActiveAfterCallL()
    {
    MUS_LOG(
    	"[MUSSET] -> CMusSettingsContainer::ShowNewProfileActiveAfterCallL()" )
    TPSCTsyCallState callState;
    User::LeaveIfError( RProperty::Get( KPSUidCtsyCallInformation,
                    KCTsyCallState,
                    (TInt&)callState) );
    
    if ( callState == EPSCTsyCallStateAlerting ||
         callState ==  EPSCTsyCallStateRinging ||
         callState ==  EPSCTsyCallStateDialling ||
         callState ==  EPSCTsyCallStateAnswering ||
         callState ==  EPSCTsyCallStateDisconnecting ||
         callState ==  EPSCTsyCallStateConnected ||
         callState ==  EPSCTsyCallStateHold)
        {
        HBufC* infoTxt = StringLoader::LoadLC( R_QTN_MSH_SET_PROFILE_NOTE );
        CAknInformationNote* note = new ( ELeave ) CAknInformationNote( ETrue );
        note->ExecuteLD( infoTxt->Des() );
        CleanupStack::PopAndDestroy( infoTxt );
        }
    
    MUS_LOG(
        "[MUSSET] <- CMusSettingsContainer::ShowNewProfileActiveAfterCallL()" )
    }

// -----------------------------------------------------------------------------
// void CMusSettingsContainer::HideItemsL(TInt aItemIndex)
// Some items are unwanted by operators.Hide those items from listbox item
// -----------------------------------------------------------------------------
//
void CMusSettingsContainer::HideItemsL(TInt aItemIndex)
    {
    MUS_LOG1("[MUSSET] -> CMusSettingsContainer::HideItemsL() %d",aItemIndex )    
    iListboxItemArray->SetItemVisibilityL( aItemIndex,
            	CGSListBoxItemTextArray::EInvisible );
    MUS_LOG("[MUSSET] <- CMusSettingsContainer::HideItemsL()" )	
    }

