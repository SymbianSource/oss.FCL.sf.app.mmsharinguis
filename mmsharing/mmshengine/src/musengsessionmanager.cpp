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
#include "musenglivesession.h"
#include "musengclipsession.h"
#include "musengreceivesession.h"
#include "musengtwowaysession.h"
#include "musengtwowayrecvsession.h"
#include "mussessionproperties.h"
#include "muspropertywatch.h"
#include <lcsession.h>
#include <lcuiprovider.h>

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
    delete iUseCaseWatch;
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
    TInt useCase;
    User::LeaveIfError( RProperty::Get( NMusSessionApi::KCategoryUid, 
                                        NMusSessionApi::KUseCase, 
                                        useCase ) );    
    iSession = CreateMusSessionL( 
        static_cast< MultimediaSharing::TMusUseCase >( useCase ) );
    
    iUseCaseWatch = CMusPropertyWatch::NewL( 
        *this, NMusSessionApi::KCategoryUid, NMusSessionApi::KUseCase );
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
// If usecase changes while background startup is active, client is
// notified about change by bringing it to foreground and changing current
// active session. Old session is terminated immediately after notification.
// -----------------------------------------------------------------------------
//
void CMusEngSessionManager::PropertyChanged( 
    const TUint /*aKey*/, 
    const TInt aValue )
    {
    MUS_LOG1( "mus: [ENGINE] -> CMusEngSessionManager::PropertyChanged, val:%d", 
              aValue )
    
    if ( iSession->IsBackgroundStartup() && aValue != iCurrentUseCase )
        {
        MUS_LOG( "mus: [ENGINE]    Usecase changed!" )
        MLcUiProvider* uiProvider = iSession->LcUiProvider();
        if ( uiProvider )
            {
            CMusEngMceSession* newSession = NULL;
            TRAP_IGNORE( newSession = CreateMusSessionL( 
                    static_cast< MultimediaSharing::TMusUseCase >( aValue ) ) );
            if ( newSession )
                {
                MUS_LOG( "mus: [ENGINE]     Set new active session" )
                CMusEngMceSession* oldSession = iSession;
                iSession = newSession;
                iSession->SetForegroundStatus( ETrue );
                uiProvider->HandleForegroundStatus( ETrue );
                delete oldSession;
                }
            }
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngSessionManager::PropertyChanged" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngSessionManager::HandlePropertyError( const TInt /*aReason*/ )
    {
    // NOP
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngSessionManager::IsFeatureSupported( TLcFeature aLcFeature )
    {
    return ( aLcFeature == ELcShowInvitingNote ||
             aLcFeature == ELcShowWaitingNote ||
             aLcFeature == ELcShowAcceptQuery );
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
            session = CMusEngLiveSession::NewL();
            break;
            }
        case MultimediaSharing::EMusClipVideo:
            {
            session = CMusEngClipSession::NewL();
            break;
            }            
        case MultimediaSharing::EMusReceive:
            {
            session = CMusEngReceiveSession::NewL();
            break;
            }
        case MultimediaSharing::EMusTwoWayVideo:    
            {
            session = CMusEngTwoWaySession::NewL();
            break;
            }
        case MultimediaSharing::EMusReceiveTwoWayVideo:    
            {
            session = CMusEngTwoWayRecvSession::NewL();
            break;
            }
        default:
            User::Leave( KErrNotSupported );
            break;
        }
    iCurrentUseCase = aUseCase;
    return session;
    }

// End of File
