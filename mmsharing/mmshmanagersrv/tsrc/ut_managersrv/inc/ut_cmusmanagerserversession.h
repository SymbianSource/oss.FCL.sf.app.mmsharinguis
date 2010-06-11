/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit tests for CMusManagerServerSession class.
*
*/


#ifndef UT_CMUSMANAGERSERVERSESSION_H
#define UT_CMUSMANAGERSERVERSESSION_H

#include "musmanagerserversessionobserver.h"
#include <digia/eunit/ceunittestsuiteclass.h>

class CMusManagerServerSession;
class CMusManagerServer;
class CSession2;
class MMusMonitorAvailabilityObserver;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

/**
 *  Unit test class for CMusManagerServerSession.
 *  Implements unit tests for CMusManagerServerSession.
 *
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( UT_CMusManagerServerSession ) : public CEUnitTestSuiteClass,
    MMusManagerServerSessionObserver
    {
public:
    
    enum TObserved
        {
        
        EAvailabilityQueryL = 1,
        ECommandL,
        EInvestigateAvailabilityL,
        EInvitationReceivedL,
        EOptionsReceivedL,
        EStartMultimediaSharingL,
        EStopMultimediaSharingL,
        ERegisterObserverL,
        ERemoveObserver,
        ECancelMonitoring
        };

    static UT_CMusManagerServerSession* NewL();

    static UT_CMusManagerServerSession* NewLC();

    /**
     * Destructor.
     */
    ~UT_CMusManagerServerSession();


// from baseclass MMusManagerServerSessionObserver.

    MultimediaSharing::TMusAvailabilityStatus AvailabilityQueryL() 
      { iObserved = EAvailabilityQueryL;
        return MultimediaSharing::EMultimediaSharingAvailable; }
    
    void CommandL( MultimediaSharing::TCommandType /*aCommandType*/ ) 
        { iObserved = ECommandL; }
    
    void InvestigateAvailabilityL()
        { iObserved = EInvestigateAvailabilityL; }
    
    void InvitationReceivedL( TUid /*aChannelId*/ )
        { iObserved = EInvitationReceivedL; }
    
    void OptionsReceivedL( TUid /*aChannelId*/ )
        { iObserved = EOptionsReceivedL; }
    
    void StartMultimediaSharingL( MultimediaSharing::TMusUseCase /*aUseCase*/ )
        { iObserved = EStartMultimediaSharingL; }
    
    void StopMultimediaSharingL()
        { iObserved = EStopMultimediaSharingL; }

    void RegisterObserverL( MMusMonitorAvailabilityObserver* /*aObserver*/ )
        { iObserved = ERegisterObserverL; }
    
    void RemoveObserver( MMusMonitorAvailabilityObserver* /*aObserver*/ )
        { iObserved = ERemoveObserver; }
    
    void CancelMonitoring( MMusMonitorAvailabilityObserver* /*aObserver*/)
        { iObserved = ECancelMonitoring; }

    
    TObserved iObserved;

private:

    UT_CMusManagerServerSession();

    void ConstructL();

    /**
    * Sets up a test case.
    */
    void SetupL();

    /**
    * Tears down a test case.
    */
    void Teardown();

    /**
    * Invidual unit test methods.
    */
    void UT_CMusManagerServerSession_NewLL();
    void UT_CMusManagerServerSession_NewLCL();
    void UT_CMusManagerServerSession_ServiceLL();
    void UT_CMusManagerServerSession_AvailabilityChangedLL();

private: // data

    /**
     * Tested class.
     * Own.
     */
    CMusManagerServerSession* iSession;
    CSession2* iSession2;
    
    CMusManagerServer* iServer;

    EUNIT_DECLARE_TEST_TABLE;
    };

#endif // UT_CMUSMANAGERSERVERSESSION_H
