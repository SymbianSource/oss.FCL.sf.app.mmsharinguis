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


#ifndef __MUSAVACAPABQUERYOBSERVERIMP_H__
#define __MUSAVACAPABQUERYOBSERVERIMP_H__

#include <e32base.h>
#include "mmusavacapabilityqueryobserver.h"

class CMusAvaCapabilityQueryObserverImp : public CBase, 
                                          public MMusAvaCapabilityQueryObserver
                                
	{
public:

	CMusAvaCapabilityQueryObserverImp();
	
	//from MMusAvaCapabilityQueryObserver
    void CapabilitiesResolved( const CMusAvaCapabilityQueryBase& aSentQuery );
    
    void CapabilitiesResolvedL( const TDesC& aUri );
    
    void SetCapabilitiesResolvedForCingular();
    
    TBool CapabilitiesResolvedForCingular();
    
    TBool iCapabilitiesResolvedForCingular;
    
    void SipHeadersL(const MDesCArray& aHeaders,SipStrConsts::TStrings aHeaderType);
    
    void VideoCodecsResolvedL( const MDesCArray& aVideoCodecs );
    
    TBool CapabilityQueryAnswered( TBool aAnswered = EFalse );
    
    };


#endif // __MUSAVACAPABQUERYOBSERVERIMP_H__

