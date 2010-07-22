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
#include "aiwtestappcontainer.h"
#include <eikenv.h>
#include <eiklabel.h>  // for example label control

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CAiwTestContainer::ConstructL(const TRect& aRect)
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------
//
void CAiwTestContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

    iLabel = new (ELeave) CEikLabel;
    iLabel->SetContainerWindowL( *this );
    iLabel->SetTextL( _L("Label") );

    iToDoLabel = new (ELeave) CEikLabel;
    iToDoLabel->SetContainerWindowL( *this );
    iToDoLabel->SetTextL( _L("TodoLabel") );

    SetRect(aRect);
    ActivateL();
    }

// ---------------------------------------------------------
// CAiwTestContainer::~CAiwTestContainer()
// Destructor
// ---------------------------------------------------------
//
CAiwTestContainer::~CAiwTestContainer()
    {
    delete iLabel;
    delete iToDoLabel;
    }

// ---------------------------------------------------------
// CAiwTestContainer::SetLabelL(TDesC& aText)
// ---------------------------------------------------------
//
void CAiwTestContainer::SetLabelL(TDesC& /*aText*/)
    {
    }

// ---------------------------------------------------------
// CAiwTestContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CAiwTestContainer::SizeChanged()
    {
    }

// ---------------------------------------------------------
// CAiwTestContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CAiwTestContainer::CountComponentControls() const
    {
    return 2; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CAiwTestContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CAiwTestContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            return iLabel;
        case 1:
            return iToDoLabel;
        default: 
            return NULL;
        }
    }

// ---------------------------------------------------------
// CAiwTestContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CAiwTestContainer::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
    
    gc.SetPenStyle( CGraphicsContext::ENullPen ); 
    gc.SetBrushColor( KRgbGray );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    
    gc.DrawRect( aRect );      
    }

// ---------------------------------------------------------
// CAiwTestContainer::HandleControlEventL(CCoeControl* aControl, 
//    TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CAiwTestContainer::HandleControlEventL(CCoeControl* /*aControl*/, 
    TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }
    
// End of File  
