/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MCC_TRANSCODER_SESSION_INFO_H
#define MCC_TRANSCODER_SESSION_INFO_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>

#include "transcodervideocodec.h"
#include "transcoderaudiocodec.h"
#include "mcctranscoderlogs.h"

class TMccTranscoderCodecInfo;
/**
*
*/ 
class CMccTranscoderSessionInfo : public CBase 
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static   CMccTranscoderSessionInfo* NewLC();

        /**
        * Destructor.
        */
        virtual  ~CMccTranscoderSessionInfo();

    public:  // New functions

                     
        TUint32 SessionId();  
        
        void GenerateSessionId();
        
        void SetQuality( TUint32 aQuality );
        
        void CreateMovieL();
        
        CVedMovie& Movie();
        
                             
        CVedMovie::TVedMovieQuality Quality();
        
        void SetDesFileL( const TDesC& aFileName );
        
        const TDesC& DesFile();
        
        
        CMccTranscoderVideoCodec& VideoCodec();
        
        void SetVideoCodec( CMccTranscoderVideoCodec* aCodec );
        
        
        CMccTranscoderAudioCodec& AudioCodec();
        
        void SetAudioCodec( CMccTranscoderAudioCodec* aCodec );
        
      
        
        void SetVideoType( const TDesC8& aSdpNam,
                           const TInt aVideoBitRate );
                           
        void SetAudioType( const TDesC8& aSdpNam ); 
        
        void CheckVideoCodecL( const TMccTranscoderCodecInfo& aCodecInfo );
        
        void CheckAudioCodecL( const TMccTranscoderCodecInfo& aCodecInfo );
        
        void GetPutputParameters( TVedOutputParameters& params );
        
        void SetRemoved( TBool aIsRemoved );
        
        TBool IsRemoved() const;
        
        TInt Progressed( TInt aProgressPercentage );
        
        void ClipAdded();
        
        TBool QualityChangeNeeded();
        
    protected:  // Functions from base classes
    
    private:
        /**
        * C++ default constructor.
        */
        CMccTranscoderSessionInfo();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aSource    Source of the data for the payload format plugin
        */
        void ConstructL();
        
    private:
    
        TInt CalculateVideoBitrate( CVedMovie& aMovie );
        
    private: // data
    
        TUint32                          iSessionId;
        CVedMovie*                       iMovie;
        CVedMovie::TVedMovieQuality      iQuality;
        TFileName                        iDesFile;
        CMccTranscoderVideoCodec*        iVideoCodec;
        CMccTranscoderAudioCodec*        iAudioCodec;
        TBool                            iIsRemoved;
        TBool                            iAudioNotPresent;
        TInt                             iSyncIntervalInPicture;
        TInt                             iSegmentSizeInBytes;

        
    private:    // Friend classes
    
    #ifdef EUNIT_TEST
       friend class UT_CMccTranscoderSessionInfo;
    #endif
    };


#endif // end of MCC_TRANSCODER_SESSION_INFO_H
