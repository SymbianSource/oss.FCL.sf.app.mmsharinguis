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
* Description:  This observer is used by
*                ECOM clients to get function call from the availability Plug-in
*
*/



#ifndef MMUSAVAOBSERVER
#define MMUSAVAOBSERVER

#include <e32def.h>


/**
 *  MusAvailability Observer base class
 *
 *  Observer interface for ECOM clients.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class MMusAvaObserver
    {

public:

    /**  Status definition */
    enum TAvailabilityStatus
        {

        /** Availability Error States */
        
        /** Manual Activation*/
		EMusAvaManualActivation	        = -13, 
        /** Unsuccessfull response with response code 403 */
        EMusAvaFailureCode              = -12,

        /** SIP options query not available. */
        EMusAvaOptionNotAvailable       = -11,

        /** No SIP profile. */
        EMusAvaStatusNoSipProfile       = -10,

        /** Not Registered / Registering failed. */
        EMusAvaStatusNotRegistered      = -9,

        /** No Call. */
        EMusAvaStatusNoCall             = -8,

        /** Call on hold. */
        EMusAvaStatusCallOnHold         = -7,

        /** Call on hold. */
        EMusAvaStatusConferenceCall     = -6,

        /** Connection error. */
        EMusAvaConnectionErr            = -5,

        /** User not allow roaming. */
        EMusAvaRoamingError             = -4,

        /** User not allow to start application. */
        EMusActivationError             = -3,

        /** SIP Options returned a negative/erroneous result */
        EMusAvaNegativeSipOptions       = -2,

        /** Network bearer type is insuitable. */
        EMusAvaNetworkType              = -1,

        /** Availability States */

        /** No availability query is made. */
        EMusAvaStatusNotExecuted       =  0,
        EMusAvaStatusInProgress        =  1,
        EMusAvaStatusAvailable         =  2,
        /** options query pending */
		EMusAvaStatusOptionsSent	   =  3,
		/** options query is not sent */
		EMusAvaStatusOptionsNotSent	   =  4,
        /** Edge/DTM is configured but unsupported */
		EMusAvaEdgeDtmStatusUnknown       = 5, 
		
        };

     /**  Status definition */
    enum TAvailabilityName
        {
        /** No defined.*/
        EMusAvaNameNotDefined,

        // Always on class availabilities ->

        /** Invite handler.*/
        EMusAvaInviteHandler,

        // Minimum class availabilities ->

        /** Default Option handler.*/
        EMusAvaDefaultOptionHandler,

        /** Settings Availability */
        EMusAvaNameSetting,
        
        /** Sipprofile must exist */
        EMusAvaNameSipprofileExist,

        /** NetworkStatus.*/
        EMusAvaNameNetworkStatus,

        /** ContactStatus.*/
        EMusAvaNameContactStatus,

        /** Bearer Status.*/
        EMusAvaBearerStatus,

        // Mandatory class availabilities ->

        /** Registration.*/
        EMusAvaNameRegistration,

        // Extension class availabilities ->

        /** Option handler.*/
        EMusAvaOptionHandler,

        // Full availability ->

        /** Full availability, all availabilities go */
        EMusAvaFullAvailability
        };

public:

    /**
     * Availability report
     *
     * @since  S60 v3.2
     * @param aStatus New status for availability.
     */
    virtual void AvailabilityChanged(
        TAvailabilityName aName,
        TAvailabilityStatus aStatus ) = 0;

    virtual void AvailabilityError(
        TAvailabilityName aName,
        TAvailabilityStatus aStatus ) = 0;
    
    virtual void AvailabilitiesAbleToShowIndicator() = 0;
        
    };


#endif //MMUSAVAOBSERVER
