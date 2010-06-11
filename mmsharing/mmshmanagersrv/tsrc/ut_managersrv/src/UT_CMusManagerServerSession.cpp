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
* Description:  Unit tests for CMusAvailabilityPluginManager class
*
*/


#include "UT_CMusManagerServerSession.h"
#include "musunittesting.h"
#include "musmanagerserversession.h"
#include "musmanagerserver.h"
#include "musmanageripccommon.h"

/*
#include "musavaavailability.h"
#include "musavaavailabilitystub.h"
#include "musavainterface.h"
#include "cmusavainterfacestub.h"
#include "mussessionproperties.h"
*/

#include "mustesthelp.h"
#include <e32property.h>
#include <apgtask.h>
#include <digia/eunit/eunitmacros.h>


void CActiveScheduler::Install(CActiveScheduler* /*aScheduler*/)
    {
    }

void RMessagePtr2::Complete(TInt /*aReason*/) const
    {
    }

void RMessagePtr2::ReadL(TInt /*aParam*/,TDes8& aDes,TInt /*aOffset*/) const
    {
    TUint32 val = 0;
    TPckgBuf<TUint32> valPckg( val );
    aDes.Copy( valPckg );
    }


void RMessagePtr2::WriteL(TInt /*aParam*/,const TDesC8& /*aDes*/,TInt /*aOffset*/) const
    {
    
    }

TInt RMessagePtr2::GetDesMaxLength(TInt /*aParam*/) const
    {
    return 256;
    }

class RMessage2Test : public RMessage2
    {
    public:
    inline RMessage2Test( TInt aFunction );
    inline void SetHandle( TInt aHandle );
    };


inline RMessage2Test::RMessage2Test( TInt aFunction )
    : RMessage2()
    {
    iFunction = aFunction;
    }


inline void RMessage2Test::SetHandle( TInt aHandle )
    {
    iHandle = aHandle;
    }

TInt RMessagePtr2::Client(RThread& /*aClient*/, TOwnerType /*aOwnerType*/) const
    {
    return KErrNone;
    }

void RThread::RequestComplete(TRequestStatus*& /*aStatus*/,TInt /*aReason*/) const
    {
    }

void CServer2::ReStart()
    {
    }
    
// ======== MEMBER FUNCTIONS ========


UT_CMusManagerServerSession* UT_CMusManagerServerSession::NewL()
    {
    UT_CMusManagerServerSession* self = UT_CMusManagerServerSession::NewLC();
    CleanupStack::Pop();
    return self;
    }


