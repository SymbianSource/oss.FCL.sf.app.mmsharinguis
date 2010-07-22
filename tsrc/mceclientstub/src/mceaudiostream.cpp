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


#include "mceaudiostream.h"
#include "mcemanager.h"
#include "mceaudiocodec.h"
#include "mcesession.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceAudioStream::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioStream* CMceAudioStream::NewL()
    {
    CMceAudioStream* self = NewLC();
    CleanupStack::Pop( self );
    return self; 
    }
   
// -----------------------------------------------------------------------------
// CMceAudioStream::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioStream* CMceAudioStream::NewLC()
    {
    CMceAudioStream* self = new (ELeave) CMceAudioStream();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;    
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::~CMceAudioStream
// -----------------------------------------------------------------------------
//
EXPORT_C CMceAudioStream::~CMceAudioStream()
    {
    iCodecs.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::Codecs
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceAudioCodec>& CMceAudioStream::Codecs()
    {
    return iCodecs;
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::AddCodecL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAudioStream::AddCodecL(CMceAudioCodec* aCodec)
    {
    __ASSERT_ALWAYS(aCodec, User::Leave(KErrArgument));
    iCodecs.AppendL(aCodec);
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::RemoveCodecL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceAudioStream::RemoveCodecL(CMceAudioCodec& aCodec)
    {
    for(int i = 0; i < iCodecs.Count(); i++ )
        {
        if( iCodecs[i] == &aCodec )
            {
            delete iCodecs[i];
            iCodecs.Remove( i );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::CMceAudioStream
// -----------------------------------------------------------------------------
//
CMceAudioStream::CMceAudioStream()
    :CMceMediaStream()
    {
    iType = KMceAudio;
    }


// -----------------------------------------------------------------------------
// CMceAudioStream::ConstructL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMceAudioStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::InitializeL( CMceSession& aParent )
    {
    
    CMceMediaStream::InitializeL( aParent );

    InitializeL( &aParent.Manager() );
            
    for( TInt i = 0; i < iCodecs.Count(); i++ )
        {
        iCodecs[i]->InitializeL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CMceAudioStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceAudioStream::InitializeL( CMceManager* aManager )
    {
    CMceMediaStream::InitializeL( aManager );
    if ( aManager && iCodecs.Count() == 0 )
        {
        const RPointerArray<const CMceAudioCodec> supportedCodes = 
                                      aManager->SupportedAudioCodecs();
        for( TInt i = 0; i < supportedCodes.Count(); i++ )
            {
            CMceAudioCodec* codec = supportedCodes[i]->CloneL();
            CleanupStack::PushL( codec );
            iCodecs.AppendL( codec );
            CleanupStack::Pop( codec );
            }
                                      
        }
        
    }




