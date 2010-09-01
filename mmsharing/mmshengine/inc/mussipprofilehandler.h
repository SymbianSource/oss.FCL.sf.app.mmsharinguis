/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSSIPPROFILEHANDLER_H
#define MUSSIPPROFILEHANDLER_H


// INTERNAL INCLUDE
#include "musunittesting.h"

// SYSTEM INCLUDE
#include <e32base.h>
#include <sipobserver.h>
#include <sipconnectionobserver.h>
#include <sipprofileregistryobserver.h>
#include <sipprofilealrobserver.h>


class CSIPProfile;
class CSIPProfileRegistry;
class CSipProfileAlrController;
class CUri8;
class MMusSipProfileUser;

/**
*
* @lib musengine.lib
*/
class CMusSipProfileHandler : public CBase,
                              public MSIPProfileRegistryObserver,
                              public MSIPObserver,
                              public MSIPConnectionObserver,
                              public MSipProfileAlrObserver
                              
    {
    MUS_UNITTEST( UT_CMusEngSipProfileHandler )
    
    public:

		/**
        * Creates new instance of CMusSipProfileHandler
        * @param aSipProfileId profile id of requested profile
        */
        static CMusSipProfileHandler* NewL( MMusSipProfileUser& aUser );

        /**
        * Destructor
        *
        */
        ~CMusSipProfileHandler();


    public: // API

		/**
        * Creates the SIP profile used.
        * @param aSipProfileId profile ID of requested SIP profile
        */
        void CreateProfileL( TUint32 aSipProfileId );
        
        /**
        * Returns the used SIP profile.
        * @pre SipProfileL has been called
        * @return SIP profile or NULL if the SIP profile cannot be found,
        *         The ownership is not transferred.
        */ 
        CSIPProfile* Profile();
        
        /**
        * Returns the registered public user identity of the used SIP profile
        * @pre SipProfileL has been called
        * @return a pointer to the URI, the ownership is transferred 
        */       
        CUri8* UserFromProfileLC();

        /**
        * Refreshes the availabilities of the access points 
        * for the used SIP profile.
        * Should be called when a MuS session terminates.
        */
        void RefreshIapAvailabilities();

    private:

        /**
        * Constructor
        *
        */
        CMusSipProfileHandler( MMusSipProfileUser& aUser );

        /**
        * Second-phase constructor
        *
        */
        void ConstructL();


    private: // From MSIPProfileRegistryObserver

        /**
        * An event related to SIP Profile has accorred
        *
        * @param aProfileId a profile Id
        * @param aEvent an occurred event
        **/
        void ProfileRegistryEventOccurred( 
            TUint32 aProfileId,
            MSIPProfileRegistryObserver::TEvent aEvent );

        /**
        * An asynchronous error has occurred related to SIP profile
        * Event is send to those observers, who have the
        * corresponding profile instantiated.
        *
        * @param aProfileId the id of failed profile
        * @param aError an occurred error
        */
        void ProfileRegistryErrorOccurred( 
            TUint32 aProfileId,
            TInt aError );

	private: // From MSIPObserver
 	
        void IncomingRequest( TUint32 aIapId,
                              CSIPServerTransaction* aTransaction );

        void TimedOut( CSIPServerTransaction& aTransaction );

	private: // MSIPConnectionObserver must be implemented to enable SIP profile

        void IncomingRequest (CSIPServerTransaction* aTransaction );

        void IncomingRequest (CSIPServerTransaction* aTransaction,
                              CSIPDialog& aDialog );

        void IncomingResponse (CSIPClientTransaction& aTransaction );

        void IncomingResponse ( CSIPClientTransaction& aTransaction,
                                CSIPDialogAssocBase& aDialogAssoc );

        void IncomingResponse ( CSIPClientTransaction& aTransaction,
                                CSIPInviteDialogAssoc* aDialogAssoc );

        void IncomingResponse( CSIPClientTransaction& aTransaction,
                               CSIPRegistrationBinding& aRegistration );

        void ErrorOccured( TInt aError, CSIPTransactionBase& aTransaction );

        void ErrorOccured( TInt aError,
                           CSIPClientTransaction& aTransaction,
                           CSIPRegistrationBinding& aRegistration );

        void ErrorOccured( TInt aError,
                           CSIPTransactionBase& aTransaction,
                           CSIPDialogAssocBase& aDialogAssoc );

        void ErrorOccured( TInt aError, CSIPRefresh& aSIPRefresh );

        void ErrorOccured( TInt aError,
                           CSIPRegistrationBinding& aRegistration );

        void ErrorOccured( TInt aError, CSIPDialogAssocBase& aDialogAssoc );

        void InviteCompleted( CSIPClientTransaction& aTransaction );

        void InviteCanceled( CSIPServerTransaction& aTransaction );

        void ConnectionStateChanged( CSIPConnection::TState aState );

	private: // From MSipProfileAlrObserver
        
        void AlrEvent( MSipProfileAlrObserver::TEvent aEvent,
                       TUint32 aProfileId,
                       TUint32 aSnapId,
                       TUint32 aIapId );

        void AlrError( TInt aError,
                       TUint32 aProfileId,
                       TUint32 aSnapId,
                       TUint32 aIapId );
                       
	private: // New functions
        
        TUint32 ProfileId() const;

    private: // DATA
    
    	MMusSipProfileUser& iUser;
        CSIP* iSip;
        CSIPProfileRegistry* iProfileRegistry;
        CSipProfileAlrController* iProfileAlrController;
        CSIPProfile* iSipProfile;
    };

#endif // MUSSIPPROFILEHANDLER_H
