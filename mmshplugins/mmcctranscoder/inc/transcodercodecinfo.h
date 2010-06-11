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
* Description:  Class is used for configuring media codecs.
*
*/


#ifndef _MCCTRANSCODERCODECINFO_H
#define _MCCTRANSCODERCODECINFO_H

// INCLUDES
#include <e32base.h>
#include <e32des8.h>
#include <e32svr.h>
#include <vedmovie.h>


// CLASS DECLARATION
/**
*  Container class for the transcoder codec information.
*
*  @lib mmccinterface.dll
*/
class CMccTranscoderCodecInfo : public CBase
{
public:  // Constructors and destructor

    /**
    * Destructor.
    */
    virtual ~CMccTranscoderCodecInfo();

protected:  // New functions
    
    /**
    * C++ default constructor.
    */
    CMccTranscoderCodecInfo();
    
public: // New functions

    
    virtual void SetBitrate( TInt aBitrate ) = 0;
    
    virtual TInt Bitrate() = 0;
   
    virtual void SetSamplingRate( TInt aSamplingRate ) = 0;
    
    virtual TInt SamplingRate() = 0;
    
    
               
public: // get
    
    virtual void SetVideoType( TVedVideoType aType);
    
    virtual TVedVideoType VideoType();
    
    virtual void SetAudioType( TVedAudioType aType);
    
    virtual TVedAudioType AudioType();

    virtual void SetFrameRate( TReal aFrameRate );
    
    virtual TReal FrameRate();
    
    virtual void SetAudioChannelMode( TVedAudioChannelMode aMode );

    virtual TVedAudioChannelMode AudioChannelMode();
    
    virtual void  SetVideoResolution( TSize aVideoResolution );       
        
    virtual TSize VideoResolution();
 
    
private:
 
    
protected:  // Data

    TVedVideoType        iVideoType;
    TSize                iVideoResolution;
    TInt                 iBitrate;
    TReal                iFrameRate;
        
	TVedAudioType        iAudioType;
    TVedAudioChannelMode iAudioChannelMode;
    TInt                 iSamplingRate;
   
private:

  
  
};





#endif      // MCCTRANSCODERCODECINFO_H

// End of File
