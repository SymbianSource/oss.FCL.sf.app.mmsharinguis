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

#ifndef MUSUILIVESHARINGOBSERVERADAPTER_H
#define MUSUILIVESHARINGOBSERVERADAPTER_H

#include "musuidefinitions.h"
#include "musuilivesharingobserver.h"

#include <e32base.h>
#include <aknappui.h>
#include <badesca.h>

class CMusUiLiveSharingObserverAdapter : public MMusUiLiveSharingObserver
    {

    public:

        CMusUiLiveSharingObserverAdapter();
        ~CMusUiLiveSharingObserverAdapter();


        TBool SelectAddressDialogL( CDesCArray* aAddressArray,
                                            TDes& aSelectedAddress );
        void ManualAddressEntryL( const TDesC& aAddress );

        void ShowWaitDialogL( const TDesC& aPrompt );

        void DismissWaitDialogL( );

        void ShowErrorDialogL( const TDesC& aMessage );

        void ShowInformationDialogL( TInt aResourceId );

        void ShowLiveSharingPanesL();

        void ChangeOrientationL(
                                CAknAppUiBase::TAppUiOrientation aOrientation );

        void ShowNaviPaneIconL(
                        TMusUiNaviMediaDecorator aIcon,
                        TBool aVisible );

        void UpdateSessionTime( const TDesC& aSessionTime );

        void SetVolume( TBool aUp );

        void SetZoom( TBool aIn );

        void HandleError( TInt aError );

        void HandleExitL();

        void CancelInvitationTimer();


        enum TMusUiLiveSharingObserverFuncion
            {
            ENone,                          // default value
            ESelectAddressDialogL,
            EManualAddressEntryL,
            EShowWaitDialogL,
            EDismissWaitDialogL,
            EShowErrorDialogL,
            EShowInformationDialogL,
            EShowLiveSharingPanesL,
            EChangeOrientationL,
            EShowNaviPaneIconL,
            EUpdateSessionTime,
            ESetVolume,
            ESetZoom,
            EHandleError,
            EHandleExitL,
            ECancelInvitationTimer
            };



        TMusUiLiveSharingObserverFuncion iCalledObserver;

        // Return values
        TBool iBoolReturn;

        // Error code
        TInt iErrorCode;
        
        TMusUiNaviMediaDecorator iIcon;
        
        CAknAppUiBase::TAppUiOrientation iOrientation;
        
        HBufC* iAddress;
        
        TInt iError;
        TInt iShowErrorDialogErrorCode;        


    };

#endif

// end of file
