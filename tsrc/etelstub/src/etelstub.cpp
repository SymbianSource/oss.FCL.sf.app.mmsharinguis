/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  implementation
*
*/


//  INCLUDE FILES

#include "etelmm.h"

static TBool iEmergency = EFalse;
static RMobileCall::TMobileCallEvent iCallEvent = RMobileCall::ELocalBarred;
static RMobileConferenceCall::TMobileConferenceStatus iConfStatus = RMobileConferenceCall::EConferenceIdle;
static RMobileConferenceCall::TMobileConferenceEvent iConfEvent = RMobileConferenceCall::EConferenceCallAdded;
static RMobileCall::TMobileCallStatus iCallStatus = RMobileCall::EStatusIdle; 
static RMobileCall::TMobileCallDirection iDirection = RMobileCall::EDirectionUnknown;
static TBool iReqToCancel = EFalse;
static RTelSubSessionBase::TCalledFunction iCalledFunction = RTelSubSessionBase::ENone;
static RMobileCall::TMobileCallRemoteIdentityStatus iRemoteIdStatus = RMobileCall::ERemoteIdentityUnknown;


EXPORT_C TInt RTelServer::Connect( int )
    {
    return KErrNone;
    }
    
EXPORT_C TInt RPhone::Open( RTelServer&, const TDesC& )    
    {
    return KErrNone;
    }
  
EXPORT_C int RLine::Open( RPhone&, const TDesC& )  
    {
    return KErrNone;
    }

EXPORT_C RTelServer::RTelServer()
    {
    
    }
    
EXPORT_C RMobilePhone::RMobilePhone()
    {
    
    }
    
EXPORT_C RMobileLine::RMobileLine()
    {
    
    }
    
EXPORT_C RMobileCall::RMobileCall()
    {
    
    }
    
EXPORT_C void RCall::Close()
    {

    }
    
EXPORT_C void RLine::Close()
    {
    
    }
    
EXPORT_C void RPhone::Close()
    {
    
    }

EXPORT_C void RMobileLine::Destruct()
    {
    
    }
    
EXPORT_C void RMobileLine::ConstructL()
    {
    
    }
    
EXPORT_C void RMobileCall::Destruct()
    {
    
    }
    
EXPORT_C void RMobileCall::ConstructL()
    {
    
    }
    
EXPORT_C void RMobilePhone::Destruct()
    {
    
    }
    
EXPORT_C void RMobilePhone::ConstructL()
    {
    
    }
    

EXPORT_C RPhone::RPhone()
    {
    
    }
    
EXPORT_C RLine::RLine()
    {
    
    }
    
EXPORT_C RCall::RCall()
    {
    
    }
    
EXPORT_C RMobilePhone::TMultimodeType::TMultimodeType()
    {
    
    }
    
EXPORT_C RMobileCall::TMobileCallRemotePartyInfoV1::TMobileCallRemotePartyInfoV1()
    {
    
    }
    
EXPORT_C RMobilePhone::TMobileAddress::TMobileAddress()
    {
    
    }
    
EXPORT_C void RPhone::Destruct()
    {
    
    }
    
EXPORT_C void RPhone::ConstructL()
    {
    
    }
    
EXPORT_C void RLine::Destruct()
    {
    
    }
    
EXPORT_C void RLine::ConstructL()
    {
    
    }

EXPORT_C void RCall::Destruct()
    {
    
    }
    
EXPORT_C void RCall::ConstructL()
    {
    
    }
    
EXPORT_C RCall::TCallInfo::TCallInfo() 
    {

    }
    
EXPORT_C TInt RCall::GetInfo( TCallInfo& /*aCallInfo*/ ) const
    {
    return 0;
    }      
    
EXPORT_C RMobileCall::TMobileCallInfoV1::TMobileCallInfoV1()    
    {
    
    }


EXPORT_C RTelSubSessionBase::RTelSubSessionBase()
    {
    iReqStatus = NULL;
    iReqStatus2 = NULL;
    }

EXPORT_C void RMobileCall::NotifyMobileCallStatusChange( TRequestStatus& aReqStatus,
                                                         RMobileCall::TMobileCallStatus& aCallStatus) const
    {
    aCallStatus = iCallStatus;
    aReqStatus = KRequestPending;
    iReqStatus2 = &aReqStatus;
    //User::RequestComplete( &aStatus, KErrNone );
    }                                                         

EXPORT_C void RMobileCall::NotifyCallEvent( TRequestStatus& aReqStatus, 
                                            TMobileCallEvent& aEvent ) const
    {
    
    aEvent = iCallEvent ;
    aReqStatus = KRequestPending;
    iReqStatus = &aReqStatus;
    //User::RequestComplete( &aReqStatus, aEvent );
    }
    
EXPORT_C RMobileCall::TMobileCallInfoV3::TMobileCallInfoV3()
    {
    
    }
    
EXPORT_C TInt RMobileCall::GetMobileCallInfo( TDes8& aBuffer )   const 
    {
    _LIT8( KNumber, "1234" );
    
    RMobileCall::TMobileCallInfoV3 callInfo;
    callInfo.iRemoteParty.iRemoteNumber.iTelNumber.Copy( KNumber() );
    callInfo.iRemoteParty.iDirection = iDirection;
    callInfo.iEmergency = iEmergency;
    callInfo.iRemoteParty.iRemoteIdStatus = iRemoteIdStatus;
    RMobileCall::TMobileCallInfoV3Pckg pckg( callInfo );
    
    aBuffer.Copy( pckg );
    
    return KErrNone;
    }
    
