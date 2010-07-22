/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  STUB implementation for SP Settings.
*
*/

#include "spsettings.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default Constructor 
// ---------------------------------------------------------------------------
//
CSPSettings::CSPSettings() 
    {
    // Nothing to be done
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPSettings::~CSPSettings()
    {
    // Nothing to be done
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPSettings* CSPSettings::NewL()
    {
    CSPSettings* self = CSPSettings::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPSettings* CSPSettings::NewLC()
    {
    CSPSettings* self = new( ELeave ) CSPSettings;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CSPSettings::ConstructL()
    {
    //Nothing to be done
    }

// ---------------------------------------------------------------------------
// Find service provider settings entry by service ID
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPSettings::FindEntryL( TServiceId aServiceId, CSPEntry& /*aEntry*/ )
    {
    TInt serviceID = 5;
    aServiceId = serviceID;
    return KErrNone;
    }


// End of File

