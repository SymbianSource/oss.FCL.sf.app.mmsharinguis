/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "musengsessionmanager.h"
#include "musengmcesession.h"
#include "musengtwowaysession.h"
#include "lcsession.h"
#include "mccdatasender.h"
#include "mccdatareceiver.h"
#include "musenglogger.h"
#include "mccdatastructures.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngSessionManager* CMusEngSessionManager::NewL()
    {
    CMusEngSessionManager* self = new( ELeave )CMusEngSessionManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
        
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
CMusEngSessionManager::~CMusEngSessionManager()
    {
    delete iSession;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngSessionManager::CMusEngSessionManager()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngSessionManager::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngSessionManager::ConstructL()" )
    //iMsgReceiver = CMccDataReceiver::NewL(*this);
    MUS_LOG( "mus: [ENGINE]  MsgQueue1 setup complete" )
    //iMsgSender = CMccDataSender::NewL();
    MUS_LOG( "mus: [ENGINE]  MsgQueue2 setup complete" )
    iSession = CMusEngTwoWaySession::NewL();
    iSession->SetRemoteL( _L("WHAT") );
    MUS_LOG( "mus: [ENGINE]  <- CMusEngSessionManager::ConstructL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MLcSession& CMusEngSessionManager::Session()
    {
    return *iSession;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngSessionManager::DataReceived( const TDataMessage& aData )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngSessionManager::IsFeatureSupported( TLcFeature aLcFeature )
    {
    return ( aLcFeature == ELcSendVideoQuery );
    }




// End of File
