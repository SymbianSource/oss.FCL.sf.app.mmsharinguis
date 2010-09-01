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


class MMusEngClipSessionObserver;
class CMusEngLiveSession;
class CMceFileSource;

class CMusEngClipSession : public CMusEngMceOutSession
    {

    public:

        /**
        *
        * @since S60 v3.2
        */
        IMPORT_C static CMusEngClipSession* NewL(
                                const TDesC& aFileName,
                                const TRect& aRect,
                                MMusEngClipSessionObserver* aSessionObserver,
                                TUint aSipProfileId = 0 );

        /**
        *
        * @since 3.2
        */
        ~CMusEngClipSession();


    public: // new API

        /**
        * Sets file to be shared during session.
        * @pre Session must be established
        * @pre New file cannot be DRM protected
        * @leave One of the system wide error codes
        * @param aFileName Name of the file to be shared
        * @since S60 v3.2
        */
        IMPORT_C void ChangeClipL( const TDesC& aFileName );

        /**
        *
        * @since S60 v3.2
        */
        IMPORT_C void FastForwardL( TBool aUseFFWD );

        /**
        *
        * @since S60 v3.2
        */
        IMPORT_C void FastRewindL( TBool aUseFRWD );

        /**
        *
        * @since S60 v3.2
        */
        IMPORT_C TTimeIntervalSeconds PositionL();

        /**
        *
        * @since S60 v3.2
        */
        IMPORT_C TTimeIntervalSeconds DurationL();

        /**
        *
        * @since S60 v3.2
        */
        IMPORT_C void SetPositionL (const TTimeIntervalSeconds &aPosition);


    public:

        /**
        *
        * @since S60 v3.2
        */
        IMPORT_C void SetSessionObserver(
                                MMusEngClipSessionObserver* aSessionObserver );


    public: // implementation of virtual API from CMusEngMceOutSession

        /**
        * Resumes previously paused session.
        * Continues showing file on display, resumes clip's audio, continues
        * playing file and enables streaming to network.
        *
        * @since S60 v3.2
        */
        IMPORT_C void PlayL();

        /**
        * Pauses session.
        * Holds display, mutes clip's audio, pauses file and
        * disables all streaming to network.
        *
        * @since S60 v3.2
        */
        IMPORT_C void PauseL();


    protected: // implementation of virtual helper from CMusEngMceOutSession

        /**
        *
        * @since S60 v3.2
        */
        void CompleteSessionStructureL();


    private:

        /**
        *
        * @since S60 v3.2
        */
        CMusEngClipSession( MMusEngClipSessionObserver* aSessionObserver,
                            const TRect& aRect,
                            TUint aSipProfileId = 0 );

        /**
        * @leave KErrPermissionDenied if file is DRM protected
        * @since S60 v3.2
        */
        void ConstructL( const TDesC& aFileName );


    private: // HELPERS

        /**
        *
        * @since S60 v3.2
        */
        TBool IsProtectedFileL( const TDesC& aClipFile );

        /**
        * Constructs audio stream structure during session completion and adds
        * it to session.
        * @pre iSession != NULL
        * @pre iFileSource != NULL
        */
        void ConstructAudioStructureL();

    public: // Data

        CMceFileSource* iFileSource;

    };

#endif

