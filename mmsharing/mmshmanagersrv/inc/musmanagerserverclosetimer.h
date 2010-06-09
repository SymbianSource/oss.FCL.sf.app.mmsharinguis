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


#ifndef MUSMANAGERSERVERCLOSETIMER_H
#define MUSMANAGERSERVERCLOSETIMER_H

//  INCLUDES
#include <e32base.h>

/**
 *  This class contains 
 *
 *  @since S60 v3.2
 */
class CMusManagerServerCloseTimer : public CActive
    {
public:

	static CMusManagerServerCloseTimer* NewL();

    static CMusManagerServerCloseTimer* NewLC();
    
    ~CMusManagerServerCloseTimer();
    
	void StopActiveSchedulerAfter( TUint aMilliSeconds );
	void RunL();
	TInt RunError( TInt aError );
	void DoCancel();

private:

    CMusManagerServerCloseTimer();
    void ConstructL();
    
private:
    //Data    
	RTimer iTimer;
	
    };

#endif // MUSMANAGERSERVERCLOSETIMER_H

//  End of File  
