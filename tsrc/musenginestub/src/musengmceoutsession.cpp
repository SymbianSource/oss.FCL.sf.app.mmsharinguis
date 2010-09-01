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
#include "musengmceoutsession.h"
#include "musenglivesessionobserver.h"
//#include "mussipprofilehandler.h"

// SYSTEM
/*
#include <mcemediastream.h>
#include <mceoutsession.h>
#include <mcevideostream.h>
#include <mcemediasource.h>
#include <mcefilesource.h>
#include <mcemediasink.h>
#include <mcertpsink.h>
#include <mcespeakersink.h>
#include <mcedisplaysink.h>

#include <sipprofile.h>
#include <uri8.h>
#include <charconv.h>
#include <f32file.h>
#include <utf.h>
*/
const TInt KMaxUriLength = 512;
_LIT8( KMusEngAtSign, "@" );
_LIT8( KMusEngPlusSign, "+" );
_LIT8( KMusEngSipPrefix, "sip:" );
_LIT8( KMusEngTelPrefix, "tel:" );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceOutSession::~CMusEngMceOutSession()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceOutSession::InviteL( const TDesC& aRecipient )
    {
    iInvited = ETrue;
    User::LeaveIfError( iErrorCode );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceOutSession::CancelInviteL()
    {
    iInvited = EFalse;
    User::LeaveIfError( iErrorCode );
    }


// -----------------------------------------------------------------------------
// Handle MCE session termination. Called by MCE observer function of the
// base class.
// -----------------------------------------------------------------------------
//

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceOutSession::CMusEngMceOutSession( const TRect& aRect,
                                            TUint aSipProfileId )
    : CMusEngMceSession( aRect ),
      iSipProfileId( aSipProfileId )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::CreateMceSessionStructureL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MMusEngOutSessionObserver* CMusEngMceOutSession::OutSessionObserver()
    {
    return static_cast<MMusEngOutSessionObserver*>(iSessionObserver);
    }


// -----------------------------------------------------------------------------
// TODO: Using UTF8 is to wide. It should handle the escape characters
// correctly. UTF7 is not working, since it converts + as +-
// -----------------------------------------------------------------------------
//
HBufC8* CMusEngMceOutSession::ParseRecipientUriL( const TDesC& aRecipient )
    {
    }



