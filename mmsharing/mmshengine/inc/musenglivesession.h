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


#ifndef MUSHENGLIVESESSION_H
#define MUSHENGLIVESESSION_H

// USER
#include "musengmceoutsession.h"
#include "musunittesting.h"

// SYSTEM
#include <ecam.h>

// FORWARD DECLARATIONS
class MMusEngLiveSessionObserver;
class CMusEngOrientationHandler;

class CMusEngLiveSession : public CMusEngMceOutSession
    {
    MUS_UNITTEST( UT_CMusEngOutSession )
    MUS_UNITTEST( UT_CMusEngLiveSession )
    MUS_UNITTEST( UT_CMusEngOrientationHandler )

    public:

        /**
        * Creates new MultimediaSharing Live session.
        *
        * @param aFileName Media filename
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aSessionObserver Interface for session specific callbacks
        * @param aOutSessionObserver Interface for outsession specific callbacks
        * @param aLiveSessionObserver interface for live session specific
        *        callbacks
        * @param aSipProfileId SIP profile to be used, 0 is default profile
        * @return CMusEngLiveSession* New instanse of specified class
        */
        IMPORT_C static CMusEngLiveSession* NewL( 
							const TDesC& aFileName,
                            const TRect& aRect,
                            MMusEngSessionObserver& aSessionObserver,
                            MMusEngOutSessionObserver& aOutSessionObserver,
                            MMusEngLiveSessionObserver& aLiveSessionObserver,
                            TUint aSipProfileId = 0);
                            
        /**
        * Creates new MultimediaSharing Live session.
        *
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aSessionObserver Interface for session specific callbacks
        * @param aOutSessionObserver Interface for outsession specific callbacks
        * @param aLiveSessionObserver interface for live session specific
        *        callbacks
        * @param aSipProfileId SIP profile to be used, 0 is default profile
        * @return CMusEngLiveSession* New instanse of specified class
        */
        IMPORT_C static CMusEngLiveSession* NewL( 
                            const TRect& aRect,
                            MMusEngSessionObserver& aSessionObserver,
                            MMusEngOutSessionObserver& aOutSessionObserver,
                            MMusEngLiveSessionObserver& aLiveSessionObserver,
                            TUint aSipProfileId = 0 );


    public:

        /**
        * Destructor
        */
        ~CMusEngLiveSession();



    public: // NEW LIVE SESSION SPECIFIC API FUNCTIONS

        /**
        * Gets current zoom factor
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt CurrentZoomL() const;
        
        /**
        * Sets new zoom factor
        * 
        * @pre Session is established
        * @pre MinZoomL <= aNewZoomFactor <= MaxZoomL
        * @leave KErrNotReady if session is not established
        */
        IMPORT_C void SetZoomL( TInt aNewZoomFactor );
        
        /**
        * Gets minimum zoom factor
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt MinZoomL() const;

        /**
        * Gets maximum zoom factor
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt MaxZoomL() const;
        
        /**
        * Increases zoom factor by one.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void ZoomInL();

        /**
        * Decreases zoom factor by one.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void ZoomOutL();

        /**
        * Sets zoom factor to default.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void ZoomDefaultL();

        /**
        * Sets current brightness
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void SetBrightnessL( TInt aBrightness ) const;


        /**
        * Gets current brightness
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt CurrentBrightnessL() const;

        /**
        * Gets maximum brightness
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt MaxBrightnessL() const;

        /**
        * Gets minimum brightness
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt MinBrightnessL() const;

        /**
        * Increases brightness by one.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void IncreaseBrightnessL();

        /**
        * Decreases brightness by one.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void DecreaseBrightnessL();

        /**
        * Sets brightness factor to default.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void BrightnessDefaultL();

        /**
        * Sets brightness to automatic.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void BrightnessAutoL();

        /**
        * Starts or pauses recording
        */
        IMPORT_C void RecordL( TBool aRecord );

        /**
        * Tells whether recording is paused or not
        */
        IMPORT_C TBool IsRecording();
        
        
    public: // implementation of virtual API from CMusEngMceOutSession

        /**
        * Resumes previously paused session. 
        * Continues using viewfinder and enables streaming video.
        *
        */
        IMPORT_C void PlayL();

        /**
        * Pauses session.
        * Holds display and disables streaming video to network.
        *
        */
        IMPORT_C void PauseL();

        /**
        * Tells whether session is paused or not
        */
        IMPORT_C TBool IsPlayingL();
        
        void EnableDisplayL( TBool aEnable );
        
        void RefreshOrientationL();


    protected: // inherited from CMusEngMceOutSession

        /**
        * @param aLocalBundle All local streams are supposed to be added to
        *        this bundle
        */
		void CompleteSessionStructureL( CMceStreamBundle& aLocalBundle );


    private: // overrides ancestor class versions
    
        /**
        * The state of the session has changed
        * @param aSession, the session that has changed.
        */
        virtual void HandleSessionStateChanged( CMceSession& aSession,
                                                TInt aStatusCode,
                                                const TDesC8& aReasonPhrase );
                                                
        /**
        * Sets Multimediasharing specific video codec settings like video 
        * bitrate maxbitrate. This functions gets called for every video codec 
        * in session. This function overrides function in ancestor classes. 
        * Function calls also overridden version.
        */ 
        virtual void AdjustVideoCodecL( CMceVideoCodec& aVideoCodec );
        
        /**
        * Sets Multimediasharing specific audio codec settings. This functions 
        * gets called for every audio codec in session. This function overrides 
        * function in base class. Function calls also overridden version.
        */ 
        virtual void AdjustAudioCodecL( CMceAudioCodec& aAudioCodec );     
        
        /**
        * Removes AVC if known that it is not supported
        */
        virtual void DoCodecSelectionL( CMceVideoStream& aVideoStream );
   

    private: // from MMceStreamObserver, 
             // overrides the function in CMusEngMceSession

        /**
        * The state of the sink has changed.
        *
        * @param aStream, the stream that uses the sink.
        * @param aSink, the sink that has changed.
        */
        void StreamStateChanged( CMceMediaStream& aStream,
                                 CMceMediaSink& aSink );    
                                 

    private:

        /**
        * Constructor
        */
        CMusEngLiveSession( MMusEngSessionObserver& aSessionObserver,
                            MMusEngOutSessionObserver& aOutSessionObserver,
                            MMusEngLiveSessionObserver& aLiveSessionObserver,
        					const TRect& aRect,
        					const TDesC& aRecordedFile = KNullDesC );

        /**
        * Second-phase constructor
        */
        void ConstructL( TUint aSipProfileId );

    
    private: // Helpers
    
        /**
        * Reads from CenRep and sets encoding device for a codec.
        */
        void SetEncodingDeviceL( CMceVideoCodec& aVideoCodec );

        /**
        * Sets configuration key for a codec
        */
        void SetCodecConfigKeyL( CMceVideoCodec& aVideoCodec );
    
        /**
        * Reads proper configuration key for a codec. NULL if not available.
        * Ownership is transferred.
        */
        HBufC8* ReadCodecConfigKeyL( const CMceVideoCodec& aVideoCodec ) const;
        
        /**
        * Constructs and stores to cenrep a concatenation of all AVC config keys.
        */
        void StoreEncoderConfigInfoL() const;
        
        /**
        * Returns config key id for the provided codec. Ownership is transferred
        */
        const TPtrC8 ConfigKeyIdL( const CMceVideoCodec& aVideoCodec ) const;
        

        /**
        *    Calculate the zoom step size based time escaped. 
        * 
        * @param aTime, start time. pass iZoomInTime or iZoomOutTime as parameter
        * @return TInt, return calculate result of the step size  
        */
        TInt ZoomStepSize( TInt64& aTime );
        
        /**
         *  Calculates iMinZoomStep and iMaxZoomStep zoom step sizes,
         *  should be called before first ZoomIn()/ZoomOut() call
         */
        void InitializeZoomStepSize();
        
        
    private:

        /**
        * Reference to live session observer interface.
        */
        MMusEngLiveSessionObserver& iLiveSessionObserver;

        /**
        *
        */
        TCameraInfo iCameraInfo;

        /**
        *
        */
        TInt iDefaultZoomFactor;

        /**
        *
        */
        TInt iDefaultBrightness;

        /**
        * File name for recording. If KNullDesC no recording performed.
        */
        TFileName iRecordedFile;
        
        /**
        * If true, configuration key of current AVC codec must be written to
        * CenRep after session establishment.
        */
        TBool iStoreEncoderConfigInfo;
        
        /**
        * latest Zoomin time
        */

        TInt64 iZoomInTime;

        /**
        * latest Zoomout time
        */
        TInt64 iZoomOutTime;        
                       
        /**
        *
        */
        TInt iSmallZoomStep;

        /**
        *
        */
        TInt iBigZoomStep;
        
        CMusEngOrientationHandler* iOrientationHandler;
        
    };

#endif

