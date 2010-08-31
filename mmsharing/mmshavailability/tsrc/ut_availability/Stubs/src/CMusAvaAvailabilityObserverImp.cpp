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


#include "musavaavailabilityobserverimp.h"
#include "musavaavailabilityobserver.h"
#include "musavaobserver.h"
#include "muslogger.h"


// -------------------------------------------------------------------------
// Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaAvailabilityObserverImp* CMusAvaAvailabilityObserverImp::NewL()
	{
	MUS_LOG( " [MUSAVA ] -> CMusAvaAvailabilityObserverImp::NewL" );
	CMusAvaAvailabilityObserverImp* self = new (ELeave) CMusAvaAvailabilityObserverImp();
	CleanupStack::PushL (self);
	self->ConstructL();
	CleanupStack::Pop();
	MUS_LOG( " [MUSAVA ] <- CMusAvaAvailabilityObserverImp::NewL" );
	return self;
	}
	
// -------------------------------------------------------------------------
// C++ destructor.
// -------------------------------------------------------------------------
//
CMusAvaAvailabilityObserverImp::~CMusAvaAvailabilityObserverImp()
    {
    MUS_LOG( " [MUSAVA ] -> CMusAvaAvailabilityObserverImp::~CMusAvaAvailabilityObserverImp" );
 
    MUS_LOG( " [MUSAVA ] <- CMusAvaAvailabilityObserverImp::~CMusAvaAvailabilityObserverImp" );
    }
    
// -------------------------------------------------------------------------
// Symbian second-phase constructor.
// -------------------------------------------------------------------------
//
void CMusAvaAvailabilityObserverImp::ConstructL()
	{
	MUS_LOG( " [MUSAVA ] -> CMusAvaAvailabilityObserverImp::ConstructL" );

	MUS_LOG( " [MUSAVA ] <- CMusAvaAvailabilityObserverImp::ConstructL" );
	}

// -------------------------------------------------------------------------
// C++ constructor.
// -------------------------------------------------------------------------
//
CMusAvaAvailabilityObserverImp::CMusAvaAvailabilityObserverImp() 
	{

	}
// -----------------------------------------------------------------------------
//  Availability report.
// -----------------------------------------------------------------------------
//    
void CMusAvaAvailabilityObserverImp::AvailabilityChanged(
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
void CMusAvaAvailabilityObserverImp::AvailabilityError(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus )
	{
	iStatus= aStatus;
    iName = aName; 		
	}	
	
// -----------------------------------------------------------------------------
//  Current availability status
// -----------------------------------------------------------------------------
//        
MMusAvaObserver::TAvailabilityStatus CMusAvaAvailabilityObserverImp::AvailabilityPluginState()
	{
	return iStatus;	
	}
// -----------------------------------------------------------------------------
//  Checks if the current availability is at least the same as given as 
//  parameter
// -----------------------------------------------------------------------------
//
TBool CMusAvaAvailabilityObserverImp::Available(
    MMusAvaObserver::TAvailabilityName aAvailability )
    {
    return iName > aAvailability
           || ( iName == aAvailability
           && MMusAvaObserver::EMusAvaStatusAvailable == iStatus );
    }

