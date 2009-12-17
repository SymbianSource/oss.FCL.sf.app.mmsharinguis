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
* Description:   mussipobserver is SIP API observer class for the MUS availability plug-in.
*
*/



#include "musavasipprofileregistryobserver.h"
#include "musavasipprofileregistryadapter.h"
#include "muslogger.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CMusAvaSipProfileRegistryObserver* CMusAvaSipProfileRegistryObserver::NewL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipProfileRegistryObserver::NewL" )
    CMusAvaSipProfileRegistryObserver* self
                          = new( ELeave ) CMusAvaSipProfileRegistryObserver();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipProfileRegistryObserver::NewL" )
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CMusAvaSipProfileRegistryObserver::~CMusAvaSipProfileRegistryObserver()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipProfileRegistryObserver::~CMusAvaSipProfileRegistryObserver" )
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipProfileRegistryObserver::~CMusAvaSipProfileRegistryObserver" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSipProfileRegistryObserver::AddAdapterL( MMusAvaSipProfileRegistryAdapter& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipProfileRegistryObserver::AddAdapter" )
    iAdapter = &aAdapter;
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipProfileRegistryObserver::AddAdapter" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSipProfileRegistryObserver::RemoveAdapter( MMusAvaSipProfileRegistryAdapter& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipProfileRegistryObserver::RemoveAdapter" )
    if ( iAdapter == &aAdapter )
        {
        iAdapter = NULL;
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipProfileRegistryObserver::RemoveAdapter" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSipProfileRegistryObserver::ProfileRegistryEventOccurred(TUint32 aProfileId,
                                          TEvent aEvent)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipProfileRegistryObserver::ProfileRegistryEventOccurred" )
    if ( iAdapter )
        {
        iAdapter->ProfileRegistryEventOccurred( aProfileId, aEvent );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipProfileRegistryObserver::ProfileRegistryEventOccurred" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaSipProfileRegistryObserver::ProfileRegistryErrorOccurred(TUint32 aProfileId,
                                          TInt aError)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSipProfileRegistryObserver::ProfileRegistryErrorOccurred" )
    if ( iAdapter )
        {
        iAdapter->ProfileRegistryErrorOccurred( aProfileId, aError );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSipProfileRegistryObserver::ProfileRegistryErrorOccurred" )
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------    
CMusAvaSipProfileRegistryObserver::CMusAvaSipProfileRegistryObserver()
    {
    }

