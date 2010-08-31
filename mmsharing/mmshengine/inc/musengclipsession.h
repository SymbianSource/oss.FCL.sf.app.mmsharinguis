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


#ifndef MUSHENGCLIPSESSION_H
#define MUSHENGCLIPSESSION_H

// USER
#include "musengmceoutsession.h"
#include "musunittesting.h"

class MMusEngClipSessionObserver;
class CMusEngLiveSession;
class CMceFileSource;
class CMceAudioStream;
class CMceVideoStream;
class CMceAudioCodec;
class CMusEngClipVideoPlayer;


class CMusEngClipSession : public CMusEngMceOutSession
    {
    MUS_UNITTEST( UT_CMusEngClipSession )
    MUS_UNITTEST( UT_CMusEngMceSession )
    
    public: // Constructors and destructor

        /**
        * Constructor
        */
        static CMusEngClipSession* NewL();
                                                  
        /**
        * Destructor
        */
        ~CMusEngClipSession();

    public: // from MLcSession
    
        MLcVideoPlayer* LocalVideoPlayer();


    protected: // implementation of virtual helper from CMusEngMceOutSession

        /**
        * @param aLocalBundle All local streams are supposed to be added to
        *        this bundle
        */
        void CompleteSessionStructureL( CMceStreamBundle& aLocalBundle );        


    protected: // overrides function from CMusEngMceOutSession
    
        /**
        * Checks that there is no need for transcoding before calling
        * overridden base class variant of this function.
        */
        void EstablishSessionL();
        
    
    private: // from MMceStreamObserver, 
             // overrides the function in CMusEngMceSession

        /**
        * The state of the stream has changed.
        *
        * @param aStream, the stream that has changed.
        */
        void StreamStateChanged( CMceMediaStream& aStream );
        
        /**
        * The state of the source has changed.
        * @param aStream, the stream that uses the source.
        * @param aSource, the source that has changed.
        */
        void StreamStateChanged( CMceMediaStream& aStream,
                                 CMceMediaSource& aSource );

    private:

        CMusEngClipSession();

        void ConstructL();


    private: // HELPERS
        
        void AddAmrCodecL( CMceAudioStream& aAudioStream );
        
        void AddVideoCodecL( CMceVideoStream& aVideoStream );
        
        void ConstructAudioStructureL( CMceStreamBundle& aLocalBundle );
        
        void DetermineBufferingPeriod( CMceMediaStream& aStream );    

    private: // Data
        
        CMusEngClipVideoPlayer* iClipVideoPlayer;
        TTime iBufferingStartedTime;
    };

#endif // MUSHENGCLIPSESSION_H

