/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  project specification
*
*/



// USER
#include "muswpitem.h"
#include "mussettings.h"
#include "muslogger.h"

_LIT( KMusWpCapabilityQueryParallel, "1" ); // (from w9017.txt)
_LIT( KMusWpCapabilityQuerySequential, "2" ); // (from w9017.txt)

_LIT( KMusWpActivationConditionAlways, "1" ); // (from w9017.txt)
_LIT( KMusWpActivationConditionInHome, "2" ); // (from w9017.txt)
_LIT( KMusWpActivationConditionNever, "3" ); // (from w9017.txt)

// -----------------------------------------------------------------------------
// Symbian two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusWpItem* CMusWpItem::NewL()
    {
    MUS_LOG( "mus: [MUSWP]  ->  CMusWpItem::NewL" )
    CMusWpItem* self = new( ELeave ) CMusWpItem;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSWP]  <- CMusWpItem::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMusWpItem::~CMusWpItem()
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpItem::~CMusWpItem" )
 
    delete iProviderId;
    delete iName;
    delete iToAppRef;
 
    MUS_LOG( "mus: [MUSWP]  <- CMusWpItem::~CMusWpItem" )
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
const TDesC& CMusWpItem::ProviderId() const
    {
    if ( iProviderId )
        {
        return *iProviderId;
        }
    
    return KNullDesC;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusWpItem::SetProviderIdL( const TDesC& aProviderId )
    {
    HBufC* newProviderId = aProviderId.AllocL();
    delete iProviderId;
    iProviderId = newProviderId;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
const TDesC& CMusWpItem::Name() const
    {
    if ( iName )
        {
        return *iName;
        }
    
    return KNullDesC;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusWpItem::SetNameL( const TDesC& aName)
    {
    HBufC* newName = aName.AllocL();
    delete iName;
    iName = newName;
    }
    
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
const TDesC& CMusWpItem::ToAppRef() const
    {
    if ( iToAppRef )
        {
        return *iToAppRef;
        }
    
    return KNullDesC;
    }
    
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusWpItem::SetToAppRefL( const TDesC& aToAppRef )
    {
    HBufC* newToAppRef = aToAppRef.AllocL();
    delete iToAppRef;
    iToAppRef = newToAppRef;
    }
   

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusWpItem::SetEdgeDtmSupport( TBool aEdgeDtmSupport )
    {
    if ( aEdgeDtmSupport )
        {
        iEdgeDtmSupport = MusSettingsKeys::EDtmModeAllowed;
        }
    else
        {
        iEdgeDtmSupport = MusSettingsKeys::EDtmModeNotAllowed;
        }
    }
  

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusWpItem::SetForceInternetSignaling( TBool aInternetSignaling )
    {
    if ( aInternetSignaling )
        {
        iForceInternetSignaling = MusSettingsKeys::EForceInternetSignaling;
        }
    else
        {
        iForceInternetSignaling = MusSettingsKeys::EFollowProfileConfiguration;
        }
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusWpItem::SetCapabilityQuery( const TDesC& aValue )
    {
    if ( aValue == KMusWpCapabilityQueryParallel() )
        {    
        iCapabilityQuery = MusSettingsKeys::EParallel;   
        }
    else if ( aValue == KMusWpCapabilityQuerySequential() )
        {
        iCapabilityQuery = MusSettingsKeys::ESequential;
        }
    else
        {
        // To keep PC-Lint happy
        }
    }
    
        
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusWpItem::SetActivationCondition( const TDesC& aValue )
    {
    if ( aValue == KMusWpActivationConditionAlways() )
        {
        iActivationCondition = MusSettingsKeys::EAlwaysActive;
        }
    else if ( aValue == KMusWpActivationConditionInHome() )
        {
        iActivationCondition = MusSettingsKeys::EAlwaysActive;
        }
    else if ( aValue == KMusWpActivationConditionNever )
        {
        iActivationCondition = MusSettingsKeys::ENever;
        }
    else
        {
        // To keep PC-Lint happy
        }
    }
            

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusWpItem::SaveL()
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpItem::SaveL" )
    
    // Uncomment next line to enable provisioning of EDGE / DTM setting
    // MultimediaSharingSettings::SetEdgeDtmSupportSettingL( iEdgeDtmSupport );
    MultimediaSharingSettings::SetForceInternetSignalingSettingL( 
                                                    iForceInternetSignaling );
    MultimediaSharingSettings::SetCapabilityQuerySettingL( iCapabilityQuery );
    MultimediaSharingSettings::SetActivationSettingL( iActivationCondition );
    
    MUS_LOG( "mus: [MUSWP]  <- CMusWpItem::SaveL" )
    }


// -----------------------------------------------------------------------------
// C++ default constructor
// Default setting for activation is temporarily changed to EAlways
// -----------------------------------------------------------------------------
//
CMusWpItem::CMusWpItem() 
    :iEdgeDtmSupport( MusSettingsKeys::EDtmModeNotAllowed ),
     iForceInternetSignaling( MusSettingsKeys::EFollowProfileConfiguration ),
     iCapabilityQuery( MusSettingsKeys::ENoOptions ),
     //iActivationCondition( MusSettingsKeys::EActiveInHomeNetworks )
     iActivationCondition( MusSettingsKeys::EAlwaysActive )
    {
    }


// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CMusWpItem::ConstructL()
    {
    }






