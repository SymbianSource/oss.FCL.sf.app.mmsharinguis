/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Document class of MUS App.
*
*/



#include "musuidocument.h"        // Header for this class
#include "musuiappui.h"           // Application UI class

#include <AknGlobalNote.h>
#include <musui.rsg>


// -----------------------------------------------------------------------------
// CMusUiDocument::CMusUiDocument
// C++ default constructor.
// -----------------------------------------------------------------------------
CMusUiDocument::CMusUiDocument( CEikApplication& aApp ) : CAknDocument( aApp )
    {
    }


// -----------------------------------------------------------------------------
// CMusUiDocument::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
CMusUiDocument* CMusUiDocument::NewL( CEikApplication& aApp )
    {
    CMusUiDocument* self = new( ELeave ) CMusUiDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMusUiDocument::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
void CMusUiDocument::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// CMusUiDocument::~CMusUiDocument
// Destructor.
// -----------------------------------------------------------------------------
CMusUiDocument::~CMusUiDocument()
    {
    }


// -----------------------------------------------------------------------------
// CMusUiDocument::CreateAppUiL()
// Constructs CMusUiAppUi.
// -----------------------------------------------------------------------------
CEikAppUi* CMusUiDocument::CreateAppUiL()
    {
    return new ( ELeave ) CMusUiAppUi( );
    }


// end of file
