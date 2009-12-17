/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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


// INCLUDE FILES
#include <E32base.h>
#include <vedmovie.h>
#include <mmcctranscoder.h>
#include <mmcccodecinformation.h>
#include <mmccinterfacedef.h>

#include "transcoderimpl.h"
#include "mcctranscoderlogs.h"



// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccTranscoder::CMccTranscoder
//
// CMccTranscoder default constructor, can NOT contain any code,
// that might leave
// Phase #1 of 2-phase constructor
// -----------------------------------------------------------------------------
//
CMccTranscoder::CMccTranscoder()
    {
        
    }

// -----------------------------------------------------------------------------
// CMccTranscoder::NewL
// -----------------------------------------------------------------------------
//
CMccTranscoder* CMccTranscoder::NewL()
    {
    CMccTranscoder* self = new ( ELeave ) CMccTranscoder;
	CleanupStack::PushL ( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;   
    }

// -----------------------------------------------------------------------------
// CMccTranscoder::ConstructL
// -----------------------------------------------------------------------------
//
void CMccTranscoder::ConstructL()
    {
    iTranscoderImpl = CMccTranscoderImpl::NewL();   
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoder::~CMccTranscoder
// -----------------------------------------------------------------------------
//
CMccTranscoder::~CMccTranscoder()
    {
    delete iTranscoderImpl;  
    }
 
// -----------------------------------------------------------------------------
// CMccTranscoder::RegisterEventObserver
// -----------------------------------------------------------------------------
// 
void CMccTranscoder::RegisterEventObserver( MMccTranscoderObserver& aObserver )
    {
    __TRANSCODER( "CMccTranscoder::RegisterEventObserver" )

    if ( iTranscoderImpl )   
        {
        iTranscoderImpl->RegisterEventObserver( aObserver );
        }
    }

// -----------------------------------------------------------------------------
// CMccTranscoder::UnregisterEventObserver
// -----------------------------------------------------------------------------
// 
void CMccTranscoder::UnregisterEventObserver()
    {
    __TRANSCODER( "CMccTranscoder::UnregisterEventObserver" )

    if ( iTranscoderImpl )
        {
        iTranscoderImpl->UnregisterEventObserver(); 
        }
    }

// -----------------------------------------------------------------------------
// CMccTranscoder::TranscodeFile
// -----------------------------------------------------------------------------
// 
void CMccTranscoder::TranscodeFileL( TUint32& aSessionId, const TDesC8& aVal )
    {
    __ASSERT_ALWAYS( iTranscoderImpl, User::Leave( KErrNotReady ) );
    
    iTranscoderImpl->TranscodeFileL( aSessionId,  aVal);
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoder::CancelTranscodeFile
// -----------------------------------------------------------------------------
// 
void CMccTranscoder::CancelTranscodeFileL( TUint32 aSessionId )
    {
    __TRANSCODER( "CMccTranscoder::CancelTranscodeFileL" )

    iTranscoderImpl->CancelTranscodeFileL( aSessionId );
    }


// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
