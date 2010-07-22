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


#include "muscommonstubs.h"


// ---- MEMBER FUNCTIONS OF CMusEngObserverStub --------------------------------


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusCommonObserverStub::CMusCommonObserverStub()
    :iCallEvent( static_cast<RMobileCall::TMobileCallEvent>(-1) )
    {
    // NOP
    }
        

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusCommonObserverStub::~CMusCommonObserverStub()
    {
    delete iTelNumber;
    iTelNumber = NULL;
    }


// ----- From MMusCallMonitorObserver ------------------------------------------

 
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusCommonObserverStub::CallConnectedL( const TDesC& aTelNumber,
											 TBool aIsSipUri )
    {
    delete iTelNumber;
    iTelNumber = NULL;
    iTelNumber = aTelNumber.AllocL();
    iCallConnectedCalled = ETrue;
    iIsSipUri = aIsSipUri;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	    
void CMusCommonObserverStub::CallHoldL( const TDesC& aTelNumber,
										TBool aIsSipUri )
    {
    delete iTelNumber;
    iTelNumber = NULL;
    iTelNumber = aTelNumber.AllocL();
    iCallHoldCalled = ETrue;
    iIsSipUri = aIsSipUri;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusCommonObserverStub::ConferenceCallL()
    {
    iConferenceCallCalled = ETrue;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusCommonObserverStub::NoActiveCallL()
    {
    iNoActiveCallCalled = ETrue;
    }




// ----- HELPERS ---------------------------------------------------------------


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
TBool CMusCommonObserverStub::IsReseted()
    {
    return ( iCallConnectedCalled == EFalse && 
             iCallHoldCalled == EFalse &&
             iConferenceCallCalled == EFalse &&
             iNoActiveCallCalled == EFalse &&
             !iTelNumber &&
             iCallEvent == -1 );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusCommonObserverStub::Reset()
    {
    iCallConnectedCalled = EFalse;
    iCallHoldCalled = EFalse;
    iConferenceCallCalled = EFalse;
    iNoActiveCallCalled = EFalse;
    
    delete iTelNumber;
    iTelNumber = NULL;
    
    iCallEvent = static_cast<RMobileCall::TMobileCallEvent>(-1);
    }

