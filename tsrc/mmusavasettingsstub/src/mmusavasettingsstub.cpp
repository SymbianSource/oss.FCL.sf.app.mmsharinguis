/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Used for unit testing other classes.
*
*/


#include "mmusavasettingsstub.h"

_LIT( KTelNumber, "123" );
_LIT( KContactName, "Jane Doe" );
_LIT( KVideoCodec, "H264" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Initializes setting data.
// ---------------------------------------------------------------------------
//
void CMusAvaSettings2::ConstructL()
    {
    iTelNumber = KTelNumber().AllocL();
    iContactName = KContactName().AllocL();

    _LIT( KAddress1, "sip:stadi@hesa.fi" );

    iSipAddresses = new(ELeave)CDesCArrayFlat( 1 );
    iSipAddresses->AppendL( KAddress1() );
    iVideoCodecs = new(ELeave)CDesCArrayFlat( 1 );
    iVideoCodecs->AppendL( KVideoCodec );
    iAudioCodecs = new(ELeave)CDesCArrayFlat( 1 );
    iSipAddressProposal = KAddress1().AllocL();
    }

CMusAvaSettings2::~CMusAvaSettings2()
    {
    delete iContactName;
    delete iTelNumber;

    delete iSipAddresses;
    delete iVideoCodecs;
    delete iAudioCodecs;
    delete iSipAddressProposal;
    }

const TDesC& CMusAvaSettings2::TelNumber() const
    {
    return *iTelNumber;
    }

const TDesC& CMusAvaSettings2::ContactName() const
    {
    return *iContactName;
    }

const TDesC& CMusAvaSettings2::SipAddressProposal() const
    {
    return *iSipAddressProposal;
    }

const MDesCArray& CMusAvaSettings2::SipAddresses() const
    {
    return *iSipAddresses;
    }

TInt CMusAvaSettings2::ContactId() const
    {
    return iContactId;
    }

const MDesCArray& CMusAvaSettings2::VideoCodecs() const
    {
    return *iVideoCodecs;
    }

const MDesCArray& CMusAvaSettings2::AudioCodecs() const
    {
    return *iAudioCodecs;
    }

TInt CMusAvaSettings2::SipProfileId() const
    {
    return iSipProfileId;
    }
