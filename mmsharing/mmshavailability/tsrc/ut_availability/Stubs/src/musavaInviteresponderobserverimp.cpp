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

#include "musavaInviteresponderobserverimp.h"
#include "musavaobserver.h"
#include "CSipSseTestTls.h"
#include "muslogger.h"


#include <e32base.h>
#include <e32std.h>

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusAvaInviteResponderObserverImp* CMusAvaInviteResponderObserverImp::NewL( )
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaInviteResponderObserverImp::NewL" );
	CMusAvaInviteResponderObserverImp* self = new (ELeave) CMusAvaInviteResponderObserverImp();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	MUS_LOG( " [MUSAVA ] <-  CMusAvaInviteResponderObserverImp::NewL" );
	return self;
	}
	
// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaInviteResponderObserverImp::~CMusAvaInviteResponderObserverImp()
    {
    MUS_LOG( " [MUSAVA ] ->  CMusAvaInviteResponderObserverImp::~CMusAvaInviteResponderObserverImp" );
    
    MUS_LOG( " [MUSAVA ] <-  CMusAvaInviteResponderObserverImp::~CMusAvaInviteResponderObserverImp" );
    }
    
// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAvaInviteResponderObserverImp::ConstructL()
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaInviteResponderObserverImp::ConstructL" );
	iApplicationState = MMusAvaSettingsObserver::EApplicationStateNotDefined;
	MUS_LOG( " [MUSAVA ] <-  CMusAvaInviteResponderObserverImp::ConstructL" );
	}

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusAvaInviteResponderObserverImp::CMusAvaInviteResponderObserverImp( )
	{
	MUS_LOG( "[MUSAVA ] -> CMusAvaInviteResponderObserverImp::CMusAvaInviteResponderObserverImp" );

	MUS_LOG( "[MUSAVA ] <- CMusAvaInviteResponderObserverImp::CMusAvaInviteResponderObserverImp" );
	} 		

// -----------------------------------------------------------------------------
//  Availability report.
// -----------------------------------------------------------------------------
//    
MMusAvaSettingsObserver::TApplicationState CMusAvaInviteResponderObserverImp::ApplicationState()
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaInviteResponderObserverImp::AvailabilityReport" );
    return iApplicationState;
	MUS_LOG( " [MUSAVA ] <-  CMusAvaInviteResponderObserverImp::AvailabilityReport" );
	}
	
// -----------------------------------------------------------------------------
// MusAvailability shutdown is completed.
// -----------------------------------------------------------------------------
//    
void CMusAvaInviteResponderObserverImp::InvitationAnsweredLD()
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaInviteResponderObserverImp::StopComplete" );

	MUS_LOG( " [MUSAVA ] ->  CMusAvaInviteResponderObserverImp::StopComplete" );
	}

     	


