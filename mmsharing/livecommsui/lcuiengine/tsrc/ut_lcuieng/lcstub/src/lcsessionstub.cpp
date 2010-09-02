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
#include "lcenginestub.h"
#include "lcsessionstub.h"
#include "lcvideoplayer.h"
#include "lcsessionobserver.h"
#include "lcuiprovider.h"
#include "lcvideoplayerstub.h"

// SYSTEM
#include <QStringList>


static QStringList lcutStub_LcSession_calledMethods;
static MLcUiProvider* lcutStub_LcSession_uiProvider = 0;
 
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcSession* CLcSession::NewL( )
    {
    CLcSession* self = new( ELeave ) CLcSession(  );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcSession::~CLcSession()
    {
    //delete iRemoteDisplayName;
    delete iRemotePlayer;
    delete iLocalPlayer;
    delete iLocalDisplayName;
    delete iRemoteDisplayName;
    delete iRemoteDetails;
    }     

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
 
 MLcSession::TLcSessionState  CLcSession::LcSessionState() const
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CLcSession::SetLcSessionObserver( MLcSessionObserver* aObserver )
    {
    iSessionObserver = aObserver;
    }
             
// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CLcSession::SetLcUiProvider( MLcUiProvider* aUiProvider )
    {
    iLcUiProvider = aUiProvider;
    lcutStub_LcSession_uiProvider = aUiProvider;
    }       

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CLcSession::EstablishLcSessionL( )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcSession_calledMethods.insert(0,lcutStub_LcSession_establishSession );
    }    

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CLcSession::TerminateLcSessionL( )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcSession_calledMethods.insert(0,lcutStub_LcSession_terminateSession );
    }       

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CLcSession::UpdateLcSessionL( )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcSession_calledMethods.insert(0,lcutStub_LcSession_updateSession );
    }       

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//

TBool CLcSession::SendDialTone( TChar /*aKey*/)
    {
    return ETrue;
    }


// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CLcSession::RemoteVideoPlayer( )
    {
    return iRemotePlayer;
    }     

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CLcSession::LocalVideoPlayer( )
    {
    return iLocalPlayer;           
    }       

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
const TDesC& CLcSession::LocalDisplayName( )
    {
    return *iLocalDisplayName;
    }    

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
const TDesC& CLcSession::RemoteDisplayName( )
    {
    return *iRemoteDisplayName;
    } 
// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
 TInt CLcSession::SetParameter( TInt aId, TInt aValue )
    {
    mParams.insert( aId, aValue );
    return 0;
    } 
       
// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TInt CLcSession::ParameterValue( TInt /*aId*/ )
    {
    return 0;
    }        


// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
 TBool CLcSession::IsBackgroundStartup()
    {
    return iIsForeground;
    }

 // -----------------------------------------------------------------------------
 // From MLcSession
 // -----------------------------------------------------------------------------
 // 
TInt CLcSession::SetForegroundStatus( TBool aIsForeground )
    {
    iIsForeground = aIsForeground;
    return iForegroundChange;
    }

 // -----------------------------------------------------------------------------
 // From MLcSession
 // -----------------------------------------------------------------------------
 //     
const TDesC& CLcSession::RemoteDetails()
    {
    return *iRemoteDetails;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcSession::CLcSession( )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CLcSession::ConstructL()
    {
    iLocalDisplayName = KLcStubLocalDisplayName().AllocL();
    iRemoteDisplayName = KLcStubRemoteDisplayName().AllocL();
    iLocalPlayer = CLcVideoPlayer::NewL();
    iRemotePlayer = CLcVideoPlayer::NewL();
    iRemoteDetails = KLcStubRemoteDetails().AllocL();
    }


//stub control

bool lcutStub_LcSession_expectCall( const QString& call, int callNdx )
{
    if ( callNdx >= lcutStub_LcSession_calledMethods.count() ) {
        return call == lcutStub_LcSession_NoCall;
    }

    return lcutStub_LcSession_calledMethods.count() > 0 ?
           lcutStub_LcSession_calledMethods[callNdx] == call :
           call == lcutStub_LcSession_NoCall;
}

    
void lcutStub_LcSession_reset()
{
    lcutStub_LcSession_calledMethods.clear();
}

MLcUiProvider* lcutStub_LcSession_lcUiProvider()
{
    return lcutStub_LcSession_uiProvider;
}
