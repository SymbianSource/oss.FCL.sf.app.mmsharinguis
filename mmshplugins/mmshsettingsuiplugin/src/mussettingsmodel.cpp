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
* Description:  Document model class for MUSSettingsPlugin.
*  Version     : %version: 17 % << Don't touch! Updated by Synergy at check-out.
*
*/


#include    "mussettingsmodel.h"
#include    "muslogger.h"
#include    "mussipprofilemodel.h"
#include    "mussettings.h"
#include    "mmussipprofilehandler.h"

#include    <escapeutils.h>
#include    <centralrepository.h>
#include    <CAknMemorySelectionDialogMultiDrive.h>
#include    <AknCommonDialogsDynMem.h>
#include    <mussettingsuirsc.rsg> // GUI Resource



const TInt CMusSettingsModel::KVsSipProfileDefault      = 0;
const TInt CMusSettingsModel::KVsSipProfileSelect       = 1;
const TInt CMusSettingsModel::KVsSipProfileSelectNone   = 2;
const TInt KSIPGranularity = 5;


// ======== MEMBER FUNCTIONS ========


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusSettingsModel* CMusSettingsModel::NewL( MMusSIPProfileHandler& aHandler )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsModel::NewL()" )
    CMusSettingsModel* self = new( ELeave ) CMusSettingsModel( aHandler );
    CleanupStack::PushL( self );
    self->ConstructL();

    CleanupStack::Pop( self );
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::NewL()" )
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusSettingsModel::CMusSettingsModel( MMusSIPProfileHandler& aHandler )
    : iHandler( aHandler )
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::CMusSettingsModel()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSettingsModel::ConstructL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsModel::ConstructL()" )
    InitializeProfileEnablerL();
    static_cast<CMusSIPProfileModel*> (&iHandler)->SetCMusSettingsModel(this);
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::ConstructL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusSettingsModel::~CMusSettingsModel()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsModel::~CMusSettingsModel()" )
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::~CMusSettingsModel()" )
    }

// -----------------------------------------------------------------------------
// Returns recorded video saving mode.
// -----------------------------------------------------------------------------
//
TInt CMusSettingsModel::VSSettingsRecordedVideoSavingL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsModel::VSSettingsRecordedVideoSavingL()" )
    TInt vsVideoLocationMode = MultimediaSharingSettings::VideoLocationSettingL();
    MUS_LOG1(
    "[MUSSET] <- CMusSettingsContainer::VSSettingsRecordedVideoSavingL()( %d )",
        vsVideoLocationMode )
    return vsVideoLocationMode;
    }

// ----------------------------------------------------------------------------
// Change Activation.
// ----------------------------------------------------------------------------
//
void CMusSettingsModel::SetActivationItem( TBool aActive )
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::SetActivationItem()" )
    MUS_LOG1( "            Profile disabled? ( %d )", aActive )
    iProfileDisabled = aActive;
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::SetActivationItem()" )
    }

// ----------------------------------------------------------------------------
// Get Activation item.
// ----------------------------------------------------------------------------
//
TBool CMusSettingsModel::ActivationItem()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsContainer::SetActivationItem()" )
    MUS_LOG( "[MUSSET] <- CMusSettingsContainer::SetActivationItem()" )
    return iProfileDisabled;
    }

// -----------------------------------------------------------------------------
// Returns the list of profile names
// -----------------------------------------------------------------------------
CDesCArray* CMusSettingsModel::ListOfProfileNamesL()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsModel::ListOfProfileNamesL()" )
    RPointerArray<CSIPProfile>& list = iHandler.ProfileArrayL();

    CDesCArray* array = new ( ELeave ) CDesCArrayFlat( KSIPGranularity );
    CleanupStack::PushL( array );

    for ( TInt i = 0; i < list.Count(); i++ )
        {
        const TDesC8* providerName = 0;
        User::LeaveIfError(
            list[i]->GetParameter( KSIPProviderName, providerName ) );

        HBufC8* decodedProvider =
            EscapeUtils::EscapeDecodeL( *providerName );
        CleanupStack::PushL( decodedProvider );

        HBufC* providerName16 =
            EscapeUtils::ConvertToUnicodeFromUtf8L( decodedProvider->Des() );
        CleanupStack::PushL( providerName16 );

        array->AppendL( providerName16->Des() );

        CleanupStack::PopAndDestroy( providerName16 );
        CleanupStack::PopAndDestroy( decodedProvider );
        }

    CleanupStack::Pop( array );
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::ListOfProfileNamesL()" )
    return array;
    }


