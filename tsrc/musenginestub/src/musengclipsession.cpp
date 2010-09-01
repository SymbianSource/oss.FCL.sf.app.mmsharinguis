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
#include "musengclipsession.h"
#include "musengclipsessionobserver.h"
#include "musengsessionobserver.h"
#include "musengmceoutsession.h"
#include "musenglivesession.h"


// SYSTEM
/*#include <mcesession.h>
#include <mcefilesource.h>
#include <mcemediastream.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <drmcommon.h>
*/

// CONSTANTS

const TInt KMicroSecondsInOneSecond = 1000000;


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngClipSession* CMusEngClipSession::NewL(
                        const TDesC& aFileName,
                        const TRect& aRect,
                        MMusEngClipSessionObserver* aSessionObserver,
                        TUint aSipProfileId )
    {
    CMusEngClipSession* self = new( ELeave ) CMusEngClipSession(
                                                    aSessionObserver,
                                                    aRect,
                                                    aSipProfileId );
    CleanupStack::PushL( self );
    self->ConstructL( aFileName );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipSession::~CMusEngClipSession()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::ChangeClipL( const TDesC& aFileName )
    {
    }


// -----------------------------------------------------------------------------
// TODO: At the moment behavior of MCE is not known and all the FFWD commands
// are only forwarded to MCE.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::FastForwardL( TBool aUseFFWD )
    {
    }


// -----------------------------------------------------------------------------
// TODO: At the moment behavior of MCE is not known and all the FRWD commands
// are only forwarded to MCE.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::FastRewindL( TBool aUseFRWD )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalSeconds CMusEngClipSession::PositionL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalSeconds CMusEngClipSession::DurationL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::SetPositionL (
                        const TTimeIntervalSeconds& aPosition )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::SetSessionObserver(
                        MMusEngClipSessionObserver* aSessionObserver)
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::PlayL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::PauseL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::CompleteSessionStructureL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipSession::CMusEngClipSession(
                        MMusEngClipSessionObserver* aSessionObserver,
                        const TRect& aRect,
                        TUint aSipProfileId )
    : CMusEngMceOutSession( aRect, aSipProfileId )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::ConstructL( const TDesC& aFileName )
    {
    }


// -----------------------------------------------------------------------------
// Check is file DRM protected.
//
// -----------------------------------------------------------------------------
//
TBool CMusEngClipSession::IsProtectedFileL( const TDesC& aClipFile )
    {
    }


// -----------------------------------------------------------------------------
// If member file contains audio, add appropriate amount of audio streams to
// session structure
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::ConstructAudioStructureL()
    {
    }


