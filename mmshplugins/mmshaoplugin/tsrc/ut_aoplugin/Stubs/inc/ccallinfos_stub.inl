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
* Description:  ?Description
*
*/


#include <s32mem.h> 

const TInt KCallServiceId = 5;

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CCallInfos::CCallInfos()
    {
    //Nothing to be done Here.
    }

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
//
inline CCallInfos* CCallInfos::NewL()
    {
    return new( ELeave )CCallInfos();
    }

// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
//
inline CCallInfos* CCallInfos::NewLC()
    {
    CCallInfos* self = new( ELeave )CCallInfos();
    CleanupStack::PushL( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Desctructor.
// -----------------------------------------------------------------------------
//
inline CCallInfos::~CCallInfos()
    {
    if ( iCallInfos.Count())
        {
        iCallInfos.Reset();
        }
    iCallInfos.Close();
    }

// -----------------------------------------------------------------------------
// Returns information about all calls.
// -----------------------------------------------------------------------------
//
inline const RArray<TCallInfo>&  CCallInfos::CallInfos( ) const
    {
    return iCallInfos;
    }



// -----------------------------------------------------------------------------
// Add Dummy Call Info.
// -----------------------------------------------------------------------------
//
inline void  CCallInfos::AddCallInfosL( )
    {
    }


// End of File.

 

