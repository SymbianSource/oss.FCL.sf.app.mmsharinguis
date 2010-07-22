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
#ifndef S60UILABLISTBOX2_H
#define S60UILABLISTBOX2_H

// INCLUDES
#include <e32base.h>
#include <AknSettingItemList.h>
#include "cenrepeditor.hrh"
#include <cenrepeditor.rsg>
#include "cenrepeditordocument.h"
#include "cenrepeditorview3container.h"
#include "cenrepeditormodel.h"


// FORWARD DECLARATIONS
class CCenrepEditorView3;
class CMusApplication;
// CLASS DECLARATION

/**
* CAknExSettingListContainer  container control class.
*/
class CMusApplicationListbox : public CAknSettingItemList 
    {
    public:
        CAknSettingItem* CreateSettingItemL( TInt identifier );
        void SetData(CMusApplication* iMusApplication); 
        

    public: // Implementation from base class
        void EditItemL ( TInt aIndex, TBool aCalledFromMenu );
        void ChangeSelectedItemL();
        void LoadSettingValuesL();
	    void SaveSettingValuesL();
        virtual TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
      //  void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);            
      // TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);            

    private:        
        void SizeChanged();

    private:
        CMusApplication* iMusApplication;
    };

#endif