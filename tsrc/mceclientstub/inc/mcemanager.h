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


#ifndef CMCEMANAGER_H
#define CMCEMANAGER_H

// INCLUDES
#include <e32base.h> 
#include <badesca.h>  
//#include <mcedefs.h>

// FORWARD DECLARATIONS
class MMceInSessionObserver;
class MMceInEventObserver;
class MMceInReferObserver;
class MMceSessionObserver;
class MMceEventObserver;
class MMceReferObserver;
class MMceStreamObserver;
class MMceRtpObserver;
class CMceSession;
class CMceEvent;
class CMceRefer;
class TMceMediaId;
class CMceAudioCodec;
class CMceVideoCodec;
class TMceTransactionDataContainer;

// CLASS DECLARATION

/**
*  Class for connecting to MCE server.
*
*  This class provides the connection to the MCE server and
*  provides functions for setting observers for asynchronous
*  events.
* 
*  The user should create only one instance of this class.
*
* @lib mceclient.lib
*/
class CMceManager : public CBase
	{

	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    * @param aAppUid, uid of the application.
	    * @param aContainer, if set, detailed data provided in observers.
	    */
		IMPORT_C static CMceManager* NewL(
	                            	const TUid& aAppUid, 
	                            	TMceTransactionDataContainer* aContainer = 0);

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceManager();

	public: // Functions

	    /**
	    * Set observer for incoming sessions.
	    * @param aInSessionObserver, observer of the sessions.
	    */
		IMPORT_C void SetInSessionObserver( 
		             MMceInSessionObserver* aInSessionObserver );
		             
	    /**
	    * Set observer for incoming events.
	    * @param aInEventObserver, observer of the events.
	    */
		IMPORT_C void SetInEventObserver( 
		             MMceInEventObserver* aInEventObserver );
		             
	    /**
	    * Set observer for incoming refers.
	    * @param aInReferObserver, observer of the refers.
	    */
		IMPORT_C void SetInReferObserver( 
		             MMceInReferObserver* aInReferObserver );
		             
	    /**
	    * Set observer for sessions.
	    * @param aSessionObserver, observer of the sessions.
	    */
		IMPORT_C void SetSessionObserver( 
		             MMceSessionObserver* aSessionObserver );

	    /**
	    * Set observer for medias
	    * @param aMediaObserver, observer of medias.
	    */
		IMPORT_C void SetMediaObserver( 
		             MMceStreamObserver* aMediaObserver );

	    /**
	    * Set observer for rtp
	    * @param aRtpObserver, observer of rtp
	    */
		IMPORT_C void SetRtpObserver( 
		             MMceRtpObserver* aRtpObserver );

        /**
        * Sets observer for incoming events.
        * @param aEventObserver, observer of the event. Ownership is
		*	     not transferred.
        */
		IMPORT_C void SetEventObserver(
					 MMceEventObserver* aEventObserver);
					 
        /**
        * Sets observer for incoming REFER events.
        * @param aReferObserver, observer of the event. Ownership is
		*	     not transferred.
        */
		IMPORT_C void SetReferObserver(
					 MMceReferObserver* aReferObserver);
		
	    /**
	    * Gets currently ongoing sessions
	    * @return RPointerArray array of sessions.
	    */
		IMPORT_C const RPointerArray< CMceSession >& Sessions() const;

        /**
        * Gets currently ongoing events
        * @return RPointerArray array of events.
        */
		IMPORT_C const RPointerArray< CMceEvent >& Events() const;

        /**
        * Sets observer for incoming SUBSCRIBE events.
        * @param aObserver, observer of the event. Ownership is
		*	     not transferred.
        */
		IMPORT_C const RPointerArray< CMceRefer >& Refers() const;
						
	    /**
	    * Returns supported audio codecs of the system.
	    * @return supported audio codecs of the system.
	    */
	    IMPORT_C const RPointerArray<const CMceAudioCodec>& SupportedAudioCodecs() const;
	    
	    /**
	    * Returns supported video codecs of the system.
	    * @return supported video codecs of the system.
	    */
	    IMPORT_C const RPointerArray<const CMceVideoCodec>& SupportedVideoCodecs() const;
	    
	    /*
		* Set default headers for the method. The headers will always be added
		* to the given SIP methods unless overwritten with
		* headers given as parameter for the API method triggering the 
		* sending of request/response.
		* @param aMethods, the SIP methods the headers are added to.
		* @param aHeaders, the default headers, ownership is transferred.
		*/
		IMPORT_C void SetDefaultHeadersL( CDesC8Array* aMethods, CDesC8Array* aHeaders );
        
        /*
        * Get the default headers assigned for the given method.
        * If none is assigned, and empty array is returned.
        * @param aMethod, the method for which the headers are queried.
        * @return the default headers, ownership is transfered.
        */	    
	    IMPORT_C CDesC8Array* DefaultHeadersL( const TDesC8& aMethod ) const;

	    
	public: // Functions

	    /**
	    * Gets observer for incoming sessions
	    * @return incoming session observer
	    */
	    MMceInSessionObserver* InSessionObserver() const;
	    
	    /**
	    * Gets session observer
	    * @return session observer
	    */
	    MMceSessionObserver* SessionObserver() const;

	    /**
	    * Gets media observer
	    * @return media observer
	    */
	    MMceStreamObserver* MediaObserver() const;

	    /**
	    * Gets rtp observer
	    * @return rtp observer
	    */
	    MMceRtpObserver* RtpObserver() const;

	    	    
        /**
        * Returs transaction container for detailed data. 
        * Returns NULL, if none exists.
        */
		TMceTransactionDataContainer* TransactionContainer() const;        
	    
	private:

	    /**
	    * C++ default constructor.
	    * @param aAppUid, uid of the application.
	    * @param aContainer, if set, detailed data provided in observers.
	    */
	    CMceManager( const TUid& aAppUid, 
	                 TMceTransactionDataContainer* aContainer = 0);

	    /**
	     * By default Symbian 2nd phase constructor is private.
	     */
	    void ConstructL();	
	    
        
	public: // Stub data

	    /**
	     * detailed callbacks wanted
	     */
	    TBool iDetailedCallbacks;    
		
	    /**
	     * sessions
	     */
	    RPointerArray<CMceSession> iSessions;
        
	    /**
	     * supported audio codecs
	     */
	    RPointerArray<const CMceAudioCodec> iSupportedAudioCodecs;

        /**
	     * supported audio codecs
	     */
	    RPointerArray<const CMceVideoCodec> iSupportedVideoCodecs;
	
    	MMceSessionObserver* iSessionObserver;
	    MMceStreamObserver* iMediaObserver;
	    MMceRtpObserver* iRtpObserver;
	    MMceInSessionObserver* iInSessionObserver;
	    
        TMceTransactionDataContainer* iContainer;
          
	    friend class CMceSession;
	};

#endif 
