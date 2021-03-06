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
#include "musenglivesession.h"
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
    //iMsgSender = CMccDataSender::NewL();
    //iSession = CMusEngTwoWaySession::NewL();
    iSession = CMusEngLiveSession::NewL();
    _LIT(KDisplayName,"+358504871872");
    iSession->SetRemoteL( KDisplayName() );
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



// End of File
