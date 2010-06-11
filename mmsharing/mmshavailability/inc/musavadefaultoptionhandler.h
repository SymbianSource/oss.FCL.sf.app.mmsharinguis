/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provide interface for the client requestin network availability.
*
*/



#ifndef __MUSAVADEFAULTOPTIONEHANDLER_H__
#define __MUSAVADEFAULTOPTIONEHANDLER_H__


#include "musavaavailability.h"
#include "musavaobserver.h"
#include "musavainviteresponderobserver.h"
#include "musavasipconnectionadapter.h"
#include "musavasipadapter.h"
#include "mussettingskeys.h"
#include "musunittesting.h"
#include <e32base.h>
#include <sipstrconsts.h>


class CSIPServerTransaction;
class CMusAvaInviteResponder;
class CMusAvaSharedObject;
class CSIPResponseElements;
class MMusAvaAvailabilityObserver;
class CSIPResponse;
class CSdpDocument;
class CSdpOriginField;
class RStringF;
class CSdpMediaField;
class CSdpFmtAttributeField;
class CMusAvaSettingsImp;


/**
 *  Implements handling of non-desired SIP invites.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaDefaultOptionHandler : public CMusAvaAvailability,
                             public MMusAvaSipConnectionAdapter,
                             public MMusAvaSipAdapter
	{
	public:
	/**
     * Two-phased constructor
     *
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     * @return Returns pointer to CMusAvaInterface object
     */

	static CMusAvaDefaultOptionHandler* NewL(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aConcreteSettings );

	/**
     * Two-phased constructor
     *
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     * @return Returns pointer to CMusAvaInterface object
     */

	static CMusAvaDefaultOptionHandler* NewLC(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aConcreteSettings );

	/**
     * Destructor.
     */
	~CMusAvaDefaultOptionHandler();

private:

	/**
     * Handles incoming Option request
     *
     * @param aTransaction Pointer to CSIPServerTransaction
     */
    void IncomingOptionRequestL( CSIPServerTransaction* aTransaction );

    /**
     * Send Response to reques.
     *
     * @param aResponseElements Pointer to CSIPResponseElements
     * @param aTransaction Pointer to CSIPServerTransaction
     */
    void ResponseL( CSIPResponseElements* aResponseElements,
                    CSIPServerTransaction* aTransaction );

   /**
     * Send No capability response to request.
     *
     * @param aTransaction Reference to CSIPServerTransaction
	 * @param aStatusCode a known SIP response status code
	 * @param aReason reason string
     */
    void NoCapabilityResponseL( CSIPServerTransaction* aTransaction,
                                TUint aStatusCode,
                                SipStrConsts::TStrings aReason );

   /**
    * Create CSIPResponseElements for the OPTION response.
    *
    * @param aTransaction Reference to CSIPServerTransaction
    * @return CSIPResponseElements is returned,
    */
	CSIPResponseElements* CreateResponseL( 
	                                CSIPServerTransaction& aTransaction );

   /**
    * Create Sdp for the OPTION response.
    *
    * @param aTransaction Pointer to CSIPServerTransaction
    * @return CSdpDocument is returned,
    */
	CSdpDocument* CreateSDPL( CSIPServerTransaction& aTransaction  );

   /**
    * Create Origin Field for SDP.
    *
    * @param aLocalHost Reference to IP address
    * @param aHostType Reference to IP address type
    * @return CSdpOriginField is returned.
    */
	CSdpOriginField* CreateOriginFieldL(
								const TDesC8& aLocalHost,
								const RStringF& aHostType) const;
   /**
    * Creates SDP string to pool
    *
    * @param aString the string as descriptor
    * @return SDP String
    */
    static RStringF SDPStringL( const TDesC8& aString );

   /**
    * Creates SDP string to pool
    *
    * @param aStringIndex index for SDP strings
    * @return SDP String
    */
    static RStringF SDPStringL( TInt aStringIndex );

   /**
    * Resolves local host
    *
    * @param aLocalHost IP address to be set
    * @param aHostType IP address type to be set
    * @return feature set
    */
    static void LocalHostL( const CSIPServerTransaction& aRequest,
                            TPtrC8& aLocalHost,
                            RStringF& aHostType);

 	/**
     * Creates media line
     *
     * @param aMedia name of the media
     * @param aFormatList format list
     * @return media line
     */
    CSdpMediaField* MediaLineLC( const RStringF& aMedia,
                                 const TDesC8& aFormatList ) const;


    /**
     * Creates rtpmap line
     *
     * @param aCodec name of coded
     * @param aPayloadType payload type
     * @param aClockRate clockrate
     * @param aParams optional params
     * @return rtpmap line
     */
    CSdpFmtAttributeField* RtpMapLineLC( const TDesC8& aCodec,
                                 const TUint64 aPayloadType,
                                 const TUint64 aClockRate,
                                 const TDesC8& aParams = KNullDesC8 ) const;

    /**
     * Helper to transform SDP document to text
     *
     * @param aSdpContent SDP document
     * @return SDP document as text
     */
    static HBufC8* ToTextL( CSdpDocument& aSdpContent );




public: //From MMusAvaSipConnectionAdapter

   /**
	* A SIP request outside a dialog has been received from the network.
    *
    * @pre aTransaction != 0
	* @param aTransaction SIP server transaction. The ownership is
    *   transferred.
    */
	virtual TInt IncomingRequest (CSIPServerTransaction* aTransaction);

public: //From MMusAvaSipAdapter

   /**
    * A SIP request has been received from the network.
    * This function is called when the SIP request was received using such
    * an IAP, for which the application has not created a CSIPConnection
    * object.
    * @pre aTransaction != 0
    * @param aIapId The IapId from which the SIP request was received.
    * @param aTransaction contains local address, remote address of a SIP
    *        message, as well as optional SIP message method, headers and
    *        body. The ownership is transferred.
    */
    virtual TInt IncomingRequest(TUint32 aIapId,
                                 CSIPServerTransaction* aTransaction);

   /**
    * The received SIP request time-outed and it is invalid i.e. cannot be used
    * anymore.
    * This will be called if the user fails to create a SIP connection and
    * does not send an appropriate SIP response.
    * @param aTransaction The time-outed transaction.
    */
    virtual TInt TimedOut(CSIPServerTransaction& aTransaction);

protected:

	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     * @return
     */

	CMusAvaDefaultOptionHandler(
	    MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aConcreteSettings );



	/**
     * Perform the second phase construction of a
     *             CMusAvaDefaultOptionHandler object
     *
     * @since S60 v3.2
     * @param
     * @return
     */

	void ConstructL();



// from base class CMusAvaAvailability

public:

    /**
     * Executes for the availability.
     *
     * @since S60 v3.2
     * @return KErrNone if ExecutePatternL operation was
     *         successfully executed; system wide error otherwise
     */
    virtual void DoExecuteL();


    /**
     * Return availability name.
     *
     * @since S60 v3.2
     * @param
     * @return TAvailabilityName is returned
     */
    virtual MMusAvaObserver::TAvailabilityName Name();


private:

    /**
     * Reference to availability settings (data store)
     */
    CMusAvaSettingsImp& iSettings;

	/**
     * Pointer to CMusAvaSharedObject object
     */
	CMusAvaSharedObject* iSharedObj;
	
	/**
	 * Operator variant indicator
	 */
    MusSettingsKeys::TOperatorVariant iOperatorVariant;	 

	MUS_UNITTEST ( UT_CMusAvaDefaultOptionHandler )
    };


#endif // __MUSAVADEFAULTOPTIONEHANDLER_H__