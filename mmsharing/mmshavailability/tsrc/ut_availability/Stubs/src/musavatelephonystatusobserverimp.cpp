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

#include "musavatelephonystatusobserverimp.h"
#include "musavaobserver.h"
#include "CSipSseTestTls.h"
#include "muslogger.h"
#include "CSipSseTestTls.h"

#include <e32base.h>
#include <e32std.h>
#include <etelmm.h>
// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusAvaTelephonyStatusObserverImp* CMusAvaTelephonyStatusObserverImp::NewL( )
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaTelephonyStatusObserverImp::NewL" );
	CMusAvaTelephonyStatusObserverImp* self =
	new (ELeave) CMusAvaTelephonyStatusObserverImp();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	MUS_LOG( " [MUSAVA ] <-  CMusAvaTelephonyStatusObserverImp::NewL" );
	return self;
	}
	
// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaTelephonyStatusObserverImp::~CMusAvaTelephonyStatusObserverImp()
    {
    MUS_LOG( " [MUSAVA ] ->  CMusAvaTelephonyStatusObserverImp::\
    ~CMusAvaTelephonyStatusObserverImp" );
    
    MUS_LOG( " [MUSAVA ] <-  CMusAvaTelephonyStatusObserverImp::\
    ~CMusAvaTelephonyStatusObserverImp" );
    }
    
// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAvaTelephonyStatusObserverImp::ConstructL()
	{
	MUS_LOG( " [MUSAVA ] ->  CMusAvaTelephonyStatusObserverImp::ConstructL" );
	
	MUS_LOG( " [MUSAVA ] <-  CMusAvaTelephonyStatusObserverImp::ConstructL" );
	}

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusAvaTelephonyStatusObserverImp::CMusAvaTelephonyStatusObserverImp( )
	{
	MUS_LOG( "[MUSAVA ] -> CMusAvaTelephonyStatusObserverImp::\
	CMusAvaTelephonyStatusObserverImp" );

	MUS_LOG( "[MUSAVA ] <- CMusAvaTelephonyStatusObserverImp::\
	CMusAvaTelephonyStatusObserverImp" );
	} 		

	
// -------------------------------------------------------------------------
//  
// -------------------------------------------------------------------------
//
void CMusAvaTelephonyStatusObserverImp::NetworkRegistrationStatus( 
                         RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaTelephonyStatusObserverImp::\
    NetworkRegistrationStatus()" );
    
    iPhoneRegistrationStatus = aRegStatus;
  
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaTelephonyStatusObserverImp::\
    NetworkRegistrationStatus()" );
    }
    
// -------------------------------------------------------------------------
//  
// -------------------------------------------------------------------------
//
void CMusAvaTelephonyStatusObserverImp::PhoneNetworkModeStatus( 
                         RMobilePhone::TMobilePhoneNetworkMode aStatus )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaTelephonyStatusObserverImp::\
    PhoneNetworkModeStatus()" );
    
    iPhoneNetworkMode = aStatus;
  
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaTelephonyStatusObserverImp::\
    PhoneNetworkModeStatus()" );
    }
     	


