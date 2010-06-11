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
* Description:  Implements Settings Availability
*
*/



#ifndef __MUSAVASETTINGAVAILABILITY_H__
#define __MUSAVASETTINGAVAILABILITY_H__

class CMusAvaAvailability;
class MMusAvaAvailabilityObserver;
class MMusAvaObserver;

/**
 *  Implemets settings availability.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaSettingAvailability : public CMusAvaAvailability
                                    
	{
public:

	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aModules is data container
     * @return Returns pointer to CMusAvaInterface object
     */
	static CMusAvaSettingAvailability* NewL(
        MMusAvaAvailabilityObserver& aObserver);


	static CMusAvaSettingAvailability* NewLC(
	    MMusAvaAvailabilityObserver& aObserver);

    /**
     * Destructor
     */

    ~CMusAvaSettingAvailability();

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
    CMusAvaSettingAvailability(
        MMusAvaAvailabilityObserver& aObserver);

    void ConstructL();

// from base class CMusAvaAvailability

public:

   /**
    * Executes for the availability.
    */
    virtual void DoExecuteL();

   /**
    * Stop the execution.
    */
    virtual void Stop();
    
    /**
     * From CMusAvaAvailability.
     * Returns availability name.
     *
     * @return Name of *this* availability.
     */
    virtual MMusAvaObserver::TAvailabilityName Name();

private:
    
    /**
     * Checks the options response state.
     *
     * @since S60 v3.2          
     * @return Returns ETrue if options response is 200
     *                 else otherwise.
     */
    TBool IsForbiddenByOptionsResponse();


    /**
     * Checks the activation & Options response state.
     *
     * @since S60 v3.2               
     */
    void CheckActivationState();
 


    MUS_UNITTEST( UT_CMusAvaSettingAvailability )
    };


#endif // __MUSAVASETTINGAVAILABILITY_H__

