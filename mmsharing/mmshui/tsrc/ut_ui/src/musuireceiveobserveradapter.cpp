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

#include "musuireceiveobserveradapter.h"


CMusUiReceiveObserverAdapter::CMusUiReceiveObserverAdapter()
    {
    iAddress = NULL;
    iErrorCode = KErrNone;
    iShowErrorDialogErrorCode = KErrNone;
    }

CMusUiReceiveObserverAdapter::~CMusUiReceiveObserverAdapter()
    {
    delete iAddress;
    }



void CMusUiReceiveObserverAdapter::ShowErrorDialog( const TDesC& aPrompt )
    {
    iCalledObserver = EShowErrorDialog;
    }



void CMusUiReceiveObserverAdapter::ShowErrorDialogL( const TDesC& aMessage )
    {
    iCalledObserver = EShowErrorDialogL;
    User::LeaveIfError( iShowErrorDialogErrorCode );
    }


void CMusUiReceiveObserverAdapter::ShowInvitationQueryL( const TDesC& aQuery )
    {
    iCalledObserver = EShowInvitationQueryL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::DismissInvitationQuery( TBool aDelete )
    {
    iCalledObserver = EDismissInvitationQuery;
    }



void CMusUiReceiveObserverAdapter::ShowWaitDialogL( const TDesC& aPrompt )
    {
    iCalledObserver = EShowWaitDialogL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::DismissWaitDialogL( )
    {
    iCalledObserver = EDismissWaitDialogL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::ShowInformationDialogL( TInt aResourceId )
    {
    iResourceId = aResourceId;
    iCalledObserver = EShowInformationDialogL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::ShowNaviPaneIconL(
                TMusUiNaviMediaDecorator aIcon,
                TBool aVisible )
    {
    iIcon = aIcon;
    iVisible = aVisible;
    iCalledObserver = EShowNaviPaneIconL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::ShowStatusPaneL( TBool aVisible )
    {
    iCalledObserver = EShowStatusPaneL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::ChangeOrientationL(
                        CAknAppUiBase::TAppUiOrientation aOrientation )
    {
    iOrientation = aOrientation;
    iCalledObserver = EChangeOrientationL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::UpdateSessionTime( const TDesC& aSessionTime )
    {
    iCalledObserver = EUpdateSessionTime;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::SetVolume( TBool aUp )
    {
    iCalledObserver = ESetVolume;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::HandleError( TInt aError )
    {
    iError = aError;
    iCalledObserver = EHandleError;
    //User::LeaveIfError( iErrorCode );
    }


void CMusUiReceiveObserverAdapter::HandleExitL()
    {
    iCalledObserver = EHandleExitL;
    User::LeaveIfError( iErrorCode );
    }
