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


#include "mcevideocodec.h"

// Avc config keys for all the .
// Getter is not virtual so keys must be returned from base class and
// the level values defined in mceavccodec.h are redefined.
_LIT8( KMceStubAvcBitrateLevel1ConfigKey, "TestTextForAvcBrL1Level" );
_LIT8( KMceStubAvcBitrateLevel1bConfigKey, "TestTextForAvcBrL1bLevel" );
_LIT8( KMceStubAvcBitrateLevel1_1ConfigKey, "TestTextForAvcBrL1_1Level" );
_LIT8( KMceStubAvcBitrateLevel1_2ConfigKey, "TestTextForAvcBrL1_2Level" );
_LIT8( KMceStubAvcBitrateLevel1_3ConfigKey, "TestTextForAvcBrL1_3Level" );
_LIT8( KMceStubAvcBitrateLevel2ConfigKey, "TestTextForAvcBrL2Level" );
const TUint KMceStubAvcBitrateLevel1 = 0x0001;
const TUint KMceStubAvcBitrateLevel1b = 0x0002;
const TUint KMceStubAvcBitrateLevel1_1 = 0x0004;
const TUint KMceStubAvcBitrateLevel1_2 = 0x0008;
const TUint KMceStubAvcBitrateLevel1_3 = 0x0010;
const TUint KMceStubAvcBitrateLevel2 = 0x0020;

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceVideoCodec::~CMceVideoCodec
// -----------------------------------------------------------------------------
//
CMceVideoCodec::~CMceVideoCodec()
    {
    delete iConfigKey;
    }


// -----------------------------------------------------------------------------
// CMceVideoCodec::AllowedFrameRates
// -----------------------------------------------------------------------------
//	    
EXPORT_C TUint CMceVideoCodec::AllowedFrameRates() const
    {
    return iAllowedFrameRates;
    }

    
// -----------------------------------------------------------------------------
// CMceVideoCodec::FrameRate
// -----------------------------------------------------------------------------
//	    
EXPORT_C TReal CMceVideoCodec::FrameRate() const
    {
    return iFrameRate;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C HBufC8* CMceVideoCodec::ConfigKeyL() const
    {
    HBufC8* configKey = NULL;

    if ( iConfigKey )
        {
        configKey = iConfigKey->AllocL();
        }

    if ( !configKey && SdpName() == KMceSDPNameH264() )
        {
        if ( AllowedBitrates() & KMceStubAvcBitrateLevel1 )
            {
            configKey = KMceStubAvcBitrateLevel1ConfigKey().AllocL();
            }
        else if ( AllowedBitrates() & KMceStubAvcBitrateLevel1b )
            {
            configKey = KMceStubAvcBitrateLevel1bConfigKey().AllocL();
            }
        else if ( AllowedBitrates() & KMceStubAvcBitrateLevel1_1 )
            {
            configKey = KMceStubAvcBitrateLevel1_1ConfigKey().AllocL();
            }
        else if ( AllowedBitrates() & KMceStubAvcBitrateLevel1_2 )
            {
            configKey = KMceStubAvcBitrateLevel1_2ConfigKey().AllocL();
            }
        else if ( AllowedBitrates() & KMceStubAvcBitrateLevel1_3 )
            {
            configKey = KMceStubAvcBitrateLevel1_3ConfigKey().AllocL();
            }
        else if ( AllowedBitrates() & KMceStubAvcBitrateLevel2 )
            {
            configKey = KMceStubAvcBitrateLevel2ConfigKey().AllocL();
            }
        else
            {
            User::Leave( KErrNotFound );
            }
        }
         
    return configKey;
    } 


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	   
EXPORT_C TUint CMceVideoCodec::MaxBitRate() const
    {
    return iMaxBitRate;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//		
EXPORT_C TUint CMceVideoCodec::AllowedResolutions() const
    {
    return iAllowedResolutions;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	    
EXPORT_C TSize CMceVideoCodec::Resolution() const
    {
    return TSize( iResolutionWidth, 
                  iResolutionHeight );
    }

            
// -----------------------------------------------------------------------------
// CMceVideoCodec::CMceVideoCodec
// -----------------------------------------------------------------------------
//
CMceVideoCodec::CMceVideoCodec()
    {
    iType = KMceVideoCodec;
    }
 
