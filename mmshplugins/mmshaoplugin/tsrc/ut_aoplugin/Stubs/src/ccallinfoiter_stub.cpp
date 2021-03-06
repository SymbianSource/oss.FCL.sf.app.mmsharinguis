/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Iterator class for going through ongoing calls.
*
*/


#include <callinformation.h>    //Call Information Mediator API
#include "ccallinfoiter.h"
#include "mcall.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCallInfoIter::CCallInfoIter( CCallInfos& aCallInfos ): iCallInfos( aCallInfos )
    { 
    First();    
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallInfoIter::~CCallInfoIter()
    {
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCallInfoIter* CCallInfoIter::NewLC( CCallInfos& aCallInfos )
    {
    CCallInfoIter* self = new (ELeave)CCallInfoIter( aCallInfos );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCallInfoIter* CCallInfoIter::NewL( CCallInfos& aCallInfos )
    {
    CCallInfoIter* self = CCallInfoIter::NewLC( aCallInfos );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CCallInfoIter::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Returns the amount of calls
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CCallInfoIter::Count() const
    {
    TUint count = 1;
    return count; 
    }

// ---------------------------------------------------------------------------
// Returns the current call.
// ---------------------------------------------------------------------------
//
EXPORT_C const MCall& CCallInfoIter::Current() const
    {
    return iCallInfos.CallInfos()[iCurrentIndex];       
    }

// ---------------------------------------------------------------------------
// Intializes the current call to the first call.
// ---------------------------------------------------------------------------
//
EXPORT_C void CCallInfoIter::First()
    {
    iCurrentIndex = 0; 
    }


// End of File.
