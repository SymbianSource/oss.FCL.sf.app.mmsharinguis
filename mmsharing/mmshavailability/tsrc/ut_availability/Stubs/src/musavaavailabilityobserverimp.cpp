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


#include "musunittesting.h"

#include "musavaavailabilityobserverimp.h"
#include "musavaobserver.h"
#include "CSipSseTestTls.h"
#include "muslogger.h"


#include <e32base.h>
#include <e32std.h>

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusAvaAvailabilityObserverImp* CMusAvaAvailabilityObserverImp::NewL( )
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaAvailabilityObserverImp::NewL" );
	CMusAvaAvailabilityObserverImp* self = new (ELeave) CMusAvaAvailabilityObserverImp();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	MUS_LOG( " [MUSAVA ] <-  CMusAvaAvailabilityObserverImp::NewL" );
	return self;
	}
	
// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaAvailabilityObserverImp::~CMusAvaAvailabilityObserverImp()
    {
    MUS_LOG( " [MUSAVA ] ->  CMusAvaAvailabilityObserverImp::~CMusAvaAvailabilityObserverImp" );
    
    MUS_LOG( " [MUSAVA ] <-  CMusAvaAvailabilityObserverImp::~CMusAvaAvailabilityObserverImp" );
    }
    
// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAvaAvailabilityObserverImp::ConstructL()
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaAvailabilityObserverImp::ConstructL" );
	
	MUS_LOG( " [MUSAVA ] <-  CMusAvaAvailabilityObserverImp::ConstructL" );
	}

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusAvaAvailabilityObserverImp::CMusAvaAvailabilityObserverImp( )
	{
	MUS_LOG( "[MUSAVA ] -> CMusAvaAvailabilityObserverImp::CMusAvaAvailabilityObserverImp" );

	MUS_LOG( "[MUSAVA ] <- CMusAvaAvailabilityObserverImp::CMusAvaAvailabilityObserverImp" );
	} 		

     
 

// -----------------------------------------------------------------------------
//  Clear atributes.
// -----------------------------------------------------------------------------
// 	
void CMusAvaAvailabilityObserverImp::Clear()
    {
    iError = KErrNone;
    iStatus = MMusAvaObserver::EMusAvaStatusNotAvailable;
    iName = MMusAvaObserver::EMusAvaNameNotDefined;	
    }
// -----------------------------------------------------------------------------
// MusAvailability shutdown is completed.
// -----------------------------------------------------------------------------
//    
/*void CMusAvaAvailabilityObserverImp::StopComplete(
                                    MMusAvaObserver::TAvailabilityName aName,
                                    TInt aSystemError  )
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaAvailabilityObserverImp::StopComplete" );
	CompineStopResult( aName, aSystemError );
	MUS_LOG( " [MUSAVA ] ->  CMusAvaAvailabilityObserverImp::StopComplete" );
	}*/

     	


