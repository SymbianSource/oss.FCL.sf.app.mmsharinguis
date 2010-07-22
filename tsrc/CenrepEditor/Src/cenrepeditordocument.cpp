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
#include "cenrepeditordocument.h"
#include "cenrepeditorappui.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCenrepEditorDocument::CCenrepEditorDocument(CEikApplication& aApp)
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCenrepEditorDocument::CCenrepEditorDocument(CEikApplication& aApp)
: CAknDocument(aApp)    
    {
    }

// -----------------------------------------------------------------------------
// CCenrepEditorDocument::~CCenrepEditorDocument()
// Destructor.
// -----------------------------------------------------------------------------
//
CCenrepEditorDocument::~CCenrepEditorDocument()
    {
     
    }

// -----------------------------------------------------------------------------
// CCenrepEditorDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCenrepEditorDocument::ConstructL()
    {   
    
    }

// -----------------------------------------------------------------------------
// CCenrepEditorDocument::NewL(CEikApplication& aApp)
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCenrepEditorDocument* CCenrepEditorDocument::NewL(CEikApplication& aApp)     // CCenrepEditorApp reference
    {
    CCenrepEditorDocument* self = new (ELeave) CCenrepEditorDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
// ----------------------------------------------------
// CCenrepEditorDocument::CreateAppUiL()
// Constructs CCenrepEditorAppUi.
// ----------------------------------------------------
//
CEikAppUi* CCenrepEditorDocument::CreateAppUiL()
    {
    return new (ELeave) CCenrepEditorAppUi;
    }