// -----------------------------------------------------------------------------
// Returns the SIP profile name. Returns NULL buffer if no profile is found.
// -----------------------------------------------------------------------------
HBufC* CMusSettingsModel::ProfileNameL( TInt aId )
    {
    MUS_LOG1(
    "[MUSSET] -> CMusSettingsContainer::ProfileNameL()( %d )",
        aId )
    const TDesC8* providerName = 0;
    TInt index = ProfileIndexByIdL( aId );
    HBufC* buf = NULL;

    if ( index >= 0 )
        {
        if ( KErrNone == iHandler.ProfileArrayL()[index]->
                GetParameter( KSIPProviderName, providerName ) )
            {
            HBufC8* decodedProvider =
                EscapeUtils::EscapeDecodeL( *providerName );
            CleanupStack::PushL( decodedProvider );
            buf = EscapeUtils::ConvertToUnicodeFromUtf8L( *providerName );
            CleanupStack::PopAndDestroy( decodedProvider );
            }
        }

    if ( buf )
        {
        MUS_LOG_TDESC( "[MUSSET] <- CMusSettingsModel::ProfileNameL(): Profile name: ", buf->Des() )
        }
    else
        {
        MUS_LOG( "[MUSSET] <- CMusSettingsModel::ProfileNameL(): No profile found! " )
        }
    return buf;
    }


// -----------------------------------------------------------------------------
// Returns default SIP profile index.
// -----------------------------------------------------------------------------
TInt CMusSettingsModel::DefaultProfileIndex()
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::DefaultProfileIndex()" )
    return iHandler.DefaultProfileIndex();
    }


// -----------------------------------------------------------------------------
// Returns id of default SIP profile.
// -----------------------------------------------------------------------------
TUint32 CMusSettingsModel::DefaultProfileId()
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::DefaultProfileId()" )
    return iHandler.DefaultProfileId();
    }


// -----------------------------------------------------------------------------
// Returns SIP profile index based on the Id of SIP profile.
// -----------------------------------------------------------------------------
TInt CMusSettingsModel::ProfileIndexByIdL( TUint32 aId )
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::ProfileIndexByIdL()" )
    return iHandler.ProfileIndexByIdL( aId );
    }


// -----------------------------------------------------------------------------
// Returns SIP profile Id based on the index of SIP profile.
// -----------------------------------------------------------------------------
TUint32 CMusSettingsModel::ProfileIdByIndex( TUint aIndex )
    {
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::ProfileIdByIndex()" )
    return iHandler.ProfileIdByIndex( aIndex );
    }

// -----------------------------------------------------------------------------
// Initialize SIP profile enabler data.
// -----------------------------------------------------------------------------
void CMusSettingsModel::InitializeProfileEnablerL() 
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsModel::InitializeProfileEnabler()" )
    if ( MultimediaSharingSettings::OperatorVariantSettingL() ==
               MusSettingsKeys::EOperatorSpecific )
       {
       TBool enabled(EFalse);
       
       CMusSIPProfileModel& handler = 
           static_cast<CMusSIPProfileModel&>( iHandler );
       TRAPD( error, enabled = handler.ProfileEnabledL());
       if ( error != KErrNone )
           {
           MUS_LOG( "Error returned" )
           // Problems with re-reading profiles; use existing array
           SetActivationItem( EFalse );
           }
       else
           {
           MUS_LOG1("SIP registration service( %d )",
           enabled )
           if( enabled )
               {
               SetActivationItem( enabled );
               }
           else
               {
               SetActivationItem( enabled);
               }
           }
       }
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::InitializeProfileEnabler()" )
    }

// -----------------------------------------------------------------------------
// Returns memory selction dialog pointer.
// -----------------------------------------------------------------------------
CAknMemorySelectionDialogMultiDrive* CMusSettingsModel::MemorySelectionDialogLC()
    {
    MUS_LOG( "[MUSSET] -> CMusSettingsModel::MemorySelectionDialogLC()" )
    TInt supportedMemTypes( AknCommonDialogsDynMem::EMemoryTypePhone|
                        AknCommonDialogsDynMem::EMemoryTypeInternalMassStorage|
                        AknCommonDialogsDynMem::EMemoryTypeMMCExternal|
                        AknCommonDialogsDynMem::EMemoryTypeRemote );    
    CAknMemorySelectionDialogMultiDrive* dlg = 
                               CAknMemorySelectionDialogMultiDrive::NewL(
                                    ECFDDialogTypeNormal,
                                    R_VS_RECORDED_VIDEO_SAVING_SETTING_PAGE,
                                    ETrue,
                                    supportedMemTypes );
    CleanupStack::PushL( dlg );
    MUS_LOG( "[MUSSET] <- CMusSettingsModel::MemorySelectionDialogLC()" )
    return dlg;
    }

