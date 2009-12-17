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

// SYSTEM
#include <ecam.h>

// FORWARD DECLARATIONS
class MMusEngLiveSessionObserver;


class CMusEngLiveSession : public CMusEngMceOutSession
    {

    public:

        /**
        * Creates new MultimediaSharing Live session.
        *
        * @since S60 v3.2
        * @param aFileName Media filename
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aSessionObserver Session specific callbacks
        * @param aSipProfileId SIP profile to be used, 0 is default profile
        * @return CMusEngLiveSession* New instanse of specified class
        */
        IMPORT_C static CMusEngLiveSession* NewL(
                                const TDesC& aFileName,
                                const TRect& aRect,
                                MMusEngLiveSessionObserver* aSessionObserver,
                                TUint aSipProfileId = 0);

        /**
        * Creates new MultimediaSharing Live session.
        *
        * @since S60 v3.2
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aSessionObserver Session specific callbacks
        * @param aSipProfileId SIP profile to be used, 0 is default profile
        * @return CMusEngLiveSession* New instanse of specified class
        */
        IMPORT_C static CMusEngLiveSession* NewL(
                                const TRect& aRect,
                                MMusEngLiveSessionObserver* aSessionObserver,
                                TUint aSipProfileId = 0 );

    public:

        /**
        * Destructor
        *
        * @since S60 v3.2
        */
        ~CMusEngLiveSession();



    public: // NEW API FUNCTIONS live session specific

        /**
        * Sets callback interface pointer
        *
        * @since S60 v3.2
        * @param aSessionObserver Pointer to class which implements interface
        */
        IMPORT_C void SetSessionObserver(
                                MMusEngLiveSessionObserver* aSessionObserver );

        /**
        * Gets current zoom factor
        *
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        * @since S60 v3.2
        */
        IMPORT_C TInt CurrentZoomL() const;

        /**
        * Gets maximum zoom factor
        *
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        * @since S60 v3.2
        */
        IMPORT_C TInt MaxZoomL() const;

        IMPORT_C TInt MinZoomL() const;

        /**
        * Increases zoom factor by one.
        *
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        * @since S60 v3.2
        */
        IMPORT_C void ZoomInL();

        /**
        * Decreases zoom factor by one.
        *
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        * @since S60 v3.2
        */
        IMPORT_C void ZoomOutL();

        /**
        * Sets zoom factor to default.
        *
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        * @since S60 v3.2
        */
        IMPORT_C void ZoomDefaultL();

    public: // implementation of virtual API from CMusEngMceOutSession

        /**
        * Resumes previously paused session.
        * Continues using viewfinder and enables streaming video.
        *
        * @since S60 v3.2
        */
        IMPORT_C void PlayL();

        /**
        * Pauses session.
        * Holds display and disables streaming video to network.
        *
        * @since S60 v3.2
        */
        IMPORT_C void PauseL();


    protected: // inherited from CMusEngMceOutSession

        /**
        *
        * @since S60 v3.2
        */
        void CompleteSessionStructureL();


    private:

        /**
        * Constructor

        * @since S60 v3.2
        */
        CMusEngLiveSession( MMusEngLiveSessionObserver* aSessionObserver,
                            const TRect& aRect,
                            TUint aSipProfileId = 0 );

        /**
        * Second-phase constructor
        *
        * @since S60 v3.2
        */
        void ConstructL();


    public:

        /**
        *
        * @since S60 v3.2
        */
        //TCameraInfo iCameraInfo;

        /**
        *
        * @since S60 v3.2
        */
        TInt iDefaultZoomFactor;
        TInt iPlaying;
        TInt iCurrentZoom;


    };

#endif

