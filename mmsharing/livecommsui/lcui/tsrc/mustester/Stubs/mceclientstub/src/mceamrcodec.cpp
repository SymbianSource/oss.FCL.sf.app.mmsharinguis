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


#include "mceamrcodec.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceAmrCodec::~CMceAmrCodec
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAmrCodec::~CMceAmrCodec()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceAmrCodec::CloneL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioCodec* CMceAmrCodec::CloneL() const
    {
    CMceAmrCodec* clone = CMceAmrCodec::NewLC( iSdpName );
    
    if ( iFmtpAttr )
	    {
        clone->iFmtpAttr = iFmtpAttr->AllocL();
        }
    clone->iEnableVAD = iEnableVAD;
	clone->iSamplingFreq = iSamplingFreq;
	clone->iPTime = iPTime;
	clone->iMaxPTime = iMaxPTime;
	clone->iBitrate = iBitrate;
	clone->iAllowedBitrates = iAllowedBitrates;
	clone->iPayloadType = iPayloadType;
	clone->iCodecMode = iCodecMode;
	clone->iFourCC = iFourCC;
	clone->iFrameSize = iFrameSize;
	
	CleanupStack::Pop( clone );
	
	return clone;
    }
    
// -----------------------------------------------------------------------------
// CMceAmrCodec::EnableVAD
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::EnableVAD( TBool aEnableVAD )
    {
    iEnableVAD = aEnableVAD;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetBitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetBitrate(TUint aBitrate)
    {
    iBitrate = aBitrate;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetAllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetAllowedBitrates(TUint aBitrates)
    {
    iAllowedBitrates = aBitrates;
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMceAmrCodec::SetSamplingFreq
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetSamplingFreq(TUint aSamplingFreq)
    {
    iSamplingFreq = aSamplingFreq;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetPTime(TUint aPTime)
    {
    iPTime = aPTime;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetMaxPTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetMaxPTime(TUint aMaxPTime)
    {
    iMaxPTime = aMaxPTime;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetPayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetPayloadType(TUint8 aPayloadType)
    {
    iPayloadType = aPayloadType;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::SetCodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceAmrCodec::SetCodecMode(TUint aCodecMode)
    {
    iCodecMode = aCodecMode;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::NewL
// -----------------------------------------------------------------------------
//
CMceAmrCodec* CMceAmrCodec::NewL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAmrCodec* self = NewLC( aSdpName );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::NewL
// -----------------------------------------------------------------------------
//
CMceAmrCodec* CMceAmrCodec::NewLC( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    CMceAmrCodec* self = new (ELeave) CMceAmrCodec();
    CleanupStack::PushL( self );
    self->ConstructL( aSdpName );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceAmrCodec::CMceAmrCodec
// -----------------------------------------------------------------------------
//
CMceAmrCodec::CMceAmrCodec()
 : CMceAudioCodec()
    {
    }


// -----------------------------------------------------------------------------
// CMceAmrCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceAmrCodec::ConstructL( TBuf8<KMceMaxSdpNameLength> aSdpName )
    {
    iSdpName = aSdpName;
    }


// -----------------------------------------------------------------------------
// CMceAmrCodec::SetSdpNameL
// -----------------------------------------------------------------------------
//
void CMceAmrCodec::SetSdpNameL( const TDesC8& aSdpName )
    {
    __ASSERT_ALWAYS( aSdpName.Length() <= KMceMaxSdpNameLength, 
                     User::Leave( KErrArgument ) );
    iSdpName.Copy( aSdpName );
    }
    
