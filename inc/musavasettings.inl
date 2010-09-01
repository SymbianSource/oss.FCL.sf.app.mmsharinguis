/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   ECOM interface default implementation. 
*
*/



#ifndef MUSAVASETTING_INL
#define MUSAVASETTING_INL

#include "musavasettings.h"

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline MMusAvaSettings::TManualActivation MMusAvaSettings::ManualActivation()
	{
	User::Leave( KErrNotSupported );
	return MMusAvaSettings::EActivationNotExecuted;	
	}
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline const TDesC& MMusAvaSettings::TelNumber() const
    {
    User::Leave( KErrNotSupported );
    
    // this line is never reached; exists only to avoid warning
    return KNullDesC();
    }    	


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline const TDesC& MMusAvaSettings::ContactName() const
    {
    User::Leave( KErrNotSupported );
    
    // this line is never reached; exists only to avoid warning
    return KNullDesC();
    }    	


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline const MDesCArray& MMusAvaSettings::SipAddresses() const
    {
    User::Leave( KErrNotSupported );

    // this line is never reached; exists only to avoid warning
    return ( MDesCArray& ) KErrNotSupported;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline const TDesC& MMusAvaSettings::SipAddressProposal() const
    {
    User::Leave( KErrNotSupported );
    
    // this line is never reached; exists only to avoid warning
    return KNullDesC();
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline TInt MMusAvaSettings::ContactId() const
    {
    User::Leave( KErrNotSupported );
    
    // this line is never reached; exists only to avoid warning
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline const MDesCArray& MMusAvaSettings::VideoCodecs() const
    {
    User::Leave( KErrNotSupported );

    // this line is never reached; exists only to avoid warning
    return ( MDesCArray& ) KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline const MDesCArray& MMusAvaSettings::AudioCodecs() const
    {
    User::Leave( KErrNotSupported );

    // this line is never reached; exists only to avoid warning
    return ( MDesCArray& ) KErrNotSupported;
    }    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline TInt MMusAvaSettings::SipProfileId() const
    {
    User::Leave( KErrNotSupported );

    // this line is never reached; exists only to avoid warning
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline void MMusAvaSettings::SetObserver( MMusAvaSettingsObserver& /*aObserver*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline void MMusAvaSettings::SetManualActivation( TManualActivation /*aManualActivation*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline TInt MMusAvaSettings::CallPrivacy()
	{
    User::Leave( KErrNotSupported );
    return 0; // To avoid "return value expected" warning
    }

#endif // MUSAVASETTING_INL
