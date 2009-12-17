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

#include <coemain.h>
#include <IndicatorTestApp.rsg>

#include "IndicatorTestAppAppView.h"

// Standard construction sequence
CIndicatorTestAppAppView* CIndicatorTestAppAppView::NewL(const TRect& aRect)
    {
    CIndicatorTestAppAppView* self = CIndicatorTestAppAppView::NewLC(aRect);
    CleanupStack::Pop(self);
    return self;
    }

CIndicatorTestAppAppView* CIndicatorTestAppAppView::NewLC(const TRect& aRect)
    {
    CIndicatorTestAppAppView* self = new (ELeave) CIndicatorTestAppAppView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
    }

CIndicatorTestAppAppView::CIndicatorTestAppAppView()
    {
	// no implementation required
    }

CIndicatorTestAppAppView::~CIndicatorTestAppAppView()
    {
	// no implementation required
    }

void CIndicatorTestAppAppView::ConstructL(const TRect& aRect)
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect(aRect);

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// Draw this application's view to the screen
void CIndicatorTestAppAppView::Draw(const TRect& /*aRect*/) const
    {
    // Get the standard graphics context 
    CWindowGc& gc = SystemGc();
    
    // Gets the control's extent
    TRect rect = Rect();
    
    // Clears the screen
    gc.Clear(rect);
    }


