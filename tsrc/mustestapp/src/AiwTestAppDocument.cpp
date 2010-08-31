/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/


// INCLUDE FILES
#include "aiwtestappdocument.h"
#include "aiwtestappappui.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CAiwTestDocument::CAiwTestDocument(CEikApplication& aApp)
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAiwTestDocument::CAiwTestDocument(CEikApplication& aApp)
: CAknDocument(aApp)    
    {
    }

// -----------------------------------------------------------------------------
// CAiwTestDocument::~CAiwTestDocument()
// Destructor.
// -----------------------------------------------------------------------------
//
CAiwTestDocument::~CAiwTestDocument()
    {
    }

// -----------------------------------------------------------------------------
// CAiwTestDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAiwTestDocument::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CAiwTestDocument::NewL(CEikApplication& aApp)
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAiwTestDocument* CAiwTestDocument::NewL(CEikApplication& aApp)     // CAiwTestApp reference
    {
    CAiwTestDocument* self = new (ELeave) CAiwTestDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
// ----------------------------------------------------
// CAiwTestDocument::CreateAppUiL()
// Constructs CAiwTestAppUi.
// ----------------------------------------------------
//
CEikAppUi* CAiwTestDocument::CreateAppUiL()
    {
    return new (ELeave) CAiwTestAppUi;
    }

// End of File  
