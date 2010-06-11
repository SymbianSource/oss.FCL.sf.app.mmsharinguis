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

//#include "musresourcefinderutil.h"
#include <apgtask.h>
#include <aknglobalnote.h>

/*
HBufC* MusResourceFinderUtil::ResourcePathL( const TDesC& aFileName )
    {
    return aFileName.AllocL();
    }
*/

const TUid uid = 
    {
    0x1234
    };


CAknNotifyBase::CAknNotifyBase( TUid )
    {
    
    }

void CAknNotifyBase::SetSecondaryDisplayData(CAknSDData* /*aData*/)
    {
    
    }

CAknNotifyBase::~CAknNotifyBase()
    {
    
    }



void CAknNotifyBase::ConstructL()
    {
    
    }

CAknGlobalNote::CAknGlobalNote()
: CAknNotifyBase( uid )
    {
    
    }

/**
* Two-phased constructor.
* @return Pointer to new global note object.
*/
CAknGlobalNote* CAknGlobalNote::NewL()
    {
    return new( ELeave ) CAknGlobalNote();
    }

/**
* Two-phased constructor.
* @return Pointer to new global note object.
*/
CAknGlobalNote* CAknGlobalNote::NewLC()
    {
    CAknGlobalNote* note = new( ELeave ) CAknGlobalNote();
    CleanupStack::PushL( note );
    return note;
    }

/**
* Destructor.
*/
CAknGlobalNote::~CAknGlobalNote()
    {
    
    }

/**
* Enable or disable all text processing done by the dialog.
* This includes text wrapping, text truncation
* and reordering of bidirectional text.
*
* By default, it is enabled.
*
* If text processing is disabled, lines are broken only at explicit line 
* end characters and they are not truncated, but drawn as long as they
* fit. Also, the dialog does not handle reordering of bidirectional text.
*
* @since Series 60 2.0
* @param aEnabled Enable or disable all text processing.
*/
void CAknGlobalNote::SetTextProcessing(TBool /*aEnabled*/)
    {
    
    }

/**
* Displays the global note.
* @param aType Note type.
* @param aNoteText Note text.
* @return Note ID.
*/
TInt CAknGlobalNote::ShowNoteL( TAknGlobalNoteType /*aType*/,
                                const TDesC& /*aNoteText*/)
    {
    return KErrNone;
    
    }

/**
* Displays the global note.
* @param aStatus Reference to request status.
* @param aType Note type.
* @param aNoteText Note text.
* @return Note ID.
*/
TInt CAknGlobalNote::ShowNoteL(TRequestStatus& aStatus, 
                        TAknGlobalNoteType /*aType*/,
                        const TDesC& /*aNoteText*/)
    {
    TRequestStatus* stat = &aStatus;
    User::RequestComplete( stat, KErrNone );
    return KErrNone;
    }

/**
* Cancels the global note.
* @param aNoteId Note ID.
*/
void CAknGlobalNote::CancelNoteL(TInt /*aNoteId*/)
    {
    
    }


EXPORT_C MWsClientClass::MWsClientClass()
    {
    
    }

EXPORT_C RWsSession::RWsSession()
    {
    
    }
    
EXPORT_C TInt RWsSession::Connect()
    {
    return KErrNone;
    }


EXPORT_C void RWsSession::Close()
    {
    
    }









