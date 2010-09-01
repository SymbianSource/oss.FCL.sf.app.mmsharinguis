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
*                clients to get and availability setting for Video Charging
*
*/


#ifndef __MUSAVASETTINGS_H__
#define __MUSAVASETTINGS_H__

//  INCLUDES
#include <badesca.h>
#include <e32base.h>
#include <e32std.h>

class MMusAvaSettingsObserver;

/**
 *  Defines setting interface for the MusAvailability Plug-in
 *
 *  ECOM clients set and get setting are used by MusAvailability Plug-in.
 *
 *  @lib musavailabilityplugin.lib
 */

class MMusAvaSettings
	{
	
public: 

    enum TManualActivation
        {
        EActivationNotExecuted,
        EActivationExecuted,
        EActivationAllowed,
        EActivationNotAllowed
        };
public:
    
     /**
     * Returns the manual activation
     *
     */   		
    virtual TManualActivation ManualActivation();
    	
    /**
     * Returns the telephone number of remote host of active CS call.
     *
     * @return The telephone number.
     */   		
    virtual const TDesC& TelNumber() const;    	

    /**
     * Returns the contact name of remote host of active CS call.
     *
     * @return The contact name.
     */   		
    virtual const TDesC& ContactName() const;    	
    
    /**
     * Returns the possible SIP addresses of the remote host.
     *
     * @return The SIP addresses in an array.
     */   		
    virtual const MDesCArray& SipAddresses() const;
    
    /**
     * Returns the proposal for the remote host SIP address.
     *
     * @return The telephone number.
     */   		
    virtual const TDesC& SipAddressProposal() const;
    
    /**
     * Returns the contact id of the remote host.
     *
     * @return The contact id; KErrNotFound, if not found any.
     */   		
    virtual TInt ContactId() const;

    /**
     * Returns the supported video codecs.
     *
     * @return The video codecs
     */   		
    virtual const MDesCArray& VideoCodecs() const;
    
    /**
     * Returns the supported audio codecs.
     *
     * @return The supported audio codecs.
     */   		
    virtual const MDesCArray& AudioCodecs() const;

    /**
     * Returns the id of the preferable SIP profile.
     *
     * @return The SIP profile id.
     */   		
    virtual TInt SipProfileId() const;

    /**
     * Sets the settings observer pointer
     *
     */   		
    virtual void SetObserver( MMusAvaSettingsObserver& aObserver );
    
     /**
     * Sets the for the manual activation
     *
     */   		
    virtual void SetManualActivation( TManualActivation aManualActivation );
    
    /**
    * Returns the direction of the mobile call.
    *
    * @return 0, if the direction of the call is unknown.
    *         1, call is from public number.
    *         2, call is from private number   
    */    		
    virtual TInt CallPrivacy();

	};

#include "musavasettings.inl"

#endif

