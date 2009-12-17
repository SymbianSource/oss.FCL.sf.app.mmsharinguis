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

#include "musuilivesharingobserveradapter.h"


CMusUiLiveSharingObserverAdapter::CMusUiLiveSharingObserverAdapter()
    {
    iAddress = NULL;
    iErrorCode = KErrNone;
    iShowErrorDialogErrorCode = KErrNone;
    }

CMusUiLiveSharingObserverAdapter::~CMusUiLiveSharingObserverAdapter()
    {
    delete iAddress;
    }


TBool CMusUiLiveSharingObserverAdapter::SelectAddressDialogL( CDesCArray* aAddressArray,
                                    TDes& aSelectedAddress )
    {


    TInt selectedIndex( 0 );
    aSelectedAddress = (*aAddressArray)[ selectedIndex ];
    delete aAddressArray;
    aAddressArray = NULL;

    iCalledObserver = ESelectAddressDialogL;
    User::LeaveIfError( iErrorCode );
    return ETrue;
    }


void CMusUiLiveSharingObserverAdapter::ManualAddressEntryL( const TDesC& aAddress )
    {
    delete iAddress;
    iAddress = aAddress.AllocL();
    iCalledObserver = EManualAddressEntryL;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::ShowWaitDialogL( const TDesC& aPrompt )
    {
    iCalledObserver = EShowWaitDialogL;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::DismissWaitDialogL( )
    {
    iCalledObserver = EDismissWaitDialogL;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::ShowErrorDialogL( const TDesC& aMessage )
    {
    iCalledObserver = EShowErrorDialogL;
    User::LeaveIfError( iShowErrorDialogErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::ShowInformationDialogL( TInt aResourceId )
    {
    iCalledObserver = EShowInformationDialogL;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::ShowLiveSharingPanesL()
    {
    iCalledObserver = EShowLiveSharingPanesL;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::ChangeOrientationL(
                        CAknAppUiBase::TAppUiOrientation aOrientation )
    {
    iOrientation = aOrientation;
    iCalledObserver = EChangeOrientationL;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::ShowNaviPaneIconL(
                TMusUiNaviMediaDecorator aIcon,
                TBool aVisible )
    {
    iIcon = aIcon;
    iCalledObserver = EShowNaviPaneIconL;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::UpdateSessionTime( const TDesC& aSessionTime )
    {
    iCalledObserver = EUpdateSessionTime;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::SetVolume( TBool aUp )
    {
    iCalledObserver = ESetVolume;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::SetZoom( TBool aIn )
    {
    iCalledObserver = ESetZoom;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::HandleError( TInt aError )
    {
    iError = aError;
    iCalledObserver = EHandleError;
    //User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::HandleExitL()
    {
    iCalledObserver = EHandleExitL;
    User::LeaveIfError( iErrorCode );
    }



void CMusUiLiveSharingObserverAdapter::CancelInvitationTimer()
    {
    iCalledObserver = ECancelInvitationTimer;
    User::LeaveIfError( iErrorCode );
    }


