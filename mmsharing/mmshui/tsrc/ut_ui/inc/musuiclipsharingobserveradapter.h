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

#ifndef __MUSUICLIPSHARINGOBSERVER_H__
#define __MUSUICLIPSHARINGOBSERVER_H__

#include "musuiclipsharingobserver.h"

#include <e32base.h>



class CMusUiClipSharingObserverAdapter : public CBase,
                                         public MMusUiClipSharingObserver
    {



    public:

        CMusUiClipSharingObserverAdapter();

        ~CMusUiClipSharingObserverAdapter();


    public:

        // MMusUiClipSharingObserver
        void RunFetchDialogL();

        TBool SelectAddressDialogL( CDesCArray* aAddressArray,
                                            TDes& aSelectedAddress );

        void ManualAddressEntryL( const TDesC& aAddress );

        void ShowWaitDialogL( const TDesC& aPrompt );

        void DismissWaitDialogL( );

        void DismissWaitDialogL( const TDesC& aReason );

        void ShowErrorDialogL( const TDesC& aMessage );

        void ShowInformationDialogL( TInt aResourceId );

        void ShowClipSharingPanesL();

        void ChangeOrientationL(
                                CAknAppUiBase::TAppUiOrientation aOrientation );

        void ShowNaviPaneIconL( TMusUiNaviMediaDecorator aIcon,
                                TBool aVisible );

        void UpdateSessionTime( const TDesC& aSessionTime );

        void SetVolume( TBool aUp );

        void HandleError( TInt aError );

        void HandleExitL();

        void EndOfClip();


        enum TMusUiClipSharingObserverFunction
            {
            ENone,                          // default value
            ERunFetchDialogL,
            ESelectAddressDialogL,
            EManualAddressEntryL,
            EShowWaitDialogL,
            EDismissWaitDialogL,
            EDismissWaitDialogL2,
            EShowErrorDialogL,
            EShowInformationDialogL,
            EShowClipSharingPanesL,
            EChangeOrientationL,
            EShowNaviPaneIconL,
            EUpdateSessionTime,
            ESetVolume,
            EHandleError,
            EHandleExitL,
            EEndOfClip

            };


        TMusUiClipSharingObserverFunction iCalledObserver;
        TInt iErrorCode;
        TInt iError;
        TInt iShowErrorDialogErrorCode ;
        
        HBufC* iAddress;
        
    };

#endif