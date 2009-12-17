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
* Description:  This observer is used by master - slave pattern to get 
*                function call from the slave (availability)
*
*/


#ifndef MUSAVATELEPHONYSTATUSOBSERVER_H
#define MUSAVATELEPHONYSTATUSOBSERVER_H

/**
 *  Availability Observer base class 
 *
 *  Observer interface for master clients.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
 
#include <etelmm.h>

class MMusAvaTelephonyStatusObserver
    {

public:
                        
    /**
     * Current Network Mode Status
     *
     * @since  S60 v3.2
     * @return 
     */
    virtual void PhoneNetworkModeStatus( 
                            RMobilePhone::TMobilePhoneNetworkMode aStatus ) = 0;
    
   /**
     * Current Network Registration Status
     *
     * @since  S60 v3.2
     * @return 
     */
    virtual void NetworkRegistrationStatus( 
                RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus ) = 0;
    };
    


#endif //MUSAVATELEPHONYSTATUSOBSERVER_H
