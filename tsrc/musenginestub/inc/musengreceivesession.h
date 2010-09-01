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


#ifndef MUSHENGREVEIVESESSION_H
#define MUSHENGREVEIVESESSION_H



#include "musengmcesession.h"


class MMusEngReceiveSessionObserver;

class CMusEngReceiveSession : public CMusEngMceSession

    {

    public: // Contructors and destructor

       /**
        * Creates new MultimediaSharing Receive session.
        *
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aEngObserver Engine specific callbacks
        * @param aSessionObserver Session specific callbacks
        * @return CMusEngReceiveSession* New instanse of specified class
        */
        IMPORT_C static CMusEngReceiveSession* NewL(
                            const TRect& aRect,
                            MMusEngReceiveSessionObserver* aSessionObserver );
       /**
        * Destructor
        *
        * @since S60 v3.2
        */
        ~CMusEngReceiveSession();


    public: // API

       /**
        * Accept or deny processed invitation
        *
        * @param aAccept ETrue if session is to be accepted and EFalse if
        *        to be rejected.
        */
        IMPORT_C void AcceptInvitationL(const TBool& aAccept);


    public:

       /**
        * Sets callback interface pointer
        *
        * @param aSessionObserver Pointer to class which implements interface
        */
        IMPORT_C void SetSessionObserver(
                                MMusEngReceiveSessionObserver* aSessionObserver );



    protected: // internal API

        /**
        *
        * @since S60 v3.2
        */
        TBool IsRtpcInactivityTimoutSupported();


    private: // CONSTRUCTORS

        CMusEngReceiveSession( MMusEngReceiveSessionObserver* aSessionObserver,
                               const TRect& aRect );

        void ConstructL();

    public: // HELPERS

        //CMceInSession* InSession();

        MMusEngReceiveSessionObserver* ReceiveSessionObserver();

        // void CompleteSessionStructureL( CMceInSession& aInSession );


    public: // DATA

       // CMceInSession* iTemporaryInSession; // Owned
       TBool iAccepInvitation;

    };

#endif

