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



#ifndef MCCTRANSCODER_VIDEO_CODEC_H
#define MCCTRANSCODER_VIDEO_CODEC_H

//  INCLUDES
#include "transcodercodecinfo.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Container class for the codec information.
*
*/
class CMccTranscoderVideoCodec : public CMccTranscoderCodecInfo
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccTranscoderVideoCodec* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccTranscoderVideoCodec();

    public: // New functions

    public: // Functions from base classes

    public: // From CMccCodecInformation
   
        void SetBitrate( TInt aBitrate );
    
        TInt Bitrate();
   
        void SetSamplingRate( TInt aSamplingRate );
    
        TInt SamplingRate();
        
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * C++ default constructor.
        */
        CMccTranscoderVideoCodec();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public:     // Data
    
    protected:  // Data

    private:    // Data

    public:     // Friend classes

    protected:  // Friend classes
    
    private:    // Friend classes

    };

#endif      // MCCTRANSCODER_VIDEO_CODEC_H   
            
// End of File
