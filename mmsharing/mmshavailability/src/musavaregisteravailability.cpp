/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Provide interface for the client requestin availability class.
*
*/



#include "musavaregisteravailability.h"
#include "musavaavailabilityobserver.h"
#include "musunittesting.h"
#include "musavaobserver.h"
#include "musavaavailability.h"
#include "musavasharedobject.h"
#include "muslogger.h"
#include "musavasettingsimp.h"
#include "musavasip.h"
#include "musavaconnectionmonitor.h"
#include "mussettings.h"
#include "musavaclientresolverutil.h"

#include <e32base.h>
#include <sipprofile.h>
#include <sipprofileregistry.h>
#include <sipprofileregistryobserver.h>
#include <escapeutils.h>


// -----------------------------------------------------------------------------
// Symbian two-phase constructor
// -----------------------------------------------------------------------------
//
CMusAvaRegisterAvailability* CMusAvaRegisterAvailability::NewL(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    CMusAvaRegisterAvailability* self =
        CMusAvaRegisterAvailability::NewLC(
            aObserver,
            aSettings );
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
//  Two-phased constructor.
// ----------------------------------------------------------------------------
//
CMusAvaRegisterAvailability* CMusAvaRegisterAvailability::NewLC(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    CMusAvaRegisterAvailability* self =
        new (ELeave) CMusAvaRegisterAvailability(
            aObserver,
            aSettings );
    CleanupStack::PushL (self);
    self->ConstructL();
    return self;
    }


// ------------------------------------------------------------------------------
//  Destructor.
// ------------------------------------------------------------------------------
//
CMusAvaRegisterAvailability::~CMusAvaRegisterAvailability()
    {
    if ( iSharedObj )
        {
        iSharedObj->MusAvaSip().RemoveAdapter( *this );
        iSharedObj->DeleteSingleton();
        }
    }


// ------------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ------------------------------------------------------------------------------
//
void CMusAvaRegisterAvailability::ConstructL()
    {
    iSharedObj = CMusAvaSharedObject::GetSingletonL();
    iSharedObj->MusAvaSip().AddAdapterL( *this );
    }


// -----------------------------------------------------------------------------
//  Constructor.
// -----------------------------------------------------------------------------
//
CMusAvaRegisterAvailability::CMusAvaRegisterAvailability(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    :CMusAvaAvailability( aObserver ),
    iSettings( aSettings )
    {
    }


// -----------------------------------------------------------------------------
// Executes for the master - salve pattern.
// -----------------------------------------------------------------------------
//
void CMusAvaRegisterAvailability::DoExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaRegisterAvailability::DoExecuteL " )
    SetState( MMusAvaObserver::EMusAvaStatusInProgress );
    RegisterL();
    // TBD: SetState( MMusAvaObserver::EMusAvaNameRegistration );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::DoExecuteL " )
    }


// -----------------------------------------------------------------------------
// Stop the execution.
// -----------------------------------------------------------------------------
//
void CMusAvaRegisterAvailability::Stop()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaRegisterAvailability::Stop" )
    CSIPProfile* profile = iSharedObj->MusAvaSip().Profile();
    if ( profile )
        {
        CSIPProfileRegistry* registry = NULL; // not own, so not deleted
        TRAPD( error_register, registry = 
        	&iSharedObj->MusAvaSip().ProfileRegistryL() );
        if ( error_register == KErrNone )
            {
            MUS_LOG( "mus: [MUSAVA]    Sip profile registry found" )
            TInt error = registry->Disable( *profile );
            MUS_LOG1( "mus: [MUSAVA]	Disable Sip profile ( %d )", error )
            }
        }
    
    SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::Stop" )
    }


// -----------------------------------------------------------------------------
// CMusAvaRegisterAvailability::Name
//
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaRegisterAvailability::Name()
    {
    return MMusAvaObserver::EMusAvaNameRegistration;
    }


