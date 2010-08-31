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


#include <sipprofile.h>
#include "mceoutsession.h"
#include "mcemediastream.h"
#include "mcemanager.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession* CMceOutSession::NewL(
                        CMceManager& aManager,
                        CSIPProfile& aProfile,
                        const TDesC8& aRecipient,
						HBufC8* aOriginator )
    {
    // Check that profile has been registered
    TBool profileRegistered = EFalse;   
        
    aProfile.GetParameter( KSIPProfileRegistered, profileRegistered  );  
    __ASSERT_ALWAYS( profileRegistered, User::Leave( KErrCouldNotConnect ) );        
    
    CMceOutSession* self = new (ELeave) CMceOutSession( &aManager, 0 );
    CleanupStack::PushL( self );
    self->ConstructL( aRecipient, aOriginator );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession* CMceOutSession::NewL( CMceEvent& /*aEvent*/ )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMceOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession* CMceOutSession::NewL( CMceRefer& /*aRefer*/ )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMceOutSession::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession* CMceOutSession::NewL()
    {
    return new( ELeave )CMceOutSession( NULL, 0 );
    }

// -----------------------------------------------------------------------------
// CMceOutSession::~CMceOutSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMceOutSession::~CMceOutSession()
    {
    }


// -----------------------------------------------------------------------------
// CMceOutSession::EstablishL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceOutSession::EstablishL(
                        TUint32 aTimeout,
                        CDesC8Array* aHeaders, 
                        HBufC8* aContentType,
                        HBufC8* aContent,
                        CDesC8Array* aContentHeaders ) 
    {
    User::LeaveIfError( State() == EIdle ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || 
       !( aContentType || aContent || aContentHeaders ) ? 
        KErrNone : KErrArgument );
    
    if ( iFailWithCode != KErrNone )
        {
        TInt failCode = iFailWithCode;
        iFailWithCode = KErrNone;
        User::Leave( failCode );
        }
    
    iTimeout = aTimeout;

    //start establishing the session

    delete iHeaders;
    iHeaders = aHeaders;
    delete aContentType;
    delete aContent;
    delete aContentHeaders;

	iState = CMceSession::EOffering; 
    }


// -----------------------------------------------------------------------------
// CMceOutSession::AddStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceOutSession::AddStreamL( CMceMediaStream* aMediaStream )
    {
    if ( iManager )
        {
        //initialize stream (to get supported codecs)
        aMediaStream->InitializeL( iManager );
        }
    else
        {
        //internalizing => no initialize yet
        }
    
    CMceSession::AddStreamL( aMediaStream );
    }


// -----------------------------------------------------------------------------
// CMceOutSession::CancelL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceOutSession::CancelL()
    {
    User::LeaveIfError( State() != EIdle ? KErrNone : KErrNotReady );
    
	iState = CMceSession::ECancelling;
	
    }
    

// -----------------------------------------------------------------------------
// CMceOutSession::CMceOutSession
// -----------------------------------------------------------------------------
//
CMceOutSession::CMceOutSession( CMceManager* aManager, TUint32 aProfileId ) 
    : CMceSession( aManager, aProfileId )
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// CMceOutSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceOutSession::ConstructL( const TDesC8& aRecipient, HBufC8* aOriginator )
    {
    delete iRecipient;
    iRecipient = NULL;
    iRecipient = aRecipient.AllocL();

    delete iOriginator;
    iOriginator = NULL;
    if ( aOriginator )
        {
        iOriginator = aOriginator->AllocL();
        delete aOriginator;    
        }
        
    CMceSession::ConstructL();
    }
