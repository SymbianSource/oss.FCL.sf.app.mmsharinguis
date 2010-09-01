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
* Description:   This is default implementation class for CMusAvaInterface
*
*/



#include "musavadefaultimp.h"
#include "musavaavailabilityobserver.h"
#include "musavaobserver.h"
#include "musavasettingsimp.h"
#include "musavasharedobject.h"
#include "muslogger.h"
#include "musavainvitehandler.h"
#include "musavanetworkavailability.h"
#include "musavacontactavailability.h"
#include "musavaconnectionavailability.h"
#include "musavaregisteravailability.h"
#include "musavaoptionhandler.h"
#include "musavadefaultoptionhandler.h"
#include "musavasipprofileavailability.h"
#include "musavasettingavailability.h"

#include "mussesseioninformationapi.h"

#include <e32property.h>


const MMusAvaObserver::TAvailabilityName KMinimumAvailability
    = MMusAvaObserver::EMusAvaBearerStatus;

const MMusAvaObserver::TAvailabilityName KMandatoryAvailability
    = MMusAvaObserver::EMusAvaNameRegistration;

const MMusAvaObserver::TAvailabilityName KExtensionAvailability
    = MMusAvaObserver::EMusAvaOptionHandler;


// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusAvaDefaultImp* CMusAvaDefaultImp::NewL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::NewL" )
    CMusAvaDefaultImp* self = new (ELeave) CMusAvaDefaultImp();
    CleanupStack::PushL (self);
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaDefaultImp::~CMusAvaDefaultImp()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::~CMusAvaDefaultImp" )
    this->Stop();
    iAvailabilities.ResetAndDestroy();
    delete iSettings;
    iSettings = NULL;
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::~CMusAvaDefaultImp" )
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::ConstructL" )
    iSettings = CMusAvaSettingsImp::NewL();
    CreateAvailabilityModulesL();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::ConstructL" )
    }


// -----------------------------------------------------------------------------
// C++ Constructor
// -----------------------------------------------------------------------------
//
CMusAvaDefaultImp::CMusAvaDefaultImp()
    :iObserver( NULL ),
    iCurrentAvailability( MMusAvaObserver::EMusAvaNameNotDefined ),
    iAvailabilityStatus ( MMusAvaObserver::EMusAvaStatusNotExecuted ),
    iStopping( EFalse ),
    iExecuteStarted( EFalse )
    {
    }


// -----------------------------------------------------------------------------
// Creates concrete factory and all availability objects
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::CreateAvailabilityModulesL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::\
        CreateAvailabilityModulesL" )
    
    if ( iAvailabilities.Count() )
        {
        Stop();
        iAvailabilities.ResetAndDestroy();
        }

    /* We need handlers all the time to respond incoming Invite/Options 
       So construct this modules first.
    */
    ConstuctHandlerAvailabilityModulesL();

    /* We must check the static availability modules like sipprofile 
       mus activation settings. if this fails then this 
       will be fatal and no need to go and construct further.
    */
    ConstructStaticAvailabilityModulesL();

    /* Check the handler and static availability modules are OK . If it is not
     * OK then dont need to create further availability modules.
     */
    for ( TInt i = 0; i < iAvailabilities.Count(); i++ )
        {
        if ( !iAvailabilities[i]->Available())
            {
            MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaDefaultImp:: \
                      CreateAvailabilityModulesL Fails! Module = %d ",i )
            return ;
            }
        }    
    
    /* This should construct modules in where availability could change 
     * dynamically such as network,bearer and call status etc.    
     */
    ConstructDynamicAvailabilityModulesL();

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::\
        CreateAvailabilityModulesL" )
    }

// -----------------------------------------------------------------------------
// Creates concrete standard availability objects
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::ConstuctHandlerAvailabilityModulesL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::\
                                        ConstuctHandlerAvailabilityModulesL" )

    CMusAvaInviteHandler* inviteHandler = CMusAvaInviteHandler::NewLC(
                                                           *this, *iSettings ) ;
    iAvailabilities.AppendL( inviteHandler );
    CleanupStack::Pop( inviteHandler );    

    CMusAvaDefaultOptionHandler* defaultOptionHandler =
                            CMusAvaDefaultOptionHandler::NewLC( *this, *iSettings );
	iAvailabilities.AppendL( defaultOptionHandler );
    CleanupStack::Pop( defaultOptionHandler );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::\
        ConstuctHandlerAvailabilityModulesL" )
    }

// -----------------------------------------------------------------------------
// Creates concrete standard availability objects
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::ConstructStaticAvailabilityModulesL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::\
                                            ConstructStaticAvailabilityModulesL" )
    
    /* Mus Activation Settings */
    CMusAvaSettingAvailability* settingAvailability =
                            CMusAvaSettingAvailability::NewLC( *this );
	iAvailabilities.AppendL( settingAvailability );
    CleanupStack::Pop( settingAvailability );

    /* Existence of Sip profile */
    CMusAvaSipprofileAvailability* sipprofileExist =
                            CMusAvaSipprofileAvailability::NewLC( *this ) ;
    iAvailabilities.AppendL( sipprofileExist );
    CleanupStack::Pop( sipprofileExist );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp:: \
                                            ConstructStaticAvailabilityModulesL" )
    }

