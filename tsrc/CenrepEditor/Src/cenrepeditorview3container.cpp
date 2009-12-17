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
* Description:
*
*/


// INCLUDE FILES
#include "cenrepeditorview3container.h"
#include "cenrepeditordocument.h"
#include "cenrepeditorappui.h"
#include  "cenrepeditormodel.h"



#include <eiklabel.h>  // for example label control


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCenrepEditorView3Container::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CCenrepEditorView3Container::ConstructL(const TRect& aRect,TInt aResourceId, CMusApplication*  aMusApplication)
    {      
    CreateWindowL(); 
    iMusApplicationListbox = new(ELeave)CMusApplicationListbox;
    // and put the data in the list
    iMusApplicationListbox->SetData(aMusApplication);
    // setting list is constructed from resources
    iMusApplicationListbox->ConstructFromResourceL(aResourceId);     
    SetRect(aRect);
    iMusApplicationListbox->MakeVisible(ETrue);
    iMusApplicationListbox->SetRect(aRect);    
	iMusApplicationListbox->LoadSettingsL();    
    iMusApplicationListbox->ActivateL();                
    ActivateL();
    }

// Destructor
CCenrepEditorView3Container::~CCenrepEditorView3Container()
    {
    iMusApplicationListbox->StoreSettingsL();
    delete iMusApplicationListbox;
	iMusApplicationListbox = NULL;
    }

// ---------------------------------------------------------
// CCenrepEditorView3Container::StoreSettingsL()
// Store the Settings to Variable
// ---------------------------------------------------------
//
void CCenrepEditorView3Container::StoreSettingsL()
    {    
    iMusApplicationListbox->StoreSettingsL();
    }

// ---------------------------------------------------------
// CCenrepEditorView3Container::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CCenrepEditorView3Container::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    iMusApplicationListbox->SetRect(Rect());  
    }

// ---------------------------------------------------------
// CCenrepEditorView3Container::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CCenrepEditorView3Container::CountComponentControls() const
    {
    TInt count = 0;    
    if(iMusApplicationListbox)
        count++; 
    return count;
    }

// ---------------------------------------------------------
// CCenrepEditorView3Container::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CCenrepEditorView3Container::ComponentControl(TInt /*aIndex*/) const
    {
    return iMusApplicationListbox;     
    }

// ---------------------------------------------------------
// CCenrepEditorView3Container::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CCenrepEditorView3Container::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();    
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbGray );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );    
    gc.DrawRect( aRect );   
    }


TKeyResponse CCenrepEditorView3Container::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    if(aType != EEventKey) // Is not key event?
        {
        return EKeyWasNotConsumed;
        }       
    switch (aKeyEvent.iCode)
        {
            case EKeyLeftArrow:
            case EKeyRightArrow:
                 return EKeyWasNotConsumed;
            default:
                 if(iMusApplicationListbox)
                    {
                    return iMusApplicationListbox->OfferKeyEventL( aKeyEvent, aType );   
                    }
                 else
                    {
                    return EKeyWasNotConsumed;
                    }
                 
        }    
    }    




// End of File  