// -----------------------------------------------------------------------------
// An event related to SIP Profile has accorred
// -----------------------------------------------------------------------------
//
void CMusAvaRegisterAvailability::ProfileRegistryEventOccurred(
                        TUint32 aProfileId,
                        MSIPProfileRegistryObserver::TEvent aEvent)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaRegisterAvailability::\
             ProfileRegistryEventOccurred " )

    CSIPProfile* profile = iSharedObj->MusAvaSip().Profile();
    if ( !profile )
        {
        MUS_LOG( "mus: [MUSAVA]    no sip profile found, returning!" )
        return;
        }


    TUint32 profileId = 0;
    TUint connectionId = 0;

    profile->GetParameter( KSIPProfileId, profileId );

    if ( aProfileId == profileId )
        {
        switch (aEvent)
            {
            case MSIPProfileRegistryObserver::EProfileCreated:
                {
                MUS_LOG( "mus: [MUSAVA]     Profile is created" )
                MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability\
                        ::ProfileRegistryEventOccurred" )
                }
                break;

            case MSIPProfileRegistryObserver::EProfileUpdated:
                {
                MUS_LOG( "mus: [MUSAVA]     Profile is updated" )
                }
                break;

            case MSIPProfileRegistryObserver::EProfileRegistered:
                {
                MUS_LOG( "mus: [MUSAVA]     Profile is registred" )
                
                // Make sure that valid connection exists when registered
                CSIPConnection* sipConnection = NULL;
                TRAPD( err, sipConnection = iSharedObj->MusAvaSip().ConnectionL() );
                if ( err != KErrNone || 
                    !sipConnection || 
                     sipConnection->State() == CSIPConnection::EUnavailable ||
                     sipConnection->State() == CSIPConnection::ESuspended ||
                     sipConnection->State() == CSIPConnection::EInactive )
                    {
                    MUS_LOG1( "mus: [MUSAVA]    CreateSIPConnectionL \
                            leave #%d", err )
                    SetState( MMusAvaObserver::MMusAvaObserver::EMusAvaConnectionErr );
                    }
                else
                    {
    				TRAP( err, connectionId =
    					iSharedObj->ConnectionMonitor().ConnectionIDL() );
    	            if ( err == KErrNotFound )
    	            	{
    	            	SetState( MMusAvaObserver::EMusAvaConnectionErr );
    	            	}
    	           	else
    	           		{
    	           		iSharedObj->ConnectionMonitor().SetConnectionID(
    	           										connectionId );
    	           		if( err != KErrNone )
                            {
                            MUS_LOG1( "mus: [MUSAVA]     ConnectionIdL \
                            leave #%d", err )
                            }
                        }
                    }
                
                MUS_LOG( "mus: [MUSAVA]     Enable profile." )
                TRAP( err, EnableRegisterL() );
                if( err != KErrNone )
                    {
                    MUS_LOG1( "mus: [MUSAVA]     EnableRegisterL leave #%d",
                    err )
                    }
                                
                TRAP( err, CreateSipAddressProposalL(); );
                if( err != KErrNone )
                    {
                    MUS_LOG1( "mus: [MUSAVA]     CreateSipAddressProposalL \
                    err #%d", err )
                    }
                
                SetState( MMusAvaObserver::EMusAvaStatusAvailable );

                // update sipprofile in settings
                if ( profileId != iSettings.SipProfileId() )
                    {
                    MUS_LOG1( "mus: [MUSAVA]     Profile id in settings before\
                    update:%d", iSettings.SipProfileId() )
                    iSettings.SetSipProfileId( profileId );
                    MUS_LOG1( "mus: [MUSAVA]     Profile id in settings after\
                    update:%d", iSettings.SipProfileId() )
                    }
                else
                    {
                    MUS_LOG( "Profile is not needed to update!!!!!" )
                    }
                }
                break;

            case MSIPProfileRegistryObserver::EProfileDeregistered:
                {
                MUS_LOG( "mus: [MUSAVA]     Profile is deregistered" )
                SetState( MMusAvaObserver::EMusAvaStatusNotRegistered );
                }
                break;

            case MSIPProfileRegistryObserver::EProfileDestroyed:
                {
                MUS_LOG( "mus: [MUSAVA]     Profile is destroyed " )
                SetState( MMusAvaObserver::EMusAvaStatusNotRegistered );
                }
                break;

            default:
                MUS_LOG( "mus: [MUSAVA]     Unknown Profile Id" )
                break;
            }
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA]    aProfileId != profileId" )
        }

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::\
             ProfileRegistryEventOccurred" )
    }