UT_CMusManagerServerSession* UT_CMusManagerServerSession::NewLC()
    {
    UT_CMusManagerServerSession* self = new( ELeave ) UT_CMusManagerServerSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusManagerServerSession::~UT_CMusManagerServerSession()
    {
    }


UT_CMusManagerServerSession::UT_CMusManagerServerSession()
    {
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerSession::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }




// ---------------------------------------------------------------------------
// Setups the test by instantiating tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerSession::SetupL()
    {
    iServer = CMusManagerServer::NewL();
    iSession = CMusManagerServerSession::NewL(*iServer, *this );
    
    TVersion version;
    RMessage2 message;    
    iSession2 = iServer->NewSessionL( version, message );
    iObserved = (TObserved)0;
    }


// ---------------------------------------------------------------------------
// Finalizes test by deleting instance of tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerSession::Teardown()
    {
    delete iSession;
    delete iSession2;
    iServer->RunError( KErrNone );
    delete iServer;
    PropertyHelper::Close(); 
    
    }


// ======== TEST METHODS ========


// ---------------------------------------------------------------------------
// Asserts that instance creation is successful.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerSession::UT_CMusManagerServerSession_NewLL()
    {
    EUNIT_ASSERT( iSession );
    }


// ---------------------------------------------------------------------------
// Asserts that instance creation is successful.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerSession::UT_CMusManagerServerSession_NewLCL()
    {
    delete iSession;
    iSession = NULL;
    iSession = CMusManagerServerSession::NewLC( *iServer, *this );
    EUNIT_ASSERT( iSession );
    CleanupStack::Pop( iSession );
    }



// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerSession::
    UT_CMusManagerServerSession_ServiceLL()
    {
    RMessage2Test msg1( EMusManagerIpcAvailabilityQuery );
    RMessage2Test msg2( EMusManagerIpcInvestigateAvailability );
    RMessage2Test msg3( EMusManagerIpcInvitationReceived );
    RMessage2Test msg4( EMusManagerIpcOptionsReceived );
    RMessage2Test msg5( EMusManagerIpcStartMultimediaSharing );
    RMessage2Test msg6( EMusManagerIpcStopMultimediaSharing );
    RMessage2Test msg6_1( EMusManagerIpcStartObservingAvailability );
    RMessage2Test msg7( EMusManagerIpcMonitorAvailability );
    RMessage2Test msg8( EMusManagerIpcCancelMonitoring );
    RMessage2Test msg9( EMusManagerIpcHandleCommand );
    
    
    iSession->ServiceL( msg7 );
    EUNIT_ASSERT( iObserved == (TObserved)0 );
    iObserved = (TObserved)0;
    iSession->ServiceL( msg1 );
    EUNIT_ASSERT( iObserved == EAvailabilityQueryL );
    iObserved = (TObserved)0;
    iSession->ServiceL( msg2 );
    EUNIT_ASSERT( iObserved == EInvestigateAvailabilityL );
    iObserved = (TObserved)0;
    iSession->ServiceL( msg3 );
    EUNIT_ASSERT( iObserved == EInvitationReceivedL );
    iObserved = (TObserved)0;
    iSession->ServiceL( msg4 );
    EUNIT_ASSERT( iObserved == EOptionsReceivedL );
    iObserved = (TObserved)0;
    iSession->ServiceL( msg5 );
    EUNIT_ASSERT( iObserved == EStartMultimediaSharingL );
    iObserved = (TObserved)0;
    iSession->ServiceL( msg6 );
    EUNIT_ASSERT( iObserved == EStopMultimediaSharingL );
    iObserved = (TObserved)0;
    iSession->ServiceL( msg6_1 );
    EUNIT_ASSERT( iObserved == ERegisterObserverL );
    iObserved = (TObserved)0;
    iSession->ServiceL( msg8 );
    EUNIT_ASSERT( iObserved == ECancelMonitoring );
    iObserved = (TObserved)0;
    iSession->ServiceL( msg9 );
    EUNIT_ASSERT( iObserved == ECommandL );
    iObserved = (TObserved)0;
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerSession::
    UT_CMusManagerServerSession_AvailabilityChangedLL()
    {
    
    iSession->AvailabilityChangedL(
        (MultimediaSharing::TMusAvailabilityStatus) KErrNone );

    RMessage2Test msg7( EMusManagerIpcMonitorAvailability );
    iSession->ServiceL( msg7 );
    
    iSession->AvailabilityChangedL(
        (MultimediaSharing::TMusAvailabilityStatus) KErrNone );
    
    iSession->AvailabilityChangedL(
        (MultimediaSharing::TMusAvailabilityStatus) KErrNone );
    
    msg7.SetHandle( 1 );
    iSession->ServiceL( msg7 );
    
    iSession->AvailabilityChangedL(
        (MultimediaSharing::TMusAvailabilityStatus) KErrNone );

    iSession->ServiceL( msg7 );

    iSession->AvailabilityChangedL(
        (MultimediaSharing::TMusAvailabilityStatus) KErrNone );

    }




// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusManagerServerSession,
    "CMusManagerServerSession",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusManagerServerSession",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerSession_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CMusManagerServerSession",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerSession_NewLCL, Teardown)

EUNIT_TEST(
    "ServiceL - test ",
    "CMusManagerServerSession",
    "ServiceL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerSession_ServiceLL, Teardown)

EUNIT_TEST(
    "AvailabilityChangedL - test ",
    "CMusManagerServerSession",
    "AvailabilityChangedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerSession_AvailabilityChangedLL, Teardown)



EUNIT_END_TEST_TABLE
