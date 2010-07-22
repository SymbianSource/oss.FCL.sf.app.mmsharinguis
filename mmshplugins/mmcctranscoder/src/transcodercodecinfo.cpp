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


#include "transcodercodecinfo.h"

// INCLUDE FILES

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::CMccTranscoderCodecInfo
// default constructor
// -----------------------------------------------------------------------------
//
CMccTranscoderCodecInfo::CMccTranscoderCodecInfo() :
    iVideoType( EVedVideoTypeUnrecognized ),
    iVideoResolution( TSize( 0, 0 ) ),
    iBitrate( 0 ),
    iFrameRate( 0 ),
    iAudioType( EVedAudioTypeUnrecognized ),
    iAudioChannelMode( EVedAudioChannelModeSingleChannel ),
    iSamplingRate( 0 )
    {
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::~CMccTranscoderCodecInfo
// Destructor
// -----------------------------------------------------------------------------
//
CMccTranscoderCodecInfo::~CMccTranscoderCodecInfo()
    {
   
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::SetAudioType
// -----------------------------------------------------------------------------
//
void CMccTranscoderCodecInfo::SetAudioType( TVedAudioType aType )
    {
    iAudioType = aType;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::AudioType
// -----------------------------------------------------------------------------
//
TVedAudioType CMccTranscoderCodecInfo::AudioType()
    {
    return iAudioType;  
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::SetAudioChannelMode
// -----------------------------------------------------------------------------
//
void CMccTranscoderCodecInfo::SetAudioChannelMode( TVedAudioChannelMode aMode )
    {
    iAudioChannelMode = aMode;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::AudioChannelMode
// -----------------------------------------------------------------------------
//
TVedAudioChannelMode CMccTranscoderCodecInfo::AudioChannelMode()
    {
    return iAudioChannelMode;   
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::SetVideoType
//
// -----------------------------------------------------------------------------
//
void CMccTranscoderCodecInfo::SetVideoType( TVedVideoType aType )
    {
    iVideoType = aType;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::VideoType
// -----------------------------------------------------------------------------
//
TVedVideoType CMccTranscoderCodecInfo::VideoType()
    {
    return iVideoType;  
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::SetFrameRate
// -----------------------------------------------------------------------------
//
void CMccTranscoderCodecInfo::SetFrameRate( TReal aFrameRate )
    {
    iFrameRate = aFrameRate;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::FrameRate
// -----------------------------------------------------------------------------
//
TReal CMccTranscoderCodecInfo::FrameRate()
    { 
    return iFrameRate;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::SetVideoResolution
// -----------------------------------------------------------------------------
//
void  CMccTranscoderCodecInfo::SetVideoResolution( TSize aVideoResolution )
    {
    iVideoResolution  = aVideoResolution;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderCodecInfo::VideoResolution
// -----------------------------------------------------------------------------
//
TSize CMccTranscoderCodecInfo::VideoResolution()
    {
    return iVideoResolution; 
    }



