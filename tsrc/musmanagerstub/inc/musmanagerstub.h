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

#include <e32base.h>
#include <sipstrconsts.h> 



class CMusManagerImpl;

#undef IMPORT_C
#define IMPORT_C
/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
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
    
public: // first phase constructors

    IMPORT_C static CMusManager* NewL();

    IMPORT_C static CMusManager* NewLC();
    
    virtual ~CMusManager();


public: // new functions
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
     * This function allows clients to monitor the availability
     * status of MultimediaSharing. 
     *
     * The request will return when the availability status changes.
     *
     * @since S60 v3.2
     * @param aStatus TRequestStatus object of the listener.
     * @param aAvailabilityStatus Contains the new status, when request complete.
     */
    IMPORT_C void MonitorAvailabilityL( 
                    TRequestStatus& aRequestStatus,
                    MultimediaSharing::TMusAvailabilityStatus& aAvailabilityStatus );
    
    /**
     * Cancels the currently active availability monitoring.
     *
     * @since S60 v3.2
     */
    IMPORT_C void CancelMonitoring();
    
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
     * @since S60 v3.2
     */
    IMPORT_C void StopApplicationL();

private: // second phase constructors

    CMusManager();

    void ConstructL();


public: // new functions for testing purposes

    IMPORT_C static void SetStartError( TInt aError );
    
    IMPORT_C static void SetAvailability( TInt aAvailability );
    
    IMPORT_C static void SetUseCase( MultimediaSharing::TMusUseCase aUseCase );
    
    IMPORT_C static MultimediaSharing::TMusUseCase UseCase();
    
public: // data

    MultimediaSharing::TMusAvailabilityStatus iAvailability;
    
    TInt iExamineLeaveCode;
    
    TInt iStartError;
    
    TRequestStatus* iStatus;
    
    TRequestType iHandleSipRequestLCalledWithParam;
    
    };

#endif // ? C_CLASSNAME_H
