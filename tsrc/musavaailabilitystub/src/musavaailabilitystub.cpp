/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provide interface for the client requestin availability class.
*
*/


#include "musavaavailabilitystub.h"
#include "musunittesting.h"
#include "musavasettingsimp.h"
#include "musavaobserver.h"
#include "musavaavailability.h"
#include "muslogger.h"

#include <e32base.h>

// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaAvailabilityStub* CMusAvaAvailabilityStub::NewL(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    CMusAvaAvailabilityStub* self =
        CMusAvaAvailabilityStub::NewLC(
            aObserver,
            aSettings );
    CleanupStack::Pop( self );
    return self;
    }

// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaAvailabilityStub* CMusAvaAvailabilityStub::NewLC(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    CMusAvaAvailabilityStub* self =
     new( ELeave ) CMusAvaAvailabilityStub(
        aObserver,
        aSettings );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// -------------------------------------------------------------------------
//  Destructor.
// -------------------------------------------------------------------------
//
CMusAvaAvailabilityStub::~CMusAvaAvailabilityStub()
    {

    }

// -------------------------------------------------------------------------
//  Starts the loopy execution.
// -------------------------------------------------------------------------
//
void CMusAvaAvailabilityStub::DoExecuteL()
    {
    if ( iExecuteLeave != 0 )
        {
        User::Leave( iExecuteLeave );
        }
    else if ( iExecuteError)
    	{
    	iObserver.AvailabilityChanged( 
    						(MMusAvaObserver::TAvailabilityName)iNameStub, 
    						(MMusAvaObserver::TAvailabilityStatus)iStatusStub );	
    	}
    else
        {
        iObserver.AvailabilityError( 
        					(MMusAvaObserver::TAvailabilityName)iNameStub, 
        					(MMusAvaObserver::TAvailabilityStatus)iStatusStub );
        }
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaAvailabilityStub::Name()
    {
    return iNameStub;
    }

// -------------------------------------------------------------------------
//  Constructor function.
// -------------------------------------------------------------------------
//
CMusAvaAvailabilityStub::CMusAvaAvailabilityStub(
								MMusAvaAvailabilityObserver& aObserver,
                                CMusAvaSettingsImp& aSettings)
    :CMusAvaAvailability( aObserver ),
    iSettings( aSettings )
    {
    }


// -------------------------------------------------------------------------
//  Second phase constructor.
// -------------------------------------------------------------------------
//
void CMusAvaAvailabilityStub::ConstructL()
    {
  
    }