// -----------------------------------------------------------------------------
// An asynchronous error has occurred related to SIP profile
// Event is send to those observers, who have the
// corresponding profile instantiated.
// -----------------------------------------------------------------------------
//
void CMusAvaRegisterAvailability::ProfileRegistryErrorOccurred(
    TUint32 aProfileId,
    TInt /* aError */ )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaRegisterAvailability::\
             ProfileRegistryErrorOccurred" )

    CSIPProfile* profile = iSharedObj->MusAvaSip().Profile();
    if ( !profile )
        {
        return;
        }

    TUint32 profileId;

    profile->GetParameter( KSIPProfileId, profileId );

    if( profileId == aProfileId )
        {
        SetState( MMusAvaObserver::EMusAvaStatusNotRegistered );
        }

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::\
             ProfileRegistryErrorOccurred" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaRegisterAvailability::CreateSipAddressProposalL()
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaRegisterAvailability::\
        CreateSipAddressProposalL() " )

    HBufC* domain = iSharedObj->MusAvaSip().OwnDomainLC();

    const TDesC& telNumber = iSettings.TelNumber();

    iSettings.SetSipAddressProposalL( telNumber, *domain );

    CleanupStack::PopAndDestroy( domain );

    MUS_LOG( "mus: [MUSAVA] <- CMusAvaRegisterAvailability::\
        CreateSipAddressProposalL() " )
    }


// -----------------------------------------------------------------------------
// Connection state has changed.
// iSipConnection has to be instantiated for us to get these callbacks.
// from base class MSIPConnectionObserver
// -----------------------------------------------------------------------------
//
TInt CMusAvaRegisterAvailability::ConnectionStateChanged(
    CSIPConnection::TState aState )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaRegisterAvailability::\
    ConnectionStateChanged" )
    TInt err = 0;
    switch ( aState )
        {
        case CSIPConnection::EInit:
            {
            MUS_LOG( "mus: [MUSAVA] aState == CSIPConnection::EInit" )
            break;
            }
        case CSIPConnection::EActive:
            {
            MUS_LOG( "mus: [MUSAVA] aState == CSIPConnection::EActive" )
            break;
            }
        case CSIPConnection::ESuspended:
            {
            MUS_LOG( "mus: [MUSAVA] aState == CSIPConnection::ESuspended" )
            SetState( MMusAvaObserver::EMusAvaConnectionErr );
            break;
            }
        case CSIPConnection::EInactive:
            {
            MUS_LOG( "mus: [MUSAVA] aState == CSIPConnection::EInactive" )
            // If we get here, then PDP context is probably
            // unavailable => close an existing PDP context.
            MUS_LOG( "mus: [MUSAVA] now calling CloseExistingConnectionL" )
            iSharedObj->ConnectionMonitor().KillPdpContext();
            TRAPD( leaveCode, err = RegisterL() )
            if ( !leaveCode || err )
                {
                SetState( MMusAvaObserver::EMusAvaConnectionErr );
                }
            else
                {
                SetState( MMusAvaObserver::EMusAvaStatusAvailable );
                }
            break;
            }
        case CSIPConnection::EUnavailable:
            {
            MUS_LOG( "mus: [MUSAVA] aState == CSIPConnection::EUnavailable" )
            SetState( MMusAvaObserver::EMusAvaConnectionErr );
            break;
            }
        default:
            {
            MUS_LOG( "mus: [MUSAVA] in default branch => ESipConnUnknown" )
            break;
            }
        }

    MUS_LOG( "mus: [MUSAVA]: <- CMusAvaRegisterAvailability::ConnectionStateChanged" )

    return ETrue;
    }

// -----------------------------------------------------------------------------
// Initialize SIP Registration.
// -----------------------------------------------------------------------------
//
TBool CMusAvaRegisterAvailability::InitializeRegister()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaRegisterAvailability::\
              InitializeRegister " )

    MUS_LOG( "mus: [MUSAVA] Create SIP Profile " )
    TBool profileCreated = EFalse;
    TRAPD( err, profileCreated = iSharedObj->MusAvaSip().CreateProfileL() );
    if ( err || !profileCreated )
        {
        MUS_LOG( "mus: [MUSAVA] Profile is not created " )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::\
                  InitializeRegister " )
        SetState( MMusAvaObserver::EMusAvaStatusNoSipProfile );
        return EFalse;
        }
        
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::\
              InitializeRegister " )

    return ETrue;
    }


// -----------------------------------------------------------------------------
// SIP Registration.
// -----------------------------------------------------------------------------
//
TBool CMusAvaRegisterAvailability::RegisterL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaRegisterAvailability::RegisterL " )

    MUS_LOG( "mus: [MUSAVA]     Checks if the profile is not enabled - \
                 Enable" )
    if ( !InitializeRegister())
        {
        MUS_LOG( "mus: [MUSAVA]  SIP register initilisation fails " )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::\
        Register " )
        return EFalse;
        }
        
    EnableRegisterL();
   	if ( !UpdateSIPProfile()  )
   		{
   		return EFalse;	
   		}
    return RegistrationStatusL();
    }

