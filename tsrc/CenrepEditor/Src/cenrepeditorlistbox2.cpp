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
#include <aknnotewrappers.h>
#include <akntextsettingpage.h> 
#include "cenrepeditorListbox2.h"

#include "cenrepeditorListbox2.h"
#include <e32debug.h>

CAknSettingItem* COtherResourcesListbox::CreateSettingItemL( TInt aIdentifier )
    {
    CAknSettingItem* settingItem = NULL;

    // create a setting item for each setting list resource
    switch (aIdentifier)
    {
    case EOtherResourceActivation:        
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iActivation);
        break;
    case EOtherResourceOperatorVariant:        
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iOperatorVariant);
        break;    
    case EOtherResourceAuditoryNotification:        
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iAuditoryNotification);
        break;  
   case EOtherResourcePopupNotification:        
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iPopupNotification);
        break;
    case EOtherResourcePopupNotificationType:        
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iPopupNotificationType);
        break;
    case EOtherResourceEdgeDtmSupport:        
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iEdgeDtmSupport);
        break;
    case EOtherResourceSessionSetupMethod:        
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iSessionSetupMethod);
        break;
    case EOtherRResourcePrivacyVariationMode:
        settingItem=ConstructByValueL( aIdentifier, iOtherResources->iPrivacyVariation ); 
        break;  
    case EOtherResourceAutoRecord:        
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iAutoRecord);
        break;
    case EOtherResourceVideoLocation:        
        settingItem=new (ELeave) 
            CAknIntegerEdwinSettingItem(aIdentifier,iOtherResources->iVideoLocation);
        break;
    case EOtherResourceSipProfileId:
        settingItem = new (ELeave) 
            CAknIntegerEdwinSettingItem(aIdentifier, iOtherResources->iSipProfileId);        
        break;
    case EOtherResourceUiOrientation:        
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iUiOrientation);
        break;
    case EOtherResourceCapabilityQuery:    	
        settingItem=ConstructByValueL( aIdentifier,iOtherResources->iCapabilityQuery);
    	break; 
    case EOtherResourceProductModeVariationKey:    	
        settingItem = new (ELeave) 
            CAknIntegerEdwinSettingItem( aIdentifier, iOtherResources->iProductModeVariation);
    	break;
    case EOtherResourceEncoderConfigurationInfo:
        settingItem = new (ELeave)
            CAknTextSettingItem( aIdentifier, iOtherResources->iEncoderInfo );
        settingItem->SetSettingPageFlags(CAknTextSettingPage::EZeroLengthAllowed);

        break;   
    case EOtherResourceEncodingDevice:
        settingItem = new (ELeave) 
           CAknTextSettingItem( aIdentifier, iOtherResources->iEncodingDeviceStr ); 
    	break;             
    default:
        break;
        }
    return settingItem;
    }


void COtherResourcesListbox::SetData(COtherResources* aOtherResources)
    {
    iOtherResources = aOtherResources;
    }

void COtherResourcesListbox::SizeChanged()
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

CAknSettingItem* COtherResourcesListbox::ConstructByValueL( TInt aIdentifier,  
                                            TInt& aVal )
    {    
    CAknSettingItem* settingItem = NULL ;
    if(aVal<0 )
        {        
        settingItem = new (ELeave) CAknSettingItem(aIdentifier);
        settingItem->SetProtectionState(CAknSettingItem::ESettingItemProtected);
        }
    else
        {
        settingItem = new (ELeave) 
            CAknEnumeratedTextPopupSettingItem(aIdentifier,aVal);
        }
    return settingItem ;
    }


void COtherResourcesListbox::EditItemL(TInt aIndex, TBool aCalledFromMenu)
    {
    CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
    
    CAknSettingItem* currentItem = SettingItemArray()->At(aIndex);
    if ( currentItem->Identifier() == EOtherResourceEncodingDevice )
        {
        TLex lex( currentItem->SettingTextL() );
        TUint uid;
        TInt err = lex.Val(uid, EHex);
        
        if ( (err != KErrNone) || !lex.Eos() )
            {
            currentItem->LoadL();
            currentItem->UpdateListBoxTextL();

            CAknWarningNote* warningNote = new (ELeave) CAknWarningNote;
            warningNote->ExecuteLD(_L("Invalid value"));
            }
         }
    }
