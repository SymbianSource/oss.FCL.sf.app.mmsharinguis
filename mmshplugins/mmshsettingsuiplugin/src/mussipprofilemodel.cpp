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
*  Version     : %version: 14 % << Don't touch! Updated by Synergy at check-out.
*
*/



#include "mussipprofilemodel.h"
#include "muslogger.h"
#include "mussettings.inl"
#include <aknnotewrappers.h>
#include <StringLoader.h>
#include <aknview.h>
#include <sipmanagedprofileregistry.h>
#include <sipprofileregistry.h>


const TInt KSIPGranularity   = 5;
const TInt KUnknownProfileId = 0;

/** 
* VSC Enable/Disable is of type TBool
*/
const TUint32 KMuSVSCDisable = 200;


// ======== MEMBER FUNCTIONS ========


CMusSIPProfileModel::CMusSIPProfileModel()
    {
    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::CMusSIPProfileModel()" )
    }


void CMusSIPProfileModel::ConstructL()
    {
    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::ConstructL()" )
    iEngine = CSIPManagedProfileRegistry::NewL( *this );
    ReadArrayFromEngineL();
    }


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


CMusSIPProfileModel::~CMusSIPProfileModel()
    {
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::~CMusSIPProfileModel()" )
    DeleteProfiles();
    delete iEngine;
    iEngine = NULL;
    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::~CMusSIPProfileModel()" )
    }


