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
* Description:  Provide interface for the client requestin availability class.
*
*/



#ifndef __CMUSAVAAVAILABILITY_H__
#define __CMUSAVAAVAILABILITY_H__


#include <e32def.h>
#include <e32base.h>
#include "musavaobserver.h"
#include "musavaavailabilityobserver.h"
#include "muslogger.h"
#include "musunittesting.h"

class MMusAvaAvailabilityObserver;

/**
 *  Availability interface class definition.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 */
class CMusAvaAvailability : public CBase
	{

public:

	/**
	* Current availability plugin state.
	*
	* @return Availability state
	*/
	MMusAvaObserver::TAvailabilityStatus AvailabilityPluginState()
		{
		return iObserver.AvailabilityPluginState();
		}

	/**
	* Executes for the availability.
	*
	* @since S60 v3.2
	* @return KErrNone if ExecutePatternL operation was successfully executed;
	*         system wide error otherwise
	*/
    void Execute()
        {
        MMusAvaObserver::TAvailabilityName name = Name();
        MUS_LOG1( "mus: [MUSAVA]  -> CMusAvaAvailability::Execute(%d)", name )
        if ( !Executing() && !Available() )
            {
            TRAPD( error, DoExecuteL() )
            if ( error )
                {
                MUS_LOG1( "mus: [MUSAVA]     CMusAvaAvailability::Execute error:%d", error )
                iObserver.AvailabilityError( name, (MMusAvaObserver::TAvailabilityStatus)error );
                }
            }
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaAvailability::Execute()" )
        }

	/**
	* Executes *this*  availability.
	*
	*/
    virtual void DoExecuteL() = 0;

	/**
	* Returns availability name.
	*
	* @return Name of *this* availability.
	*/
    virtual MMusAvaObserver::TAvailabilityName Name() = 0;

	/**
	 * Returns execution state.
	 *
	 * @return KErrNone if ExecutePatternL operation was successfully executed;
	 *         system wide error otherwise
	 */
    TBool Executing()
        {
        return iState == MMusAvaObserver::EMusAvaStatusInProgress;
        }

	/**
	* Returns *this* availability state.
	*
	* @return ETrue if is available, EFalse otherwise
	*/
    TBool Available()
        {
        return iState == MMusAvaObserver::EMusAvaStatusAvailable;
        }

	/**
	* Stop the execution.
	*
	* @since S60 v3.2
	* @return KErrNone if stop operation was successfully executed;
	*         system wide error otherwise
	*/
    virtual void Stop()
        {
        MUS_LOG( "mus: [MUSAVA]  -> CMusAvaAvailability::Stop()" )
        iState = MMusAvaObserver::EMusAvaStatusNotExecuted;
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaAvailability::Stop()" )
        }

	/**
	* Returns *this* availability state.
	*
	* @return Availability state.
	*/
    MMusAvaObserver::TAvailabilityStatus State()
        {
        return iState;
        }

	/**
	* Set availability state.
	*
	*/
    void SetState( MMusAvaObserver::TAvailabilityStatus aState )
        {
        MUS_LOG1( "mus: [MUSAVA]  -> CMusAvaAvailability::SetState(%d)", aState )
        if ( iState != aState )
            {
            iState = aState;
            iObserver.AvailabilityChanged( Name(), iState );
            }
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaAvailability::SetState" )
        }

	/**
	* Calculate the availabilited to decide to show the toolbar or not.
	*
	*/
    void AvailabilitiesAbleToShowIndicator()
    	{
    	iObserver.AvailabilitiesAbleToShowIndicator();
    	}

   /**
	* Returns specific availability state.
	*
	* @return Availability state.
	*/    
    TBool Available( MMusAvaObserver::TAvailabilityName aAvailability )
        {
        return iObserver.Available( aAvailability );
        }
        

    virtual ~CMusAvaAvailability() {};

protected:
    CMusAvaAvailability( MMusAvaAvailabilityObserver& aObserver )
        :iObserver( aObserver ),
        iState( MMusAvaObserver::EMusAvaStatusNotExecuted ) {}

protected: // data
    MMusAvaAvailabilityObserver& iObserver;
    MMusAvaObserver::TAvailabilityStatus iState;

	MUS_UNITTEST ( CMusAvaAvailabilityStub )
	MUS_UNITTEST ( CMusAvaDefaultImp )
	MUS_UNITTEST ( UT_CMusAvaConnectionAvailability )
    MUS_UNITTEST ( UT_CMusAvaDefaultImp )
    MUS_UNITTEST ( UT_CMusAvaOptionHandler )
    MUS_UNITTEST ( UT_CMusAvaDefaultOptionHandler )
    MUS_UNITTEST ( UT_CMusAvaSettingAvailability )
    };


#endif // __CMUSAVAAVAILABILITY_H__