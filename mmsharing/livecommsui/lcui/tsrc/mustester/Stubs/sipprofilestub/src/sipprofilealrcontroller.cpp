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

#include <sipprofilealrcontroller.h>


// CSipProfileAlrController STUB
CSipProfileAlrController* CSipProfileAlrController::NewL( 
    CSIPProfileRegistryBase& aRegistry, 
    MSipProfileAlrObserver& /*aObserver*/ )
    {
    return new( ELeave )CSipProfileAlrController( aRegistry );
    }

CSipProfileAlrController::~CSipProfileAlrController()
    {
    }

void CSipProfileAlrController::AllowMigrationL(
    TUint32 /*aProfileId*/,
    TUint32 aIapId )
    {
    delete HBufC::NewL( 1 );
    //iAllowMigrationCalled = ETrue;
    //iLastUsedIap = aIapId;
    }

void CSipProfileAlrController::DisallowMigrationL(
    TUint32 /*aProfileId*/,
    TUint32 aIapId )
    {
    delete HBufC::NewL( 1 );
    //iDisallowMigrationCalled = ETrue;
    //iLastUsedIap = aIapId;
    }

void CSipProfileAlrController::RefreshIapAvailabilityL( 
    TUint32 /*aProfileId*/ )
    {
    delete HBufC::NewL( 1 );
    }

CSipProfileAlrController::CSipProfileAlrController( 
    CSIPProfileRegistryBase& aRegistry )
    : iRegistry( aRegistry )
    {
    }
