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


#ifndef CSIPSERVERSTUB_H
#define CSIPSERVERSTUB_H

#include <e32base.h>    // CBase
#include <stringpool.h>

class CSIPRequestElements;
class CSIPResponseElements;


class CSIPServerStub : public CBase

    {
public:
    
    static void OpenL();
    static void Close();
    static CSIPServerStub* Server();
    
    ~CSIPServerStub();
    
    void Reset();
    
    void ClientRequest( CSIPRequestElements* aRequest );
    void ClientResponse( RStringF aMethod, CSIPResponseElements* aResponse );
	
public: //data


    CSIPRequestElements* iClientRequest;
    RStringF iResponseToRequest;
    CSIPResponseElements* iClientResponse;

	
	};


#endif //CSIPSERVERSTUB_H