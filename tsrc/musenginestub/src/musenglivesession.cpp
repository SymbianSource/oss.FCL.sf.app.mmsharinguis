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


// USER
#include "musenglivesession.h"
#include "musenglivesessionobserver.h"
//#include "musunittesting.h"
//#include "musengmceutils.h"

// SYSTEM
/*
#include <mcecamerasource.h>
#include <mcevideostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcesession.h>
#include <mcevideocodec.h>
*/



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngLiveSession* CMusEngLiveSession::NewL(
                    const TDesC& /*aFileName*/,
                    const TRect& aRect,
                    MMusEngLiveSessionObserver* aSessionObserver,
                    TUint aSipProfileId )
    {
    CMusEngLiveSession* self = new( ELeave ) CMusEngLiveSession(
                                                    aSessionObserver,
                                                    aRect,
                                                    aSipProfileId );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngLiveSession* CMusEngLiveSession::NewL(
                        const TRect& aRect,
                        MMusEngLiveSessionObserver* aSessionObserver,
                        TUint aSipProfileId )
    {
    CMusEngLiveSession* self = new( ELeave ) CMusEngLiveSession(
                                                    aSessionObserver,
                                                    aRect,
                                                    aSipProfileId);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::~CMusEngLiveSession()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::SetSessionObserver(
                        MMusEngLiveSessionObserver* aSessionObserver )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngLiveSession::CurrentZoomL() const
    {
    return iCurrentZoom;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngLiveSession::MaxZoomL() const
    {
    return iCurrentZoom;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngLiveSession::MinZoomL() const
    {
    return iCurrentZoom;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::ZoomInL()
    {
    iCurrentZoom++;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::ZoomOutL()
    {
    iCurrentZoom--;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::ZoomDefaultL()
    {
    }


// -----------------------------------------------------------------------------
// Enable camera
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::PlayL()
    {
    iPlaying = ETrue;
    }


// -----------------------------------------------------------------------------
// Disable camera
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::PauseL()
    {
    iPlaying = EFalse;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::CompleteSessionStructureL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::CMusEngLiveSession(
                        MMusEngLiveSessionObserver* aSessionObserver,
                        const TRect& aRect,
                        TUint aSipProfileId )
    :CMusEngMceOutSession( aRect, aSipProfileId ),
     iDefaultZoomFactor( -1 )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::ConstructL( /*const TDesC& aFileName*/ )
    {
    }






