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
* Description:  Capability query observer 
*
*/

#include "musavacapabilityqueryobserverimp.h"
//#include "musavacapabilityquerybase.h"


// -------------------------------------------------------------------------
// C++ constructor.
// -------------------------------------------------------------------------
//
CMusAvaCapabilityQueryObserverImp::CMusAvaCapabilityQueryObserverImp() 
	{
	
	}
	
	
void CMusAvaCapabilityQueryObserverImp::CapabilitiesResolved( 
                            const CMusAvaCapabilityQueryBase& /*aSentQuery*/ )
    {
        
    }
    
void CMusAvaCapabilityQueryObserverImp::CapabilitiesResolvedL( 
                             const TDesC& /*aUri*/ )
    {
    
    }
    
void CMusAvaCapabilityQueryObserverImp::SetCapabilitiesResolvedForCingular()
    {
    
    }
    
TBool CMusAvaCapabilityQueryObserverImp::CapabilitiesResolvedForCingular()
    {
    
    return iCapabilitiesResolvedForCingular;
    
    }

void CMusAvaCapabilityQueryObserverImp::SipHeadersL(
                                   const MDesCArray& /*aHeaders*/,
                                   SipStrConsts::TStrings aHeaderType)
    {
    if (aHeaderType==SipStrConsts::EEmpty)
        {
        User::Leave(KErrArgument);
        }
    else
        {
        // nothing.
        }
    
    }

void CMusAvaCapabilityQueryObserverImp::VideoCodecsResolvedL( const MDesCArray& aVideoCodecs )
    {

    }

TBool CMusAvaCapabilityQueryObserverImp::CapabilityQueryAnswered( TBool aAnswered )
    {
    return EFalse;
    }
