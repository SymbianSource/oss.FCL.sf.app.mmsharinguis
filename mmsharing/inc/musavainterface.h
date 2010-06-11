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
* Description:  ECOM interface definition. This interface is used by
*               clients to get availability for Video Charging
*
*/



#ifndef __MUSAVAINTERFACE_H__
#define __MUSAVAINTERFACE_H__

// SYSTEM
#include "musavaobserver.h"
#include <e32base.h>
#include <ecom/ecom.h>

class MMusAvaObserver;
class MMusAvaSettings;
class MMusAvaObserver;
class MMusAvaSettingsObserver;


/**
 *  Interface class definition 
 *
 *  ECOM interface definition for MusAvailability Plug-in
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
 

class CMusAvaInterface : public CBase
	{
public: 

    // Constructors and destructor

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @return Returns pointer to CMusAvaInterface object
     */
     
	static CMusAvaInterface* NewL();

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */
	virtual ~CMusAvaInterface(); 
	
public: 
     
     /**
     * Called by the ECOM framework when MusAvailability Plug-in is request
     * retuns setting interface for the client.
     *
     * @since S60 v3.2
         * @return CMusAvaSettings
     */
    virtual MMusAvaSettings& Settings() = 0;
    
     /**
     * Called by the ECOM framework when MusAvailability Plug-in is for
     * setting Observer interface for the client.
     *
     * @since S60 v3.2
     * @param aObserver Obser interface
     * @return 
     */
     virtual void SetObserver( MMusAvaObserver& aObserver ) = 0;
             
    /**
     * Called by the ECOM framework when MusAvailability Plug-in is for
     * setting Observer interface for the client.
     *
     * @since S60 v3.2
     * @param aObserver MMusAvaObserver interface
     */
    virtual void SetSettingsObserver( MMusAvaSettingsObserver& aObserver ) =0;

     
     /**
     * Called by the ECOM framework when MusAvailability Plug-in is request
     * to investigate availabilites for the client.
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */
    virtual void StartL() = 0;
    
    /**
     * Called by the ECOM framework when MusAvailability Plug-in is request
     * to stop investigate or monitoring availabilites for the client.
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */
    virtual void Stop() = 0;

// TBD: COMMENTS
    virtual MMusAvaObserver::TAvailabilityName CurrentAvailability() = 0; // tbd:availabilityname
    virtual MMusAvaObserver::TAvailabilityStatus AvailabilityStatus() = 0;
    virtual TBool MinimumAvailability() = 0;
    virtual TBool MandatoryAvailability() = 0;
    virtual TBool ExtensionAvailability() = 0;
    virtual TBool Available( MMusAvaObserver::TAvailabilityName aAvailability ) = 0;
    virtual MMusAvaObserver::TAvailabilityStatus AvailabilityState( MMusAvaObserver::TAvailabilityName aAvailability ) = 0;
    virtual void PrepareForReceivedInviteL() = 0;
    
private: //Data

   	/** 
   	 * Instance identifier key. When instance of an
	 * implementation is created by ECOM framework, the
	 * framework will assign UID for it. The UID is used in
	 * destructor to notify framework that this instance is
	 * being destroyed and resources can be released.
     */
	TUid iDtor_ID_Key;

	};
	
#include "musavainterface.inl"

#endif

