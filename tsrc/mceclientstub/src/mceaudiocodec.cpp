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


#include "mceaudiocodec.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcemediastream.h"


#define _FLAT_DATA static_cast<CMceComAudioCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceAudioCodec::InitializeL
// -----------------------------------------------------------------------------
//
void CMceAudioCodec::InitializeL( CMceMediaStream& aParent )
    {
    CMceCodec::InitializeL( aParent );
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::~CMceAudioCodec
// -----------------------------------------------------------------------------
//
CMceAudioCodec::~CMceAudioCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::SamplingFreq
// -----------------------------------------------------------------------------
//
TUint CMceAudioCodec::SamplingFreq() const
    {
    return iSamplingFreq;
    
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::VAD
// -----------------------------------------------------------------------------
//
TBool CMceAudioCodec::VAD() const
    {
    return iEnableVAD;
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::PTime
// -----------------------------------------------------------------------------
//
TUint CMceAudioCodec::PTime() const        
    {
    return iPTime;
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::MaxPTime
// -----------------------------------------------------------------------------
//
TUint CMceAudioCodec::MaxPTime() const        
    {
    return iMaxPTime;
    }
            
// -----------------------------------------------------------------------------
// CMceAudioCodec::CMceAudioCodec
// -----------------------------------------------------------------------------
//
CMceAudioCodec::CMceAudioCodec() :
    CMceCodec()
    {
    }
