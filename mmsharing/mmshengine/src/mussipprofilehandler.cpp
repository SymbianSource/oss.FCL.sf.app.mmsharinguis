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
* Description:  Utilities to get and set profile used with SWIS.
*
*/


// USER
#include "mussipprofilehandler.h"
#include "musuid.hrh"
#include "muslogger.h"
#include "mussipprofileuser.h"

// SYSTEM
#include <sip.h>
#include <sipservertransaction.h>
#include <sipinvitedialogassoc.h>
#include <sipprofile.h>
#include <sipprofileregistry.h>
#include <sipprofilealrcontroller.h>
#include <uri8.h>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusSipProfileHandler* CMusSipProfileHandler::NewL( MMusSipProfileUser& aUser )
    {
    CMusSipProfileHandler* self = 
        new (ELeave) CMusSipProfileHandler( aUser );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self);
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusSipProfileHandler::CMusSipProfileHandler( MMusSipProfileUser& aUser )
    : iUser( aUser )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusSipProfileHandler::ConstructL()" )
    
    // Use NULL Uid since we do not want to receive any requests.
    // All requests should be handled by MCE.
    iSip = CSIP::NewL( TUid::Null(), *this );
    iProfileRegistry = CSIPProfileRegistry::NewL( *iSip, *this );   
    iProfileAlrController = 
        CSipProfileAlrController::NewL( *iProfileRegistry , *this );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusSipProfileHandler::ConstructL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusSipProfileHandler::~CMusSipProfileHandler()
    {
    delete iProfileAlrController;
    delete iSipProfile;
    delete iProfileRegistry;
    delete iSip;
    MUS_LOG( "mus: [ENGINE]     CMusSipProfileHandler::~CMusSipProfileHandler()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::CreateProfileL( TUint32 aSipProfileId )
    {
    CSIPProfile* profile( NULL );
    if ( aSipProfileId != 0 )
        {
        profile = iProfileRegistry->ProfileL( aSipProfileId );
        }
    else
        {
        profile = iProfileRegistry->DefaultProfileL();
        }
    CleanupStack::PushL( profile );
    TBool profileRegistered = EFalse;   
    User::LeaveIfError( profile->GetParameter( KSIPProfileRegistered,
                                                   profileRegistered  ) );
    CleanupStack::Pop( profile );   
    delete iSipProfile;
    iSipProfile = profile;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CSIPProfile* CMusSipProfileHandler::Profile()
    {
    return iSipProfile;
    }

#if 0 //TODO: should this be removed?
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CSIPProfile& CMusSipProfileHandler::SipProfileL( TInt aSipProfileId )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusSipProfileHandler::SipProfileL(),\
              od #%d", aSipProfileId )

    if ( !iSipProfile )
        {        
        if ( aSipProfileId != 0 )
            {
            iSipProfile = iProfileRegistry->ProfileL( aSipProfileId );
            }
        else
            {
            iSipProfile = iProfileRegistry->DefaultProfileL();
            }
        }

    // The parameter val indicates if the profile can be immediately
    // used for creating a session, or if the client must wait for
    // the profile to be registered.
    TBool profileRegistered = EFalse;
    
    User::LeaveIfError( iSipProfile->GetParameter( KSIPProfileRegistered,
                                                   profileRegistered  ) );
                                                       
    if ( !profileRegistered )
        {

        #if (defined (__WINS__) || defined(__WINSCW__))
        
        iProfileRegistry->EnableL( *iSipProfile, *this );            

        User::LeaveIfError( iSipProfile->GetParameter( KSIPProfileRegistered,
                                                       profileRegistered  ) );
                                               
        if( !profileRegistered )
            {
            CActiveScheduler::Start();

            User::LeaveIfError( iSipProfile->GetParameter( 
                                                    KSIPProfileRegistered,
                                                    profileRegistered  ) );
            
            if ( !profileRegistered )
                {
                User::Leave( KErrNotReady );
                }
            }

        #else

        User::Leave( KErrNotReady );

        #endif

        }
    
        
    MUS_LOG( "mus: [ENGINE]  <- CMusSipProfileHandler::SipProfileL()" )
    
    return *iSipProfile;
    }
#endif


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CUri8* CMusSipProfileHandler::UserFromProfileLC()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusSipProfileHandler::UserFromProfileLC()" );
    
    __ASSERT_ALWAYS( iSipProfile != NULL, User::Leave( KErrNotReady ) );

    const MDesC8Array* aors = NULL;
    User::LeaveIfError( iSipProfile->GetParameter( KSIPRegisteredAors, aors ) );
    if( !aors || aors->MdcaCount() <= 0 ) 
        {    
        User::Leave( KErrArgument );            
        }
    TUriParser8 parser;
    User::LeaveIfError( parser.Parse( aors->MdcaPoint( 0 ) ) );
    MUS_LOG( "mus: [ENGINE]  <- CMusSipProfileHandler::UserFromProfileLC()" )
    
    return CUri8::NewLC( parser );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::RefreshIapAvailabilities()
    {
    TRAP_IGNORE( iProfileAlrController->RefreshIapAvailabilityL( ProfileId() ) )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ProfileRegistryEventOccurred(
    TUint32 /*aProfileId*/,
    MSIPProfileRegistryObserver::TEvent /*aEvent*/ )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ProfileRegistryErrorOccurred(
    TUint32 /*aProfileId*/,
    TInt /*aError*/)
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::IncomingRequest( 
    TUint32 /*aIapId*/,
    CSIPServerTransaction* aTransaction )
    {
    // Ownership transferred, should not ever be called by SIP though
    delete aTransaction; 
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::TimedOut( 
    CSIPServerTransaction& /*aTransaction*/ )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::IncomingRequest(
    CSIPServerTransaction* aTransaction )
    {
    // Ownership transferred, should not ever be called by SIP though
    delete aTransaction; 
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::IncomingRequest(
    CSIPServerTransaction* aTransaction,
    CSIPDialog& /*aDialog*/)
    {
    // Ownership transferred, should not ever be called by SIP though
    delete aTransaction; 
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::IncomingResponse(
    CSIPClientTransaction& /*aTransaction*/)
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::IncomingResponse( 
    CSIPClientTransaction& /*aTransaction*/,
    CSIPDialogAssocBase& /*aDialogAssoc*/)
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::IncomingResponse( 
    CSIPClientTransaction& /*aTransaction*/,
    CSIPInviteDialogAssoc* aDialogAssoc)
    {
    // Ownership transferred, should not ever be called by SIP though
    delete aDialogAssoc;     
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::IncomingResponse( 
    CSIPClientTransaction& /*aTransaction*/,
    CSIPRegistrationBinding& /*aRegistration*/ )
    {
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ErrorOccured( 
    TInt /*aError*/,
    CSIPTransactionBase& /*aTransaction*/ )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ErrorOccured( 
    TInt /*aError*/,
    CSIPClientTransaction& /*aTransaction*/,
    CSIPRegistrationBinding& /*aRegistration*/ )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ErrorOccured( 
    TInt /*aError*/,
    CSIPTransactionBase& /*aTransaction*/,
    CSIPDialogAssocBase& /*aDialogAssoc*/ )                                     
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ErrorOccured( 
    TInt /*aError*/, 
    CSIPRefresh& /*aSIPRefresh*/ )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ErrorOccured( 
    TInt /*aError*/,
    CSIPRegistrationBinding& /*aRegistration*/ )                                
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ErrorOccured( 
    TInt /*aError*/,
    CSIPDialogAssocBase& /*aDialogAssoc*/ )                
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::InviteCompleted( 
    CSIPClientTransaction& /*aTransaction*/ )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::InviteCanceled( 
                                    CSIPServerTransaction& /*aTransaction*/ )
    {
    MUS_LOG( "mus: [ENGINE]      CMusSipProfileHandler::CSIPServerTransaction")
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::ConnectionStateChanged( 
    CSIPConnection::TState /*aState*/ )
    {
    }


// -----------------------------------------------------------------------------
// CMusSipProfileHandler::AlrEvent
// From MSipProfileAlrObserver
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::AlrEvent(
    MSipProfileAlrObserver::TEvent aEvent,
    TUint32 aProfileId,
    TUint32 /*aSnapId*/,
    TUint32 aIapId )
    {
    if ( aEvent == MSipProfileAlrObserver::EIapAvailable )
        {
        if ( iSipProfile &&
             aProfileId == ProfileId() &&
             !iUser.IsRoamingBetweenAPsAllowed() )
           {
           // Disallow roaming only if the profile is used by MuS and
           // the MuS has an ongoing  session 
           TRAP_IGNORE( 
               iProfileAlrController->DisallowMigrationL( aProfileId, aIapId ) )
           }
       else
           {
           // In all other cases allow roaming
           TRAP_IGNORE( 
               iProfileAlrController->AllowMigrationL( aProfileId, aIapId ) )
           }
        }
    }

// -----------------------------------------------------------------------------
// CMusSipProfileHandler::AlrError
// From MSipProfileAlrObserver
// -----------------------------------------------------------------------------
//
void CMusSipProfileHandler::AlrError(
    TInt /*aError*/,
    TUint32 /*aProfileId*/,
    TUint32 /*aSnapId*/,
    TUint32 /*aIapId*/ )
    {
    // Ignore ALR related errors as they should not happen 
    // as migration is not allowed when 
    // the instance of CMusSipProfileHandler exists
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TUint32 CMusSipProfileHandler::ProfileId() const
    {
    TUint32 sipProfileId( 0 );
    if ( iSipProfile )
        {
        iSipProfile->GetParameter( KSIPProfileId, sipProfileId );
        }
    return sipProfileId;
    }
