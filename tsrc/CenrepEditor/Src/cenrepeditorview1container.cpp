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
#include "cenrepeditorview1container.h"
#include "cenrepeditordocument.h"
#include "cenrepeditorappui.h"
#include  "cenrepeditormodel.h"



#include <eiklabel.h>  // for example label control


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCenrepEditorView1Container::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CCenrepEditorView1Container::ConstructL(const TRect& aRect,TInt aResourceId, CPhoneResources*  aPhoneResources)
    {      
    CreateWindowL(); 
    iPhoneResourceListBox = new(ELeave)CPhoneResourcesListbox;
    // and put the data in the list
    iPhoneResourceListBox->SetData(aPhoneResources);
    // setting list is constructed from resources
    iPhoneResourceListBox->ConstructFromResourceL(aResourceId);     
    SetRect(aRect);
    iPhoneResourceListBox->MakeVisible(ETrue);
    iPhoneResourceListBox->SetRect(aRect);
    iPhoneResourceListBox->LoadSettingsL();
    iPhoneResourceListBox->ActivateL();                
    ActivateL();
    }

// Destructor
CCenrepEditorView1Container::~CCenrepEditorView1Container()
    {
    iPhoneResourceListBox->StoreSettingsL();
    delete iPhoneResourceListBox;
	iPhoneResourceListBox = NULL;
    }

// ---------------------------------------------------------
// CCenrepEditorView1Container::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CCenrepEditorView1Container::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    iPhoneResourceListBox->SetRect(Rect());  
    }

// ---------------------------------------------------------
// CCenrepEditorView1Container::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CCenrepEditorView1Container::CountComponentControls() const
    {
    TInt count = 0;    
    if(iPhoneResourceListBox)
        count++; 
    return count;
    }

// ---------------------------------------------------------
// CCenrepEditorView1Container::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CCenrepEditorView1Container::ComponentControl(TInt /*aIndex*/) const
    {
    return iPhoneResourceListBox;     
    }

// ---------------------------------------------------------
// CCenrepEditorView1Container::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CCenrepEditorView1Container::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();    
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbGray );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );    
    gc.DrawRect( aRect );

    }

// ---------------------------------------------------------
// CCenrepEditorView1Container::OfferKeyEventL(
//                                const TKeyEvent& aKeyEvent,
//                                TEventCode aType )
// ---------------------------------------------------------
//
TKeyResponse CCenrepEditorView1Container::OfferKeyEventL(
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
                 if(iPhoneResourceListBox)
                    {
                    return iPhoneResourceListBox->OfferKeyEventL( aKeyEvent, aType );   
                    }
                 else
                    {
                    return EKeyWasNotConsumed;
                    }
                 
        }    
    }    

// ---------------------------------------------------------
// CCenrepEditorView1Container::StoreSettingsL()
// Store the Settings to Variable
// ---------------------------------------------------------
//
void CCenrepEditorView1Container::StoreSettingsL()
    {    
    iPhoneResourceListBox->StoreSettingsL();
    }


// End of File  
