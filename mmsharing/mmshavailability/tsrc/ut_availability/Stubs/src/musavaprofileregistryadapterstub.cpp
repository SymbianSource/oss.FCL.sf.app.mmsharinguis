/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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


#include "musavaprofileregistryadapterstub.h"
#include "musavaobserver.h"
#include "musavaavailability.h"
#include "musavasharedobject.h"
#include "muslogger.h"
#include "musavasettingsimp.h"
#include "musavasip.h"
#include "musavaconnectionmonitor.h"
#include "mussettings.h"
#include "EscapeUtils.h"

#include <e32base.h>
#include <sipprofileregistryobserver.h>


// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
CMusAvaProfileRegistryAdapterStub* CMusAvaProfileRegistryAdapterStub::NewL()
    {
    CMusAvaProfileRegistryAdapterStub* self =
        CMusAvaProfileRegistryAdapterStub::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaProfileRegistryAdapterStub* CMusAvaProfileRegistryAdapterStub::NewLC()
    {
    CMusAvaProfileRegistryAdapterStub* self =
        new (ELeave) CMusAvaProfileRegistryAdapterStub();
    CleanupStack::PushL (self);
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
//  Destructor.
// ---------------------------------------------------------------------------
//
CMusAvaProfileRegistryAdapterStub::~CMusAvaProfileRegistryAdapterStub()
    {
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMusAvaProfileRegistryAdapterStub::ConstructL()
    {
    }


// -------------------------------------------------------------------------
//  Constructor.
// -------------------------------------------------------------------------
//
CMusAvaProfileRegistryAdapterStub::CMusAvaProfileRegistryAdapterStub()
    {
    iProfileId = 0;
    iEvent = MSIPProfileRegistryObserver::EProfileCreated;
    iError = -1;
    }

// -------------------------------------------------------------------------
// An event related to SIP Profile has accorred
// -------------------------------------------------------------------------
//
void CMusAvaProfileRegistryAdapterStub::ProfileRegistryEventOccurred(
                        TUint32 aProfileId,
                        MSIPProfileRegistryObserver::TEvent aEvent)
    {
    iProfileId = aProfileId;
    iEvent = aEvent;
    }

// -------------------------------------------------------------------------
// An asynchronous error has occurred related to SIP profile
// Event is send to those observers, who have the
// corresponding profile instantiated.
// -------------------------------------------------------------------------
//
void CMusAvaProfileRegistryAdapterStub::ProfileRegistryErrorOccurred(
    TUint32 aProfileId,
    TInt aError )
    {
    iProfileId = aProfileId;
    iError = aError;
    }

