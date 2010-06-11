/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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

#include "IndicatorTestAppDocument.h"
#include "IndicatorTestAppApplication.h"

// UID for the application, this should correspond to the uid defined in the mmp file
static const TUid KUidIndicatorTestAppApp = {0x0B821B86};

CApaDocument* CIndicatorTestAppApplication::CreateDocumentL()
    {  
    // Create an IndicatorTestApp document, and return a pointer to it
    CApaDocument* document = CIndicatorTestAppDocument::NewL(*this);
    return document;
    }

TUid CIndicatorTestAppApplication::AppDllUid() const
    {
    // Return the UID for the IndicatorTestApp application
    return KUidIndicatorTestAppApp;
    }

