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


#include "musavasettingsobserverimp.h"
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
CMusAvaSettingsObserverImp* CMusAvaSettingsObserverImp::NewL( )
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaSettingsObserverImp::NewL" );
	CMusAvaSettingsObserverImp* self = new (ELeave) CMusAvaSettingsObserverImp();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	MUS_LOG( " [MUSAVA ] <-  CMusAvaSettingsObserverImp::NewL" );
	return self;
	}
	
// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaSettingsObserverImp::~CMusAvaSettingsObserverImp()
    {
    MUS_LOG( " [MUSAVA ] ->  CMusAvaSettingsObserverImp::~CMusAvaSettingsObserverImp" );
    
    MUS_LOG( " [MUSAVA ] <-  CMusAvaSettingsObserverImp::~CMusAvaSettingsObserverImp" );
    }
    
// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsObserverImp::ConstructL()
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaSettingsObserverImp::ConstructL" );
	
	MUS_LOG( " [MUSAVA ] <-  CMusAvaSettingsObserverImp::ConstructL" );
	}

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusAvaSettingsObserverImp::CMusAvaSettingsObserverImp( )
	{
	MUS_LOG( "[MUSAVA ] -> CMusAvaSettingsObserverImp::CMusAvaSettingsObserverImp" );

	MUS_LOG( "[MUSAVA ] <- CMusAvaSettingsObserverImp::CMusAvaSettingsObserverImp" );
	} 		

     
 
// -----------------------------------------------------------------------------
//  Availability report.
// -----------------------------------------------------------------------------
//    
MMusAvaSettingsObserver::TApplicationState CMusAvaSettingsObserverImp::ApplicationState()
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaSettingsObserverImp::ApplicationState" );
	MUS_LOG( " [MUSAVA ] <-  CMusAvaSettingsObserverImp::ApplicationState" );
	return MMusAvaSettingsObserver::EApplicationIsRunning;
	}
	

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
//    
TBool CMusAvaSettingsObserverImp::OptionAllowed()
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaSettingsObserverImp::OptionAllowed" );
	MUS_LOG( " [MUSAVA ] <-  CMusAvaSettingsObserverImp::OptionAllowed" );
	return iOptionAllowed;
	}     	


