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
* Description:  SIP Server Stub
*
*/


#include "CSIPServerStub.h"
#include <sipmessageelements.h>
#include <sipresponseelements.h>
#include <siprequestelements.h>
#include <sipstrings.h>
#include <SipStrConsts.h>


void CSIPServerStub::OpenL ()
	{
    CSIPServerStub* self = new (ELeave) CSIPServerStub();
    self->Reset();
    Dll::SetTls( self );
	}

void CSIPServerStub::Close ()
	{
	CSIPServerStub* self = Server();
    delete self;
    Dll::SetTls( NULL );
	}

	
CSIPServerStub* CSIPServerStub::Server()
	{
	return static_cast<CSIPServerStub*>(Dll::Tls());
	}


void CSIPServerStub::ClientRequest( CSIPRequestElements* aRequest )
    {
    delete iClientRequest;
    iClientRequest = aRequest;
    }
	
void CSIPServerStub::ClientResponse( RStringF aMethod, CSIPResponseElements* aResponse )
    {
    iResponseToRequest = aMethod;
    delete iClientResponse;
    iClientResponse = aResponse;
    }


void CSIPServerStub::Reset()
    {
    ClientRequest( NULL );
    ClientResponse( SIPStrings::StringF( SipStrConsts::EEmpty ), NULL );
    }
	

CSIPServerStub::~CSIPServerStub()
	{
	
	delete iClientRequest;
	delete iClientResponse;
	
	}


	
//end of file