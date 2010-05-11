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

class CMusEngClipSession : public CMusEngMceOutSession
    {
    MUS_UNITTEST( UT_CMusEngClipSession )
    
    public:

        /**
        *
        */
        IMPORT_C static CMusEngClipSession* NewL( 
                                const TRect& aRect,
                                MMusEngSessionObserver& aSessionObserver,
                                MMusEngOutSessionObserver& aOutSessionObserver,
                                MMusEngClipSessionObserver& aClipSessionObserver,
                                TUint aSipProfileId = 0 );
                                                  
        /**
        *
        */
        ~CMusEngClipSession();


    public: // new API
        
        /**
        * Sets file to be shared during session.
        * @pre File cannot be DRM protected
        * @post InviteL can be called
        * @leave One of the system wide error codes
        * @param aFileName Name of the file to be shared
        */
        IMPORT_C void SetClipL( const TDesC& aFileName );
        
        /**
        * Starts or stops fast forwarding the clip. Clip is left paused after
        * stopping the fast forwarding.
        * @pre Session is ongoing
        * @param aUseFFWD ETrue to start FFWD, EFalse to stop it.
        */
        IMPORT_C void FastForwardL( TBool aUseFFWD );

        /**
        * Starts or stops fast rewinding the clip. Clip is left paused after
        * stopping the fast rewinding.
        * @pre Session is ongoing
        * @param aUseFRWD ETrue to start FRWD, EFalse to stop it.
        */
        IMPORT_C void FastRewindL( TBool aUseFRWD );

        /**
        * Returns current position as time interval.
        */
        IMPORT_C TTimeIntervalSeconds PositionL();

        /**
        * Returns duration of current clip as time interval
        */
        IMPORT_C TTimeIntervalSeconds DurationL();

        /**
        *
        */
        IMPORT_C void SetPositionL (const TTimeIntervalSeconds &aPosition);

        /**
        * Starts transcoding of file. After the transcoding is complete,
        * file to be shared is changed into the transcoded file.
        * @pre MMusEngClipSessionObserver::TranscodingNeeded is called
        * @param aFileName Name of the transcoding output file.
        */
        IMPORT_C void TranscodeL( const TFileName& aFileName );

        /**
        * Cancels transcoding of file. 
        * @pre TranscodeL has been called succesfully and transcoding 
        *      is not completed
        */
        IMPORT_C void CancelTranscodeL();


    public: // implementation of virtual API from CMusEngMceOutSession

        /**
        * Resumes previously paused session. 
        * Continues showing file on display, resumes clip's audio, continues
        * playing file and enables streaming to network.
        * @leave KErrNotReady if called during FFWD or FRWD
        */
        IMPORT_C void PlayL();

        /**
        * Pauses session.
        * Holds display, mutes clip's audio, pauses file and 
        * disables all streaming to network.
        * @leave KErrNotReady if called during FFWD or FRWD
        */
        IMPORT_C void PauseL();

        /**
        * Tells whether session is paused or not
        */
        IMPORT_C TBool IsPlayingL();


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

        /**
        *
        */
        CMusEngClipSession( MMusEngSessionObserver& aSessionObserver,
                            MMusEngOutSessionObserver& aOutSessionObserver,
                            MMusEngClipSessionObserver& aClipSessionObserver,
                            const TRect& aRect );

        /**
        * @leave KErrPermissionDenied if file is DRM protected
        */
        void ConstructL( TUint aSipProfileId );


    private: // HELPERS

        /**
        *
        */
        TBool IsProtectedFileL( const TDesC& aClipFile );
        
        void AddAmrCodecL( CMceAudioStream& aAudioStream );
        
        void AddVideoCodecL( CMceVideoStream& aVideoStream, TBool aIgnoreNegotiated = EFalse );
        
        TBool HasClipEnded();

        /**
        * Returns current position as microseconds.
        */
        TTimeIntervalMicroSeconds PositionMicroSecondsL();

        /**
        * Constructs audio stream structure during session completion and adds
        * it to session.
        * @param aLocalBundle All local streams are supposed to be added to
        *        this bundle
        * @pre iSession != NULL
        */
        void ConstructAudioStructureL( CMceStreamBundle& aLocalBundle );
        
        void DetermineBufferingPeriod( CMceMediaStream& aStream );
            
        TBool IsH264Supported() const;
        
        void HandleTranscodingFailureL( TInt aError );
        
        TInt DoCompleteTranscoding();
        
        void DeleteTranscodingDestinationFileL();

    private: // Data
    
        /**
        * Reference to live session observer interface.
        */
        MMusEngClipSessionObserver& iClipSessionObserver;
        
        TFileName iFileName;
        
        TBool iTranscodingOngoing;
        
        TTime iFFWDStartTime;
        TTime iFRWDStartTime;
        
        TTime iBufferingStartedTime;
        TTimeIntervalMicroSeconds iBufferingPeriod;
        
        TTimeIntervalMicroSeconds iDelayFileEndingPos; 
        TTimeIntervalMicroSeconds iPreviousPos;
        TBool iClipEnded;
        
        TFileName iTranscodingDestFileName;
        TBool iRewindedToBeginning;
        TBool iTranscodingRequiredDueMissingOptions;
        
    };

#endif

