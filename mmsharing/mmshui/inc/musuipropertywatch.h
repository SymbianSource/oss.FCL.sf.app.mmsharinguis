/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*
*/



#ifndef MUSUIPROPERTYWATCH_H
#define MUSUIPROPERTYWATCH_H

#include "musunittesting.h"

#include <e32base.h>
#include <e32property.h>

class MMusUiPropertyObserver;

class CMusUiPropertyWatch : public CActive
    {
    
    MUS_UNITTEST( UT_CMusUiPropertyWatch; )
        
public: // constructors and destructor

    /**
     * Two-phased constructor. Leaves on failure.
     * @return The constructed CMusUiPropertyWatch object.
     */
    static CMusUiPropertyWatch* NewL( MMusUiPropertyObserver& aObserver, 
                                      TUid aCategory, 
                                      const TInt aPropertyName );

    /**
     * Destructor.
     */
    ~CMusUiPropertyWatch();

private:

    /**
     * C++ constructor.
     */
    CMusUiPropertyWatch( MMusUiPropertyObserver& aObserver, 
                         TUid aCategory, 
                         const TInt aPropertyName );

    /**
     * Symbian 2nd-phase constructor.
     * @leave If return value of RProperty::Attach != KErrNone, e.q. KErrNoMemory
     *        or one of the system wide error codes if RunL() leaves
     */
    void ConstructL();


public: // new functions

    /**
    * Reads a P&S integer value from the category KCategoryUid
    * @pre P&S value has to be defined.
    * @leave One of the system wide error codes, if RProperty::Get()
    *        returns an error value.
    * @param aKey Name of the P&S Key, which value should be read
	* @returns The integer value of the P&S Key.
    */
    static TInt ReadIntPropertyL( TUint aKey );
    
    /**
    * Reads a P&S descriptor value from the category KCategoryUid
    * @pre P&S value has to be defined.
    * @leave One of the system wide error codes, if RProperty::Get()
    *        returns an error value or if descriptor AllocL() leaves.
    * @param aKey Name of the P&S Key, which value should be read
	* @returns The descriptor value of the P&S Key.
    *          Ownership is transferred
    */
    static HBufC* ReadDescPropertyL( TUint aKey );


private: // functions from base class CActive

    /**
     *
     */
    void DoCancel();

    /**
    * Reads a P&S integer value from the category KCategoryUid
    * and starts to monitor changes in the property value.
    * Calls observer function for read property values.
    * @pre P&S value has to be defined as an integer.
    * @leave One of the system wide error codes, if RProperty::Get()
    *        returns an error value.
    */
    void RunL();

    /**
     *
     */
	TInt RunError(TInt aError);
	
private:    // Data

	RProperty iProperty;
	
	TUid iPropertyCategory;
	
    TUint iPropertyKey;
    
    MMusUiPropertyObserver& iObserver;

    };

#endif // MUSUIPROPERTYWATCH_H

// end of file
