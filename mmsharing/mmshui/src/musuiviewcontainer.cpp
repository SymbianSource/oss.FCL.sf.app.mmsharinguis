/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The MUS application's UI class.
*
*/


#include "musuiviewcontainer.h"
#include "musuidialogutil.h"
#include "musuidefinitions.h"
#include "muslogger.h" // debug logging
#include "musuigeneralview.h"
#include <musui.rsg>

#include <AknWaitDialog.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiViewContainer::~CMusUiViewContainer()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiViewContainer::~CMusUiViewContainer" );
    
    CancelWaitTimer();
    delete iWaitDialogTimer;

    if ( iWaitDialog )
        {
        TRAP_IGNORE( iWaitDialog->ProcessFinishedL() );
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiViewContainer::~CMusUiViewContainer" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiViewContainer::CancelWaitTimer()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiViewContainer::CancelWaitTimer" );
    if ( iWaitDialogTimer )
        {
        iWaitDialogTimer->Cancel();
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiViewContainer::CancelWaitTimer" );
    }  


// -----------------------------------------------------------------------------
// Dismisses the current active wait note, if any.
// -----------------------------------------------------------------------------
//
void CMusUiViewContainer::DismissWaitDialogL( TBool aReturnValue )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiViewContainer::DismissWaitDialogL" );
    
    if ( iWaitDialog )
        {
        iInvitationWaitDialogDismissed = ETrue;
        iWaitDialogReturnValueWhenDismissed = aReturnValue;
        
        iWaitDialog->ProcessFinishedL();
        iWaitDialog = NULL;
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiViewContainer::DismissWaitDialogL" );
    }


// -----------------------------------------------------------------------------
// Runs the Inviting recipient wait note
// -----------------------------------------------------------------------------
//
TBool CMusUiViewContainer::RunWaitDialogL( const TDesC& aPrompt,
                                           TInt aTimerInSeconds,
                                           TInt aExpirationMessageResourceId )
    {
    MUS_LOG_TDESC( "mus: [MUSUI ] -> CMusUiViewContainer::RunWaitDialogL: ", aPrompt );

    __ASSERT_ALWAYS( !iWaitDialog, User::Leave( KErrAlreadyExists ) );
    __ASSERT_ALWAYS( !iWaitDialogTimer, User::Leave( KErrAlreadyExists ) );
    
    MUS_LOG( "                 No previous wait dialog, continue " )
    
    iExpirationMessageResourceId = aExpirationMessageResourceId;
    
    iWaitDialog = new( ELeave ) CAknWaitDialog(
                    reinterpret_cast<CEikDialog**>(&iWaitDialog) ); 
                    
    iWaitDialog->SetTextL( aPrompt );

    TBool retVal = ETrue;
    iWaitDialog->PrepareLC( R_MUS_VIEW_INVITING_WAIT_NOTE );

    if ( aTimerInSeconds > 0 )
        {
        iWaitDialogTimer = CPeriodic::NewL( CActive::EPriorityStandard );
        iWaitDialogTimer->Start( KMusOneSecond * aTimerInSeconds,
                                 KMusOneSecond * aTimerInSeconds,
                                 TCallBack( DoWaitNoteTimeout, this ) );
        }

    iShowingWaitDialog = ETrue;
    retVal = iWaitDialog->RunLD();
    iShowingWaitDialog = EFalse;
    iWaitDialog = NULL;
    
    MUS_LOG( "                 Wait dialog's RunLD returned" )
    
    if ( iToolbarHandler )
        {
        MUS_LOG( "                 Completing toobar request" )
        iToolbarHandler->SetToolbarVisibility( ETrue );
        iToolbarHandler = NULL;
        }

    // Cancel timer when dialog returns.
    CancelWaitTimer();
    delete iWaitDialogTimer;
    iWaitDialogTimer = NULL;

    if ( iInvitationWaitDialogDismissed )
        {
        iInvitationWaitDialogDismissed = EFalse;
        retVal = iWaitDialogReturnValueWhenDismissed;
        }

    MUS_LOG( "mus: [MUSUI ] <- CMusUiViewContainer::RunWaitDialogL" );
    return retVal;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiViewContainer::SetWaitDialogCallbackL( MProgressDialogCallback* aCallback )
    {
    __ASSERT_ALWAYS( iWaitDialog, User::Leave( KErrNotReady ) );
    iWaitDialog->SetCallback( aCallback );
    }
	

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiViewContainer::WaitDialogShown() const
    {
    return iShowingWaitDialog;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void CMusUiViewContainer::RequestToolbarVisibilityOnceDialogDismissed( 
    CMusUiGeneralView* aToolbarHandler )
    {
    MUS_LOG1( 
        "mus: [MUSUI ]  <-> \
CMusUiViewContainer::RequestToolbarVisibilityOnceDialogDismissed, handler:%d", 
        reinterpret_cast<TUint32>( aToolbarHandler ) );
    
    iToolbarHandler = aToolbarHandler;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiViewContainer::CMusUiViewContainer()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Tick handler, callback function directly called by periodic timer
// -----------------------------------------------------------------------------
//
TInt CMusUiViewContainer::DoWaitNoteTimeout( TAny* aObject )
    {
    // Cast, and call non-static function.
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiViewContainer::DoWaitNoteTimeout" );
    
    CMusUiViewContainer* self = 
                            static_cast<CMusUiViewContainer*>( aObject );    
      
    TRAP_IGNORE( self->DismissWaitDialogL( EFalse ) );
    
    if ( self->iExpirationMessageResourceId != 0 )
        {
        TRAP_IGNORE( MusUiDialogUtil::ShowGlobalInformationDialogL( 
                                        self->iExpirationMessageResourceId ) ) 
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiViewContainer::DoWaitNoteTimeout" );
    // Return 0 ( false ) to indicate that we don't want more ticks
    return KErrNone;
    }


// end of file
