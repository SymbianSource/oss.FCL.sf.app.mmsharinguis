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
* Description:  Provide interface for the client requestin register availability.
*
*/



#ifndef __MUSAVAPROFILEREGISTRYADAPTERSTUB_H__
#define __MUSAVAPROFILEREGISTRYADAPTERSTUB_H__


#include "musunittesting.h"
#include "musavasipprofileregistryadapter.h"
#include <e32base.h>


class MSIPProfileRegistryObserver;

/**
 *  Implemets register vailability.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaProfileRegistryAdapterStub : public MMusAvaSipProfileRegistryAdapter
	{
public:

	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return Returns pointer to MMusAvaSipProfileRegistryAdapter object
     */
	static CMusAvaProfileRegistryAdapterStub* NewL();


	static CMusAvaProfileRegistryAdapterStub* NewLC();

    /**
     * Destructor
     */

    ~CMusAvaProfileRegistryAdapterStub();

    protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     * @return
     */
    CMusAvaProfileRegistryAdapterStub();

    void ConstructL();

// from base class CMusAvaAvailability

public:

   /**
    * An event related to SIP Profile has accorred
    * @param aProfileId a profile Id
    * @param aEvent an occurred event
    **/
    virtual void ProfileRegistryEventOccurred(
        TUint32 aProfileId,
        MSIPProfileRegistryObserver::TEvent aEvent );

   /**
    * An asynchronous error has occurred related to SIP profile
    * Event is send to those observers, who have the
    * corresponding profile instantiated.
    * @param aProfileId the id of failed profile
    * @param aError an occurred error
    */
    virtual void ProfileRegistryErrorOccurred(
        TUint32 aProfileId,
        TInt aError );


private: // Data

    TUint32 iProfileId;
    MSIPProfileRegistryObserver::TEvent iEvent;
    TInt iError;
    
    MUS_UNITTEST( UT_CMusAvaSipProfileRegistryObserver )
    };


#endif // __MUSAVAPROFILEREGISTRYADAPTERSTUB_H__