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
#include "lcenginestub.h"
#include "lcsessionstub.h"
#include "mussessionproperties.h"
#include "lcsessionobserver.h"
#include "lcuiprovider.h"
#include "lcvideoplayerstub.h"
#include "musmanagercommon.h"

static int lcutStub_LcEngine_failureValue = 0;
static bool lcutStub_LcEngine_lcFeatureSupported = false;


TAny* REComSession::CreateImplementationL( TUid /*aInterfaceUid*/, 
                                           TInt32 /*aKeyOffset*/, 
                                           const TEComResolverParams& /*aResolutionParameters*/ )
{
    User::LeaveIfError( lcutStub_LcEngine_failureValue );
    return CLcEngineStub::NewL();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcEngineStub* CLcEngineStub::NewL()
    {
    
    CLcEngineStub* self = new( ELeave )CLcEngineStub();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
        
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
CLcEngineStub::~CLcEngineStub()
    {
    delete iSession;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcEngineStub::CLcEngineStub( )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CLcEngineStub::ConstructL()
    { 
    iSession = CLcSession::NewL(); 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MLcSession& CLcEngineStub::Session()
    {
    return *iSession;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CLcEngineStub::IsFeatureSupported( TLcFeature /*aLcFeature*/ )
    {
    return lcutStub_LcEngine_lcFeatureSupported;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcSession* CLcEngineStub::CreateMusSessionL( int aUseCase )
    {
    CLcSession* session; 
    switch ( aUseCase ) 
        {
        case MultimediaSharing::EMusReceive:
            {
            session = CLcSession::NewL(); 
            session->iRemotePlayer = CLcVideoPlayer::NewL(); 
            break;
            }
            
        case MultimediaSharing::EMusLiveVideo:
            {
            session = CLcSession::NewL();
            session->iLocalPlayer = CLcVideoPlayer::NewL(); 
            break;
            }
        case MultimediaSharing::EMusTwoWayVideo:
        case MultimediaSharing::EMusReceiveTwoWayVideo:
            {
            session = CLcSession::NewL();
            session->iLocalPlayer = CLcVideoPlayer::NewL(); 
            session->iRemotePlayer = CLcVideoPlayer::NewL(); 
            break;
            }
            
        default:
            User::Leave( KErrNotSupported );
            break;
        }
    return session;
    }


//stub control

    
void lcutStub_LcEngine_reset()
    {
    lcutStub_LcEngine_failureValue = 0;
    lcutStub_LcEngine_lcFeatureSupported = false;
    }

void lcutStub_LcEngine_setFailure( int failure )
    {
    lcutStub_LcEngine_failureValue = failure;
    }

int lcutStub_LcEngine_failure()
    {
    return lcutStub_LcEngine_failureValue;
    }

void lcutStub_LcEngine_setLcFeatureSupported( bool supported )
    {
    lcutStub_LcEngine_lcFeatureSupported = supported;
    }

// End of File
