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

#include "musuieventobserveradapter.h"


// ---- MEMBER FUNCTIONS OF CMusUiObserversStub --------------------------------


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiEventObserverAdapter::CMusUiEventObserverAdapter()
    {
    // NOP
    // Initialize...
    iCalledObserver = ENone;
    iBoolReturn = EFalse;
    iErrorCode = KErrUnknown;
    iPrompt = NULL;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiEventObserverAdapter::~CMusUiEventObserverAdapter()
    {
    delete iPrompt;
    }


// -------- Functions from MMusUiEventObserver -----------------

void CMusUiEventObserverAdapter::ActivateLocalMusViewL( TUid aViewId )
    {
    iCalledObserver = EActivateLocalMusViewL;
    iViewId = aViewId;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiEventObserverAdapter::ActivateLocalMusViewL( TUid aViewId,
                                    TUid aCustomMessageId,
                              const TDesC8& aCustomMessage )
    {
    iCalledObserver = EActivateLocalMusViewL2;
    iViewId = aViewId;
    User::LeaveIfError( iErrorCode );
    }


TBool CMusUiEventObserverAdapter::ShowConfirmationQueryDialogL( const TDesC& aPrompt )
    {
    iPrompt = aPrompt.AllocL();
    iCalledObserver = EShowConfirmationQueryDialogL;
    User::LeaveIfError( iErrorCode );
    }

void CMusUiEventObserverAdapter::ShowInformationDialogL( const TDesC& aPrompt )
    {
    iCalledObserver = EShowInformationDialogL;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiEventObserverAdapter::ActivateContactViewL(  TContactItemId aId )
    {
    iCalledObserver = EActivateContactViewL;
    iActivatedContactId = aId;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiEventObserverAdapter::HandleError( const TInt aReason )
    {
    iHandleErrorReason = aReason;
    iCalledObserver = EHandleError;
    }


void CMusUiEventObserverAdapter::HandleExitL()
    {
    iCalledObserver = EHandleExitL;
    User::LeaveIfError( iErrorCode );
    }

