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


#ifndef MUSHENGMCEOUTSESSION_H
#define MUSHENGMCEOUTSESSION_H

// USER
#include "musengmcesession.h"


// FORWARD DECLARATIONS
class CSIPProfile;
class CMusSipProfileHandler;
class MMusEngOutSessionObserver;

// CLASS DECLARATION
/**
* Class for handling MCE out sessions. Direct inherit classes are
* CMusEngLiveSession and CMusEngClipSession.
*
* @lib musengine.lib
* @since S60 v3.2
*/
class CMusEngMceOutSession : public CMusEngMceSession
    {

    public:

        /**
        * Destructor
        *
        * @since S60 v3.2
        */
        ~CMusEngMceOutSession();


    public: // NEW API FUNCTIONS

        /**
        * Sends invite to specified address
        * @leave KErrAlreadyExists if session establishment is already
        *        in progress
        *
        * @since S60 v3.2
        */
        IMPORT_C void InviteL( const TDesC& aRecipient );

        /**
        * Cancels Invite.
        * @pre Invite is sent.
        * @since S60 v3.2
        */
        IMPORT_C void CancelInviteL( );


    public: // VIRTUAL API

        /**
        * Resumes previously paused session. Exact behavior depends on
        * exact session type.
        *
        * @since S60 v3.2
        */
        IMPORT_C virtual void PlayL() = 0;

        /**
        * Pauses session. Exact behavior depends on exact session type.
        *
        * @since S60 v3.2
        */
        IMPORT_C virtual void PauseL() = 0;


    protected: // Must be implemented in derived class

        /**
        *
        * @since S60 v3.2
        */
        virtual void CompleteSessionStructureL() = 0;


    protected: // helper function inherited from CMusEngMceSession

        /**
        * Function handling MCE session termination.
        *
        * @since S60 v3.2
        */
        //void HandleTermination( TMceTransactionDataContainer* aContainer );


    protected:

        /**
        * Constructor
        *
        * @since S60 v3.2
        */
        CMusEngMceOutSession( const TRect& aRect, TUint aSipProfileId = 0 );

        /**
        * Second-phase constructor
        *
        * @since S60 v3.2
        */
        void ConstructL();


    protected: // HELPERS

        #if (defined (__WINS__) || defined(__WINSCW__))
        /**
        *
        * @since S60 v3.2
        */
        void SipProfileChanged();
        #endif


    private:

        /**
        *
        * @since S60 v3.2
        */
        void CreateMceSessionStructureL();

        /**
        *
        * @since S60 v3.2
        */
        MMusEngOutSessionObserver* OutSessionObserver();

        /**
        * Parses, validates and creates new recipient address
        * out of given address.
        * @return Parsed recipient URI. Ownership is transferred.
        * @since S60 v3.2
        */
        HBufC8* ParseRecipientUriL(  const TDesC& aRecipient  );


    protected: // DATA

        /**
        * Repository id of SipProfile
        *
        * @since S60 v3.2
        */
        TUint iSipProfileId;

        /**
        * Owned by iSipProfileHandler
        *
        * @since S60 v3.2
        */
        //CSIPProfile* iSipProfile;

        /**
        * Callback pointer to interface
        *
        * @since S60 v3.2
        */
        //CMusSipProfileHandler* iSipProfileHandler;


    public: // DATA

        /**
        *
        * @since S60 v3.2
        */
        HBufC8* iRecipient;
        TBool iInvited;

    };
#endif //MUSHENGMCESESSION_H
