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


#include "mcertpsink.h"
#include "mcemediastream.h"
#include "mcesession.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceRtpSink::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSink* CMceRtpSink::NewL()
    {
    CMceRtpSink* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMceRtpSink::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSink* CMceRtpSink::NewLC()
    {
    CMceRtpSink* self = new (ELeave) CMceRtpSink();
    CleanupStack::PushL( self );
    self->ConstructL( EFalse );
    return self;    
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSink* CMceRtpSink::NewL( TBool aSuppressRTCP, 
		                                 HBufC8* /*aIdentity*/ )
    {
    
    CMceRtpSink* self = new (ELeave) CMceRtpSink();
    CleanupStack::PushL( self );
    self->ConstructL( aSuppressRTCP );
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::~CMceRtpSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMceRtpSink::~CMceRtpSink()
    {
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSink::EnableL() 
    {
    CMceMediaSink::DoEnableL();
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::DisableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSink::DisableL() 
    {
    CMceMediaSink::DoDisableL();
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::SendSRL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSink::SendSRL()
    {
    __ASSERT_ALWAYS( iStream &&
                     iStream->Session() &&
                     iStream->Session()->State() != CMceSession::EIdle &&
                     iStream->Session()->State() != CMceSession::EIncoming,
                     User::Leave( KErrNotReady ) );
	// NOP
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::LastPacket
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceRtpSink::LastPacket() const
    {    
    return iLastPacket;    
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::Ssrc
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceRtpSink::Ssrc() const
    {    
    return iSSRC;    
    }
    
// -----------------------------------------------------------------------------
// CMceRtpSink::UpdateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceRtpSink::UpdateL( TBool aSuppressRTCP, 
		                            HBufC8* /*aIdentity*/ )
    {    
    iSuppressRTCP = aSuppressRTCP;
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::CMceRtpSink
// -----------------------------------------------------------------------------
//
CMceRtpSink::CMceRtpSink()
   : CMceMediaSink()
    {
    iType = KMceRTPSink;
    }

// -----------------------------------------------------------------------------
// CMceRtpSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceRtpSink::ConstructL( TBool aSuppressRTCP )
    {
    iSuppressRTCP = aSuppressRTCP;
    }
