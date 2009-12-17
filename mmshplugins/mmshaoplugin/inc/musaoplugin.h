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
* Description:  Main plugin class
*
*/


#ifndef MUSAOPLUGIN_H
#define MUSAOPLUGIN_H


#include <e32base.h>    
#include <AlwaysOnlineEComInterface.h>
#include <ecom/implementationproxy.h>
#include <etel.h>    
#include <etelmm.h>  
#include <e32property.h>

#include "musunittesting.h"

// Enable the below line if Kodiak Ptt has to be monitered
//class CMusPttCallMonitor;
class CMusPropertyMonitor;
class CMusTsyPropertyMonitor;

IMPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount );

/**
 * Main plugin class
 *
 * @lib musaoplugin.dll
 */
class CMusAoPlugin : public CAlwaysOnlineEComInterface                      
    {
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return New CMusAoPlugin instance
     */
    static CMusAoPlugin* NewL();

    /**
     * C++ destructor.
     */
    virtual ~CMusAoPlugin();
    
    
public: // new API

    /**
     * Define start up resource properties
     *
     * @since S60 v3.2
     */
    void DefinePropertiesL();

    /**
     * Delete when AO end
     *
     * @since S60 v3.2
     */

    void DeleteProperties();

private: // constructors

    /**
     * C++ constructor.
     */
    CMusAoPlugin();

    /**
     * Symbian second-phase constructor.
     */
    void ConstructL();
    
    /**
     * Utility function to define and set keys.
     */
	void DefinePropertyL(TInt aKey,RProperty::TType aType,TInt aVal);

    /**
     * Utility function to define and set keys.
     */
	void DefinePropertyL(TInt aKey,RProperty::TType aType,const TDesC& aVal);

    /**
     * Utility function to delete keys.
     */
	void DeleteProperty(TInt aKey);
	
public: // from base class CAlwaysOnlineEComInterface

    /**
     *  Commands from alwaysonlinemanager to plugin
     *
     * @since S60 v3.2
     * @param aCommand, command Id
     * @param aParameters, parameter pack
     * @return return value
     */
    virtual TAny* HandleServerCommandL( TInt aCommand,
                                        TDesC8* aParameters );	

private: // data
   
    /* Property Monitor  */
    CMusPropertyMonitor*      iPropertyMonitor;

    /** 
     *  Telephony Property Key monitor.
     *  Monitors the call connect/disconnect events.
     */
    CMusTsyPropertyMonitor*   iTsyPropertyMonitor;

    /* Ptt Call Monitor  */
    // Enable the below line if Kodiak Ptt has to be monitered
    // CMusPttCallMonitor*       iPttCallMonitor;

    /**
     * RTelServer handle
     */
    RTelServer iServer;

    /**
     * RMobilePhone handle
     */
    RMobilePhone iPhone;    

    /** 
     *  Error status.
     */
    TInt iError;
   
    MUS_UNITTEST( UT_CMusAoPlugin )

    };

#endif // MUSAOPLUGIN_H
