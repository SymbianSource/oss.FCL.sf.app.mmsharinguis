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


#include "musengsession.h"
#include "musengpreviewsession.h"

#include "musengpreviewsessionobserver.h"

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::CMusEngPreviewSession
// -----------------------------------------------------------------------------
//
CMusEngPreviewSession::CMusEngPreviewSession( MMusEngPreviewSessionObserver* aSessionObserver,
                                                const TRect& aRect )
                                                : CMusEngSession( aRect )
    {
    iSessionObserver = aSessionObserver;
    }

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::~CMusEngPreviewSession
// -----------------------------------------------------------------------------
//
CMusEngPreviewSession::~CMusEngPreviewSession()
    {
    }

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::~CMusEngPreviewSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngPreviewSession* CMusEngPreviewSession::NewL( const TDesC& aFileName,
                                                         const TRect& aRect,
                                                         MMusEngPreviewSessionObserver* aSessionObserver)
    {
    CMusEngPreviewSession* self = NULL;
    self = new( ELeave ) CMusEngPreviewSession( aSessionObserver, aRect);
    CleanupStack::PushL( self );
    self->ConstructL(/*aFileName*/);
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMusEngPreviewSession::ConstructL( /*const TDesC& aFileName*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::SetClipSessionObserver
// -----------------------------------------------------------------------------
//
void CMusEngPreviewSession::SetSessionObserver(MMusEngPreviewSessionObserver* aSessionObserver)
    {
        iSessionObserver = aSessionObserver;
    }

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::Play
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngPreviewSession::Play()
    {
    }

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::Pause
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngPreviewSession::Pause()
    {
    }

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::Close
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngPreviewSession::Close()
    {
    }

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::Mute
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngPreviewSession::Mute()
    {
    }

// -----------------------------------------------------------------------------
// CMusEngPreviewSession::Unmute
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngPreviewSession::Unmute()
    {
    }




