/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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

#include "transcoderaudiocodec.h"

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccTranscoderAudioCodec::CMccTranscoderAudioCodec
// default constructor
// -----------------------------------------------------------------------------
//
CMccTranscoderAudioCodec::CMccTranscoderAudioCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMccTranscoderAudioCodec::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccTranscoderAudioCodec::ConstructL( )
    {

    }

// -----------------------------------------------------------------------------
// CMccTranscoderAudioCodec::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccTranscoderAudioCodec* CMccTranscoderAudioCodec::NewL()
    {
    CMccTranscoderAudioCodec* self = new (ELeave) CMccTranscoderAudioCodec;
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self ); 
	return self;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderAudioCodec::~CMccTranscoderAudioCodec
// Destructor
// -----------------------------------------------------------------------------
//
CMccTranscoderAudioCodec::~CMccTranscoderAudioCodec()
    {
    }
 
// -----------------------------------------------------------------------------
// CMccTranscoderAudioCodec::SetBitrate
// Destructor
// -----------------------------------------------------------------------------
//   
void CMccTranscoderAudioCodec::SetBitrate( TInt aBitrate )
    {
    iBitrate = aBitrate; 
    }

// -----------------------------------------------------------------------------
// CMccTranscoderAudioCodec::Bitrate
// Destructor
// -----------------------------------------------------------------------------
//
TInt CMccTranscoderAudioCodec::Bitrate()
    {
    return iBitrate;  
    }

// -----------------------------------------------------------------------------
// CMccTranscoderAudioCodec::SetSamplingRate
// Destructor
// -----------------------------------------------------------------------------
//
void CMccTranscoderAudioCodec::SetSamplingRate( TInt aSamplingRate )
    {
    iSamplingRate = aSamplingRate; 
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderAudioCodec::SamplingRate
// Destructor
// -----------------------------------------------------------------------------
//
TInt CMccTranscoderAudioCodec::SamplingRate()
    {
    return iSamplingRate;  
    }

