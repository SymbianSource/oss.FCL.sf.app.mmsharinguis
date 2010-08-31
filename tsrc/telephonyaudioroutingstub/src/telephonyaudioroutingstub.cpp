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


#include <telephonyaudiorouting.h>




// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CTelephonyAudioRouting* CTelephonyAudioRouting::NewL(
                            MTelephonyAudioRoutingObserver& aObserver)
	{
    CTelephonyAudioRouting* self = 
                        new (ELeave) CTelephonyAudioRouting( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
	}


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CTelephonyAudioRouting::~CTelephonyAudioRouting()
    {
    iAvailableOutputs.Reset();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
const TArray<CTelephonyAudioRouting::TAudioOutput>& 
                                CTelephonyAudioRouting::AvailableOutputs()
    {
    return iAvailableOutputs.Array();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CTelephonyAudioRouting::TAudioOutput CTelephonyAudioRouting::Output()
    {
    return iCurrentOutput;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CTelephonyAudioRouting::SetOutputL( TAudioOutput aOutput )
    {
    __ASSERT_ALWAYS( aOutput != ENone, User::Leave( KErrArgument ) );
    
    if ( iForceFailWithCode != KErrNone )
        {
        TInt leaveValue = iForceFailWithCode;
        iForceFailWithCode = KErrNone;
        User::Leave( leaveValue );
        }
        
    iCurrentOutput = aOutput;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CTelephonyAudioRouting::TAudioOutput CTelephonyAudioRouting::PreviousOutput()
    {
    return iPreviousOutput;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CTelephonyAudioRouting::SetShowNote( TBool aMode )
    {
    iShowNoteMode = aMode;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CTelephonyAudioRouting::GetShowNote( TBool& aMode )
    {
    aMode = iShowNoteMode;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CTelephonyAudioRouting::CTelephonyAudioRouting( 
                                    MTelephonyAudioRoutingObserver& aObserver )
    : iObserver( aObserver )                               
    {
    iForceFailWithCode = KErrNone;
    iCurrentOutput = EHandset;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CTelephonyAudioRouting::ConstructL()
    {
    
    }








   
    
