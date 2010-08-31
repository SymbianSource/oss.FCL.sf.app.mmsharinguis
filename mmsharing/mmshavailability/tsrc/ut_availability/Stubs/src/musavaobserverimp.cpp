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
* Description:  This observer is used by
*                ECOM clients to get function call from the availability Plug-in
*
*/


#include "musavaobserverimp.h"
#include "musavaavailabilityobserver.h"
#include "musavaobserver.h"
#include "muslogger.h"


// -------------------------------------------------------------------------
// Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaObserverImp* CMusAvaObserverImp::NewL()
	{
	MUS_LOG( " [MUSAVA ] -> CMusAvaObserverImp::NewL" );
	CMusAvaObserverImp* self = new (ELeave) CMusAvaObserverImp();
	CleanupStack::PushL (self);
	self->ConstructL();
	CleanupStack::Pop();
	MUS_LOG( " [MUSAVA ] <- CMusAvaObserverImp::NewL" );
	return self;
	}
	
// -------------------------------------------------------------------------
// C++ destructor.
// -------------------------------------------------------------------------
//
CMusAvaObserverImp::~CMusAvaObserverImp()
    {
    MUS_LOG( " [MUSAVA ] -> CMusAvaObserverImp::~CMusAvaObserverImp" );
 
    MUS_LOG( " [MUSAVA ] <- CMusAvaObserverImp::~CMusAvaObserverImp" );
    }
    
// -------------------------------------------------------------------------
// Symbian second-phase constructor.
// -------------------------------------------------------------------------
//
void CMusAvaObserverImp::ConstructL()
	{
	MUS_LOG( " [MUSAVA ] -> CMusAvaObserverImp::ConstructL" );

	MUS_LOG( " [MUSAVA ] <- CMusAvaObserverImp::ConstructL" );
	}

// -------------------------------------------------------------------------
// C++ constructor.
// -------------------------------------------------------------------------
//
CMusAvaObserverImp::CMusAvaObserverImp() 
	{

	}
// -----------------------------------------------------------------------------
//  Availability report.
// -----------------------------------------------------------------------------
//    
void CMusAvaObserverImp::AvailabilityChanged(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus )
	{
    iStatus= aStatus;
    iName = aName; 	
	}
	
// -----------------------------------------------------------------------------
//  Availability error report.
// -----------------------------------------------------------------------------
// 
void CMusAvaObserverImp::AvailabilityError(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus )
	{
	iStatus= aStatus;
    iName = aName; 		
	}	
	


