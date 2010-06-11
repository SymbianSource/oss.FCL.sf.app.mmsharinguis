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
#include <sipcontactheader.h>
#include "musavacapabilitytesthelper.h"


CSIPAddress* CSIPContactHeader::SIPAddress()
    {
    CSIPAddress* ret = iSIPAddress;

    if (HelpFlags::iUseTestSipAddress)
        {
        ret = HelpFlags::iTestSipAddress;    
        }

    return ret;
    }

RPointerArray<CSIPContactHeader> CSIPContactHeader::DecodeL(const TDesC8& aValue)
    {
    RPointerArray<CSIPContactHeader> headers;
    if ( !HelpFlags::iEmptyContactHeader )
        {
        CleanupClosePushL(headers);
        
        CSIPAddress* address = CSIPAddress::DecodeL( aValue ); 
        
        CleanupStack::PushL( address );        
        CSIPContactHeader* contactHeader = CSIPContactHeader::NewL( address );
        CleanupStack::Pop( address );
        
        CleanupStack::PushL( contactHeader );
        headers.AppendL( contactHeader );
        CleanupStack::Pop(contactHeader);
        
        CleanupStack::Pop(); //headers
        }
    return headers;
    }


