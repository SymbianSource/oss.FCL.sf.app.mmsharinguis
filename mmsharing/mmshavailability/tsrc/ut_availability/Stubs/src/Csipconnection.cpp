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
* Description:
*
*/

#include "sipconnection.h"
#include "sip.h"
#include "siperr.h"
#include "musunittesting.h"
#include "SIPRequestElements.h"
#include "SIPClientTransaction.h"
#include "CSipSseTestTls.h"

CSIPConnection::CSIPConnection(CSIP& aSIP,
                               TUint32 aIapId,
                               MSIPConnectionObserver& aObserver)
 : iIapId(aIapId), iObserver(aObserver), iState(EActive)
	{
	iSIP = &aSIP;
	}

void CSIPConnection::ConstructL()
	{
	}


EXPORT_C CSIPConnection* CSIPConnection::NewL(CSIP& aSIP,
									 TUint32 aIapId,
									 MSIPConnectionObserver& aSIPConnectionObserver)
	{

	CSIPConnection* self = new(ELeave) CSIPConnection(aSIP, aIapId, aSIPConnectionObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;

	}


EXPORT_C CSIPConnection::~CSIPConnection()
	{
	}

EXPORT_C TUint32 CSIPConnection::IapId() const
	{
	return iIapId;
	}
	
EXPORT_C CSIPConnection::TState CSIPConnection::State() const
    {
    return iState; 
    }
    
EXPORT_C CSIPClientTransaction* CSIPConnection::SendRequestL(
                 CSIPRequestElements* aElements,
                 const MSIPRegistrationContext& /*aContext*/ )
    {
    CSIPClientTransaction* trx = CSIPClientTransaction::NewL( aElements->Method() );
    
    CSipSseTestTls* tls = CSipSseTestTls::Storage();
    if ( tls )
        {
        tls->ClientRequest( aElements );
        }
    else
        {
        delete aElements;
        }
        
    return trx;
    }

EXPORT_C void CSIPConnection::GetLocalAddrL(TInetAddr& aAddr)const
    {
    _LIT(add,"0.0.0.0");
    aAddr.Input(add);
    }


EXPORT_C void CSIPConnection::SetState(TState aState)
    {
    iState = aState;
    }
    
//TODO:Check if these new functions are needed in sip_stub

// -----------------------------------------------------------------------------
// CSIPConnection::SIP
// -----------------------------------------------------------------------------
//
EXPORT_C CSIP* CSIPConnection::SIP()
	{
	return iSIP;
	}
 
// -----------------------------------------------------------------------------
// CSIPConnection::SIP
// -----------------------------------------------------------------------------
//      
EXPORT_C const CSIP* CSIPConnection::SIP() const
	{
	return iSIP;
	}
