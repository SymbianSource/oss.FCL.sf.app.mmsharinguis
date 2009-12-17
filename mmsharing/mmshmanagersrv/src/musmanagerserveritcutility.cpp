/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


#include <e32cmn.h>

#include "muslogger.h"
#include "musmanagerserveritcutility.h"

// ---------------------------------------------------------------------------
// MusManagerServerItcUtility::WriteAvailabilityL
// ---------------------------------------------------------------------------
//
void MusManagerServerItcUtility::WriteAvailabilityL( 
    const RMessage2& aMessage, 
    TInt aAvailability )
    {
    TPckg<TInt> capPckg( aAvailability );
    WriteL( 
        EMusManagerIpcArgAvailability,
        aMessage,
        capPckg );
    }


// ---------------------------------------------------------------------------
// MusManagerServerItcUtility::WriteAvailabilityReportL
// ---------------------------------------------------------------------------
//
void MusManagerServerItcUtility::WriteAvailabilityReportL( 
    const RMessage2& aMessage, 
    TInt aAvailability )
    {
    TPckg<TInt> capPckg( aAvailability );
    WriteL( 
        EMusManagerIpcArgAvailabilityStatus,
        aMessage,
        capPckg );
    }
// ---------------------------------------------------------------------------
// MusManagerServerItcUtility::ReadChannelIdL
// ---------------------------------------------------------------------------
//
TUid MusManagerServerItcUtility::ReadChannelIdL( 
    const RMessage2& aMessage )
    {
    TPckgBuf<TUid> uidPckg;
    aMessage.ReadL(
        EMusManagerIpcArgChannelUid, 
        uidPckg );
        
    return uidPckg();
    }

// ---------------------------------------------------------------------------
// MusManagerServerItcUtility::ReadCommandL
// ---------------------------------------------------------------------------
//
TInt MusManagerServerItcUtility::ReadCommandL( 
    const RMessage2& aMessage )
    {
    TPckgBuf<TInt> intPckg;
    aMessage.ReadL(
        EMusManagerIpcArgCommandType, 
        intPckg );
        
    return intPckg();
    }


  
// ---------------------------------------------------------------------------
// MusManagerServerItcUtility::ReadRequestStatusL
// ---------------------------------------------------------------------------
//
TRequestStatus MusManagerServerItcUtility::ReadRequestStatusL( 
    const RMessage2& aMessage )
    {
   	TPckgBuf<TInt> intPckg;
    aMessage.ReadL(
        EMusManagerIpcArgRequestStatus, 
        intPckg );
        
    return  TRequestStatus( intPckg() );
    }    

// ---------------------------------------------------------------------------
// MusManagerServerItcUtility::ReadUseCaseL
// ---------------------------------------------------------------------------
//
TInt MusManagerServerItcUtility::ReadUseCaseL( 
    const RMessage2& aMessage )
    {
    TPckgBuf<TInt> intPckg;
    aMessage.ReadL(
        EMusManagerIpcArgUseCase, 
        intPckg );
        
    return intPckg();
    }

// ----------------------------------------------------------------------------
// MusManagerServerItcUtility::WriteL
// ----------------------------------------------------------------------------
//
void MusManagerServerItcUtility::WriteL (
    TMusManagerIpcArguments aIndex,
    const RMessage2& aMessage,
    const TDesC8& aDes)
    {
    TInt length = aMessage.GetDesMaxLength( aIndex );
    
    if (length < 0)
        {
        User::Leave( KErrArgument );
        }
    if (length < aDes.Length())
        {
        User::Leave( KErrOverflow );
        }

    aMessage.WriteL( aIndex, aDes );               
    }



