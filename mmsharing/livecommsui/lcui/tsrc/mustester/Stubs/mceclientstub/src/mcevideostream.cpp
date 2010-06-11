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
#include "mcevideostream.h"
#include "mcevideocodec.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcefilesource.h"
#include "mcemediasink.h"
#include "mcecamerasource.h"
//#include "mcertpsource.h"
#include "mcertpsink.h"
#include "mcedisplaysink.h"
#include "mceh263codec.h"

#define _FLAT_DATA static_cast<CMceComVideoStream*>( iFlatData )




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Factory method
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoStream* CMceVideoStream::NewL()
    {
    CMceVideoStream* self = CMceVideoStream::NewLC();
	CleanupStack::Pop( self );
	return self; 
    }

// -----------------------------------------------------------------------------
// Factory method
// Leaves instance on CleanupStack.
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoStream* CMceVideoStream::NewLC()
    {
    CMceVideoStream* self = new (ELeave) CMceVideoStream();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
    }


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMceVideoStream::~CMceVideoStream()
    {
	iCodecs.ResetAndDestroy();
	iSupportedVideoCodecs.ResetAndDestroy();
    }


// -----------------------------------------------------------------------------
// Returns codec.
// Returns the codecs available to the session.
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceVideoCodec>& CMceVideoStream::Codecs()
    {
    return iCodecs;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceVideoStream::AddCodecL( CMceVideoCodec* aCodec )
    {
    __ASSERT_ALWAYS(aCodec, User::Leave(KErrArgument));
    iCodecs.AppendL(aCodec);
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceVideoStream::RemoveCodecL( CMceVideoCodec& aCodec )
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
// NOT IMPLEMENTED YET
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceVideoStream::ReplaceCodecsL( RPointerArray<CMceVideoCodec>* /*aCodecs*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<const CMceVideoCodec>& 
        CMceVideoStream::SupportedVideoCodecs() const
    {
	return iSupportedVideoCodecs;
    }


// -----------------------------------------------------------------------------
// CMceVideoStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceVideoStream::InitializeL( CMceSession& aParent )
    {
	CMceMediaStream::InitializeL( aParent );

    InitializeL( &aParent.Manager() );
            
    for( TInt i = 0; i < iCodecs.Count(); i++ )
        {
        iCodecs[i]->InitializeL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CMceVideoStream::InitializeL
// -----------------------------------------------------------------------------
//
void CMceVideoStream::InitializeL( CMceManager* aManager )
    {
    CMceMediaStream::InitializeL( aManager );
    if ( aManager && iCodecs.Count() == 0 )
        {
        const RPointerArray<const CMceVideoCodec> supportedCodecs = 
                                      aManager->SupportedVideoCodecs();

        TBool fileCodecAdded( EFalse );
        for( TInt i = 0; i < supportedCodecs.Count() && !fileCodecAdded; i++ )
            {
            if ( iSource && iSource->Type() == KMceFileSource )
                {
                // Fake codec using file name     
                CMceFileSource* fileSource =
                                        static_cast<CMceFileSource*>(iSource);
                TBuf8<256*2> fileName;
                fileName.Copy( fileSource->iFileName );
                const TDesC8& currentCodecName( supportedCodecs[i]->SdpName() ); 
    
                if ( ( fileName.FindF( KMceSDPNameH264() ) >= 0 &&
                       currentCodecName.FindF( KMceSDPNameH264() ) >= 0 ||
                     ( fileName.FindF( KMceSDPNameH263() ) >= 0 &&
                       currentCodecName.FindF( KMceSDPNameH263() ) >= 0 ) ) )
                    {
                    CMceVideoCodec* codec = supportedCodecs[i]->CloneL();
                    CleanupStack::PushL( codec );
                    iCodecs.AppendL( codec );
                    CleanupStack::Pop( codec );
                    iState = EInitialized;
                    fileCodecAdded = ETrue;
                    }
                else
                    {
                    iState = ETranscodingRequired;
                    }
                
                }
            else
                {
                CMceVideoCodec* codec = supportedCodecs[i]->CloneL();
                CleanupStack::PushL( codec );
                iCodecs.AppendL( codec );
                CleanupStack::Pop( codec );
                }
            }
        }
    }


// -----------------------------------------------------------------------------
// CMceVideoStream::IsAllowedCombination
// -----------------------------------------------------------------------------
//    
TBool CMceVideoStream::IsAllowedCombination()
    {
    return ETrue;
    }


// -----------------------------------------------------------------------------
// Default C++ constructor.
// -----------------------------------------------------------------------------
//
CMceVideoStream::CMceVideoStream()
    :CMceMediaStream()
    {
    iType = KMceVideo;
    }


// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//    
void CMceVideoStream::ConstructL()
    {
    }

