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

#include "musuiclipsharingobserveradapter.h"


CMusUiClipSharingObserverAdapter::CMusUiClipSharingObserverAdapter()
    {
    iAddress = NULL;
    iCalledObserver = ENone;
    iErrorCode = KErrNone;
    iShowErrorDialogErrorCode  = KErrNone;
    }


CMusUiClipSharingObserverAdapter::~CMusUiClipSharingObserverAdapter()
    {
    delete iAddress;
    }


void CMusUiClipSharingObserverAdapter::RunFetchDialogL()
    {
    iCalledObserver = ERunFetchDialogL;
    User::LeaveIfError( iErrorCode );
    }

TBool CMusUiClipSharingObserverAdapter::SelectAddressDialogL( CDesCArray* aAddressArray,
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


void CMusUiClipSharingObserverAdapter::ManualAddressEntryL( const TDesC& aAddress )
    {
    delete iAddress;
    iAddress = aAddress.AllocL();
    iCalledObserver = EManualAddressEntryL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::ShowWaitDialogL( const TDesC& aPrompt )
    {
    iCalledObserver = EShowWaitDialogL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::DismissWaitDialogL( )
    {
    iCalledObserver = EDismissWaitDialogL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::DismissWaitDialogL( const TDesC& aReason )
    {
    iCalledObserver = EDismissWaitDialogL2;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::ShowErrorDialogL( const TDesC& aMessage )
    {
    
    iCalledObserver = EShowErrorDialogL;
    User::LeaveIfError( iShowErrorDialogErrorCode  );
    }


void CMusUiClipSharingObserverAdapter::ShowInformationDialogL( TInt aResourceId )
    {
    iCalledObserver = EShowInformationDialogL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::ShowClipSharingPanesL()
    {
    iCalledObserver = EShowClipSharingPanesL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::ChangeOrientationL(
                        CAknAppUiBase::TAppUiOrientation aOrientation )
    {
    iCalledObserver = EChangeOrientationL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::ShowNaviPaneIconL( TMusUiNaviMediaDecorator aIcon,
                        TBool aVisible )
    {
    iCalledObserver = EShowNaviPaneIconL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::UpdateSessionTime( const TDesC& aSessionTime )
    {
    iCalledObserver = EUpdateSessionTime;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::SetVolume( TBool aUp )
    {
    iCalledObserver = ESetVolume;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::HandleError( TInt aError )
    {
    iError = aError;
    iCalledObserver = EHandleError;
    //User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::HandleExitL()
    {
    iCalledObserver = EHandleExitL;
    User::LeaveIfError( iErrorCode );
    }


void CMusUiClipSharingObserverAdapter::EndOfClip()
    {
    iCalledObserver = EEndOfClip;
    User::LeaveIfError( iErrorCode );
    }

