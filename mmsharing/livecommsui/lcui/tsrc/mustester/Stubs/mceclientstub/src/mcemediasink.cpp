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


#include "mcemediasink.h"
#include "mcestreamobserver.h"
#include "mcesession.h"
#include "mcemanager.h"



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceMediaSink::~CMceMediaSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSink::~CMceMediaSink()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSink::Type
// -----------------------------------------------------------------------------
//
EXPORT_C TMceSinkType CMceMediaSink::Type() const
    {
    return iType;
    }
    


// -----------------------------------------------------------------------------
// CMceMediaSink::IsEnabled
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceMediaSink::IsEnabled() const
    {
    return iIsEnabled;
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSink::CMceMediaSink
// -----------------------------------------------------------------------------
//
CMceMediaSink::CMceMediaSink()
    :iIsEnabled( ETrue ),
	 iReferenceCount( 0 )
    {
    }


// -----------------------------------------------------------------------------
// CMceMediaSink::DoEnableL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::DoEnableL() 
    {
    iIsEnabled = ETrue;
    }


// --------------------------------------- --------------------------------------
// CMceMediaSink::DoDisableL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::DoDisableL() 
    {
    iIsEnabled = EFalse;
    }


// -----------------------------------------------------------------------------
// CMceMediaSink::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::InitializeL( CMceMediaStream& aParent )
    {
    iStream = &aParent;
    }


// -----------------------------------------------------------------------------
// CMceMediaSink::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaSink::InitializeL( CMceManager* /*aManager*/ )
    {
    // NOP
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSink::ReferenceCount
// -----------------------------------------------------------------------------
//
TInt& CMceMediaSink::ReferenceCount()
    {
    return iReferenceCount;
    }

