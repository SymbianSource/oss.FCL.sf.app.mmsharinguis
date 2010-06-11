/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides information about ongoing calls.
*
*/

#include "ccallinformation.h"
#include "callinformation.h"
#include "ccallinfoiter.h"

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCallInformation::CCallInformation()
    {
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CCallInformation::ConstructL()
    {
    //iImpl = CCallInformationImpl::NewL();
    
    iInfos = CCallInfos::NewL();
    iInfoIter = CCallInfoIter::NewL( *iInfos );
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CCallInformation* CCallInformation::NewL()
    {
    CCallInformation* self = new (ELeave) CCallInformation( );    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallInformation::~CCallInformation()
    {
    //delete iImpl;
    if ( iInfos )
        {
        delete iInfos;
        iInfos = NULL;
        }
    if ( iInfoIter )
        {
        delete iInfoIter;
        iInfoIter = NULL;
        }
    }


// ---------------------------------------------------------------------------
// Get calls by given state. 
// ---------------------------------------------------------------------------
//

EXPORT_C CCallInfoIter& CCallInformation::GetCallsL( CCPCall::TCallState aState )
    {
    //return iImpl->GetCallsL( aState );
    // Add dummy data to call infos
    iInfos->AddCallInfosL();
    return *iInfoIter;
    }            

// End of File.