// -----------------------------------------------------------------------------
// Creates concrete standard availability objects
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::ConstructDynamicAvailabilityModulesL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::\
                                           ConstructDynamicAvailabilityModulesL" )       
    // Create network availability
    CMusAvaNetworkAvailability* networkAvailability =
                            CMusAvaNetworkAvailability::NewLC( *this, *iSettings ) ;
    iAvailabilities.AppendL( networkAvailability );
    CleanupStack::Pop( networkAvailability );

    // Contact availability
    CMusAvaContactAvailability* contactAvailability = 
                            CMusAvaContactAvailability::NewLC( *this, *iSettings );
    iAvailabilities.AppendL( contactAvailability );
    CleanupStack::Pop( contactAvailability );

    //Connection monitor
    CMusAvaConnectionAvailability* connectionAvailability =
                          CMusAvaConnectionAvailability::NewLC( *this, *iSettings );
    iAvailabilities.AppendL( connectionAvailability );
    CleanupStack::Pop( connectionAvailability );

    // Create register availability
    CMusAvaRegisterAvailability* registerAvailability = 
                            CMusAvaRegisterAvailability::NewLC( *this, *iSettings );
    iAvailabilities.AppendL( registerAvailability );
    CleanupStack::Pop( registerAvailability );

    // Extension availabilities ->

    // Create SIP options availability
    CMusAvaOptionHandler* optionHandler = 
                                  CMusAvaOptionHandler::NewLC( *this, *iSettings );
    iAvailabilities.AppendL( optionHandler );
    CleanupStack::Pop( optionHandler );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::ConstructDynamicAvailabilityModulesL" )
    }

// -----------------------------------------------------------------------------
// Create work for executing availability report.
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::ExecuteAvailablitityModules()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::\
        ExecuteAvailablitityModules" )
    iExecuteStarted = ETrue;
    TBool goOn = ETrue;
    for ( TInt i = 0; i < iAvailabilities.Count() && goOn; i++ )
        {
        if ( !iAvailabilities[i]->Available() &&  
            !iAvailabilities[i]->Executing())
            {
            iAvailabilities[i]->Execute();
            if ( iAvailabilities[i]->State() < MMusAvaObserver::EMusAvaStatusInProgress )
                {
                goOn = EFalse;
                }
            }
        }
    iExecuteStarted = EFalse;
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::\
        ExecuteAvailablitityModules" )
    }


// -----------------------------------------------------------------------------
// Retuns setting interface for the client.
// -----------------------------------------------------------------------------
//
MMusAvaSettings& CMusAvaDefaultImp::Settings()
    {
    return *iSettings;
    }


// -----------------------------------------------------------------------------
// Set Observer interface for the client.
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::SetObserver( MMusAvaObserver& aObserver )
    {
    iObserver = &aObserver;
    }

// -----------------------------------------------------------------------------
// Set MMusAvaSettingsObserver interface for the client.
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::SetSettingsObserver( MMusAvaSettingsObserver& aObserver )
    {
    iSettings->SetObserver( aObserver );
    }
// -----------------------------------------------------------------------------
// Requests the implementation to start to investigate availabilites.
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::StartL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::StartL" )
    ExecuteAvailablitityModules();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::StartL" )
    }


// -----------------------------------------------------------------------------
// Requests the implementation to stop investigating or monitoring availabilites
// for the client.
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::Stop()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::Stop" )
    iStopping = ETrue;
    for ( TInt i = iAvailabilities.Count() - 1; i >= 0; i-- )
        {
        if ( iAvailabilities[i]->State() > MMusAvaObserver::EMusAvaStatusNotExecuted )
            {
            iAvailabilities[i]->Stop();
            }
        }
    CalculateAvailability();
    iStopping = EFalse;
    if ( iObserver )
        {
        iObserver->AvailabilityChanged(
            CurrentAvailability(),
            AvailabilityStatus() );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::Stop" )
    }


// -----------------------------------------------------------------------------
//  Availability report.
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::AvailabilityChanged(
    MMusAvaObserver::TAvailabilityName aName,
    MMusAvaObserver::TAvailabilityStatus aStatus )
    {
    MUS_LOG2( "mus: [MUSAVA]  -> CMusAvaDefaultImp::AvailabilityChanged(%d,%d)",
        aName, aStatus )

    CalculateAvailability();
    if ( iObserver )
        {
        iObserver->AvailabilityChanged( aName, aStatus );
        }
        
    if ( !iStopping && aStatus >= MMusAvaObserver::EMusAvaStatusNotExecuted )
        {
        if ( !iExecuteStarted )
            {
            ExecuteAvailablitityModules();
            }
        }
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::AvailabilityChanged()" )
    }


