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

#ifndef MMUSAVACAPABILITYQUERYOBSERVER_H
#define MMUSAVACAPABILITYQUERYOBSERVER_H

#include <sipstrconsts.h>
#include <bamdesca.h>

class CMusAvaCapabilityQueryBase;



/**
 * Capability Query Observer
 *
 * @lib musavailabilityplugin.lib
 */
class MMusAvaCapabilityQueryObserver
    {
public: 

    virtual void CapabilitiesResolved( const CMusAvaCapabilityQueryBase& aSentQuery ) = 0;
    
    virtual void CapabilitiesResolvedL( const TDesC& aUri ) = 0;
    
    virtual void SetCapabilitiesResolvedForCingular() = 0;
    
    virtual TBool CapabilitiesResolvedForCingular() = 0;

    /**
     * This is to advertise MMusAvaCapabilityQueryObserver implementors
     * about sip header change. It is mainly used in below scneraio but could
     * be used in future if anyother sip headers has to be set.
     * 
     * Incase if we receive OPTIONS request already and 
     * P-Asserted-Identity Header is present then OPTIONS
     * request should be sent to this ID. This function should be
     * called to configure this id in CMusAvaSettingsImp& iSettings
     *
     * @aHeaders Array of SIP Headers.    
     * @aHeaderType Type of the header.
     * Leavs if Array is empty or pointer is NULL.
     * Ownership Transferred.
     */
    virtual void SipHeadersL(const MDesCArray& aHeaders,
                          SipStrConsts::TStrings aHeaderType)=0;
    
    /**
     * Advertise about video codec capabilities     
     */
    virtual void VideoCodecsResolvedL( const MDesCArray& aVideoCodecs ) = 0;
    
    virtual TBool CapabilityQueryAnswered( TBool aAnswered = EFalse ) = 0;
    
    };


#endif //MMUSAVACAPABILITYQUERYOBSERVER_H

