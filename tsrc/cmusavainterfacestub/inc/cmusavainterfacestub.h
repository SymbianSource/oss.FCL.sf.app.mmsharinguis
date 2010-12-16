/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Stub class used for unit testing
*
*/


#ifndef CMUSAVAINTERFACESTUB_H
#define CMUSAVAINTERFACESTUB_H

#include "mmusavasettingsstub.h"
#include "musavaobserver.h"
#include "musavainterface.h"
#include "musunittesting.h"

class CMusAvaAvailability;
class CMusAvaAvailabilityObserverImp;

/**
 *  Class that provides a stub implementation of CMusAvainterface class.
 *  Used for unit testing other classes.
 *
 *  @since S60 v3.2
 */
class CMusAvaInterface2 : public CMusAvaInterface, MMusAvaObserver
	{

public:

    static CMusAvaInterface2* NewL();

     /**
     * Destructor.
     */
	virtual ~CMusAvaInterface2();

    /**
     * Requests the implementation to provide availabilites.
     *
     * @since  S60 v3.2
     */
    virtual void Availability();

     /**
     * Called by the ECOM framework when MusAvailability Plug-in is request
     * retuns setting interface for the client.
     *
     * @since S60 v3.2
     * @return Returns reference to multimediasharing avaialability settings.
     */
    virtual MMusAvaSettings& Settings();

     /**
     * Called by the ECOM framework when MusAvailability Plug-in is for
     * setting Observer interface for the client.
     *
     * @since S60 v3.2
     * @param aObserver Reference to observer interface.
     */
    virtual void SetObserver( MMusAvaObserver& aObserver );

     /**
     * Called by the ECOM framework when MusAvailability Plug-in is request
     * to investigate availabilites for the client.
     *
     * @since S60 v3.2
     */
    void StartL();

    /**
     * Called by the ECOM framework when MusAvailability Plug-in is request
     * to stop investigate or monitoring availabilites for the client.
     *
     * @since S60 v3.2
     */
    void Stop();
    
    
    virtual MMusAvaObserver::TAvailabilityName CurrentAvailability(); // tbd:availabilityname
    virtual MMusAvaObserver::TAvailabilityStatus AvailabilityStatus();
    virtual TBool MinimumAvailability();
    virtual TBool MandatoryAvailability();
    virtual TBool ExtensionAvailability();
    virtual TBool Available( MMusAvaObserver::TAvailabilityName aAvailability );
    virtual MMusAvaObserver::TAvailabilityStatus AvailabilityState( MMusAvaObserver::TAvailabilityName aAvailability );
    
public: //From MMusAvaObserver

	/**
     * Availability report
     *
     * @since  S60 v3.2
     * @param aStatus New status for availability.
     */
    virtual void AvailabilityChanged(
        TAvailabilityName aName,
        TAvailabilityStatus aStatus );

    virtual void AvailabilityError(
        TAvailabilityName aName,
        TAvailabilityStatus aStatus );

    virtual void  AvailabilitiesAbleToShowIndicator();

    virtual void SetSettingsObserver( MMusAvaSettingsObserver& aObserver );

private:

    CMusAvaInterface2();

    void ConstructL();
    
public: //data

  	TBool iStart;

private: // data

    /**
     * Instance of settings class stub.
     * Own.
     */
    CMusAvaSettings2* iSettings;

    /**
     * Pointer to observer interface.
     * Not own.
     */
    MMusAvaObserver* iObserver;
    
    
    RPointerArray<CMusAvaAvailability> iAvailabilities;

    MMusAvaObserver::TAvailabilityName iCurrentAvailability;
    MMusAvaObserver::TAvailabilityStatus iAvailabilityStatus;
    
    CMusAvaAvailabilityObserverImp* iMusAvailObs;
    
    MUS_UNITTEST( UT_CMusAvailabilityPluginManager )
    MUS_UNITTEST( UT_CMusManagerServerCore )
	};

#endif // CMUSAVAINTERFACESTUB_H
