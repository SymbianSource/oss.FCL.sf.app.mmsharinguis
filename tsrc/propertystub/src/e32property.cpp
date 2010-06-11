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

#include "e32property.h"
#include "mussessionproperties.h"
#include "mussesseioninformationapi.h"
#include <e32cmn.h>

static RArray<TUint> iIntProperty;
static TInt iErrorCode = KErrNone;
static RProperty::TPropertyCalledFunctions iCalledFunction = RProperty::ENone;
// Large enough to store maximum length descriptor for RProperty
static TBuf<RProperty::KMaxPropertySize> iString = KNullDesC();
static TBuf<RProperty::KMaxPropertySize> iVideoCodecList = KNullDesC();
static TBuf<RProperty::KMaxPropertySize> iSessionRecipient = KNullDesC();
static TBuf<RProperty::KMaxPropertySize> iEngineName = KNullDesC();
static TInt iValue = KErrNotFound;


_LIT( KMusTestPropertyReadDesc, "TEST" );

// Function definitions

TInt RProperty::Define( TUid /*aCategory*/, TUint /*aKey*/, TInt /*aAttr*/,TInt /*aPreallocate=0*/ )
    {    
    iCalledFunction = RProperty::EDefine;
    return iErrorCode;
    }
    
TInt RProperty::Delete( TUid /*aCategory*/, TUint aKey)
    {
    for(TInt i=0;i<iIntProperty.Count();i=i+2)
        {
        if( aKey == iIntProperty[i] )
            {
            iIntProperty.Remove( i );
            iIntProperty.Remove( i );
            return KErrNone;
            }
        }
    
    return iErrorCode;
    }
    

TInt RProperty::Get( TUid /*aCategory*/, TUint aKey, TInt& aVal)
    {
    if ( !iErrorCode )
        {
        // if errorcode not set , then try to get value from array.
        TInt error = Get(aKey,aVal);
        if ( error != KErrNone )
            {
            aVal = iValue;
            error = iValue == KErrNotFound ? error : KErrNone;
            }
        return error;
        }
    else
        {
        // if error code is set value doesnt matter.
        return iErrorCode;    
        }    
    }

TInt RProperty::Get( TInt& aValue)
    {
    TInt error = iErrorCode;
    aValue = iPValue;
    return error;
    }


TInt RProperty::Get( TUid /*aCategory*/, TUint /*aKey*/, TDes8& aValue)
    {
    // This is not widely used in Mush , so who cares in stub.
    aValue.Append( KMusTestPropertyReadDesc );
    return iErrorCode;
    }


TInt RProperty::Get( TUid /*aCategory*/, TUint aKey, TDes16& aValue )
    {
    if ( aKey == NMusSessionInformationApi::KMUSCallProvider )
         {
         aValue = iEngineName;
         }
    else if ( aKey == NMusSessionApi::KVideoCodecs )
        {
        aValue = iVideoCodecList;
        }
    else if ( aKey == NMusSessionApi::KRemoteSipAddress )
        {
        aValue = iSessionRecipient;
        }
    else
        {
        aValue = iString;
        }
    
    return iErrorCode;
    }

TInt RProperty::Set( TUid /*aCategory*/, TUint aKey, TInt aValue)
    {    
    for(TInt i=0;i<iIntProperty.Count();i=i+2)
        {
        if( aKey == iIntProperty[i] )
            {
            iIntProperty[i+1] = aValue;
            return KErrNone;
            }
        }

    TInt error = KErrNone;
    error = iIntProperty.Append(aKey);
    if (!error)
        {
        error = iIntProperty.Append(aValue);
        }
    return iErrorCode ? iErrorCode : error;
    }

TInt RProperty::Set( TUid /*aCategory*/, TUint /*aKey*/, const TDesC8& /*aValue*/)
    {
    // This is not widely used in Mush , so who cares in stub.
    return iErrorCode;
    }


TInt RProperty::Set( TUid /*aCategory*/, TUint aKey, const TDesC& aValue )
    {

    if ( aKey == NMusSessionInformationApi::KMUSCallProvider )
        {
        iEngineName = aValue;
        }
    
    else if ( aKey == NMusSessionApi::KVideoCodecs )
        {
        iVideoCodecList = aValue;
        }
    else if ( aKey == NMusSessionApi::KRemoteSipAddress )
        {
        iSessionRecipient = aValue;
        }
    else
        {
        iString = aValue;
        }
    return iErrorCode;
    }


TInt RProperty::Attach(  TUid /*aCategory*/, TUint /*aKey*/)
    {
    return iErrorCode;
    }   


void RProperty::Cancel()
    {    
    iCalledFunction = RProperty::ECancel;
    if ( iRequestStatus )
        {
        User::RequestComplete( iRequestStatus, KErrCancel );
        }
    iRequestStatus = NULL;
    }

TInt RProperty::Set(TInt aValue) // Non-Static
    {
    iPValue = aValue;
    return iErrorCode;
    }
    

void RProperty::Subscribe(TRequestStatus& aRequest)
    {
    aRequest = KRequestPending;
    iRequestStatus = &aRequest;
    
    }

TInt RProperty::Get(TUint& aKey,TInt& aVal)
    {    
    for(TInt i=0;i<iIntProperty.Count();i=i+2)
        {
        if( aKey == iIntProperty[i] )
            {
            aVal = iIntProperty[i+1];
            return KErrNone;
            }
        }
    return KErrNotFound;
    }

/*
 * Stub helper function to set error codes
 */
void PropertyHelper::SetErrorCode(TInt aVal)
    {
    iErrorCode = aVal;
    }

RProperty::TPropertyCalledFunctions PropertyHelper::GetCalledFunction()
    {
    return iCalledFunction;
    }

void RProperty::Close()
    {
    Cancel();
    }

void PropertyHelper::Close()
    {
    iIntProperty.Close();
    iErrorCode = KErrNone;
    iCalledFunction = RProperty::ENone;
    iString = KNullDesC();
    iEngineName = KNullDesC();
    iValue = KErrNotFound;
    }

