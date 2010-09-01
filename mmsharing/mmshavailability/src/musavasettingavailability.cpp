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
* Description:   Implements Settings Availability
*
*/



#include "musavaavailabilityobserver.h"
#include "musavaavailability.h"
#include "musunittesting.h"
#include "musavaobserver.h"
#include "muslogger.h"
#include "mussettings.h"
#include <e32base.h>
#include <e32property.h>

#include "musavasettingavailability.h"
#include "mussesseioninformationapi.h"



// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
CMusAvaSettingAvailability* CMusAvaSettingAvailability::NewL(
    MMusAvaAvailabilityObserver& aObserver)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSettingAvailability::NewL " )
    CMusAvaSettingAvailability* self =
        CMusAvaSettingAvailability::NewLC(
            aObserver);
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSettingAvailability::NewL " )
    return self;
    }


// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaSettingAvailability* CMusAvaSettingAvailability::NewLC(
    MMusAvaAvailabilityObserver& aObserver)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSettingAvailability::NewLC " )
    CMusAvaSettingAvailability* self =
        new (ELeave) CMusAvaSettingAvailability(
            aObserver);
    CleanupStack::PushL (self);
    self->ConstructL();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSettingAvailability::NewLC " )
    return self;
    }


// ---------------------------------------------------------------------------
//  Destructor.
// ---------------------------------------------------------------------------
//
CMusAvaSettingAvailability::~CMusAvaSettingAvailability()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSettingAvailability::\
                                            ~CMusAvaSettingAvailability " )    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSettingAvailability::\
                                            ~CMusAvaSettingAvailability " )
    
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMusAvaSettingAvailability::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSettingAvailability::ConstructL " )
    // set this only for static availability modules
    CheckActivationState();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSettingAvailability::ConstructL " )
    }


// -------------------------------------------------------------------------
//  Constructor.
// -------------------------------------------------------------------------
//
CMusAvaSettingAvailability::CMusAvaSettingAvailability(
    MMusAvaAvailabilityObserver& aObserver)
    :CMusAvaAvailability( aObserver )
    {
    }


// -------------------------------------------------------------------------
// Executes for the master - salve pattern.
// -------------------------------------------------------------------------
//
void CMusAvaSettingAvailability::DoExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSettingAvailability::DoExecuteL " )
    CheckActivationState();
    SetState(iState);
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSettingAvailability::DoExecuteL " )
    }
   
// -------------------------------------------------------------------------
// CMusAvaSettingAvailability::CheckActivationState
// -------------------------------------------------------------------------
//
void CMusAvaSettingAvailability::CheckActivationState()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSettingAvailability::SetActivationState " )
    MusSettingsKeys::TActivation activation = MusSettingsKeys::EAlwaysActive;
    TRAPD( err, activation = 
                       MultimediaSharingSettings::ActivationSettingL() );
    if( err == KErrNone )
        {
        switch( activation )
            {                
            case MusSettingsKeys::ENever:
            case MusSettingsKeys::EActiveInHomeNetworks: 
                 iState = MMusAvaObserver::EMusActivationError ;    
                 break;
            case MusSettingsKeys::EAlwaysActive:  
            default:                 
                if ( !IsForbiddenByOptionsResponse())  
                    {
                    iState = MMusAvaObserver::EMusAvaStatusAvailable ;       
                    }
                 else
                    {
                    iState = MMusAvaObserver::EMusAvaFailureCode ;
                    }
                 break;               
            }
        }
    else
        {
        iState = MMusAvaObserver::EMusActivationError ;
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSettingAvailability::SetActivationState " )
    }
// -------------------------------------------------------------------------
// Stop the execution.
// -------------------------------------------------------------------------
//
void CMusAvaSettingAvailability::Stop()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSettingAvailability::Stop" )    
    SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSettingAvailability::Stop" )
    }


// -------------------------------------------------------------------------
// CMusAvaSettingAvailability::Name
//
// -------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaSettingAvailability::Name()
    {
    return MMusAvaObserver::EMusAvaNameSetting;
    }


// -------------------------------------------------------------------------
// CMusAvaSettingAvailability::IsForbiddenByOptionsResponse
//
// -------------------------------------------------------------------------
//
TBool CMusAvaSettingAvailability::IsForbiddenByOptionsResponse()
    {
    MUS_LOG( "mus: [MUSAVA]  -> \
                 CMusAvaSettingAvailability::IsForbiddenByOptionsResponse" )        
    TInt forbidden;
    TInt err= RProperty::Get( NMusSessionInformationApi::KCategoryUid ,
	                NMusSessionInformationApi::KMUSForbidden ,
	                forbidden );
    TBool result = EFalse;	
    /* If the property is not defined then it means options/operator
       specification may not exist . So we return it is Not forbidden
    */
	if ( err==KErrNone  &&
         NMusSessionInformationApi::EMUSForbidden == forbidden )
		{
        result = ETrue;
		}    

    MUS_LOG1( "mus: [MUSAVA]  <- \
        CMusAvaSettingAvailability::IsForbiddenByOptionsResponse = %d",result )    
    return result;
    }
 



// end of file


