/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*  Version     : %version:  30.1.5 % << Don't touch! Updated by Synergy at check-out.
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   This is main SIP class for MusAvailability Plug-in
*
*/



// INCLUDE FILES
#include "musavasip.h"
#include "musavasharedobject.h"
#include "musavasipobserver.h"
#include "musavasipconnectionobserver.h"
#include "musavailabilityplugin.hrh"
#include "musavasipprofileregistryobserver.h"
#include "muslogger.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "musavaclientresolverutil.h"

#include <charconv.h>
#include <sip.h>
#include <sipobserver.h>
#include <sipconnectionobserver.h>
#include <sipprofile.h>
#include <sipprofileregistry.h>
#include <utf.h>
#include <siperr.h>

const TInt KMaxUriLength = 512;

// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaSip::~CMusAvaSip()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::~CMusAvaSip()" )
    // Try to remove client information from CenRep of client resolver.
    // This functionality is needed in __VOIP enabled builds.
    // In normal build this will fail because of a lack of needed CenRep 
    // UID. To avoid branching, we just try to do this and let it fail in 
    // normal build.
    MUS_LOG( "mus: [MUSAVA]     Deleting CSIPProfile instance" )
    if ( iSipProfile && iClientResolverUtil )
        {
        TRAP_IGNORE( 
            iClientResolverUtil->UnRegisterClientWithUserL( *iSipProfile ) )
        }
    delete iSipProfile;    
    delete iClientResolverUtil;
        
    MUS_LOG( "mus: [MUSAVA]     Deleting CSIPProfileRegistry instance" )
    delete iSipProfileRegistry;
    MUS_LOG( "mus: [MUSAVA]     Deleting SIP registry observer" )
    delete iSipProfileRegistryObserver;
    MUS_LOG( "mus: [MUSAVA]     Deleting CSIPConnection instance" )
    delete iSipConnection;
    MUS_LOG( "mus: [MUSAVA]     Deleting SIP connection observer" )
    delete iSipConnectionObserver;
    MUS_LOG( "mus: [MUSAVA]     Deleting CSIP instance" )
    delete iSip;
    delete iSipObserver;
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::~CMusAvaSip()" )
    }


// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusAvaSip* CMusAvaSip::NewL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::NewL()" )
    CMusAvaSip* self = new (ELeave) CMusAvaSip();
    CleanupStack::PushL (self);
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::NewL()" )
    return self;
    }