// -----------------------------------------------------------------------------
// Enable SIP Registration.
// -----------------------------------------------------------------------------
//
void CMusAvaRegisterAvailability::EnableRegisterL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaRegisterAvailability::EnableRegisterL " )		
 	MUS_LOG( "mus: [MUSAVA]   Fetch SIP ProfileRegistry" )
 	
    CSIPProfileRegistry& registry = iSharedObj->MusAvaSip().ProfileRegistryL();
    MUS_LOG( "mus: [MUSAVA]   Profile status" )
    if ( !registry.IsEnabled( *iSharedObj->MusAvaSip().Profile() ) )
        {
        MUS_LOG("mus: [MUSAVA]     Calling EnableL()" )
        registry.EnableL( *iSharedObj->MusAvaSip().Profile(), 
                        iSharedObj->MusAvaSip().ConnectionObserver() );
        }
       
    // Try to write client information to CenRep of client resolver.
    // This functionality is needed in __VOIP enabled terminals.
    // In __VOIP disabled builds this will fail because of a lack of needed 
    // CenRep UID. To avoid branching, we just try to do this and let it fail 
    // in normal build.
    CMusAvaClientResolverUtil* resolver = 
                                iSharedObj->MusAvaSip().ClientResolverUtil();
    if ( resolver )
        {
        TRAP_IGNORE( resolver->RegisterClientWithUserL( 
                *iSharedObj->MusAvaSip().Profile() ) )
        }
                    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::EnableRegisterL " )		
    }


// -----------------------------------------------------------------------------
// Update SIP Registration.
// -----------------------------------------------------------------------------
//
TBool CMusAvaRegisterAvailability::UpdateSIPProfile()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaRegisterAvailability::UpdateSIPProfile " )
    TUint32 profileId = 0; // Initial value doesn't matter. 
                           // but better to set it to default profile
    TInt err = iSharedObj->MusAvaSip().Profile()->GetParameter( 
        KSIPProfileId, profileId );
    if( err == KErrNone)
        {
        MUS_LOG1("mus: [MUSAVA]    Set SIP Profile ID in Central Repositary \
            to %d",profileId )
        iSettings.SetSipProfileId( profileId );
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::UpdateSIPProfile " )
        return ETrue;
        }
    else
    	{
    	MUS_LOG1("mus: [MUSAVA]   SIP Profile ID is not found. \
    	remove Id away from Central Repositary %d",err )
    	iSettings.SetSipProfileId( err );
    	MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::UpdateSIPProfile " )
    	return EFalse;	
    	}
    }
    
// -----------------------------------------------------------------------------
// SIP profile Registration status.
// -----------------------------------------------------------------------------
//
TBool CMusAvaRegisterAvailability::RegistrationStatusL()
	{
	MUS_LOG( "mus: [MUSAVA]  -> CMusAvaRegisterAvailability::RegistrationStatusL " )	
	MUS_LOG("mus: [MUSAVA]     Checking profile registration" )
	
	TBool registered;

    TInt err = iSharedObj->MusAvaSip().Profile()->GetParameter( 
        KSIPProfileRegistered, registered );

    if( err != KErrNone || !registered )
        {
        MUS_LOG2( "mus: [MUSAVA]     Profile not registered, get error=%d, \
            register value=%d", err, registered )

        SetState( MMusAvaObserver::EMusAvaStatusInProgress );
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::RegistrationStatusL " )
        return EFalse;
        }
    else
        {
        MUS_LOG("mus: [MUSAVA]     profile is registred " )
        
        iSharedObj->MusAvaSip().ConnectionL();
        
        MUS_LOG("mus: [MUSAVA]     connection created " )

        HBufC* domain = iSharedObj->MusAvaSip().OwnDomainLC();
        const TDesC& telNumber = iSettings.TelNumber();

        iSettings.SetSipAddressProposalL( telNumber, *domain );

        CleanupStack::PopAndDestroy( domain );
        SetState( MMusAvaObserver::EMusAvaStatusAvailable );
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaRegisterAvailability::RegistrationStatusL " )	
        return ETrue;
        }
    }
  
   
