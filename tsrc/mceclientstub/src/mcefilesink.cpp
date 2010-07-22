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



#include "mcefilesink.h"



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSink* CMceFileSink::NewL(
            const TFileName& aFileName )
    {
    CMceFileSink* self = CMceFileSink::NewLC( aFileName );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSink* CMceFileSink::NewLC(
            const TFileName& aFileName )
    {
    CMceFileSink* self = new (ELeave) CMceFileSink();
    CleanupStack::PushL( self );
    self->ConstructL( aFileName );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSink::~CMceFileSink()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceFileSink::EnableL()
    {
    CMceMediaSink::DoEnableL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceFileSink::DisableL()
    { 
    CMceMediaSink::DoDisableL();
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
CMceFileSink::CMceFileSink()
    {    
    iType = KMceFileSink;    
    }
    
 
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
void CMceFileSink::ConstructL( const TFileName& aFileName )
    { 
    iFileName = aFileName;
    }
