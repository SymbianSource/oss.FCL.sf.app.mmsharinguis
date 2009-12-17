/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Mus Applications Event Observer Interface
*
*/



#ifndef __MUSUIRECEIVEOBSERVERADAPTER_H__
#define __MUSUIRECEIVEOBSERVERADAPTER_H__

#include "musuidefinitions.h"
#include "musuireceiveobserver.h"

#include <e32base.h>
#include <aknappui.h>
//#include <badesca.h>

class CMusUiReceiveObserverAdapter : public MMusUiReceiveObserver
    {

    public:
    
        CMusUiReceiveObserverAdapter();
        ~CMusUiReceiveObserverAdapter();

        void ShowErrorDialog( const TDesC& aPrompt );

        void ShowErrorDialogL( const TDesC& aMessage );

        void ShowInvitationQueryL( const TDesC& aQuery );

        void DismissInvitationQuery( TBool aDelete );

        void ShowWaitDialogL( const TDesC& aPrompt );

        void DismissWaitDialogL( );

        void ShowInformationDialogL( TInt aResourceId );

        void ShowNaviPaneIconL( TMusUiNaviMediaDecorator aIcon,
                                        TBool aVisible );

        void ShowStatusPaneL( TBool aVisible );

        void ChangeOrientationL(
                                CAknAppUiBase::TAppUiOrientation aOrientation );

        void UpdateSessionTime(  const TDesC& aSessionTime );

        void SetVolume( TBool aUp );

        void HandleError( TInt aError );

        void HandleExitL();


        enum TMusUiReceiveObserverFuncion
            {
            ENone,                          // default value
            EShowErrorDialog,
            EShowErrorDialogL,
            EShowInvitationQueryL,
            EDismissInvitationQuery,
            EShowWaitDialogL,
            EDismissWaitDialogL,
            EShowInformationDialogL,
            EShowNaviPaneIconL,
            EShowStatusPaneL,
            EChangeOrientationL,
            EUpdateSessionTime,
            ESetVolume,
            EHandleError,
            EHandleExitL
            };



        TMusUiReceiveObserverFuncion iCalledObserver;

        // Return values
        TBool iBoolReturn;

        // Error code
        TInt iErrorCode;

        TMusUiNaviMediaDecorator iIcon;

        CAknAppUiBase::TAppUiOrientation iOrientation;

        HBufC* iAddress;

        TInt iError;
        TInt iShowErrorDialogErrorCode;
        TBool iVisible;
        TInt iResourceId;

    };

#endif 

// end of file