// -----------------------------------------------------------------------------
// C++ Constructor
// -----------------------------------------------------------------------------
//
CMusAvaSip::CMusAvaSip()
    {
    iSipProfileRegistry = NULL;
    iSipProfile = NULL;
    iSipConnection = NULL;
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAvaSip::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::ConstructL()" )
    // create observer classes (that are not dependant on actual instances)
    iSipObserver = CMusAvaSipObserver::NewL();
    iSipConnectionObserver = CMusAvaSipConnectionObserver::NewL();
    iSipProfileRegistryObserver = CMusAvaSipProfileRegistryObserver::NewL();

    TUid KUidMusAva = { 0x1028238D };
    // create sip instance
    iSip = CSIP::NewL( KUidMusAva, *iSipObserver );
    
    /**
     * Try to instantiate utility object that handles special client resolving 
     * in __VOIP enabled terminals. In __VOIP disabled terminals creation of
     * this object will fail because of missing UID in CenRep and thus this 
     * pointer can be NULL. If this object does not exist, standard client
     * resolving will be used.
     */
    TRAP_IGNORE( iClientResolverUtil = CMusAvaClientResolverUtil::NewL() )

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::ConstructL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSip::AddAdapterL( MMusAvaSipConnectionAdapter& aAdapter, TInt aIndex )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::\
             AddAdapterL( MMusAvaSipConnectionAdapter& aAdapter )" )
    iSipConnectionObserver->AddObserverL( aAdapter, aIndex );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::\
             AddAdapterL( MMusAvaSipConnectionAdapter& aAdapter )" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSip::RemoveAdapter( MMusAvaSipConnectionAdapter& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::\
             RemoceAdapter( MMusAvaSipConnectionAdapter& aAdapter )" )
    iSipConnectionObserver->RemoveObserver( aAdapter );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::\
             RemoceAdapter( MMusAvaSipConnectionAdapter& aAdapter )" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSip::AddAdapterL( MMusAvaSipAdapter& aAdapter, TInt aIndex  )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::\
             AddAdapterL( MMusAvaSipAdapter& aAdapter )" )
    iSipObserver->AddObserverL( aAdapter, aIndex );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::\
             AddAdapterL( MMusAvaSipAdapter& aAdapter )" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSip::RemoveAdapter( MMusAvaSipAdapter& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::RemoveAdapter( MMusAvaSipAdapter& aAdapter )" )
    iSipObserver->RemoveObserver( aAdapter );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::RemoveAdapter( MMusAvaSipAdapter& aAdapter )" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSip::AddAdapterL( MMusAvaSipProfileRegistryAdapter& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::\
             AddAdapterL( MMusAvaSipProfileRegistryAdapter& aAdapter )" )
    iSipProfileRegistryObserver->AddAdapterL( aAdapter );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::\
             AddAdapterL( MMusAvaSipProfileRegistryAdapter& aAdapter )" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSip::RemoveAdapter( MMusAvaSipProfileRegistryAdapter& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::\
             RemoveAdapter( MMusAvaSipProfileRegistryAdapter& aAdapter )" )
    iSipProfileRegistryObserver->RemoveAdapter( aAdapter );
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::\
             RemoveAdapter( MMusAvaSipProfileRegistryAdapter& aAdapter )" )
    }
  
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CSIPConnection* CMusAvaSip::ConnectionL( TBool aForceCreation )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::Connection()" )
    if ( aForceCreation )
        {
        CreateSIPConnectionL();
        }
    __ASSERT_ALWAYS( iSipConnection, User::Leave( KErrSIPInvalidRegistrationState ) );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::Connection()" )
    return iSipConnection;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
