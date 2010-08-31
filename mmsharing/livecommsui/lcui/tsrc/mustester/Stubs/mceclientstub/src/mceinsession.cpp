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


#include "mceinsession.h"
#include "mcemanager.h"



// CONSTANTS
const TUint32 KMceMinAcceptable = 200;
const TUint32 KMceMaxAcceptable = 299;
const TUint32 KMceMinError = 300;
const TUint32 KMceMaxError = 699;
const TUint32 KMceRinging = 180;
_LIT8( KMcePhraseRinging, "Ringing" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceInSession::NewL
// -----------------------------------------------------------------------------
//
CMceInSession* CMceInSession::NewL( RReadStream& /*aReadStream*/,
            						CMceManager& aManager,
            						TUint32 aProfileId )
    {
    CMceInSession* self = new (ELeave) CMceInSession( &aManager, aProfileId );    
    CleanupStack::PushL( self );
    self->ConstructL( KNullDesC8, KNullDesC8 );
    CleanupStack::Pop(self);
    return self;
        
    }

// -----------------------------------------------------------------------------
// CMceInSession::NewL
// -----------------------------------------------------------------------------
//

EXPORT_C CMceInSession* CMceInSession::NewL()
    {
    CMceInSession* self = new (ELeave) CMceInSession( NULL, 0 );
    CleanupStack::PushL( self );
    self->ConstructL( KNullDesC8, KNullDesC8 );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceInSession::~CMceInSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMceInSession::~CMceInSession()
    {
    }

// -----------------------------------------------------------------------------
// CMceInSession::RingL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::RingL( CDesC8Array* aHeaders,
					 		 		HBufC8* aContentType,
					 		 		HBufC8* aContent )
    {
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
      
    delete aHeaders;
    delete aContentType;
    delete aContent;
    
    iRingingCalled = ETrue;
    }
    
// -----------------------------------------------------------------------------
// CMceInSession::AcceptL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::AcceptL()
    {
    iState = CMceSession::EAnswering;
    }

// -----------------------------------------------------------------------------
// CMceInSession::AcceptL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::AcceptL(  const TDesC8& /*aReason*/,
        	           			        TUint32 /*aCode*/,
        	           			        CDesC8Array* aHeaders,
        					 		    HBufC8* aContentType,
        					 		    HBufC8* aContent )
    {
    iState = CMceSession::EAnswering;
    
    delete aHeaders;
    delete aContentType;
    delete aContent;
    }

// -----------------------------------------------------------------------------
// CMceInSession::RejectL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::RejectL()
    {
    if ( iFailWithCode != KErrNone )
        {
        TInt failureCode = iFailWithCode;
        iFailWithCode = KErrNone;
        User::Leave( failureCode );
        }
        
    iState = CMceSession::ETerminated;
    }

// -----------------------------------------------------------------------------
// CMceInSession::RejectL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceInSession::RejectL( const TDesC8& aReason,
            		                  TUint32 aCode,
            		                  CDesC8Array* aHeaders,
					                  HBufC8* aContentType,
					                  HBufC8* aContent )
    {
    
    User::LeaveIfError( 
        ( aContentType && aContent ) || !( aContentType || aContent ) ? 
        KErrNone : KErrArgument );
    User::LeaveIfError( aCode >= KMceMinError && aCode <= KMceMaxError ?
        KErrNone : KErrArgument );

    RejectL();

    iReasonPhrase = aReason;
    iReasonCode = aCode;

    delete aHeaders;
    delete aContentType;
    delete aContent;
    }

// -----------------------------------------------------------------------------
// CMceInSession::CMceInSession
// -----------------------------------------------------------------------------
//
CMceInSession::CMceInSession( CMceManager* aManager, TUint32 aProfileId )
    : CMceSession( aManager, aProfileId )
    {
    }


// -----------------------------------------------------------------------------
// CMceInSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceInSession::ConstructL( const TDesC8& aOriginator, const TDesC8& aRecipient )
    {
    
    delete iOriginator;
    iOriginator = 0;
    iOriginator = aOriginator.AllocL();
    
    delete iRecipient;
    iRecipient = 0;
    iRecipient = aRecipient.AllocL();
    
    CMceSession::ConstructL();
    
    iState = EIncoming;
    }

// -----------------------------------------------------------------------------
// Stub constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMceInSession* CMceInSession::NewL( CMceManager& aManager,
                                             const TDesC8& aOriginator )
    {
    CMceInSession* self = new (ELeave) CMceInSession( &aManager, 0 );
    CleanupStack::PushL( self );
    self->ConstructL( aOriginator, KNullDesC8 );
    CleanupStack::Pop( self );
    return self;
    }
