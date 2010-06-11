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


#include "mcesession.h"
//#include "mceinsession.h"
#include "mcemanager.h"
#include "mcestreambundle.h"
#include "mcemediastream.h"
#include "mceinsessionobserver.h"
#include "mcesessionobserver.h"
#include "mcestreamobserver.h"
#include "mcertpobserver.h"
#include "mcetransactiondatacontainer.h"
#include "mussessionsimulator.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceSession::~CMceSession
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSession::~CMceSession()
    {
    delete iHeaders;
    delete iSessionSDPLines;
    
    delete iOriginator;
    delete iRecipient;
    iMediaStreams.ResetAndDestroy();
    iBundles.ResetAndDestroy();
    
    }


// -----------------------------------------------------------------------------
// CMceSession::State
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSession::TState CMceSession::State() const
    {
    return iState;
    }


// -----------------------------------------------------------------------------
// CMceSession::ConnectionActive
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceSession::ConnectionActive() const
    {
    return iIsConnectionActive;
    }


// -----------------------------------------------------------------------------
// CMceSession::RemoveStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::RemoveStreamL( CMceMediaStream& aMediaStream )
    {
    for ( TInt i = 0; i < iMediaStreams.Count(); ++i )
        {
        if ( &aMediaStream == iMediaStreams[i] )
            {
            delete iMediaStreams[i];
            iMediaStreams.Remove( i );
            return;
            }
        }
    
    User::Leave( KErrNotFound );
    }
    

// -----------------------------------------------------------------------------
// CMceSession::AddStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::AddStreamL( CMceMediaStream* aMediaStream )
    {
    User::LeaveIfError( iMediaStreams.Find( aMediaStream ) != KErrNotFound ? 
                                            KErrArgument : KErrNone );
    
    aMediaStream->InitializeL( *this ); // Initialize before taking ownership
    
    iMediaStreams.AppendL( aMediaStream );
    
    }


// -----------------------------------------------------------------------------
// CMceSession::UpdateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::UpdateL(
             TUint32 aTimeout,
             CDesC8Array* aHeaders, 
             HBufC8* aContentType,
             HBufC8* aContent,
             CDesC8Array* aContentHeaders ) 
    {
    User::LeaveIfError( iState != EIdle ? KErrNone : KErrNotReady );
    User::LeaveIfError( 
        ( aContentType && aContent ) || 
       !( aContentType || aContent || aContentHeaders ) ? 
        KErrNone : KErrArgument );
    
    // Leave if in incoming state givem too small timeout value
    User::LeaveIfError( !( aTimeout > 0 && State() == CMceSession::EIncoming ) ||
        aTimeout <= SessionTimer() ? KErrNone : KErrArgument );
    
    InitializeL(); 
    
    iTimeout = aTimeout;
    
    delete iHeaders; 
    iHeaders = aHeaders;
    delete aContentType;
    delete aContentHeaders;
    
    if ( iState == EIncoming )
        {
        iState = EReserving;
        RestartSimulator();
        }
    else
        {
        iState = EOffering;
        }
    }

// -----------------------------------------------------------------------------
// CMceSession::InitializeL
// -----------------------------------------------------------------------------
//
/*
void CMceSession::InitializeL()
    {
    for(int i=0; i<iMediaStreams.Count();i++)
        {
        iMediaStreams[i]->InitializeL( *this );
        }
    }
  */  


// -----------------------------------------------------------------------------
// CMceSession::TerminateL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::TerminateL( CDesC8Array* aHeaders,
					                   HBufC8* aContentType,
					                   HBufC8* aContent ) 
    {
    __ASSERT_ALWAYS( iState != CMceSession::EIdle, User::Leave( KErrNotReady ) );
    
    delete iHeaders;
    iHeaders = aHeaders;
    delete aContentType;
    delete aContent;
    
   	iState = CMceSession::ETerminating;
   	RestartSimulator();
   
   	
    }


// -----------------------------------------------------------------------------
// CMceSession::Streams
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceMediaStream>& CMceSession::Streams() const
    {
    return iMediaStreams;
    }


// -----------------------------------------------------------------------------
// CMceSession::Recipient
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceSession::Recipient() const
    {
    return *iRecipient;
    }

