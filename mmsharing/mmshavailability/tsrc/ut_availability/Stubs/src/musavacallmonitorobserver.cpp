/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit test stub of MMusCallMonitorObserver
*
*/


#include "musavacallmonitorobserver.h"
#include <e32base.h>


// -----------------------------------------------------------------------------
// CMusAvaCallMonitorObserverStub::NewL()
// -----------------------------------------------------------------------------
//
CMusAvaCallMonitorObserverStub* CMusAvaCallMonitorObserverStub::NewL()
	{
	CMusAvaCallMonitorObserverStub* self =
		new ( ELeave ) CMusAvaCallMonitorObserverStub();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}
	
// -----------------------------------------------------------------------------
// CMusAvaCallMonitorObserverStub::~CMusAvaCallMonitorObserverStub()
// -----------------------------------------------------------------------------
//
CMusAvaCallMonitorObserverStub::~CMusAvaCallMonitorObserverStub()
    {
    delete iTelNumber;
    }

// -----------------------------------------------------------------------------
// CMusAvaCallMonitorObserverStub::CallConnectedL()
// -----------------------------------------------------------------------------
//
void CMusAvaCallMonitorObserverStub::CallConnectedL( const TDesC& aTelNumber,
													 TBool aIsSipUri )
	{
	CopyDescriptorL( aTelNumber );
	iIsSipUri = aIsSipUri;
	}

// -----------------------------------------------------------------------------
// CMusAvaCallMonitorObserverStub::CallHoldL()
// -----------------------------------------------------------------------------
//
void CMusAvaCallMonitorObserverStub::CallHoldL( const TDesC& aTelNumber,
												TBool aIsSipUri )
	{
	CopyDescriptorL( aTelNumber );
	iIsSipUri = aIsSipUri;
	}

// -----------------------------------------------------------------------------
// CMusAvaCallMonitorObserverStub::ConferenceCallL()
// -----------------------------------------------------------------------------
//
void CMusAvaCallMonitorObserverStub::ConferenceCallL()
	{
	}

// -----------------------------------------------------------------------------
// CMusAvaCallMonitorObserverStub::NoActiveCallL()
// -----------------------------------------------------------------------------
//
void CMusAvaCallMonitorObserverStub::NoActiveCallL()
	{
	}
	
// -----------------------------------------------------------------------------
// CMusAvaCallMonitorObserverStub::CMusAvaCallMonitorObserverStub()
// -----------------------------------------------------------------------------
//
CMusAvaCallMonitorObserverStub::CMusAvaCallMonitorObserverStub()
	{
	}		

// -----------------------------------------------------------------------------
// CMusAvaCallMonitorObserverStub::ConstructL()
// -----------------------------------------------------------------------------
//
void CMusAvaCallMonitorObserverStub::ConstructL()
	{
	}

// -----------------------------------------------------------------------------
// CMusAvaCallMonitorObserverStub::CopyDescriptorL()
// -----------------------------------------------------------------------------
//
void CMusAvaCallMonitorObserverStub::CopyDescriptorL( const TDesC& aSource )
	{
	HBufC* newDescriptor = aSource.AllocL();
	delete iTelNumber;
	iTelNumber = newDescriptor;
	}