// ----------------------------------------------------------------------------
// Returns pointer to the locally cached SIP profile array.
// ----------------------------------------------------------------------------
//
CArrayPtr<CSIPManagedProfile>* CMusSIPProfileModel::ProfileArrayL()
    {
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::ProfileArrayL()" )
    ReadProfileListFromEngineSafeL();
    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::ProfileArrayL()" )
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

    for ( TInt i = 0; i < iProfiles->Count(); i++ )
        {
        TBool defProfile( EFalse );
        if (
    	    KErrNone ==
    	    iProfiles->At( i )->GetParameter( KSIPDefaultProfile, defProfile )
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

    for ( TInt i = 0; i < iProfiles->Count(); i++ )
        {
        TBool defProfile( EFalse );
        if (
    	    KErrNone ==
    	    iProfiles->At( i )->GetParameter( KSIPDefaultProfile, defProfile )
    	    && defProfile )
            {
        	iProfiles->At( i )->GetParameter( KSIPProfileId, id );
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
    //Disable registration from profile.dat file
    MUS_LOG( "Add registration parameter profile.dat file " )
    TInt err = managedProfile->SetParameter( KMuSVSCDisable, (TBool)ETrue );
    User::LeaveIfError(err);
    iEngine->SaveL( *managedProfile );
    CleanupStack::PopAndDestroy( managedProfile );
    MUS_LOG( "[MUSSET]  <- CMusAvaRegisterAvailability::DisableProfileL " )        
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
    //Disable registration from profile.dat file
    MUS_LOG( "Add registration parameter profile.dat file " )
    TInt err = managedProfile->SetParameter( KMuSVSCDisable, (TBool)EFalse );
    User::LeaveIfError(err);
    iEngine->SaveL( *managedProfile );
    CleanupStack::PopAndDestroy( managedProfile );
    MUS_LOG( "[MUSSET]  <- CMusAvaRegisterAvailability::EnableRegisterL " )        
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
    //Activation status from profile.dat file
    TBool enabled = EFalse;
    MUS_LOG( "Get registration parameter from profile.dat file " )
    TInt err = managedProfile->GetParameter( KMuSVSCDisable, enabled ); 
    MUS_LOG1( "KMuSVSCDisable value %d )",
            enabled )
    CleanupStack::PopAndDestroy( managedProfile );
    MUS_LOG( "[MUSSET]  <- CMusAvaRegisterAvailability::ProfileEnabledL " )
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

	ReadProfileListFromEngineSafeL();
    for ( TInt i = 0; i < iProfiles->Count(); i++ )
        {
        TUint32 id( KUnknownProfileId );

        if ( KErrNone == iProfiles->At( i )->GetParameter( KSIPProfileId, id )
            && id == aId )
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
    if ( iProfiles->Count() > aIndex )
        {
        if ( iProfiles->At( aIndex )->GetParameter(
            KSIPProfileId, profileId ) )
            {
            profileId = KUnknownProfileId;
            }
        }

    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::ProfileIdByIndex()" )
    return profileId;
    }


// ----------------------------------------------------------------------------
// Reads SIP profiles from SIP Profile Client array to locally cached array.
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::ReadArrayFromEngineL()
    {
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::ReadArrayFromEngineL()" )

    DeleteProfiles();

    // Create the profile pointer array
    iProfiles =
        new ( ELeave ) CArrayPtrFlat<CSIPManagedProfile>( KSIPGranularity );

    RPointerArray<CSIPProfile> profilePointerArray;
    TCleanupItem clItem( ResetAndDestroy, &profilePointerArray );
    CleanupStack::PushL( clItem );

    iEngine->ProfilesL( profilePointerArray );
    for ( TInt i = 0; i < profilePointerArray.Count(); i++ )
        {
        iProfiles->AppendL(
            static_cast<CSIPManagedProfile*>( profilePointerArray[i] ) );
        }

    profilePointerArray.Reset();
    CleanupStack::PopAndDestroy(); // clItem (profilePointerArray)

    SortProfilesL();

    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::ReadArrayFromEngineL()" )
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
    for ( TInt a = 0; a < iProfiles->Count() - 1; a++ )
        {
        for ( TInt b = a + 1; b < iProfiles->Count(); b++ )
            {
            User::LeaveIfError( iProfiles->At( a )->GetParameter(
            	KSIPProfileId, profileIdFirst ) );
    		User::LeaveIfError( iProfiles->At( b )->GetParameter(
    			KSIPProfileId, profileIdSecond ) );
            if ( profileIdFirst > profileIdSecond )
                {
                CSIPManagedProfile* tmp = iProfiles->At( b );
                iProfiles->At( b ) = iProfiles->At( a );
                iProfiles->At( a )  = tmp;
                }
            }
        }
    }


// ----------------------------------------------------------------------------
// Deletes internally cached SIP profiles.
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::DeleteProfiles()
    {
    if ( iProfiles )
        {
        iProfiles->ResetAndDestroy();
        delete iProfiles;
        iProfiles = NULL;
        }
    }


// ----------------------------------------------------------------------------
// From class MSIPProfileRegistryObserver.
// Notifies of an event in SIP profile registry.
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::ProfileRegistryEventOccurred(
    TUint32 /*aSIPProfileId*/, TEvent /*aEvent*/ )
	{
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
// Reads SIP profile list from engine; if reading fails, old cached list is
// kept and returned.
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::ReadProfileListFromEngineSafeL()
    {
    MUS_LOG(
    	"[MUSSET] -> CMusSIPProfileModel::ReadProfileListFromEngineSafeL()" )
    // To prevent complete disaster it we'll save the pointer of the old array
    CArrayPtrFlat<CSIPManagedProfile>* profiles = iProfiles;
    iProfiles = 0;
    TRAPD( error, ReadArrayFromEngineL() );

    if ( error )
        {
        // Problems with re-reading profiles; use existing array
        DeleteProfiles();
        iProfiles = profiles;
        User::Leave( error );
        }
    else
        {
        // No problems; delete backup array
        profiles->ResetAndDestroy();
        delete profiles;
        }
    MUS_LOG(
    	"[MUSSET] <- CMusSIPProfileModel::ReadProfileListFromEngineSafeL()" )
    }


// ----------------------------------------------------------------------------
// For deleting RPointerArray in case of leave (used in association with
// TCleanupItem).
// ----------------------------------------------------------------------------
//
void CMusSIPProfileModel::ResetAndDestroy( TAny* aPointerArray )
    {
    MUS_LOG( "[MUSSET] -> CMusSIPProfileModel::ResetAndDestroy()" )
    RPointerArray<CSIPProfile>* array =
        static_cast<RPointerArray<CSIPProfile>*>( aPointerArray );
    array->ResetAndDestroy();
    array->Close();
    MUS_LOG( "[MUSSET] <- CMusSIPProfileModel::ResetAndDestroy()" )
    }
