/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This interface is used by
*               clients to get and availability setting for Video Charging
*
*/


#ifndef __MUSFACTORYSETTINGS_H__
#define __MUSFACTORYSETTINGS_H__

//  INCLUDES
#include <badesca.h>
#include <e32base.h>
#include <etelmm.h>
#include <ctsydomainpskeys.h>

class MMusAvaSettingsObserver;

/**
 *  Utility function to check the factory configurations.
 *  @lib musaoplugin.lib
 */

class MusFactorySettings
    {   
public:    
     /**
     * Checks from the factory settings whether call type is supported.
     * @aCallType : Type of the call retrived from Tsy apis.
     * 
     * @return ETrue if supported else EFalse.
     * Leaves when cenrep key is not found or cenrep related error happens.
     */         
    static TBool IsSupportedL(const TPSCTsyCallType& aCallType);
        
    /**
     * Checks from the factory settings whether network type is supported.
     * @aNetworkType : Type of the network
     * 
     * @return ETrue if supported else EFalse.
     * Leaves when cenrep key is not found or cenrep related error happens.
     */         
    static TBool IsSupportedL(const RMobilePhone::TMobilePhoneNetworkMode& aNetworkType);  

    };

#include "musfactorysettings.inl"

#endif

