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

        
// INCLUDE FILES
#include "mccdatareceiver.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccDataReceiver::CMccDataReceiver
// -----------------------------------------------------------------------------
//
CMccDataReceiver::CMccDataReceiver( MMccDataReceiverObserver& aObserver )
	: CActive( EPriorityStandard ), iObserver( &aObserver )
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CMccDataReceiver::ConstructL
// -----------------------------------------------------------------------------
//
void CMccDataReceiver::ConstructL()
    {

    TInt err = iMsgQueue.CreateGlobal(_L("IP-VT-Queue2"), 5, EOwnerProcess );
    if ( err )
        {
        User::LeaveIfError( iMsgQueue.OpenGlobal(_L("IP-VT-Queue2")));
        }

    iMsgQueue.ReceiveBlocking( iInitData );

    iMsgQueue.NotifyDataAvailable( iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CMccDataReceiver::NewL
// -----------------------------------------------------------------------------
//
CMccDataReceiver* CMccDataReceiver::NewL( MMccDataReceiverObserver& aObserver )
    {
    CMccDataReceiver* self = 
    	new ( ELeave ) CMccDataReceiver( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
	
// -----------------------------------------------------------------------------
// CMccDataReceiver::~CMccDataReceiver
// -----------------------------------------------------------------------------
//
CMccDataReceiver::~CMccDataReceiver()
    {
    Cancel();
	iMsgQueue.Close();
    }

// -----------------------------------------------------------------------------
// CMccDataReceiver::DoCancel
// -----------------------------------------------------------------------------
//
void CMccDataReceiver::DoCancel()
	{
	iMsgQueue.CancelDataAvailable();	
	}

// -----------------------------------------------------------------------------
// CMccDataReceiver::RunL
// -----------------------------------------------------------------------------
//	
void CMccDataReceiver::RunL()
	{
	TDataMessage buffer;
	iMsgQueue.Receive( buffer );
	iObserver->DataReceived( buffer );
        iMsgQueue.NotifyDataAvailable( iStatus );
        SetActive();
	}

// -----------------------------------------------------------------------------
// CMccDataReceiver::RunError
// -----------------------------------------------------------------------------
//	
TInt CMccDataReceiver::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}
