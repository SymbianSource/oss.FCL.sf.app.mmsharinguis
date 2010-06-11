/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "cenrepeditorListbox3.h"


CAknSettingItem* CMusApplicationListbox::CreateSettingItemL( TInt aIdentifier )
    {
    CAknSettingItem* settingItem = NULL;        
    switch (aIdentifier)
        {
        case EApplicationStartStop:
        settingItem = new (ELeave) 
            CAknEnumeratedTextPopupSettingItem(aIdentifier,iMusApplication->iUseCase);
            break;    
        default:
            break;
        }
    return settingItem;
    }


void CMusApplicationListbox::SetData(CMusApplication* aMusApplication)
    {
    iMusApplication = aMusApplication;
    }

void CMusApplicationListbox::SizeChanged()
    {
    if (ListBox()) 
        {
        ListBox()->SetRect(Rect());
        }   
    }

/**
 *	Handle the "Change" option on the Options menu.  This is an
 *	alternative to the Selection key that forces the settings page
 *	to come up rather than changing the value in place (if possible).
 */
void CMusApplicationListbox::ChangeSelectedItemL()
	{
	if ( ListBox()->CurrentItemIndex() >= 0 )
		EditItemL( ListBox()->CurrentItemIndex(), ETrue );
	}


TKeyResponse CMusApplicationListbox::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
	{
	if ( aKeyEvent.iCode == EKeyLeftArrow || aKeyEvent.iCode == EKeyRightArrow )
		{
		return EKeyWasNotConsumed;
		}
	
	return CAknSettingItemList::OfferKeyEventL( aKeyEvent, aType );
	}


void CMusApplicationListbox::EditItemL ( TInt aIndex, TBool aCalledFromMenu )
    {
    CAknSettingItemList::EditItemL( aIndex, aCalledFromMenu );		
    }

//End of File
