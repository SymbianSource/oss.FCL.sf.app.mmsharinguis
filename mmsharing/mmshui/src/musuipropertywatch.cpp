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



#include "musuipropertywatch.h"
#include "musuipropertyobserver.h"
#include "mussessionproperties.h"
#include "musresourceproperties.h"
#include "muslogger.h" // debug logging

using namespace NMusSessionApi;

// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusUiPropertyWatch* CMusUiPropertyWatch::NewL(
                                      MMusUiPropertyObserver& aObserver,
                                      TUid aCategory,
                                      const TInt aPropertyName )
    {
    CMusUiPropertyWatch* self = new (ELeave) CMusUiPropertyWatch(
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
CMusUiPropertyWatch::~CMusUiPropertyWatch()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiPropertyWatch::~CMusUiPropertyWatch" );
    if ( IsActive() )
        {
        Cancel();
        }
    iProperty.Close();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiPropertyWatch::~CMusUiPropertyWatch" );
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusUiPropertyWatch::CMusUiPropertyWatch( MMusUiPropertyObserver& aObserver,
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
void CMusUiPropertyWatch::ConstructL( )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiPropertyWatch::ConstructL" );
    User::LeaveIfError( iProperty.Attach( iPropertyCategory,
                                          iPropertyKey) );
    CActiveScheduler::Add( this );
    RunL();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiPropertyWatch::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiPropertyWatch::ReadIntPropertyL( TUint aKey )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiPropertyWatch::ReadIntPropertyL: aKey: [%d]",
                            aKey );
    TInt val;
    User::LeaveIfError( RProperty::Get( KCategoryUid,
                                        aKey,
                                        val ) );
    MUS_LOG1( "mus: [MUSUI ]     <- CMusUiPropertyWatch::ReadIntPropertyL: val: [%d]",
                            val );
    return val;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC* CMusUiPropertyWatch::ReadDescPropertyL( TUint aKey )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiPropertyWatch::ReadDescPropertyL: aKey: [%d]",
                            aKey );
    TBuf<RProperty::KMaxPropertySize> buffer;

    User::LeaveIfError( RProperty::Get( KCategoryUid,
                                        aKey,
                                        buffer ) );
    HBufC* desc = buffer.AllocL();
    MUS_LOG_TDESC( "mus: [MUSUI ]    <- CMusUiPropertyWatch::ReadDescPropertyL: val: ",
                            (*desc) );
    return desc;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiPropertyWatch::DoCancel()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiPropertyWatch::DoCancel" );
    iProperty.Cancel();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiPropertyWatch::DoCancel" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiPropertyWatch::RunL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiPropertyWatch::RunL" );
    TInt val;

    User::LeaveIfError( RProperty::Get( iPropertyCategory,
                                        iPropertyKey,
                                        val ) );
    iProperty.Subscribe( iStatus );
    SetActive();

    iObserver.PropertyChanged( iPropertyKey, val );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiPropertyWatch::RunL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiPropertyWatch::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiPropertyWatch::RunError [%d]", aError );
    iObserver.HandlePropertyError( aError );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiPropertyWatch::RunError" );
    return KErrNone;
    }


// end of file
