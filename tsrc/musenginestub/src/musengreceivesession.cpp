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



// USER INCLUDES

#include "musengreceivesession.h"
#include "musengreceivesessionobserver.h"
//#include "musengmceutils.h"
//#include "muslogger.h"

// SYSTEM INCLUDES
#include <mcemanager.h>
#include <mceinsession.h>
#include <mceaudiostream.h>
#include <mcevideostream.h>
#include <mcemediasource.h>
#include <mcertpsource.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>

#include <utf.h>


const TInt KMaxUriLength = 512;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngReceiveSession::~CMusEngReceiveSession()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngReceiveSession* CMusEngReceiveSession::NewL(
                        const TRect& aRect,
                        MMusEngReceiveSessionObserver* aSessionObserver)
    {
    CMusEngReceiveSession* self =
                new( ELeave ) CMusEngReceiveSession( aSessionObserver, aRect );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngReceiveSession::AcceptInvitationL(
                        const TBool& aAccept )
    {
    iAccepInvitation = aAccept;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngReceiveSession::SetSessionObserver(
                        MMusEngReceiveSessionObserver* aSessionObserver)
    {
    iSessionObserver = aSessionObserver;
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngReceiveSession::IsRtpcInactivityTimoutSupported()
{
    return ETrue;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngReceiveSession::CMusEngReceiveSession(
                        MMusEngReceiveSessionObserver* aSessionObserver,
                        const TRect& aRect )
    : CMusEngMceSession( aRect )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::ConstructL()
    {
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MMusEngReceiveSessionObserver* CMusEngReceiveSession::ReceiveSessionObserver()
    {
    }

