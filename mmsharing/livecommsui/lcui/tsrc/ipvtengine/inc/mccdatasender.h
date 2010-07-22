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
* Description:  
*
*/




#ifndef MCCDATASENDER_H
#define MCCDATASENDER_H
#include <E32Base.h>
#include <e32msgqueue.h>

#include "mccdatastructures.h"


// CLASS DECLARATION

/**
*  CMccPeriodicRunner class declaration
*/
class CMccDataSender : public CActive
	{
    public:
    
	    static CMccDataSender* NewL();
		
	    ~CMccDataSender(); 
		
	public:
	 
	 	void Send( const TDataMessage& aData );
	 	
    public: // From CActive

		void DoCancel();
		void RunL();
		TInt RunError(TInt aError);	

    private:
        CMccDataSender();
	    void ConstructL(); // second construction phase

    private:
	    
		RMsgQueue<TDataMessage> iMsgQueue;
		TDataMessage iBuffer;
		 
    };


#endif