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


#include "mceh263codec.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceH263Codec::~CMceH263Codec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceH263Codec::~CMceH263Codec()
    {
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceH263Codec* CMceH263Codec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceH263Codec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceH263Codec* CMceH263Codec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceH263Codec* self = new (ELeave) CMceH263Codec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceH263Codec::SetBitrate(TUint aBitrate)
    {
    iBitrate = aBitrate;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceH263Codec::SetAllowedBitrates(TUint aBitrates)
    {
    iAllowedBitrates = aBitrates;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceH263Codec::SetCodecMode(TUint aCodecMode)
    {
    iCodecMode = aCodecMode;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceH263Codec::SetPayloadType(TUint8 aPayloadType)
    {
    iPayloadType = aPayloadType;
    return KErrNone;
    }        

    
// -----------------------------------------------------------------------------
// CMceH263Codec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoCodec* CMceH263Codec::CloneL() const
    {
    CMceH263Codec* clone = CMceH263Codec::NewLC( iSdpName );
   
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
	
	CleanupStack::Pop( clone );
    return clone;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetMaxBitrateL( TUint aMaxBitrate )
    {
    iMaxBitRate = aMaxBitrate;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetAllowedResolutionsL( TUint aResolutions )
    {
    iAllowedResolutions = aResolutions;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetResolutionL( TSize aResolution )
    {
    iResolutionWidth = aResolution.iWidth;
    iResolutionHeight = aResolution.iHeight;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetAllowedFrameRatesL( TUint aFrameRates )
    {
    iAllowedFrameRates = aFrameRates;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetFrameRateL( TReal aFrameRate )
    {
    iFrameRate = aFrameRate;
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::SetPreferredEncodingDecodingDeviceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceH263Codec::SetPreferredEncodingDecodingDeviceL( 
    TUid /*aEncodingDecodingDevice*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::SetConfigKeyL
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMceH263Codec::SetConfigKeyL( const TDesC8& /*aConfigKey*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceH263Codec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    iSdpName.Copy( aSdpName );
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::CMceH263Codec
// -----------------------------------------------------------------------------
//
CMceH263Codec::CMceH263Codec()
    :CMceVideoCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceH263Codec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceH263Codec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    iSdpName = aSdpName;
    iClockRate = KMceH263ClockRate;
    iFrameRate = KMceH263FrameRate;
    iResolutionWidth = KMceH263FrameWidth;
    iResolutionHeight = KMceH263FrameHeight;
    }



    
