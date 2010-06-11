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


#include "mcemediasource.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcestreamobserver.h"



#define _FLAT_DATA static_cast<CMceComMediaSource*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceMediaSource::~CMceMediaSource
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSource::~CMceMediaSource()
    {
    iStreams.Reset();
    }
    
// -----------------------------------------------------------------------------
// CMceMediaSource::Type
// -----------------------------------------------------------------------------
//
EXPORT_C TMceSourceType CMceMediaSource::Type() const
    {
    return iType;
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSource::IsEnabled
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceMediaSource::IsEnabled() const
    {
    return iIsEnabled;
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::ReferenceCount
// -----------------------------------------------------------------------------
//
TInt& CMceMediaSource::ReferenceCount()
    {
    return iReferenceCount;
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::CMceMediaSource
// -----------------------------------------------------------------------------
//
CMceMediaSource::CMceMediaSource()
    :iIsEnabled( ETrue )
    {
    }


// -----------------------------------------------------------------------------
// CMceMediaSource::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::InitializeL( CMceMediaStream& aParent )
    {
    iStreams.AppendL( &aParent );
    
    }


// -----------------------------------------------------------------------------
// CMceMediaSource::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::InitializeL( CMceManager* /*aManager*/ )
    {
    // NOP
    }
    

// -----------------------------------------------------------------------------
// CMceMediaSource::DoEnableL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::DoEnableL() 
    {
    iIsEnabled = ETrue;
    }

// -----------------------------------------------------------------------------
// CMceMediaSource::DoDisableL
// -----------------------------------------------------------------------------
//
void CMceMediaSource::DoDisableL() 
    {
    
    iIsEnabled = EFalse;
       
    }
