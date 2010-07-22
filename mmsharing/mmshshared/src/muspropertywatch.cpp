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



#include "muspropertywatch.h"
#include "muspropertyobserver.h"
#include "mussessionproperties.h"
#include "musresourceproperties.h"
#include "muslogger.h" // debug logging

using namespace NMusSessionApi;

// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusPropertyWatch* CMusPropertyWatch::NewL(
                                      MMusPropertyObserver& aObserver,
                                      TUid aCategory,
                                      const TInt aPropertyName )
    {
    CMusPropertyWatch* self = new (ELeave) CMusPropertyWatch(
                                            aObserver,
                                            aCategory,
                                            aPropertyName );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusPropertyWatch::~CMusPropertyWatch()
    {
    MUS_LOG( "mus: [MUSSH ]  -> CMusPropertyWatch::~CMusPropertyWatch" );
    if ( IsActive() )
        {
        Cancel();
        }
    iProperty.Close();
    MUS_LOG( "mus: [MUSSH ]  <- CMusPropertyWatch::~CMusPropertyWatch" );
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusPropertyWatch::CMusPropertyWatch( MMusPropertyObserver& aObserver,
                                          TUid aCategory,
                                          const TInt aPropertyName ) :
    CActive( EPriorityNormal ),
    iObserver( aObserver )
    {
    iPropertyCategory = aCategory;
    iPropertyKey = aPropertyName;
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusPropertyWatch::ConstructL( )
    {
    MUS_LOG( "mus: [MUSSH ]  -> CMusPropertyWatch::ConstructL" );
    User::LeaveIfError( iProperty.Attach( iPropertyCategory,
                                          iPropertyKey) );
    CActiveScheduler::Add( this );
    
    iProperty.Subscribe( iStatus );
    SetActive();
    
    MUS_LOG( "mus: [MUSSH ]  <- CMusPropertyWatch::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusPropertyWatch::ReadIntPropertyL( TUint aKey )
    {
    MUS_LOG1( "mus: [MUSSH ]     -> CMusPropertyWatch::ReadIntPropertyL: aKey: [%d]",
                            aKey );
    TInt val;
    User::LeaveIfError( RProperty::Get( KCategoryUid,
                                        aKey,
                                        val ) );
    MUS_LOG1( "mus: [MUSSH ]     <- CMusPropertyWatch::ReadIntPropertyL: val: [%d]",
                            val );
    return val;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC* CMusPropertyWatch::ReadDescPropertyL( TUint aKey )
    {
    MUS_LOG1( "mus: [MUSSH ]     -> CMusPropertyWatch::ReadDescPropertyL: aKey: [%d]",
                            aKey );
    TBuf<RProperty::KMaxPropertySize> buffer;

    User::LeaveIfError( RProperty::Get( KCategoryUid,
                                        aKey,
                                        buffer ) );
    HBufC* desc = buffer.AllocL();
    MUS_LOG_TDESC( "mus: [MUSSH ]    <- CMusPropertyWatch::ReadDescPropertyL: val: ",
                            (*desc) );
    return desc;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusPropertyWatch::DoCancel()
    {
    MUS_LOG( "mus: [MUSSH ]  -> CMusPropertyWatch::DoCancel" );
    iProperty.Cancel();
    MUS_LOG( "mus: [MUSSH ]  <- CMusPropertyWatch::DoCancel" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusPropertyWatch::RunL()
    {
    MUS_LOG( "mus: [MUSSH ]  -> CMusPropertyWatch::RunL" );
    TInt val;

    User::LeaveIfError( RProperty::Get( iPropertyCategory,
                                        iPropertyKey,
                                        val ) );

    iObserver.PropertyChanged( iPropertyKey, val );
    MUS_LOG( "mus: [MUSSH ]  <- CMusPropertyWatch::RunL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusPropertyWatch::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSSH ]     -> CMusPropertyWatch::RunError [%d]", aError );
    iObserver.HandlePropertyError( aError );
    MUS_LOG( "mus: [MUSSH ]  <- CMusPropertyWatch::RunError" );
    return KErrNone;
    }


// end of file
