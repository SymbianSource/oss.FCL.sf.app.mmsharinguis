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
* Description:   This is client interface class for chared objects MusAvailability Plug-in
*
*/



// USER
#include "musunittesting.h"
#include "musavasharedobject.h"
#include "musavasip.h"
#include "musavasipobserver.h"
#include "musavaconnectionmonitor.h"
#include "musavanetworkregistrationstatus.h"
#include "musavanetworkmodestatus.h"
#include "muslogger.h"
#include "musavailabilityplugin.hrh"

// SYSTEM
#include <e32base.h>
#include <sip.h>

CMusAvaSharedObject* CMusAvaSharedObject::iSelf = 0;
TInt CMusAvaSharedObject::iSelfCounter = 0;

// -----------------------------------------------------------------------------
// Delete singleton object
// -----------------------------------------------------------------------------
//
void CMusAvaSharedObject::DeleteSingleton()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSharedObject::DeleteSingleton" )
    iSelfCounter--;
    if( !iSelfCounter )
        {
        delete iSelf;
        iSelf = NULL;
        }
    MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaSharedObject::DeleteSingleton count=%d",\
                                                                    iSelfCounter )
    }

// -----------------------------------------------------------------------------
// Returns singleton object
// -----------------------------------------------------------------------------
//
CMusAvaSharedObject* CMusAvaSharedObject::GetSingletonL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSharedObject::GetSingletonL" )
    if(!iSelf)
        {
        iSelf = CMusAvaSharedObject::NewL();        
        MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaSharedObject::GetSingletonL\
                                                    count=%d",iSelfCounter )
        }
    else
        {        
        MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaSharedObject::GetSingletonL\
                                                    count=%d",iSelfCounter )
        }
    iSelfCounter++;
    return iSelf;
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaSharedObject* CMusAvaSharedObject::NewL(  )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSharedObject::NewL" )
    CMusAvaSharedObject* self = new (ELeave) CMusAvaSharedObject;
    CleanupStack::PushL (self);
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSharedObject::NewL" )
    return self;
    }

// -------------------------------------------------------------------------
// C++ Constructor
// -------------------------------------------------------------------------
//
CMusAvaSharedObject::CMusAvaSharedObject()
    {

    }

// -------------------------------------------------------------------------
// Symbian second-phase constructor.
// -------------------------------------------------------------------------
//
void CMusAvaSharedObject::ConstructL()
    {
    iMusAvaSip = CMusAvaSip::NewL();
    iConnectionMonitor = CMusAvaConnectionMonitor::NewL( *this );
    iNetworkRegistrationStatus = CMusAvaNetworkRegistrationStatus::NewL( );	
    iNetworkModeStatus = CMusAvaNetworkModeStatus::NewL();
    }

// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaSharedObject::~CMusAvaSharedObject()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSharedObject::~CMusAvaSharedObject" )
    delete iMusAvaSip;
    delete iConnectionMonitor;
    delete iNetworkRegistrationStatus;
    delete iNetworkModeStatus;
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSharedObject::~CMusAvaSharedObject" )
    }

// -------------------------------------------------------------------------
// Returns CMusAvaConnectionMonitor object 
// -------------------------------------------------------------------------
//
CMusAvaConnectionMonitor& CMusAvaSharedObject::ConnectionMonitor()
    {
    return *iConnectionMonitor;
    }
    
// -------------------------------------------------------------------------
// Returns CSip object
// -------------------------------------------------------------------------
//
CSIP& CMusAvaSharedObject::Sip()
    {
    return iMusAvaSip->Sip();
    }

// -------------------------------------------------------------------------
// Returns CMusAvaSip object
// -------------------------------------------------------------------------
//
 CMusAvaSip& CMusAvaSharedObject::MusAvaSip()
    {
    return *iMusAvaSip;
    }
// -------------------------------------------------------------------------
// Returns CMusAvaNetworkRegistrationStatus object
// -------------------------------------------------------------------------
//
CMusAvaNetworkRegistrationStatus& CMusAvaSharedObject::MusAvaTelephonyStatus()
    {
    return *iNetworkRegistrationStatus;
    }
	
// -------------------------------------------------------------------------
// Returns CMusAvaNetworkModeStatus object
// -------------------------------------------------------------------------
//
CMusAvaNetworkModeStatus& CMusAvaSharedObject::NetworkModeStatus()
    {
    return *iNetworkModeStatus;
    }     





