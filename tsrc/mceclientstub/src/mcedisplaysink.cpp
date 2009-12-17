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


#include "mcedisplaysink.h"
#include "mcesession.h"
#include "mcemediastream.h"
#include "mcemanager.h"

//#include <w32std.h>


#define _FLAT_DATA static_cast<CMceComDisplaySink*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceDisplaySink* CMceDisplaySink::NewL( CMceManager& aManager )
    {
    CMceDisplaySink* self = CMceDisplaySink::NewLC( aManager );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceDisplaySink* CMceDisplaySink::NewLC( CMceManager& aManager )
    {
    CMceDisplaySink* self = new (ELeave) CMceDisplaySink();
    CleanupStack::PushL( self );
    self->ConstructL( &aManager );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceDisplaySink::~CMceDisplaySink()
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::EnableL()
    {
    CMceMediaSink::DoEnableL();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::DisableL()
    {
    CMceMediaSink::DoDisableL();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDisplaySink::DisplaysAvailable() const
    {
    return 1;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::SetDisplayIndexL( TInt aDisplayIndex )
    {
    __ASSERT_ALWAYS( aDisplayIndex >= 0 &&
                     aDisplayIndex < this->DisplaysAvailable(),
                     User::Leave( KErrArgument ) );
                     
    iDisplayIndex = aDisplayIndex;
         
    }   
	    
	    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMceDisplaySink::SetDisplay( RWindow& /*aWindow*/, 
                                           CWindowGc& /*aGc*/ )
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::SetDisplayRectL( const TRect& aRect )
    {
    iDisplayRect = aRect;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C const TRect CMceDisplaySink::DisplayRectL()
    {
    return iDisplayRect;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::SetPriorityL( TUint aPriority )
    {
    iDisplayPriority = aPriority; 
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceDisplaySink::PriorityL()
    {
    return iDisplayPriority; 
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::SetRotationL( TRotation aRotation )
    {
    iRotation = aRotation;
    }
		

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C CMceDisplaySink::TRotation CMceDisplaySink::RotationL()
    {
    return iRotation;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMceDisplaySink::InitializeL( CMceManager* /*aManager*/ )
    {
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMceDisplaySink::CMceDisplaySink()
    :CMceMediaSink(),
    iDisplayIndex( 0 ),
    iDisplayRect( TRect( 0, 0, 0, 0 ) ),
    iDisplayPriority( 0 ),
    iRotation( ENone )
    {
    iType = KMceDisplaySink;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMceDisplaySink::ConstructL( CMceManager* /*aManager*/ )
    {
    }





