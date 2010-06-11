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


#include "mcecodec.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcemediastream.h"
#include "mcestreamobserver.h"
#include "mcedefs.h"



#define _FLAT_DATA static_cast<CMceComCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceCodec::~CMceCodec
// -----------------------------------------------------------------------------
//
CMceCodec::~CMceCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceCodec::InitializeL
// -----------------------------------------------------------------------------
//
void CMceCodec::InitializeL( CMceMediaStream& aParent )
    {    
    iStream = &aParent;
    }

// -----------------------------------------------------------------------------
// CMceCodec::SetStateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetStateL( CMceCodec::TState /*aState*/ )
    {
    // NOT SUPPORTED YET
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CMceCodec::SetStandByTimerL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetStandByTimerL( TUint32 /*aTimerValue*/ )
    {
    // NOT SUPPORTED YET
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CMceCodec::SetMMFPriorityL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetMMFPriorityL( TInt aPriority )
    {
    iMMFPriority = aPriority;
    }


// -----------------------------------------------------------------------------
// CMceCodec::SetMMFPriorityPreferenceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetMMFPriorityPreferenceL( TInt aPriorityPreference )
    {
    iMMFPriorityPreference = aPriorityPreference;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetKeepAliveTimerL( TUint8 aTimerValue )
    {
    iKeepAliveTimer = aTimerValue;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetKeepAlivePayloadTypeL( TUint8 aKeepAlivePT )
    {
    iKeepAlivePayloadType = aKeepAlivePT;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetKeepAliveDataL( const TDesC8& aData )
    {
    iKeepAliveData = aData;
    }
        

// -----------------------------------------------------------------------------
// CMceCodec::State
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCodec::TState CMceCodec::State() const
    {
    return CMceCodec::EDisabled;
    }


// -----------------------------------------------------------------------------
// CMceCodec::Id
// -----------------------------------------------------------------------------
//
/*
EXPORT_C TMceMediaId CMceCodec::Id() const
    {
    return;
    }
*/  

// -----------------------------------------------------------------------------
// CMceCodec::Type
// -----------------------------------------------------------------------------
//   
EXPORT_C TMceCodecType CMceCodec::Type() const
    {
    return iType;
    }

// -----------------------------------------------------------------------------
// CMceCodec::FourCC
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceCodec::FourCC() const
    {
    return iFourCC;
    }
       
// -----------------------------------------------------------------------------
// CMceCodec::Bitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceCodec::Bitrate() const
    {
    return iBitrate;
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::AllowedBitrates
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceCodec::AllowedBitrates() const
    {
    return iAllowedBitrates;
    }
    
// -----------------------------------------------------------------------------
// CMceCodec::FrameSize
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceCodec::FrameSize() const
    {
    return iFrameSize;
    }
    
// -----------------------------------------------------------------------------
// CMceCodec::CodecMode
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceCodec::CodecMode() const
    {
    return iCodecMode;
    }    

// -----------------------------------------------------------------------------
// CMceCodec::PayloadType
// -----------------------------------------------------------------------------
//
EXPORT_C TUint8 CMceCodec::PayloadType() const
    {
    return iPayloadType;
    }

    
// -----------------------------------------------------------------------------
// CMceCodec::SdpName
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceCodec::SdpName() const
    {
    return iSdpName;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCodec::MMFPriority() const
    {
    return iMMFPriority;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCodec::MMFPriorityPreference() const
    {
    return iMMFPriorityPreference;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TUint8 CMceCodec::KeepAliveTimer() const
    {
    return iKeepAliveTimer;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TUint8 CMceCodec::KeepAlivePayloadType() const
    {
    return iKeepAlivePayloadType;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceCodec::KeepAliveData() const
    {
    return iKeepAliveData;
    }
        

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceCodec::Preference() const
    {
    return iPreference;
    }
	
	    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCodec::SetPreferenceL( TInt aPreference )
    {
    iPreference = aPreference;
    }
               

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMceCodec::SetSdpNameL( const TDesC8& aSdpName )
    {
    iSdpName = aSdpName;
    }
    
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMceCodec::CMceCodec()
    {
    }

