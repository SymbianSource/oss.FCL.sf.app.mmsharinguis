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

#include <drmcommon.h>




// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C DRMCommon* DRMCommon::NewL(void)
    {
    DRMCommon* self = new ( ELeave ) DRMCommon;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C DRMCommon::~DRMCommon(void)
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt DRMCommon::Connect(void)
    {
    return KErrNone;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt DRMCommon::Disconnect(void)
    {
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt DRMCommon::IsProtectedFile( const TDesC& aFileName, 
                                          TBool& aProtection)
    {
    if ( aFileName == KMusDrmProtectedFileName() )
        {
        aProtection = ETrue;
        }
    else
        {
        aProtection = EFalse;
        }
        
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C DRMCommon::DRMCommon(void)
    :iIsConnected( EFalse )
    {
    }
    
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void DRMCommon::ConstructL(void)
    {
    }






