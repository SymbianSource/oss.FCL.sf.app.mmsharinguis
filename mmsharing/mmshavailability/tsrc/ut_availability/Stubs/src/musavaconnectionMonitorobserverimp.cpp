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

#include "musavaconnectionMonitorobserverimp.h"
#include "musavaobserver.h"
#include "CSipSseTestTls.h"
#include "muslogger.h"
#include "CSipSseTestTls.h"

#include <e32base.h>
#include <e32std.h>

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMMusAvaConnectionMonitorObserverImp* CMMusAvaConnectionMonitorObserverImp::NewL( )
	{
	MUS_LOG( " [MUSAVA ] ->  CMMusAvaConnectionMonitorObserverImp::NewL" );
	CMMusAvaConnectionMonitorObserverImp* self = new (ELeave) CMMusAvaConnectionMonitorObserverImp();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	MUS_LOG( " [MUSAVA ] <-  CMMusAvaConnectionMonitorObserverImp::NewL" );
	return self;
	}
	
// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMMusAvaConnectionMonitorObserverImp::~CMMusAvaConnectionMonitorObserverImp()
    {
    MUS_LOG( " [MUSAVA ] ->  CMMusAvaConnectionMonitorObserverImp::~CMMusAvaConnectionMonitorObserverImp" );
    
    MUS_LOG( " [MUSAVA ] <-  CMMusAvaConnectionMonitorObserverImp::~CMMusAvaConnectionMonitorObserverImp" );
    }
    
// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMMusAvaConnectionMonitorObserverImp::ConstructL()
	{
	MUS_LOG( " [MUSAVA ] ->  CMMusAvaConnectionMonitorObserverImp::ConstructL" );
	
	MUS_LOG( " [MUSAVA ] <-  CMMusAvaConnectionMonitorObserverImp::ConstructL" );
	}

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMMusAvaConnectionMonitorObserverImp::CMMusAvaConnectionMonitorObserverImp( )
	{
	MUS_LOG( "[MUSAVA ] -> CMMusAvaConnectionMonitorObserverImp::CMMusAvaConnectionMonitorObserverImp" );

	MUS_LOG( "[MUSAVA ] <- CMMusAvaConnectionMonitorObserverImp::CMMusAvaConnectionMonitorObserverImp" );
	} 		

	
// -------------------------------------------------------------------------
//  Constructor function.
// -------------------------------------------------------------------------
//
void CMMusAvaConnectionMonitorObserverImp::EventL(const CConnMonEventBase& aConnMonEvent )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMMusAvaConnectionMonitorObserverImp::EventL()" );
    
    iConnMonEventBase = const_cast<CConnMonEventBase*>( &aConnMonEvent );
  
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::EventL()" );
    }
     	


