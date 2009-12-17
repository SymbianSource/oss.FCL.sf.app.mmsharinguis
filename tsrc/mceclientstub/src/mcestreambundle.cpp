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


#include "mcestreambundle.h"
#include "mcesession.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceStreamBundle::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceStreamBundle* CMceStreamBundle::NewL( TMceStreamBundleType aType )
    {
    CMceStreamBundle* self = NewLC( aType );
    CleanupStack::Pop( self );
    return self;   
    
    }

// -----------------------------------------------------------------------------
// CMceStreamBundle::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceStreamBundle* CMceStreamBundle::NewLC( TMceStreamBundleType aType )
    {
    CMceStreamBundle* self = new (ELeave) CMceStreamBundle();
    CleanupStack::PushL( self );
    self->ConstructL( aType );
    return self;
    }
     
// -----------------------------------------------------------------------------
// CMceStreamBundle::~CMceStreamBundle
// -----------------------------------------------------------------------------
//
EXPORT_C CMceStreamBundle::~CMceStreamBundle()
    {
    iStreams.Close();
    iTempStreams.Close();
    }

// -----------------------------------------------------------------------------
// CMceStreamBundle::Type
// -----------------------------------------------------------------------------
//
EXPORT_C CMceStreamBundle::TMceStreamBundleType CMceStreamBundle::Type() const
    {
    return iBundleType;
    }
    
// -----------------------------------------------------------------------------
// CMceStreamBundle::AddStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceStreamBundle::AddStreamL( CMceMediaStream& aStream )
    {
    iStreams.AppendL( &aStream );
    }
        
// -----------------------------------------------------------------------------
// CMceStreamBundle::RemoveStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceStreamBundle::RemoveStreamL( CMceMediaStream& aStream )
    {
    TInt index = iStreams.Find( &aStream );
    User::LeaveIfError( index );
    iStreams.Remove( index );
    }
        
// -----------------------------------------------------------------------------
// CMceStreamBundle::Streams
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray< CMceMediaStream >& CMceStreamBundle::Streams()
    {
    return iStreams;
    }
 
 
// -----------------------------------------------------------------------------
// CMceStreamBundle::InitializeL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::InitializeL( CMceSession& aParent )
    {
    iSession = &aParent;
    }
    
// -----------------------------------------------------------------------------
// CMceStreamBundle::CMceStreamBundle
// -----------------------------------------------------------------------------
//
CMceStreamBundle::CMceStreamBundle()
    {
    }

// -----------------------------------------------------------------------------
// CMceStreamBundle::ConstructL
// -----------------------------------------------------------------------------
//
void CMceStreamBundle::ConstructL( CMceStreamBundle::TMceStreamBundleType aType )
    {
    iBundleType = aType;
    
    }

