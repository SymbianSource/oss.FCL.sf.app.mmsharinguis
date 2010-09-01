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


#ifndef MUSHENGMCESESSION_H
#define MUSHENGMCESESSION_H

// USER
#include "musengsession.h"
#include "musengsessiondurationtimerobserver.h"

// SYSTEM
/*
#include <mcesessionobserver.h>
#include <mcestreamobserver.h>
#include <mcertpobserver.h>
#include <mcetransactiondatacontainer.h>
*/

/*
class CMceManager;
class CMceSession;
*/
class CMusEngSessionDurationTimer;


// CONSTANTS

_LIT8( KMusEngAcceptContactHeader,
       "Accept-Contact: *;+g.3gpp.cs-voice; explicit" );
_LIT8( KMusEngSessionSdpLineXApplication, "a=X-application:com.nokia.rtvs" );

// CLASS DECLARATION

/**
* Base class for Multimedia Sharing Engine MCE sessions, inherited by
* CMusEngLiveSession CMusEngClipSession and CMusEngReceiveSession.
*
* CMusEngMceSession encapsulates CMCESession and simplifies use of
* MCE Sessions to user interface. Also splits observers messy events
* simplified commands like a SessionDisconnected.
*
* @lib musengine.lib
* @since S60 v3.2
*/
class CMusEngMceSession : public CMusEngSession,
                          public MMusEngSessionDurationTimerObserver
    {

    public:

        /**
        * Destructor
        *
        * @since S60 v3.2
        */
        ~CMusEngMceSession();


    public: // API FUNCTIONS

        /**
        * Terminates session.
        * @pre Session is established
        * @post Session is ready to be deleted
        * @leave KErrNotReady if precondition is not fullfilled
        * @since S60 v3.2
        */
        IMPORT_C void TerminateL();

        /**
        * Releases all this session internal observers. This function is needed
        * before we could get session store out of this session.
        * TODO: Is this function really needed? At least at the moment it is
        * not called from UI.   -HR 21.8.
        *
        * @since S60 v3.2
        */
        IMPORT_C void ReleaseInternalObservers();

        /**
        * Get session time return estabilished session time
        *
        * @since S60 v3.2
        * @return TTime returns time if connection established else < 0
        */
        IMPORT_C TTimeIntervalSeconds GetSessionTime() const;

        /**
        * Returns current connection state
        *
        * @since S60 v3.2
        * @return TBool returns current connection state
        */
        IMPORT_C TBool ConnectionActive() const;


    public: // VIRTUAL API

        // No virtual api


    public: //  from CMusEngSession

        /**
        * Increases volume level by one
        *
        * @pre Session is ongoing
        * @since S60 v3.2
        */
        IMPORT_C void VolumeUpL();

        /**
        * Decreases volume level by one
        *
        * @pre Session is ongoing
        * @since S60 v3.2
        */
        IMPORT_C void VolumeDownL();

        /**
        * Permission to draw on screen device.
        *
        * @pre Session is ongoing
        * @since S60 v3.2
        */
        IMPORT_C void EnableDisplayL( TBool aEnable );

        /**
        * Implements virtual from CMusEngSession
        *
        * @pre Session is ongoing
        * @since S60 v3.2
        */
        IMPORT_C void MuteL();

        /**
        * Implements virtual from CMusEngSession
        *
        * @pre Session is ongoing
        * @since S60 v3.2
        */
        IMPORT_C void UnmuteL();


    protected: // CONSTRUCTORS

        /**
        * Constructor
        *
        * @since S60 v3.2
        */
        CMusEngMceSession( const TRect& aRect );

        /**
        * Second-phase constructor
        *
        * @since S60 v3.2
        */
        void ConstructL();


    protected: // HELPER FUNCTIONS

        /**
        *
        * @since S60 v3.2
        */
        void RectChangedL();

        /**
        * Sets session specific SDP line to a session
        *
        * @since S60 v3.2
        */
        //void SetSessionSdpLinesL( CMceSession& aSession );


    protected: // internal API

        /**
        *
        * @since S60 v3.2
        */
        virtual TBool IsRtpcInactivityTimoutSupported();


    private: // HELPER FUNCTIONS

        /**
        * Start counting session time
        *
        * @since S60 v3.2
        */
        void InitializeSessionTimer();

        /**
        * Callback
        *
        * @since S60 v3.2
        */
        void UpdateTimerEvent();

        /**
        *
        *
        * @since S60 v3.2
        */
        void DoMuteL( TBool aMute );

        /**
        * Increases or decreases current volume of all speakers in session
        * structure by one.
        * @pre Current volume of all the speakers is less than MaxVolumeL()
        *      and at least 1.
        */
        void ChangeVolumeByOneL( TBool aIncreaseVolume );


    private: // Helpers

        /**
        * Helper function to avoid three different traps.
        * Should be replaced when MCE supports different kind of method
        * detecting end of clip.
        *
        * @since S60 v3.2
        */
        void CheckClipEndL();


    public: // MEMBERS

        /**
        * Class for connecting to MCE server.
        * This class provides the connection to the MCE server and
        * provides functions
        * for setting observers for asynchronous events.
        * The user should create only one instance of this class.
        *
        * @since S60 v3.2
        */
        //CMceManager* iManager;

        /**
        * Base class for MCE sessions, inherited by both CMCEInSession and
        * CMCEOutSession. * CMCESession represents peer-to-peer connection
        * made with remote terminal. This connection is also known as dialog
        * and it is set up using SIP INVITE method. The characteristics of the
        * session are defined by user of this class by attaching media streams
        * into the session. After the session has been succesfully established,
        * it can be later updated and must be finally terminated. If for any
        * reason instance of this class is deleted while in established state,
        * session is automatically terminated by MCE server.
        *
        * @since S60 v3.2
        */
        //CMceSession* iSession;


    public:

        /**
        * It is assumed, that user has only one instance of this class.
        * The same instance is passed between MCE and user, but the data
        * inside of this container is just updated for each event.
        *
        * @since S60 v3.2
        */
        //TMceTransactionDataContainer iTransactionDataContainer;

        /**
        * Session time. Acts simultaneously as RTCP inactivity timer.
        *
        * @since S60 v3.2
        */
        TTime iStartTime;

        CMusEngSessionDurationTimer*   iUpdateTimer;

        /**
        * Value indicating seconds gone since last received RTCP sender or
        * receiver report. Value 20 indicates RTCP inactivity timeout.
        */
        TInt iSecondsFromLastRtcpReport;
        TInt iDisplayEnabled;

    };
#endif //MUSHENGMCESESSION_H
