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
* Description:  
*
*/


#include "mceavccodec.h"

// Stub constants
const TReal KMceAvcDefaultFrameRate = 15;
const TInt  KMceAvcDefaultFrameSize = 10000;
const TInt  KMceAvcDefaultAllowedBitrate = KMceAvcCodecProfileIdBaseline | 
	                                       KMceAvcCodecProfileIopConstraintSet | 
	                                       KMceAvcBitrateLevel1;
const TInt KMceAvcLevel1BitrateBps = 64000;

const TUint KMceAvcDefaultFrameHeight = 176;
const TUint KMceAvcDefaultFrameWidth = 144;




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceAvcCodec::~CMceAvcCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAvcCodec::~CMceAvcCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAvcCodec* CMceAvcCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAvcCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAvcCodec* CMceAvcCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAvcCodec* self = new (ELeave) CMceAvcCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAvcCodec::SetBitrate( TUint aBitrate )
    {
    iBitrate = aBitrate;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAvcCodec::SetAllowedBitrates(TUint aBitrates)
    {
    iAllowedBitrates = aBitrates;
    
    delete iConfigKey;
    iConfigKey = NULL; 
    
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAvcCodec::SetCodecMode( TUint aCodecMode )
    {
    if ( aCodecMode == KMceAvcModeSingleNal )
        {
        iCodecMode = KMceAvcModeSingleNal;
        return KErrNone;
        }
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAvcCodec::SetPayloadType(TUint8 aPayloadType)
    {
    iPayloadType = aPayloadType;
    return KErrNone;
    }        

    
// -----------------------------------------------------------------------------
// CMceAvcCodec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoCodec* CMceAvcCodec::CloneL() const
    {
    CMceAvcCodec* clone = CMceAvcCodec::NewLC( iSdpName );
            
    if ( iFmtpAttr )
	    {
        clone->iFmtpAttr = iFmtpAttr->AllocL();
        }
    clone->iPayloadType = iPayloadType;
    
	clone->iBitrate = iBitrate;
	clone->iAllowedBitrates = iAllowedBitrates;
    clone->iCodecMode = iCodecMode;
	clone->iFourCC = iFourCC;
	clone->iFrameSize = iFrameSize;
	
	clone->iFrameRate = iFrameRate;
	clone->iMaxBitRate = iMaxBitRate;	
	clone->iAllowedResolutions = iAllowedResolutions;	
	clone->iResolutionWidth = iResolutionWidth;	
	clone->iResolutionHeight = iResolutionHeight;	
	clone->iAllowedFrameRates = iAllowedFrameRates;	
	clone->iClockRate = iClockRate;
	
	clone->iPacketizationMode = iPacketizationMode;
	clone->iAllowedPacketizationModes = iAllowedPacketizationModes;
	
	CleanupStack::Pop( clone );
    return clone;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetMaxBitrateL( TUint aMaxBitrate )
    {
    delete HBufC::NewL( 1 ); // To cause leave in simulation
    iMaxBitRate = aMaxBitrate;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetAllowedResolutionsL( TUint aResolutions )
    {
    delete HBufC::NewL( 1 ); // To cause leave in simulation
    iAllowedResolutions = aResolutions;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetResolutionL( TSize aResolution )
    {
    delete HBufC::NewL( 1 ); // To cause leave in simulation
    iResolutionWidth = aResolution.iWidth;
    iResolutionHeight = aResolution.iHeight;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetAllowedFrameRatesL( TUint aFrameRates )
    {
    delete HBufC::NewL( 1 ); // To cause leave in simulation
    iAllowedFrameRates = aFrameRates;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetFrameRateL( TReal aFrameRate )
    {
    delete HBufC::NewL( 1 ); // To cause leave in simulation
    iFrameRate = aFrameRate;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetPreferredEncodingDecodingDeviceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAvcCodec::SetPreferredEncodingDecodingDeviceL( 
    TUid aEncodingDecodingDevice )
    {
    delete HBufC::NewL( 1 ); // To cause leave in simulation
    iEncodingDecodingDevice = aEncodingDecodingDevice;
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::SetEncodingKeyL
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMceAvcCodec::SetConfigKeyL( const TDesC8& aConfigKey )
    {
    HBufC8* configKey = aConfigKey.AllocL();
    delete iConfigKey;
    iConfigKey = configKey;
    }
    

// -----------------------------------------------------------------------------
// CMceAvcCodec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceAvcCodec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    delete HBufC::NewL( 1 ); // To cause leave in simulation
    iSdpName.Copy( aSdpName );
    }
        
        
// -----------------------------------------------------------------------------
// CMceAvcCodec::CMceAvcCodec
// -----------------------------------------------------------------------------
//
CMceAvcCodec::CMceAvcCodec()
    :CMceVideoCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceAvcCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceAvcCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    iSdpName = aSdpName;
    iCodecMode = KMceAvcModeSingleNal;
    iClockRate = KMceAvcDefaultFrameRate;
    iFrameRate = KMceAvcDefaultFrameRate;
    iResolutionWidth = KMceAvcDefaultFrameWidth;
    iResolutionHeight = KMceAvcDefaultFrameHeight;
    iBitrate = KMceAvcLevel1BitrateBps;
	iMaxBitRate = KMceAvcLevel1BitrateBps;
	iAllowedBitrates = KMceAvcDefaultAllowedBitrate;
    iFrameSize = KMceAvcDefaultFrameSize;
    // Defaults should be defined in MCE API
    const TUint8 KMceDefaultAvcPayloadType = 98;
	iPayloadType = KMceDefaultAvcPayloadType;
	
    }



    
