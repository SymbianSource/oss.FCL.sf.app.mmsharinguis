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


// USER INCLUDES
#include "musengsession.h"
#include "musengsessionobserver.h"
#include "musengtelephoneutils.h"

// SYSTEM INCLUDES

//#include <e32cmn.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngSession::~CMusEngSession() // destruct - virtual, so no export
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CMusEngSession::Rect() const
    {
    return iRect;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::SetRectL( const TRect& aRect )
    {
    iRect = aRect;
    User::LeaveIfError( iErrorCode );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::EnableLoudspeakerL( TBool aEnable,
                                                  TBool aShowDialog )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngSession::IsLoudSpeakerEnabled() const
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngSession::VolumeL() const
    {
    }


// -----------------------------------------------------------------------------
// Increases CS call volume level by one.
// Leaves if error occurs when accessing central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::VolumeUpL( )
    {
    }


// -----------------------------------------------------------------------------
// Decreases CS call volume level by one.
// Leaves if error occurs when accessing central repository.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngSession::VolumeDownL( )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngSession::CMusEngSession( const TRect& aRect )
    {
    iRect = aRect;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngSession::ConstructL() // second-phase constructor
    {
    iErrorCode = KErrNone;
    }


