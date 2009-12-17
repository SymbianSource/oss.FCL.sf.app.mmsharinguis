/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MUSMANAGERCLIENTSESSION_H
#define MUSMANAGERCLIENTSESSION_H

#include <e32std.h>

#include "musunittesting.h"
#include "musmanagercommon.h"

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class RMusManagerClientSession 
    : public RSessionBase
    {
    
    MUS_UNITTEST( UT_RMusManagerClientSession )
    
public: // constructor

    RMusManagerClientSession();

public: // new functions

    void ConnectL();
    
    void Disconnect();    

    void InvestigateAvailabilityL();

    MultimediaSharing::TMusAvailabilityStatus QueryAvailabilityL();
    
    void StartL( MultimediaSharing::TMusUseCase aUseCase );
    
    void StartObservingAvailabilityL();
    
    void StopL();
    
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
    void HandleCommandL( MultimediaSharing::TCommandType aCommandType );

    void HandleSipRequestL( TInt aUid );

    void MonitorAvailabilityL( 
                TRequestStatus& aRequestStatus,
                TIpcArgs& aIpcArgs );

	void CancelMonitoringL();

private: // data

    TVersion Version() const;
    };



#endif // MUSMANAGERCLIENTSESSION_H
