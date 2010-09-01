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
* Description:  Model for managing SIP profiles.
*  Version     : %version: 19 % << Don't touch! Updated by Synergy at check-out.
*
*/


#include <sipmanagedprofileregistry.h>
#include <sipmanagedprofile.h>

#include "mussipprofilemodel.h"
#include "muslogger.h"
#include "mussettings.inl"
#include "mussettingsmodel.h"


const TInt KUnknownProfileId = 0;


// ======== MEMBER FUNCTIONS ========


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
CMusSIPProfileModel::CMusSIPProfileModel()
    {
    }

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::ConstructL()
    {
    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::ConstructL()" )
            
    MUS_LOG( "[MUSSET]  Creating CSIPManagedProfileRegistry " )
    iEngine = CSIPManagedProfileRegistry::NewL( *this );
    MUS_LOG( "[MUSSET]  Done " )
    MUS_LOG( "[MUSSET]  Getting profiles " )
    iEngine->ProfilesL( iProfiles );
    MUS_LOG( "[MUSSET]  Done " )
    SortProfilesL();
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::ConstructL()" )
    }

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
CMusSIPProfileModel* CMusSIPProfileModel::NewL()
    {
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::NewL()" )
    CMusSIPProfileModel* self = new( ELeave ) CMusSIPProfileModel();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::NewL()" )
    return self;
    }

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
CMusSIPProfileModel::~CMusSIPProfileModel()
    {
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::~CMusSIPProfileModel()" )
    iProfiles.ResetAndDestroy();
    delete iEngine;
    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::~CMusSIPProfileModel()" )
    }

// ----------------------------------------------------------------------------
// Returns reference to the locally cached SIP profile array.
// ----------------------------------------------------------------------------
//
RPointerArray<CSIPProfile>& CMusSIPProfileModel::ProfileArrayL()
    {
    MUS_LOG( "[MUSSET] <-> CMusSIPProfileModel::ProfileArrayL()" )
    return iProfiles;
    }

// ----------------------------------------------------------------------------
// Returns index to the default SIP profile in locally cached array.
// ----------------------------------------------------------------------------
//
TInt CMusSIPProfileModel::DefaultProfileIndex()
    {
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::DefaultProfileIndex()" )
    TInt index( KErrNotFound );

    for ( TInt i = 0; i < iProfiles.Count(); i++ )
        {
        TBool defProfile( EFalse );
        if (
    	    KErrNone ==
    	    iProfiles[i]->GetParameter( KSIPDefaultProfile, defProfile )
    	    && defProfile )
            {
            index = i;
            break;
            }
        }

    MUS_LOG1( "[MUSSET] <- CMusSIPProfileModel::DefaultProfileIndex()( %d )",
              index )
    return index;
    }

// ----------------------------------------------------------------------------
// Returns id of the default SIP profile. If default SIP profile is not found
// KUnknownProfileId is returned.
// ----------------------------------------------------------------------------
//
TUint32 CMusSIPProfileModel::DefaultProfileId()
    {
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::DefaultProfileId()" )
    TUint32 id( KUnknownProfileId );

    for ( TInt i = 0; i < iProfiles.Count(); i++ )
        {
        TBool defProfile( EFalse );
        TInt error = iProfiles[i]->GetParameter( KSIPDefaultProfile, defProfile );
        if ( error == KErrNone && defProfile )
            {
            iProfiles[i]->GetParameter( KSIPProfileId, id );
            break;
            }
        }

    MUS_LOG1( "[MUSSET] <- CMusSIPProfileModel::DefaultProfileId()( %d )",
              id )
    return id;
    }