void CMusAvaDefaultImp::AvailabilitiesAbleToShowIndicator()
	{
	iObserver->AvailabilitiesAbleToShowIndicator();
	}

// -----------------------------------------------------------------------------
//  Availability error.
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::AvailabilityError(
    MMusAvaObserver::TAvailabilityName aName,
    MMusAvaObserver::TAvailabilityStatus aStatus )
    {
    MUS_LOG2( "mus: [MUSAVA]  -> CMusAvaDefaultImp::AvailabilityError\
        ( %d, %d)", aName, aStatus )

    CalculateAvailability();
    if ( iObserver )
        {
        iObserver->AvailabilityError( aName, aStatus );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::AvailabilityError()" )
    }
    
// -----------------------------------------------------------------------------
//  Checks if the current availability is at least the same as given as 
//  parameter
// -----------------------------------------------------------------------------
//
TBool CMusAvaDefaultImp::Available(
    MMusAvaObserver::TAvailabilityName aAvailability )
    {
    return iCurrentAvailability > aAvailability
      || ( iCurrentAvailability == aAvailability
       	&& MMusAvaObserver::EMusAvaStatusAvailable == iAvailabilityStatus )
      || ( iCurrentAvailability == aAvailability
       	&& ( MMusAvaObserver::EMusAvaStatusOptionsNotSent == iAvailabilityStatus
      	  ||  MMusAvaObserver::EMusAvaStatusOptionsSent == iAvailabilityStatus) );
    }
    
// -----------------------------------------------------------------------------
//  Calculates current availability
// -----------------------------------------------------------------------------
MMusAvaObserver::TAvailabilityStatus CMusAvaDefaultImp::AvailabilityPluginState()
	{
	return iAvailabilityStatus;
	}
// -----------------------------------------------------------------------------
//  Calculates current availability
// -----------------------------------------------------------------------------
//
void CMusAvaDefaultImp::CalculateAvailability()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultImp::CalculateAvailability()" )
    iCurrentAvailability = MMusAvaObserver::EMusAvaFullAvailability;
    iAvailabilityStatus = MMusAvaObserver::EMusAvaStatusAvailable;    
    TBool goOn = ETrue;
    for ( TInt i = 0; i < iAvailabilities.Count() && goOn; i++ )
        {
        if ( !iAvailabilities[i]->Available() )
            {
            iCurrentAvailability = iAvailabilities[i]->Name();
            iAvailabilityStatus = iAvailabilities[i]->State();
            goOn = EFalse;
            }
        }            
    MUS_LOG2( "mus: [MUSAVA]  Avaialability name and status (%d,%d)",
        iCurrentAvailability, iAvailabilityStatus )
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultImp::CalculateAvailability()" )
    }

// -----------------------------------------------------------------------------
//  Current availability
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaDefaultImp::CurrentAvailability()
    {
    return iCurrentAvailability;
    }


// -----------------------------------------------------------------------------
//  Current availability status
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityStatus CMusAvaDefaultImp::AvailabilityStatus()
    {
    return iAvailabilityStatus;
    }


// -----------------------------------------------------------------------------
//  Checks if current availability is at least minimum availability
// -----------------------------------------------------------------------------
//
TBool CMusAvaDefaultImp::MinimumAvailability()
    {
    return iCurrentAvailability > KMinimumAvailability;
    }


// -----------------------------------------------------------------------------
//  Checks if current availability is at least mandatory availability
// -----------------------------------------------------------------------------
//
TBool CMusAvaDefaultImp::MandatoryAvailability()
    {
    return iCurrentAvailability > KMandatoryAvailability;
    }


// -----------------------------------------------------------------------------
//  Checks if current availability is extension availability
// -----------------------------------------------------------------------------
//
TBool CMusAvaDefaultImp::ExtensionAvailability()
    {
    return iCurrentAvailability > KExtensionAvailability;
    }

// -----------------------------------------------------------------------------
//  Returns the sate of the given availability
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityStatus CMusAvaDefaultImp::AvailabilityState(
    MMusAvaObserver::TAvailabilityName aAvailability )
    {
    MUS_LOG1( "mus: [MUSAVA]  -> CMusAvaDefaultImp::AvailabilityState(%d)",
        aAvailability )
    MMusAvaObserver::TAvailabilityStatus retval = 
                                MMusAvaObserver::EMusAvaStatusNotExecuted;
    MMusAvaObserver::TAvailabilityStatus state = 
                                MMusAvaObserver::EMusAvaStatusAvailable;
                                
    for ( TInt i = 0; i < iAvailabilities.Count() 
        && state > MMusAvaObserver::EMusAvaStatusNotExecuted; i++ )
        {
        state = iAvailabilities[i]->State();
        
        if ( iAvailabilities[i]->Name() == aAvailability )
            {
            retval = iAvailabilities[i]->State();
            }
        }
    MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaDefaultImp::AvailabilityState(%d)",
        retval )
    return retval;
    }
