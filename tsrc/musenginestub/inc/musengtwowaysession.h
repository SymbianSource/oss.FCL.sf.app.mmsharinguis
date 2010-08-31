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


#ifndef MUSHENGTWOWAYSESSION_H
#define MUSHENGTWOWAYSESSION_H

// USER
#include "musenglivesession.h"
#include "musunittesting.h"
#include "mussettings.h"

// SYSTEM
#include <ecam.h>

// FORWARD DECLARATIONS
class MMusEngLiveSessionObserver;
class CMceDisplaySink;
class MMusEngTwoWaySessionObserver;

class CMusEngTwoWaySession : public CMusEngLiveSession
    {
    MUS_UNITTEST( UT_CMusEngOutSession )
    MUS_UNITTEST( UT_CMusEngTwoWaySession )

    
    public:
        /**
        * Creates new MultimediaSharing Live session.
        *
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aSessionObserver Interface for session specific callbacks
        * @param aOutSessionObserver Interface for outsession specific callbacks
        * @param aLiveSessionObserver interface for live session specific
        *        callbacks
        * @return CMusEngTwoWaySession* New instanse of specified class
        */
        IMPORT_C static CMusEngTwoWaySession* NewL( 
                            const TRect& aRemoteRect,
                            const TRect& aLocalRect,
                            MMusEngSessionObserver& aSessionObserver,
                            MMusEngOutSessionObserver& aOutSessionObserver,
                            MMusEngLiveSessionObserver& aLiveSessionObserver,
                            MMusEngTwoWaySessionObserver& aTwoWayObserver );


    public:

        /**
        * Destructor
        */
        ~CMusEngTwoWaySession();

    public:
        
        void EnableDisplayL( TBool aEnable );

        void SetOrientationL( TDisplayOrientation aOrientation );
        
    public:

        /**
        * Starts or pauses recording
        */
        void RecordL( TBool aRecord );

        /**
        * Tells whether recording is paused or not
        */
        TBool IsRecording();
        
        /**
        * Sets both remote (receiving video) and local (viewfinder) rects.
        * 
        */
        IMPORT_C void SetRectsL( const TRect& aRemoteRect,
                                 const TRect& aLocalRect );

        /**
        * Sets local rect (viewfinder).
        * 
        */
        IMPORT_C void SetLocalRectL( const TRect& aLocalRect );
        
        /**
        * Gets local rect (viewfinder).
        * 
        */
        IMPORT_C TRect LocalRect() const;
        
    protected: // inherited from CMusEngMceOutSession

        void RectChangedL();
        
        /**
        * @param aLocalBundle All local streams are supposed to be added to
        *        this bundle
        */
		void CompleteSessionStructureL( CMceStreamBundle& aLocalBundle );
		
		void AddDisplayL( CMceMediaStream& aStream );


    protected: // overrides ancestor class versions
        
        /**
        * The state of the session has changed
        * @param aSession, the session that has changed.
        */
        virtual void HandleSessionStateChanged( CMceSession& aSession,
                                                TInt aStatusCode,
                                                const TDesC8& aReasonPhrase );
   
    private: // from MMceStreamObserver, 
             // overrides the function in CMusEngMceSession
        
        void StreamStateChanged( CMceMediaStream& aStream );

        void  InactivityTimeout( CMceMediaStream& aStream,
                                 CMceRtpSource& aSource );
    
    private:

        /**
        * Constructor
        */
        CMusEngTwoWaySession( MMusEngSessionObserver& aSessionObserver,
                            MMusEngOutSessionObserver& aOutSessionObserver,
                            MMusEngLiveSessionObserver& aLiveSessionObserver,
                            MMusEngTwoWaySessionObserver& aTwoWayObserver,
                            const TRect& aRemoteRect,
                            const TRect& aLocalRect );

        /**
        * Second-phase constructor
        */
        void ConstructL();

    private:
        
        void ReceivingStarted();
        
        void ReceivingStopped();
        
    private:
        
        MMusEngTwoWaySessionObserver& iTwoWayObserver;
        
        TRect iLocalRect;
        
        TBool iReceiving;
        
        TBool iBuffered;
        
    };

#endif