// -----------------------------------------------------------------------------
// Disable SIP Registration.
// -----------------------------------------------------------------------------
//
void CMusSIPProfileModel::DisableProfileL()
    {
    MUS_LOG( "[MUSSET]  -> CMusSIPProfileModel::DisableProfileL " )        
    CSIPManagedProfile* managedProfile = static_cast<CSIPManagedProfile*>(
            iEngine->ProfileL( DefaultProfileId()) );
    CleanupStack::PushL( managedProfile );
    MUS_LOG( "[MUSSET]  Add registration parameter to profile.dat file " )
    //set autoregistration off (i.e. when needed) => disable profile
    User::LeaveIfError( managedProfile->SetParameter( KSIPAutoRegistration, EFalse ) );
    MUS_LOG( "[MUSSET]  Saving profile " )
    iEngine->SaveL( *managedProfile );
    MUS_LOG( "[MUSSET]  Saved" )
    CleanupStack::PopAndDestroy( managedProfile );
    MUS_LOG( "[MUSSET]  <- CMusSIPProfileModel::DisableProfileL " )        
    }

// -----------------------------------------------------------------------------
// Enable SIP Registration.
// -----------------------------------------------------------------------------
//
void CMusSIPProfileModel::EnableProfileL()
    {
    MUS_LOG( "[MUSSET]  -> CMusSIPProfileModel::EnableProfileL " )        
    CSIPManagedProfile* managedProfile = static_cast<CSIPManagedProfile*>(
            iEngine->ProfileL( DefaultProfileId() ) );
    CleanupStack::PushL( managedProfile );
    MUS_LOG( "[MUSSET] Add registration parameters to profile.dat file " )
    //set autoregistration on (i.e. always on) => enable profile
    User::LeaveIfError( managedProfile->SetParameter( KSIPAutoRegistration, ETrue ) );
    MUS_LOG( "[MUSSET]  Saving profile " )
    iEngine->SaveL( *managedProfile );
    MUS_LOG( "[MUSSET]  Saved" )
    CleanupStack::PopAndDestroy( managedProfile );
    MUS_LOG( "[MUSSET]  <- CMusSIPProfileModel::EnableRegisterL " )        
    }

// -----------------------------------------------------------------------------
// if SIP Registration enabled.
// -----------------------------------------------------------------------------
//
TBool CMusSIPProfileModel::ProfileEnabledL()
    {
    MUS_LOG( "[MUSSET]  -> CMusSIPProfileModel::ProfileEnabledL " ) 
    CSIPManagedProfile* managedProfile = static_cast<CSIPManagedProfile*>(
            iEngine->ProfileL( DefaultProfileId() ) );
    CleanupStack::PushL( managedProfile );
    TBool enabled;
    MUS_LOG( "[MUSSET]  Getting KSIPAutoRegistration" ) 
    User::LeaveIfError( 
            managedProfile->GetParameter( KSIPAutoRegistration, enabled ) );
    MUS_LOG( "[MUSSET]  Done" ) 
    MUS_LOG1( "[MUSSET]  KSIPAutoRegistration=%d", enabled ) 
    CleanupStack::PopAndDestroy( managedProfile );
    MUS_LOG( "[MUSSET]  <- CMusSIPProfileModel::ProfileEnabledL " )
    return enabled;
    }

// ----------------------------------------------------------------------------
// Returns index of the default SIP profile on locally cached array. If default
// SIP profile is not found, KErrNotFound is returned.
// ----------------------------------------------------------------------------
//
TInt CMusSIPProfileModel::ProfileIndexByIdL( TUint32 aId )
    {
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::ProfileIndexByIdL()" )
    TInt index( KErrNotFound );
    
    for ( TInt i = 0; i < iProfiles.Count(); i++ )
        {
        TUint32 id( KUnknownProfileId );
        TInt error = iProfiles[i]->GetParameter( KSIPProfileId, id );
        if ( error == KErrNone && id == aId )
            {
            index = i;
            break;
            }
        }

    MUS_LOG1( "[MUSSET] <- CMusSIPProfileModel::ProfileIndexByIdL()( %d )",
              index )
    return index;
    }


