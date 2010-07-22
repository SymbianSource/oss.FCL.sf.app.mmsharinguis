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
* Description:  ?Description
*
*/


#ifndef MUSMANAGER_H
#define MUSMANAGER_H

#include "musmanagercommon.h"
//#include "musunittesting.h"

#include <e32base.h>
#include <sipstrconsts.h> 

class CMusManagerImpl;
class MMusAvailabilityObserver;

/**
 *  Class to manager MultimediaSharing application.
 *
 *  Class CMusManager provides an interface to monitor
 *  the system's ability to initiate a MultimediaSharing
 *  session. Also, MultimediaSharing application can be started
 *  via this interface. Application can be ordered to close 
 *  itself by calling the stop application function.
 *
 *  MultimediaSharing availability is a variable of a system
 *  that describes that is a MultimediaSharing session currently
 *  possible, or not. It depends on call state, network state,
 *  SIP profiles and SIP OPTIONS. 
 *
 *  @lib musmanagerclient.dll
 */
class CMusManager : public CBase
    {
public:

    enum TRequestType
        {
        ESipOptions = 0x1028238D,
        ESipInviteDesired = 0x10282391,
        ESipInviteDesired2WayVideo = 0x10282392,
        ESipInviteNotDesired = 0x1028238D
        };
       

public: // first phase constructors and destructor

    IMPORT_C static CMusManager* NewL();

    IMPORT_C static CMusManager* NewLC();
    
    /**
     * Creates Mus Manager instance allowing the clients to pass 
     * an observer interface through which the clients will be capable 
     * to listen for the availability state of Multimedia Sharing
     *
     * @param aAvailabilityObserver pointer to the implementation of 
     *              availabilty observer interface
     *
     * @since S60 v5.0     
     */
    IMPORT_C static CMusManager* NewL( 
                    MMusAvailabilityObserver* aAvailabilityObserver );

    /**
     * Creates Mus Manager instance allowing the clients to pass 
     * an observer interface through which the clients will be capable 
     * to listen for the availability state of Multimedia Sharing
     * Mus Manager instance is left on cleanup stack.
     *
     * @param aAvailabilityObserver pointer to the implementation of 
     *              availabilty observer interface
     *
     * @since S60 v5.0          
     */
    IMPORT_C static CMusManager* NewLC( 
                    MMusAvailabilityObserver* aAvailabilityObserver );
    
    virtual ~CMusManager();
    
public: // functions related to availability of MultimediaSharing

    /**
     * This function causes following things:
     * - MS manager starts to monitor call and network state
     * - In case of active call, MS manager
     *     - resolves the SIP address of the remote host
     *     - makes a capability query (SIP OPTIONS)
     * 
     * This function must be called before a MultimediaSharing
     * session is tried to be established. Otherwise, the internal state of 
     * MultimediaSharing will remain as undefined. In that state,
     * MultimediaSharing Manager will not start the application.
     *
     * @since S60 v3.2
     */
    IMPORT_C void ExamineAvailabilityL();
    
    /**
     * Returns the current availability status.
     *
     * @since S60 v3.2
     * @return The current availability status.
     */
    IMPORT_C MultimediaSharing::TMusAvailabilityStatus AvailabilityL();
    
public: // functions related to application start-up   
 
 	 /**
     * This function allows clients to request coomand's to be executed by 
     * the MUS manager server.
     *
     * In erroneous cases, this function will leave
     * with an error code that describes the error. Positive leave codes
     * are defined in MultimediaSharing::TMusAvailabilityStatus.
     *
     * @param aRequest Defines the coomand type. 
     *                 
     */
    IMPORT_C void HandleCommandL( MultimediaSharing::TCommandType aCommandType );
 
 
    /**
     * Orders MultimediaSharing subsystem to handle a SIP request. 
     * In some cases, this function call starts the application.
     *
     * In erroneous cases, this function will leave
     * with an error code that describes the error. Positive leave codes
     * are defined in MultimediaSharing::TMusAvailabilityStatus.
     *
     * @param aRequest Defines the request type. 
     *                 Must be SipStrConsts::EOptions or SipStrConsts::EInvite
     */
    IMPORT_C void HandleSipRequestL( TRequestType aRequestType );
    
    /**
     * Starts the MultimediaSharing application
     *
     * If the application can't be started, this function will leave
     * with an error code that describes the error. Positive leave codes
     * are defined in MultimediaSharing::TMusAvailabilityStatus.
     *
     * @param aUseCase Describes the desired use case and 
                       affects the default view.
     */
    IMPORT_C void StartApplicationL( MultimediaSharing::TMusUseCase aUseCase );
    
public: // functions to related application closing
    
    /**
     * This function causes the application to get a stop signal.
     * Application will close itself smoothly.
     *
     */
    IMPORT_C void StopApplicationL();

private: // second phase constructors

    CMusManager();

    void ConstructL();
    
    void ConstructL( MMusAvailabilityObserver* aAvailabilityObserver );

private: // data

    /**
     * The actual implementation of the interface.
     * Own. 
     */
    CMusManagerImpl* iImpl;
    };

#endif // ? MUSMANAGER_H
