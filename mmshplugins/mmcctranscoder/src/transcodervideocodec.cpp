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

#include "transcodervideocodec.h"

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccTranscoderVideoCodec::CMccTranscoderVideoCodec
// default constructor
// -----------------------------------------------------------------------------
//
CMccTranscoderVideoCodec::CMccTranscoderVideoCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMccTranscoderVideoCodec::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccTranscoderVideoCodec::ConstructL( )
    {

    }

// -----------------------------------------------------------------------------
// CMccTranscoderVideoCodec::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccTranscoderVideoCodec* CMccTranscoderVideoCodec::NewL()
    {
    CMccTranscoderVideoCodec* self = new (ELeave) CMccTranscoderVideoCodec;
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self ); 
	return self;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderVideoCodec::~CMccTranscoderVideoCodec
// Destructor
// -----------------------------------------------------------------------------
//
CMccTranscoderVideoCodec::~CMccTranscoderVideoCodec()
    {
    }
 
// -----------------------------------------------------------------------------
// CMccTranscoderVideoCodec::SetBitrate
// 
// -----------------------------------------------------------------------------
//   
void CMccTranscoderVideoCodec::SetBitrate( TInt aBitrate )
    {
    iBitrate = aBitrate;  
    }

// -----------------------------------------------------------------------------
// CMccTranscoderVideoCodec::Bitrate
// 
// -----------------------------------------------------------------------------
//
TInt CMccTranscoderVideoCodec::Bitrate()
    {
    return iBitrate;  
    }

// -----------------------------------------------------------------------------
// CMccTranscoderVideoCodec::SetSamplingRate
// -----------------------------------------------------------------------------
//
void CMccTranscoderVideoCodec::SetSamplingRate( TInt aSamplingRate )
    {
    iSamplingRate = aSamplingRate; 
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderVideoCodec::SamplingRate
// -----------------------------------------------------------------------------
//
TInt CMccTranscoderVideoCodec::SamplingRate()
    {
    return iSamplingRate;  
    }