// ----------------------------------------------------------------------------
// Returns profile ID by index.
// ----------------------------------------------------------------------------
//
TUint32 CMusSIPProfileModel::ProfileIdByIndex( TUint aIndex )
    {
    MUS_LOG1( "[MUSSET] -> CMusSIPProfileModel::ProfileIdByIndex()( %d )",
              aIndex )
	TUint32 profileId( KUnknownProfileId );
    if ( aIndex < iProfiles.Count() )
        {
        if ( iProfiles[aIndex]->GetParameter( KSIPProfileId, profileId ) )
            {
            profileId = KUnknownProfileId;
            }
        }

    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::ProfileIdByIndex()" )
    return profileId;
    }

// ----------------------------------------------------------------------------
// Sorts internal array of SIP profiles by id. Used algorithm is generally
// slower than quicksort and selectionsort but very feasible for expected
// amount of items to be sorted and complexity vise.
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::SortProfilesL()
    {
	TUint32 profileIdFirst( 0 );
	TUint32 profileIdSecond( 0 );
    for ( TInt a = 0; a < iProfiles.Count() - 1; a++ )
        {
        for ( TInt b = a + 1; b < iProfiles.Count(); b++ )
            {
            User::LeaveIfError( iProfiles[a]->GetParameter(
            	KSIPProfileId, profileIdFirst ) );
    		User::LeaveIfError( iProfiles[b]->GetParameter(
    			KSIPProfileId, profileIdSecond ) );
            if ( profileIdFirst > profileIdSecond )
                {
                CSIPProfile* tmp = iProfiles[b];
                iProfiles[b] = iProfiles[a];
                iProfiles[a]  = tmp;
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// From class MSIPProfileRegistryObserver.
// Notifies of an event in SIP profile registry.
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::ProfileRegistryEventOccurred(
    TUint32 aSIPProfileId, TEvent aEvent )
	{  
    MUS_LOG("[MUSSET] -> CMusSIPProfileModel::ProfileRegistryEventOccurred()" )
    MUS_LOG2("            SIPProfileId is %d,Event is %d",aSIPProfileId,aEvent)
    
    // We revert back to default settings if the profile used by MuS is deleted
    if ( aEvent == EProfileDestroyed )
    	{
    	TRAP_IGNORE( 
            //if the profile is the profile used by mush
            if ( MultimediaSharingSettings::SipProfileSettingL() ==
                 aSIPProfileId )
                {
                //set the profile to default	
                MultimediaSharingSettings::SetSipProfileSettingL( 
                                    CMusSettingsModel::KVsSipProfileDefault );
                //set mush off
                MultimediaSharingSettings::SetActivationSettingL(
                                    MusSettingsKeys::EActiveInHomeNetworks );
                }
            );
		 }
    
    if ( aEvent == EProfileCreated ||
         aEvent == EProfileUpdated ||
         aEvent == EProfileDestroyed )
        {
        // Profiles have been manipulated in SIP side, we must delete client
        // side profile objects and retrieve them again. If fetching fails,
        // we have empty array which is better than showing wrong values to
        // user.
        iProfiles.ResetAndDestroy();
        TRAP_IGNORE( iEngine->ProfilesL( iProfiles ) );
        TRAP_IGNORE( SortProfilesL() );
        }
    
    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::ProfileRegistryEventOccurred()" )
	}

// ----------------------------------------------------------------------------
// From class MSIPProfileRegistryObserver.
// An asynchronous error has occurred related to SIP profile
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::ProfileRegistryErrorOccurred(
	TUint32 /*aSIPProfileId*/,
	TInt /*aError*/ )
    {
    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::ProfileRegistryErrorOccurred()" )
    }

// ----------------------------------------------------------------------------
//Set CMusSettingsModel to handle ProfileRegistry Event.
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::SetCMusSettingsModel(CMusSettingsModel* aCMusSettingsModel)
	{
	iCMusSettingsModel = aCMusSettingsModel;
	}
