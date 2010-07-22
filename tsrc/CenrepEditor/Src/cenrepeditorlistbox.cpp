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
#include "cenrepeditorListbox.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPhoneResourcesListbox::CreateSettingItemL( TInt aIdentifier )
// ---------------------------------------------------------
//
CAknSettingItem* CPhoneResourcesListbox::CreateSettingItemL( TInt aIdentifier )
    {
    CAknSettingItem* settingItem = NULL;

    // create a setting item for each setting list resource
    switch (aIdentifier)
    {
    case EPhoneResouceCamera:        
        settingItem = ConstructByValueL(aIdentifier,iPhoneResources->iCameraAvailability);               
        break;
    case EPhoneResouceKeypad:
        settingItem = ConstructByValueL(aIdentifier, iPhoneResources->iKeypadAvailability);        
        break;    
    case EPhoneResouceVideoPlayer:
        settingItem = ConstructByValueL(aIdentifier, iPhoneResources->iVideoPlayerAvailability);        
        break;  
   case EPhoneResouceCameraInformation:
        settingItem = ConstructByValueL(aIdentifier, iPhoneResources->iCameraInformation);        
        break;
    case EKodiakPhoneCall:
        settingItem = ConstructByValueL(aIdentifier, iPhoneResources->iKodiakPhoneCall);        
        break;
    default:
        break;
        }    
    return settingItem;
    }

// ------------------------------------------------------------------
// CPhoneResourcesListbox::SetData(CPhoneResources* aPhoneResources)
// ------------------------------------------------------------------
//
void CPhoneResourcesListbox::SetData(CPhoneResources* aPhoneResources)
    {
    iPhoneResources = aPhoneResources;
    }

// ------------------------------------------------------------------
// CPhoneResourcesListbox::SizeChanged()
// ------------------------------------------------------------------
//

void CPhoneResourcesListbox::SizeChanged()
    {
    if (ListBox()) 
        {
        ListBox()->SetRect(Rect());
        }    
    }

// ------------------------------------------------------------------
// CPhoneResourcesListbox::ConstructByValueL( aIdentifier, )
// ------------------------------------------------------------------
//

CAknSettingItem* CPhoneResourcesListbox::ConstructByValueL( TInt aIdentifier,  
                                            TInt& aVal )
    {    
    CAknSettingItem* settingItem = NULL ;
    if(aVal<0 )
        {        
        settingItem = new (ELeave) CAknSettingItem( aIdentifier );
        settingItem->SetProtectionState(CAknSettingItem::ESettingItemProtected);
        }
    else
        {
        settingItem = new (ELeave) 
            CAknEnumeratedTextPopupSettingItem(aIdentifier,aVal);
        }
    return settingItem ;
    }

