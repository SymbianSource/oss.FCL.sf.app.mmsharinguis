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
#include "musengtelephoneutils.h"

// SYSTEM
/*
#include <centralrepository.h>
#include <telephonyinternalcrkeys.h>
*/


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTelephoneUtils* CMusEngTelephoneUtils::NewL()
    {
    CMusEngTelephoneUtils* self = new( ELeave ) CMusEngTelephoneUtils();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTelephoneUtils::~CMusEngTelephoneUtils()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::LoudspeakerL( TBool aEnable, TBool aShowDialog )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngTelephoneUtils::IsLoudSpeakerEnabled() const
    {
    }


// -----------------------------------------------------------------------------
// Gets the CS call volume level.
// Leaves if error occurs when accessing central repository.
// -----------------------------------------------------------------------------
//
TInt CMusEngTelephoneUtils::GetVolumeL() const
    {
    }


// -----------------------------------------------------------------------------
// Sets the CS call volume level.
// Leaves if error occurs when accessing central repository.
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::SetVolumeL( TInt aVolume )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTelephoneUtils::CMusEngTelephoneUtils()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTelephoneUtils::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// Validates that requested volume level is valid (between 1-10) and if it is
// not, modifies requested volume level to be valid.  Returns validated volume.
// -----------------------------------------------------------------------------
//
TInt CMusEngTelephoneUtils::ValidateVolume( const TInt aVolume ) const
    {
    }




