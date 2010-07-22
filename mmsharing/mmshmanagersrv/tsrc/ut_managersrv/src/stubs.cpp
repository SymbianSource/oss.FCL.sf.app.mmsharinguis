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

#include <e32std.h>

// TFindProcess stub

// -------------------------------------------------------------------------
//  TFindProcess::Next
// -------------------------------------------------------------------------
//
TInt TFindProcess::Next( TFullName& aResult )
    {
    if ( Dll::Tls() != NULL )
        {
        return KErrNone;
        }
    return KErrNotFound;
    }


// RProcess stub

// -------------------------------------------------------------------------
//  RProcess::Create
// -------------------------------------------------------------------------
//
TInt RProcess::Create( 
     const TDesC& /*aFileName*/,
     const TDesC& /*aCommand*/,
     TOwnerType /*aType*/ )
    {
    return Dll::SetTls( reinterpret_cast< TAny* >( 1 ) );
    }

// -------------------------------------------------------------------------
//  RProcess::Resume
// -------------------------------------------------------------------------
//
void RProcess::Resume()
    {    
    }
