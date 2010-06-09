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



#include "musuibitmapdecoder.h"
#include "musuibitmapdecoderobserver.h"
#include "muslogger.h" // debug logging

#include <imageconversion.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiBitmapDecoder::~CMusUiBitmapDecoder()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBitmapDecoder::~CMusUiBitmapDecoder" );
    Cancel();
    iFs.Close();
    delete iData;
    delete iDecoder;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBitmapDecoder::~CMusUiBitmapDecoder" );
    }


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusUiBitmapDecoder* CMusUiBitmapDecoder::NewL(
                        MMusUiBitmapDecoderObserver& aObserver )
    {
    CMusUiBitmapDecoder* self = new (ELeave) CMusUiBitmapDecoder( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiBitmapDecoder::CMusUiBitmapDecoder(
                        MMusUiBitmapDecoderObserver& aObserver ) :
    CActive( EPriorityNormal ),
    iObserver( aObserver )
    {
    // nothing
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusUiBitmapDecoder::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBitmapDecoder::ConstructL" );
    User::LeaveIfError( iFs.Connect() );
    //delete iData;
    iData = NULL;
    //delete iDecoder;
    iDecoder = NULL;
    CActiveScheduler::Add( this );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBitmapDecoder::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiBitmapDecoder::ConvertL( HBufC8* aData, CFbsBitmap* aBitmap )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBitmapDecoder::ConvertL" );

    if ( IsActive() )
        {
        Cancel();
        }

    iData = aData->AllocL();
    iBitmap = aBitmap;

    delete iDecoder;
    iDecoder = NULL;
    iDecoder = CImageDecoder::DataNewL( iFs, *iData );

    iBitmap->Create( iDecoder->FrameInfo().iFrameCoordsInPixels.Size(),
                     iDecoder->FrameInfo().iFrameDisplayMode );

    iDecoder->Convert( &iStatus, *iBitmap, 0 );
    SetActive();

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBitmapDecoder::ConvertL" );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiBitmapDecoder::DoCancel()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBitmapDecoder::DoCancel" );
    iDecoder->Cancel();
    Cancel();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBitmapDecoder::DoCancel" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiBitmapDecoder::RunL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBitmapDecoder::RunL" );
    if (iStatus == KErrNone)
        {
        iObserver.UpdateBitmap( iBitmap );
        }
    else
        {
        MUS_LOG1( "mus: [MUSUI ]        CMusUiBitmapDecoder::RunL: iStatus: [%d]",
                                iStatus.Int() );
        User::Leave( iStatus.Int() );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBitmapDecoder::RunL" );
    }

// -------------------------------------------------------------------------
//  If RunL() leaves,It should be handled here.
// -------------------------------------------------------------------------
//
TInt CMusUiBitmapDecoder::RunError( TInt aError )
    {
	MUS_LOG( "mus: [MUSUI ]  -> CMusUiBitmapDecoder::RunError" );
    // Nothing can be done here.
    aError = KErrNone;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBitmapDecoder::RunError" );
    return aError;
    }


// end of file
