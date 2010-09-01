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
* Description:  Application class for MUS App.
*
*/



#include "musuiapp.h"           // Header file for this class
#include "musuidocument.h"      // Mus UI Document
#include "musuid.hrh"
#include "muslogger.h"          // debug logging

#include <eikstart.h>


// ============================ MEMBER FUNCTIONS ===========================

// Returns application UID
TUid CMusUiApp::AppDllUid() const
    {
    const TUid KUidRetVal = { KMusUiUid };
    return KUidRetVal;
    }

// Creates CMusUiDocument object
CApaDocument* CMusUiApp::CreateDocumentL()
    {
    return CMusUiDocument::NewL( *this );
    }


// ==================== OTHER EXPORTED FUNCTIONS ===========================

// Constructs and returns an application object
EXPORT_C CApaApplication* NewApplication()
    {
    MUS_LOG( "mus: [MUSUI ]      -------------------------------------------------" );
    return new CMusUiApp;
    }

// Main function of the application executable
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }

// end of file
