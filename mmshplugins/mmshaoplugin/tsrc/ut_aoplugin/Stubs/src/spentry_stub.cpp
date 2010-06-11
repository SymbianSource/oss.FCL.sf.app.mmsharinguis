/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service Provider Settings entry. 
*
*/

#include "spentry.h"
#include "spproperty.h"
//#include "spdefaultvalues.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPEntry::CSPEntry()
    {
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CSPEntry::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPEntry* CSPEntry::NewL()
    {
    CSPEntry* self = CSPEntry::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPEntry* CSPEntry::NewLC()
    {
    CSPEntry* self = new( ELeave ) CSPEntry;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPEntry::~CSPEntry()
    {
    iServiceName.Close();
    iPropertyArray.ResetAndDestroy();
    iPropertyArray.Close();
    }

// ---------------------------------------------------------------------------
// Returns service name of this entry
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CSPEntry::GetServiceName() const
    {
    _LIT( KTestProvideName, "VoIP" );
    return  KTestProvideName;
    }


// End of File.
