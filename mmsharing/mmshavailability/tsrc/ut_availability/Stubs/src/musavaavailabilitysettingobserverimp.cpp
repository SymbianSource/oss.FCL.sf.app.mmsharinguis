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
* Description:  Implementation class for creating master and slave pattern objects.
*
*/


#include "musavaavailabilitysettingobserverimp.h"
#include "musunittesting.h"
#include "musavaobserver.h"
#include "CSipSseTestTls.h"
#include "muslogger.h"


#include <e32base.h>
#include <e32std.h>

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusAvaAvailabilitySettingObserverImp* CMusAvaAvailabilitySettingObserverImp::NewL( )
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaAvailabilitySettingObserverImp::NewL" );
	CMusAvaAvailabilitySettingObserverImp* self = new (ELeave) CMusAvaAvailabilitySettingObserverImp();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	MUS_LOG( " [MUSAVA ] <-  CMusAvaAvailabilitySettingObserverImp::NewL" );
	return self;
	}
	
// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaAvailabilitySettingObserverImp::~CMusAvaAvailabilitySettingObserverImp()
    {
    MUS_LOG( " [MUSAVA ] ->  CMusAvaAvailabilitySettingObserverImp::~CMusAvaAvailabilitySettingObserverImp" );
    
    MUS_LOG( " [MUSAVA ] <-  CMusAvaAvailabilitySettingObserverImp::~CMusAvaAvailabilitySettingObserverImp" );
    }
    
// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAvaAvailabilitySettingObserverImp::ConstructL()
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaAvailabilitySettingObserverImp::ConstructL" );
	iAvailabilityReport = EFalse;
	iStopComplete = EFalse;
	MUS_LOG( " [MUSAVA ] <-  CMusAvaAvailabilitySettingObserverImp::ConstructL" );
	}

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusAvaAvailabilitySettingObserverImp::CMusAvaAvailabilitySettingObserverImp( )
	{
	MUS_LOG( "[MUSAVA ] -> CMusAvaAvailabilitySettingObserverImp::CMusAvaAvailabilitySettingObserverImp" );

	MUS_LOG( "[MUSAVA ] <- CMusAvaAvailabilitySettingObserverImp::CMusAvaAvailabilitySettingObserverImp" );
	} 		


void CMusAvaAvailabilitySettingObserverImp::AvailabilityReport( 
                        MMusAvaObserver::TAvailabilityState aState,
                        MMusAvaObserver::TAvailabilityStatus aStatus,
                        MMusAvaObserver::TAvailabilityName aName,
                        TInt aSystemError )
    {
    iAvailabilityReport = ETrue;
    iState = aState;
    iStatus = aStatus;
    iName = aName;
    iSystemError = aSystemError;
    }
                            
                            
 void CMusAvaAvailabilitySettingObserverImp::StopComplete( MMusAvaObserver::TAvailabilityState aState,
                               MMusAvaObserver::TAvailabilityName aName,
                               TInt aSystemError )
    {
    iStopComplete = ETrue;
    iState = aState;
    iName = aName;
    iSystemError = aSystemError;
    }
     
 
	

     	


