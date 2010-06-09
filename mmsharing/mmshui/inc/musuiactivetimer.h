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



#ifndef MUSUIACTIVETIMER_H
#define MUSUIACTIVETIMER_H

#include <e32base.h>

class MMusUiActiveTimerObserver;

class CMusUiActiveTimer : public CActive
    {

public: // constructors and destructor

    /**
     * Destructor.
     */
    ~CMusUiActiveTimer();

    /**
     * Two-phased constructor. Leaves on failure.
     * @return The constructed CMusUiActiveTimer object.
     */
    static CMusUiActiveTimer* NewL( MMusUiActiveTimerObserver* aObserver );

private:

    /**
     * C++ constructor.
     */
    CMusUiActiveTimer( MMusUiActiveTimerObserver* aObserver );

    /**
     * Symbian 2nd-phase constructor.
     */
    void ConstructL();

public: // new funtions:

    void After( TTimeIntervalMicroSeconds32 anInterval );
    
private: // functions from base class CActive

    void DoCancel();

    void RunL();
    
    TInt RunError( TInt aError );

private:    // Data

	RTimer iTimer;
    MMusUiActiveTimerObserver* iObserver;

    };

#endif // MUSUIACTIVETIMER_H

// end of file
