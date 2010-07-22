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
#include "mccdatasender.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccDataSender::CMccDataSender
// -----------------------------------------------------------------------------
//
CMccDataSender::CMccDataSender( )
	: CActive( EPriorityStandard )
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CMccDataSender::ConstructL
// -----------------------------------------------------------------------------
//
void CMccDataSender::ConstructL()
    {
    TInt err = iMsgQueue.CreateGlobal(_L("IP-VT-Queue1"), 5, EOwnerProcess );
    if ( err )
        {
        User::LeaveIfError( iMsgQueue.OpenGlobal(_L("IP-VT-Queue1")));
        }
    }

// -----------------------------------------------------------------------------
// CMccDataSender::NewL
// -----------------------------------------------------------------------------
//
CMccDataSender* CMccDataSender::NewL( )
    {
    CMccDataSender* self = 
    	new ( ELeave ) CMccDataSender();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccDataSender::Send
// -----------------------------------------------------------------------------
//
void CMccDataSender::Send( const TDataMessage& aData )
{	
    TInt err = iMsgQueue.Send( aData );
    
    if ( err == KErrOverflow )
        {
        if ( !IsActive() )
            {
            iBuffer = aData;            
            iMsgQueue.NotifySpaceAvailable( iStatus );    
    	    SetActive();
            }
        else
            {
            // TODO
            // BUFFER OR DROP
            }
        }
	}
	
// -----------------------------------------------------------------------------
// CMccDataSender::~CMccDataSender
// -----------------------------------------------------------------------------
//
CMccDataSender::~CMccDataSender()
    {
    Cancel();
	iMsgQueue.Close();
    }

// -----------------------------------------------------------------------------
// CMccDataSender::DoCancel
// -----------------------------------------------------------------------------
//
void CMccDataSender::DoCancel()
	{
	iMsgQueue.CancelSpaceAvailable();	
	}

// -----------------------------------------------------------------------------
// CMccDataSender::RunL
// -----------------------------------------------------------------------------
//	
void CMccDataSender::RunL()
	{
    iMsgQueue.Send( iBuffer );
    iBuffer.iDataId = EUninitialized;
    }

// -----------------------------------------------------------------------------
// CMccDataSender::RunError
// -----------------------------------------------------------------------------
//	
TInt CMccDataSender::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}
