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
#include "cenrepeditorview2container.h"
#include "cenrepeditordocument.h"
#include "cenrepeditorappui.h"
#include  "cenrepeditormodel.h"



#include <eiklabel.h>  // for example label control


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCenrepEditorView2Container::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CCenrepEditorView2Container::ConstructL(const TRect& aRect,TInt aResourceId, COtherResources*  aOtherResources)
    {      
    CreateWindowL(); 
    iOtherResourceListBox = new(ELeave)COtherResourcesListbox;
    // and put the data in the list
    iOtherResourceListBox->SetData(aOtherResources);
    // setting list is constructed from resources
    iOtherResourceListBox->ConstructFromResourceL(aResourceId);     
    SetRect(aRect);
    iOtherResourceListBox->MakeVisible(ETrue);
    iOtherResourceListBox->SetRect(aRect);
    iOtherResourceListBox->LoadSettingsL();
    iOtherResourceListBox->ActivateL();                
    ActivateL();
    }

// Destructor
CCenrepEditorView2Container::~CCenrepEditorView2Container()
    {
    //don't call StoreSettingsL here!, it'll result in copying data
    //to iOtherResources, which was already deleted
    //iOtherResourceListBox->StoreSettingsL();
    delete iOtherResourceListBox;
	iOtherResourceListBox = NULL;
    }

// ---------------------------------------------------------
// CCenrepEditorView2Container::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CCenrepEditorView2Container::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    iOtherResourceListBox->SetRect(Rect());  
    }

// ---------------------------------------------------------
// CCenrepEditorView2Container::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CCenrepEditorView2Container::CountComponentControls() const
    {
    TInt count = 0;    
    if(iOtherResourceListBox)
        count++; 
    return count;
    }

// ---------------------------------------------------------
// CCenrepEditorView2Container::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CCenrepEditorView2Container::ComponentControl(TInt /*aIndex*/) const
    {
    return iOtherResourceListBox;     
    }

// ---------------------------------------------------------
// CCenrepEditorView2Container::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CCenrepEditorView2Container::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();    
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbGray );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );    
    gc.DrawRect( aRect );

    }


TKeyResponse CCenrepEditorView2Container::OfferKeyEventL(
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
                 if(iOtherResourceListBox)
                    {
                    return iOtherResourceListBox->OfferKeyEventL( aKeyEvent, aType );   
                    }
                 else
                    {
                    return EKeyWasNotConsumed;
                    }
                 
        }    
    }    

// ---------------------------------------------------------
// CCenrepEditorView2Container::StoreSettingsL()
// Store the Settings to Variable
// ---------------------------------------------------------
//
void CCenrepEditorView2Container::StoreSettingsL()
    {    
    iOtherResourceListBox->StoreSettingsL();
    }


// End of File  