// -----------------------------------------------------------------------------
// CMceSession::Originator
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMceSession::Originator() const
    {
    return *iOriginator;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::SetSessionSDPLinesL( CDesC8Array* aSessionSDPLines )
    {
    delete iSessionSDPLines;
    iSessionSDPLines = aSessionSDPLines;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C MDesC8Array* CMceSession::SessionSDPLinesL()
    {
    if ( iSessionSDPLines )
        {
        CDesC8Array* array = new ( ELeave ) CDesC8ArrayFlat( 1 );
        CleanupStack::PushL( array );
        for ( TInt i = 0; i < iSessionSDPLines->Count(); ++i )
            {
            array->AppendL( iSessionSDPLines->MdcaPoint(i) );
            }
        CleanupStack::Pop( array );
        return array;
        }
        
    return NULL;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceSession::SessionTimer() const
    {
    return iTimeout;
    }

// -----------------------------------------------------------------------------
// CMceSession::AddBundleL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::AddBundleL( CMceStreamBundle* aBundle )
    {
    iBundles.AppendL( aBundle );
    }

// -----------------------------------------------------------------------------
// CMceSession::RemoveBundleL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::RemoveBundleL( CMceStreamBundle& aBundle )
    {
    TInt index = iBundles.Find( &aBundle );
    User::LeaveIfError( index );
    iBundles.Remove( index );
    }

// -----------------------------------------------------------------------------
// CMceSession::Bundles
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray< CMceStreamBundle >& CMceSession::Bundles() const
    {
    return iBundles;
    }

// -----------------------------------------------------------------------------
// CMceSession::GetModifierL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::GetModifierL( TMceSessionModifier aModifier,
                                         TUint& aValue ) const
    {
    if ( aModifier == KMcePreconditions )
        {
        if ( iForceInternetSignaling )
            {
            aValue = KMcePreconditionsNotUsed;
            }
        else
            {
            aValue = KMcePreconditionsSupported;
            }
        return;
        }
    User::Leave( KErrNotFound );
    }
    
// -----------------------------------------------------------------------------
// CMceSession::SetModifierL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceSession::SetModifierL( TMceSessionModifier aModifier, 
                                         TUint aValue )        
    {
    if ( aModifier == KMcePreconditions )
        {
        if ( aValue == KMcePreconditionsNotUsed )
            {
            iForceInternetSignaling = ETrue;
            }
        else
            {
            iForceInternetSignaling = EFalse;
            }
        return;
        }
    else if ( aModifier == KMce100Rel )
        {
        if ( aValue >= 0 && aValue <= 2 )
            {
            i100RelUsage = aValue;
            return;
            }
        }
    User::LeaveIfError( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMceSession::Profile
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CMceSession::Profile() const
    {
    return iProfileId;
    }

// -----------------------------------------------------------------------------
// CMceSession::CMceSession
// -----------------------------------------------------------------------------
//
CMceSession::CMceSession( CMceManager* aManager, TUint32 aProfileId )
 : iProfileId( aProfileId ),
   iManager( aManager ),
   iState( CMceSession::EIdle),
   iIsConnectionActive( ETrue ),
   iForceInternetSignaling( EFalse ),
   i100RelUsage( 1 ), // MCE default
   iFailWithCode( KErrNone ),
   iReasonPhrase( KNullDesC8() ),
   iReasonCode( 0 )
    {
    }


// -----------------------------------------------------------------------------
// CMceSession::Manager
// -----------------------------------------------------------------------------
//
CMceManager& CMceSession::Manager() const
    {
    return *iManager;
    }
    

// -----------------------------------------------------------------------------
// CMceSession::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSession::ConstructL()
    {
    
    }

// -----------------------------------------------------------------------------
// CMceSession::InitializeL
// -----------------------------------------------------------------------------
//
void CMceSession::InitializeL()
    {
    for (int i=0; i<iMediaStreams.Count();i++)
        {
        iMediaStreams[i]->InitializeL( *this );
        }
    
    /*    
    for(int i=0; i<iBundles.Count();i++)
        {
        iBundles[i]->InitializeL( *this );
        }
    */
    }
    

// -----------------------------------------------------------------------------
// CMceSession::RestartSimulator
// -----------------------------------------------------------------------------
//
void CMceSession::RestartSimulator()
    {
    if( iManager->iSessionSimulator->IsRunning())
        {
        iManager->iSessionSimulator->Stop();
        }
    iManager->iSessionSimulator->StartL();    
    }

