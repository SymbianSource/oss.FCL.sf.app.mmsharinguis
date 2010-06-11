/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


#ifndef MUSMAVATIMER_H
#define MUSMAVATIMER_H

//  INCLUDES
#include <e32base.h>

/**
 *  This class contains 
 *
 *  @since S60 v3.2
 */
class CMusAvaTimer : public CActive
    {
public:

	static CMusAvaTimer* NewL();

    static CMusAvaTimer* NewLC();
    
    ~CMusAvaTimer();
    
	void StopActiveSchedulerAfter( TInt aMilliSeconds, 
                                   TRequestStatus* aStatus  );
	void RunL();
	void DoCancel();

private:

    CMusAvaTimer();
    void ConstructL();
    
private:
    //Data    
	RTimer iTimer;
	//Request status
	TRequestStatus* iRequestStatus;
    };

#endif // MUSMAVATIMER_H

//  End of File  
