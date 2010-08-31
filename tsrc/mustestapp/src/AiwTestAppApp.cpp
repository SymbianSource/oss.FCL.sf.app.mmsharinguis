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
#include <eikstart.h>
#include "aiwtestappapp.h"
#include "aiwtestappdocument.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CAiwTestApp::AppDllUid()
// Returns application UID
// ---------------------------------------------------------
//
TUid CAiwTestApp::AppDllUid() const
    {
    return KUidAiwTestApp;
    }
   
// ---------------------------------------------------------
// CAiwTestApp::CreateDocumentL()
// Creates CAiwTestDocument object
// ---------------------------------------------------------
//
CApaDocument* CAiwTestApp::CreateDocumentL()
    {
    return CAiwTestDocument::NewL( *this );
    }

LOCAL_C CApaApplication* NewApplication()
    {
    return new CAiwTestApp;
    }

// ---------------------------------------------------------
// E32Main()
// Main startup entry point
// Returns: KErrNone
// ---------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }

// End of File  