MSIPConnectionObserver& CMusAvaSip::ConnectionObserver()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::ConnectionObserver()" )
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::ConnectionObserver()" )
    return *iSipConnectionObserver;
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TBool CMusAvaSip::CreateProfileL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::CreateProfileL()" )

  	TBool retVal = ETrue;
  	if ( CreateSipProfile() )
  		{
        TInt err = iSipProfile->GetParameter( KSIPAccessPointId, iIapId );
        if ( err != KErrNone )
  		    {
  		    MUS_LOG1( "SIP Access Point not found = %d", err )   
        	retVal = EFalse;
  		    }
  		}
  	else
  		{
  		retVal = EFalse;
  		}        
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::CreateProfileL()" )
    return retVal;
    }

    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TBool CMusAvaSip::CreateSipProfile()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::CreateSipProfile()" )
    if ( iSipProfile )
        {
        MUS_LOG( "mus: [MUSAVA]  Profile is already created" )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::CreateSipProfile()" )
        return ETrue;
        }
    
    CSIPProfileRegistry* registry = NULL;

    //get profile registry
    MUS_LOG( "mus: [MUSAVA]   Fetch profile registry" )
    TRAPD( error_registry, registry = &ProfileRegistryL() );
    if( error_registry )
        {
        MUS_LOG( "mus: [MUSAVA]    ProfileRegistry is not created" )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::CreateSipProfile()" )
        return EFalse;
        }
    // try to resolve the profile
    MUS_LOG( "mus: [MUSAVA]   Fetch  SIP Profile" )
    TRAPD( error_profile, iSipProfile = GetMusProfileL( *registry ) );
    if( error_profile )
        {
        MUS_LOG( "mus: [MUSAVA]    Profile is not found" )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::CreateSipProfile()" )
        return EFalse;
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA]  Profile is  created" )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::CreateSipProfile()" )
        return ETrue;
        }
    }

    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CSIPProfile* CMusAvaSip::Profile()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::Profile()" )
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::Profile()" )
    return iSipProfile;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CSIPProfileRegistry& CMusAvaSip::ProfileRegistryL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::ProfileRegistry()" )
    if ( !iSipProfileRegistry )
        {
        MUS_LOG( "mus: [MUSAVA]  create sip profile registry" )
        // create sip profile registry
        iSipProfileRegistry = CSIPProfileRegistry::NewL(
                                    *iSip,
                                    *iSipProfileRegistryObserver );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::ProfileRegistry()" )
    return *iSipProfileRegistry;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
HBufC* CMusAvaSip::OwnDomainLC()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::OwnDomainLC()" )
    
    HBufC* domain = KNullDesC().AllocLC();
    if( iSipProfile )
        {
        const MDesC8Array* domainNameArray( NULL );
        iSipProfile->GetParameter( KSIPRegisteredAors, domainNameArray );

        if( domainNameArray )
            {
            if( domainNameArray->MdcaCount() > 0 )
                {
                TBuf<KMaxUriLength> domain16;
                const TDesC8& domain8 = domainNameArray->MdcaPoint( 0 );

                _LIT8( KAt, "@" );

                TInt index = domain8.Find( KAt() );

                User::LeaveIfError(
                        CnvUtfConverter::ConvertToUnicodeFromUtf8(
                                domain16,
                                domain8.Mid( index + KAt().Length() ) ) );
                CleanupStack::PopAndDestroy( domain );
                domain = domain16.AllocLC();
                }
            }
        }
        
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::OwnDomainLC()" )
    return domain;
    }

    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CSIP& CMusAvaSip::Sip()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::Sip()" )
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::Sip()" )
    return *iSip;
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TInt CMusAvaSip::SipProfileId( TUint32& aSipProfileId )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::SipProfile()" )
    //SIP Profile ID 
    TInt profile_id_err = iSipProfile->GetParameter( KSIPProfileId, aSipProfileId );
   	MUS_LOG1( "Profile id = %d", profile_id_err )
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::SipProfile()" )	
    return profile_id_err;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CMusAvaClientResolverUtil* CMusAvaSip::ClientResolverUtil() const
    {
    return iClientResolverUtil;
    }

    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSip::CreateSIPConnectionL()
    {
    MUS_LOG1( "mus: [MUSAVA]  -> CMusAvaSip::CreateSIPConnectionL(), iapid:%d", 
              iIapId )
    
   	if ( !iSipConnection )
   	    {
   	    iSipConnection = CSIPConnection::NewL( *iSip,
                                               iIapId,
                                               *iSipConnectionObserver );
        }
   	                                           
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::CreateSIPConnectionL()" )
    }


// ---------------------------------------------------------------------------------
// Gets profile to be used with Mus.
// ---------------------------------------------------------------------------------
//
CSIPProfile* CMusAvaSip::GetMusProfileL( CSIPProfileRegistryBase& aRegistry )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSip::GetMusProfileL" )
    CSIPProfile* profile = NULL;
    TUint32 profileId = KErrNone;

    // Try to fetch profileId for profile meant to be used with Mus
    TRAPD( err, profileId =
            MultimediaSharingSettings::SipProfileSettingL() );

    if( err != KErrNone || profileId == KErrNone )
        {
        MUS_LOG( "mus: [MUSAVA]     Using default profile" )
        profile = aRegistry.DefaultProfileL();
        }
    else if ( (TInt )profileId == KErrNotFound )
        {
        MUS_LOG( "mus: [MUSAVA]     Setting: Mus is not allowed to use any \
            SIP profiles" )
        MUS_LOG1( "mus: [MUSAVA]     User::Leave( %d )",
                  KErrNotSupported )
        User::Leave( KErrNotSupported );
        }
    else 
        {
        MUS_LOG( "mus: [MUSAVA]     Try to fetch profile meant to be used with Mus" )
        profile = aRegistry.ProfileL( profileId );
        err = profile->GetParameter( KSIPProfileId,profileId );
        if( err != KErrNone)
            {
            MUS_LOG1("mus: [MUSAVA]    SIP Profile ID %d",profileId )
            }  
        } 

    // If not possible, use default profile. If not able, leave.
    if ( !profile )
        {
        MUS_LOG( "mus: [MUSAVA]     Not found" )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::GetMusProfileL" )
        User::Leave( KErrNotFound );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::GetMusProfileL" )
    return profile;
    };
        





