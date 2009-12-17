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


#include "mcemediastream.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcestreamobserver.h"
#include "mcemediasource.h"
#include "mcemediasink.h"



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceMediaStream::~CMceMediaStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaStream::~CMceMediaStream()
    {
    delete iMediaSDPLines;
    
    if ( iSource )
        {
        DeleteSource();
        }
    
    if ( iLinkOwner )
	    {
    	delete iLinkedStream;
	    }

	while( iSinks.Count() > 0 )
	    {
	    DeleteSink( 0 );
	    }
	    
    iSinks.Reset();
    iSinks.Close();
   
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaStream::TState CMceMediaStream::State() const
    {
    return iState;
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::Type
// -----------------------------------------------------------------------------
//
EXPORT_C TMceMediaType CMceMediaStream::Type() const
    {
    return iType;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::SetMediaAttributeLinesL( 
                                        CDesC8Array* aMediaSDPLines )
    {
    delete iMediaSDPLines;
    iMediaSDPLines = aMediaSDPLines;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C MDesC8Array* CMceMediaStream::MediaAttributeLinesL()
    {
    if ( iMediaSDPLines )
        {
        CDesC8Array* array = new ( ELeave ) CDesC8ArrayFlat( 1 );
        CleanupStack::PushL( array );
        for ( TInt i = 0; i < iMediaSDPLines->Count(); ++i )
            {
            array->AppendL( iMediaSDPLines->MdcaPoint(i) );
            }
        CleanupStack::Pop( array );
        return array;
        }
        
    return NULL;
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::SetStartMediaPort
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::SetLocalMediaPortL( TUint aLocalMediaPort )
    {
    iLocalMediaPort = aLocalMediaPort;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::StartMediaPort
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceMediaStream::LocalMediaPort() const        
    {
    return iLocalMediaPort;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::Session
// -----------------------------------------------------------------------------
//
EXPORT_C CMceSession* CMceMediaStream::Session() const
    {
    return iSession;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::EnableL() 
    {
    iIsEnabled = ETrue;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::DisableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::DisableL() 
    {
    iIsEnabled = EFalse;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::IsEnabled
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceMediaStream::IsEnabled() const
    {
    return iIsEnabled;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::SetSourceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::SetSourceL( CMceMediaSource* aSource )
    {
    if ( aSource )
        {
        aSource->ReferenceCount()++;
        }
    
    if ( iSource )
        {
        DeleteSource();
        }
    iSource = aSource;
    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::AddSinkL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::AddSinkL( CMceMediaSink* aSink )
    {
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    iSinks.AppendL( aSink );
    aSink->ReferenceCount()++;    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::Source
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaSource* CMceMediaStream::Source() const
    {
    return iSource;
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::Sinks
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceMediaSink>& CMceMediaStream::Sinks() const
    {
    return iSinks;
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::RemoveSinkL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::RemoveSinkL( CMceMediaSink& aSink )
    {
	for ( TInt i = 0; i < iSinks.Count(); i++ )
	    {
	    if ( iSinks[i] == &aSink )
	        {
	        DeleteSink( i );
	        iSinks.Compress();
	        return;
	        }
	    }
	User::Leave( KErrNotFound );
    }



// -----------------------------------------------------------------------------
// CMceMediaStream::BindL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMediaStream::BindL( CMceMediaStream* aStream )
    {
   
     __ASSERT_ALWAYS( !(! iLinkOwner  &&  
                          iLinkedStream  ), 
                          User::Leave( KErrArgument ) );
    
    if ( aStream )
        {
        iLinkOwner = ETrue;
        aStream->DoBindL( this );
        DoBindL( aStream );
        }
    else
        {
        iLinkOwner = EFalse;
    	delete iLinkedStream;
    	iLinkedStream = NULL;
        }
    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::BoundStreamL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMediaStream& CMceMediaStream::BoundStreamL() const
    {
    __ASSERT_ALWAYS( iLinkedStream , User::Leave( KErrNotFound ) );
    
    return *iLinkedStream;
    
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::BoundStream
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMceMediaStream::BoundStream() const
    {
    return iLinkedStream != NULL;
    }

// -----------------------------------------------------------------------------
// CMceMediaStream::Binder
// -----------------------------------------------------------------------------
//
TBool CMceMediaStream::Binder() const
    {
    return iLinkOwner;
    }
    

// -----------------------------------------------------------------------------
// CMceMediaStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::InitializeL( CMceSession& aParent )
    {
    iSession = &aParent;
    
    if ( Binder() && BoundStream() )
        {
        iLinkedStream->InitializeL( aParent );
        }
        
    if ( iSource )
        {
        iSource->InitializeL( *this );
        }
        
    for( TInt i = 0; i < iSinks.Count();i++)
        {
        iSinks[i]->InitializeL( *this );
        }

    }


// -----------------------------------------------------------------------------
// CMceMediaStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::InitializeL( CMceManager* aManager )
    {
    if ( Binder() && BoundStream() )
        {
        iLinkedStream->InitializeL( aManager );
        }
        
    if ( iSource )
        {
        iSource->InitializeL( aManager );
        }
        
    for( TInt i = 0; i < iSinks.Count();i++)
        {
        iSinks[i]->InitializeL( aManager );
        }  
    }
    
    
// -----------------------------------------------------------------------------
// CMceMediaStream::CMceMediaStream
// -----------------------------------------------------------------------------
//
CMceMediaStream::CMceMediaStream()
    :iLinkedStream( NULL ),
     iSource( NULL ),
     iLinkOwner( EFalse ),
     iIsEnabled( ETrue ),
     iState( CMceMediaStream::EUninitialized )
    {
    }


// -----------------------------------------------------------------------------
// CMceMediaStream::DeleteSink
// -----------------------------------------------------------------------------
//
void CMceMediaStream::DeleteSink( TInt aIndex )
    {
    
    iSinks[ aIndex ]->ReferenceCount()--;
    if ( iSinks[ aIndex ]->ReferenceCount() == 0 )
        {
        delete iSinks[ aIndex ];
        }
    iSinks.Remove( aIndex );
    
    }
    
// -----------------------------------------------------------------------------
// CMceMediaStream::DeleteSource
// -----------------------------------------------------------------------------
//
void CMceMediaStream::DeleteSource()
    {
    iSource->ReferenceCount()--;
    if ( iSource->ReferenceCount() == 0 )
        {
        delete iSource;
        }
    iSource = NULL;
    }



// -----------------------------------------------------------------------------
// CMceMediaStream::DoBindL
// -----------------------------------------------------------------------------
//
void CMceMediaStream::DoBindL( CMceMediaStream* aStream )
    {
    if ( Session() )
        {
        aStream->InitializeL( *Session() );
        }
    delete iLinkedStream;    
    iLinkedStream = aStream;
    }
