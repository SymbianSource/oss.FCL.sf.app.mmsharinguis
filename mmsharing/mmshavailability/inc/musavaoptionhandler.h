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
* Description:  Provide interface for the client request in capabilty
*                availability.
*
*/

#ifndef __MUSAVAOPTIONHANDLER_H__
#define __MUSAVAOPTIONHANDLER_H__

#include <e32base.h>
#include <badesca.h>
#include "musavaavailability.h"
#include "musavaobserver.h"
#include "musunittesting.h"
#include "mmusavacapabilityqueryobserver.h"

class CMusAvaSharedObject;
class MMusAvaAvailabilityObserver;
class CMusAvaCapabilitySipAgent;
class CMusAvaCapabilityExchange;
class CMusAvaCapability;
class CMusAvaCapabilityQueryBase;
class CMusAvaSettingsImp;


/**
 *  Implements handling of non-desired SIP invites.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaOptionHandler : public CMusAvaAvailability,
                             public MMusAvaCapabilityQueryObserver
	{
public:
	/**
     * Two-phased constructor
     *
     * @param aObserver Pointer to observer intercase
     * @param aSettings Availability settings.
     * @return Returns pointer to CMusAvaInterface object
     */
    static CMusAvaOptionHandler* NewL(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );

	/**
     * Two-phased constructor
     *
     * @param aObserver Pointer to observer intercase
     * @param aSettings Availability settings.
     * @return Returns pointer to CMusAvaInterface object
     */
	static CMusAvaOptionHandler* NewLC(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );

	/**
     * Destructor
     */
	~CMusAvaOptionHandler();

protected:

	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     */

	CMusAvaOptionHandler(
	    MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );

	void ConstructL();

    void ConstructCapabilityExchangeL();

    HBufC8* ConstructTerminalIdL();

public: // from base class CMusAvaAvailability

    /**
     * Executes for the availability.
     *
     * @since S60 v3.2
     * @return KErrNone if ExecutePatternL operation was
     *         successfully executed; system wide error otherwise
     */
    virtual void DoExecuteL();


    /**
     * Stop the execution.
     *
     * @since S60 v3.2
     * @return KErrNone if stop operation was successfully executed;
     *         system wide error otherwise
     */
    virtual void Stop();

    /**
     * From CMusAvaAvailability.
     * Returns availability name.
     *
     * @return Name of *this* availability.
     */
    virtual MMusAvaObserver::TAvailabilityName Name();
    

public: // functions from base class MMusAvaCapabilityQueryObserver

    void CapabilitiesResolved( const CMusAvaCapabilityQueryBase& aSentQuery );

    void CapabilitiesResolvedL( const TDesC& aUri );

    void SetCapabilitiesResolvedForCingular();

    TBool CapabilitiesResolvedForCingular();   
    
    /**
     * This is to advertise MMusAvaCapabilityQueryObserver implementors
     * about sip header change. It is mainly used in below scneraio but could
     * be used in future if anyother sip headers has to be set.
     * 
     * Incase if we receive OPTIONS request already and 
     * P-Asserted-Identity Header is present then OPTIONS
     * request should be sent to this ID. This function should be
     * called to configure this id in CMusAvaSettingsImp& iSettings
     *
     * @aHeaders Array of SIP Headers.    
     * @aHeaderType Type of the header.
     * Leavs if Array is empty or pointer is NULL.
     * Ownership Transferred.
     */
    void SipHeadersL(const MDesCArray& aHeaders,
                          SipStrConsts::TStrings aHeaderType);

    /**
     * Advertise about video codec capabilities     
     */
    void VideoCodecsResolvedL( const MDesCArray& aVideoCodecs );


    TBool CapabilityQueryAnswered( TBool aAnswered );

private:

    /**
     * Reference to availability settings (data store)
     */
    CMusAvaSettingsImp&         iSettings;

	/**
     * Pointer to CMusAvaSharedObject object
     */
	CMusAvaSharedObject*        iSharedObj;

    CMusAvaCapabilitySipAgent*  iSipAgent;
    CMusAvaCapabilityExchange*  iCapabilityExchange;
    CMusAvaCapability*          iSwisCapability;
    TBool                       iCapabilitiesRequestAnswered;
    TBool                       iCapabilityQueryAnswered;
    
    MUS_UNITTEST ( UT_CMusAvaOptionHandler )
    MUS_UNITTEST ( UT_CMusAvaCapabilityExchange )
    MUS_UNITTEST ( UT_CMusAvaCapability )
    MUS_UNITTEST ( UT_CMusAvaTerminal )
    MUS_UNITTEST ( UT_CMusAvaCapabilitySipAgent )
    MUS_UNITTEST ( UT_CMusAvaCapabilityQuery )
    };

#endif // __MUSAVAOPTIONHANDLER_H__