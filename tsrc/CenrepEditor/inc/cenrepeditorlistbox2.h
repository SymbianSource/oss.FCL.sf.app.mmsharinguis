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
#include "cenrepeditorview1container.h"
#include "cenrepeditormodel.h"


// FORWARD DECLARATIONS
class CCenrepEditorView2;
class CPhoneResources;
// CLASS DECLARATION

/**
* CAknExSettingListContainer  container control class.
*/
class COtherResourcesListbox : public CAknSettingItemList 
    {
    public:
        CAknSettingItem* CreateSettingItemL( TInt identifier );
        void SetData(COtherResources* iOtherResources); 
        
    private:        
        void SizeChanged();
        CAknSettingItem* ConstructByValueL( TInt aIdentifier,  
                                            TInt& aVal );

    private:
        COtherResources* iOtherResources;
    };

#endif