EXPORT_C TInt RCall::OpenExistingCall( class RLine&, const TDesC& )
    {
    return KErrNone;
    };
    
EXPORT_C TInt RMobileCall::GetMobileCallStatus( RMobileCall::TMobileCallStatus& aStatus )    const
    {
    aStatus = iCallStatus; 
    
    return KErrNone;
    }
    
EXPORT_C TInt RLine::EnumerateCall( TInt& aCount )    const 
    {
    aCount = 1;
   
    return KErrNone;
    }
    
EXPORT_C TInt RLine::GetCallInfo( int, RLine::TCallInfo& aInfo )   const  
    {
    _LIT( KCallName, "callname" );
    
    aInfo.iStatus = (RCall::TStatus)iCallStatus;
    aInfo.iCallName = KCallName();
    
    return KErrNone;
    }
    
EXPORT_C void RMobileLine::NotifyMobileLineStatusChange( TRequestStatus& aReqStatus, RMobileCall::TMobileCallStatus& )const
    {
    aReqStatus = KRequestPending;
    iReqStatus = &aReqStatus;
    }
    
EXPORT_C void RTelSubSessionBase::CancelAsyncRequest( TInt aReqToCancel )     const
    {    
    TRequestStatus* reqStatus = aReqToCancel == EMobileCallNotifyMobileCallStatusChange ?
                                iReqStatus2 : iReqStatus;
    
    iCalledFunction = RTelSubSessionBase::ECancelAsyncRequest; 
    if ( reqStatus )
        {
        User::RequestComplete( reqStatus, KErrCancel );
        }
    if ( aReqToCancel == EMobileCallNotifyMobileCallStatusChange )
        {
        iReqStatus2 = NULL;
        }
    else
        {
        iReqStatus = NULL;
        }
    
    }
    
EXPORT_C TInt RMobileConferenceCall::Open( RMobilePhone&)    
    {
    return KErrNone;
    }
    
EXPORT_C RMobileConferenceCall::RMobileConferenceCall()
    {
    
    }
    
EXPORT_C void RMobileConferenceCall::Close()
    {
    
    }
    
EXPORT_C TInt RMobileConferenceCall::GetConferenceStatus( RMobileConferenceCall::TMobileConferenceStatus& /*aStatus*/ ) const
    {    
    return KErrNone;
    }
    
EXPORT_C void RMobileConferenceCall::NotifyConferenceStatusChange( TRequestStatus& aReqStatus, 
                         RMobileConferenceCall::TMobileConferenceStatus& aConfStatus ) const
    {    
    aConfStatus = iConfStatus;
    aReqStatus = KRequestPending;
    iReqStatus = &aReqStatus;

    //User::RequestComplete( &aReqStatus, KErrNone );
    }
    

EXPORT_C void RMobileConferenceCall::NotifyConferenceEvent(TRequestStatus& aReqStatus,
                       TMobileConferenceEvent& aEvent, TName& /*aCallName*/) const
    {
    aEvent = iConfEvent;
    aReqStatus = KRequestPending;
    iReqStatus = &aReqStatus;
    //User::RequestComplete( &aReqStatus, KErrNone );
    
    }


/*
 * Helper function to retrive static iCalledFunction variable
 */
RTelSubSessionBase::TCalledFunction RTelHelper::GetCalledFunction()
    {
    return iCalledFunction;
    }

void RTelHelper::SetCallStatus( RMobileCall::TMobileCallStatus aCallStatus )
    {
    iCallStatus = aCallStatus;
    }

void RTelHelper::SetCallEvent( RMobileCall::TMobileCallEvent aCallEvent )
    {
    iCallEvent = aCallEvent;
    }
void RTelHelper::SetConfEvent( RMobileConferenceCall::TMobileConferenceEvent aConfEvent )
    {
    iConfEvent = aConfEvent;
    }
void RTelHelper::SetConfStatus( RMobileConferenceCall::TMobileConferenceStatus aConfStatus )
    {
    iConfStatus = aConfStatus;
    }
void RTelHelper::SetCallDirection( RMobileCall::TMobileCallDirection aDirection )
    {
    iDirection = aDirection;
    }
void RTelHelper::SetCallEmergency( TBool aVal )
    {
    iEmergency = aVal;
    }
void RTelHelper::SetRemoteIdStatus( RMobileCall::TMobileCallRemoteIdentityStatus aStatus )
    {
    iRemoteIdStatus = aStatus;
    }

EXPORT_C void
RMobilePhone::GetIdentityServiceStatus(TRequestStatus& aReqStatus,
	TMobilePhoneIdService /*aService*/,
	TMobilePhoneIdServiceStatus& /*aStatus*/,
	TMobileInfoLocation /*aLocation*/) const
	{
	aReqStatus = KRequestPending;
	}

EXPORT_C void
RMobilePhone::NotifyIdentityServiceStatus(TRequestStatus& aReqStatus,
	const TMobilePhoneIdService /*aService*/,
	TMobilePhoneIdServiceStatus& /*aStatus*/) const
	{
	aReqStatus = KRequestPending;
	}
