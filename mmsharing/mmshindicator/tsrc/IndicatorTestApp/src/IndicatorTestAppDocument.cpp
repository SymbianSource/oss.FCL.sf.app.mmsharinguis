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

#include "IndicatorTestAppAppUi.h"
#include "IndicatorTestAppDocument.h"

// Standard Symbian OS construction sequence
CIndicatorTestAppDocument* CIndicatorTestAppDocument::NewL(CEikApplication& aApp)
    {
    CIndicatorTestAppDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

CIndicatorTestAppDocument* CIndicatorTestAppDocument::NewLC(CEikApplication& aApp)
    {
    CIndicatorTestAppDocument* self = new (ELeave) CIndicatorTestAppDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

void CIndicatorTestAppDocument::ConstructL()
    {
	// no implementation required
    }    

CIndicatorTestAppDocument::CIndicatorTestAppDocument(CEikApplication& aApp) : CAknDocument(aApp) 
    {
	// no implementation required
    }

CIndicatorTestAppDocument::~CIndicatorTestAppDocument()
    {
	// no implementation required
    }

CEikAppUi* CIndicatorTestAppDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) CIndicatorTestAppAppUi;
    return appUi;
    }

