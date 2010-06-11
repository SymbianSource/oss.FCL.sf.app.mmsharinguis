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
#include <e32property.h>
#include "musenglivesession.h"
#include "musengreceivesession.h"
#include "lcsession.h"
#include "mussessionproperties.h"
#include "lcsessionobserver.h"
#include "lcuiprovider.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngSessionManager* CMusEngSessionManager::NewL( TAny* aInitParams )
    {
    __ASSERT_ALWAYS( aInitParams != NULL, User::Leave( KErrArgument ) );
    
    CMusEngSessionManager* self = 
        new( ELeave )CMusEngSessionManager( 
                *reinterpret_cast< TLcEngineParams* >( aInitParams ) );
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
CMusEngSessionManager::CMusEngSessionManager( 
    TLcEngineParams& aInitParams )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngSessionManager::ConstructL()
    {
    TInt useCase;
    User::LeaveIfError( RProperty::Get( NMusSessionApi::KCategoryUid, 
                                        NMusSessionApi::KUseCase, 
                                        useCase ) );    
    iSession = CreateMusSessionL( 
        static_cast< MultimediaSharing::TMusUseCase >( useCase ) );
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
CMusEngMceSession* CMusEngSessionManager::CreateMusSessionL( 
    MultimediaSharing::TMusUseCase aUseCase )
    {
    CMusEngMceSession* session = NULL;
    switch ( aUseCase ) 
        {
        case MultimediaSharing::EMusLiveVideo:
            {
            session = CMusEngLiveSession::NewL( TRect() );
            break;
            }
        case MultimediaSharing::EMusReceive:
            {
            session = CMusEngReceiveSession::NewL( TRect() );
            break;
            }
        default:
            User::Leave( KErrNotSupported );
            break;
        }
    return session;
    }

// End of File
