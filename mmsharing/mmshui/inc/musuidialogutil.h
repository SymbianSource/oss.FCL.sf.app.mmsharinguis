/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSDIALOGUTIL_H
#define MUSDIALOGUTIL_H

#include <e32base.h>
#include <badesca.h>

class MusUiDialogUtil
    {

    public:
        /**
         */
        static TBool ShowGlobalConfirmationQueryDialogL( const TDesC& aPrompt );

        static void ShowInformationDialogL( const TDesC& aPrompt );

        static void ShowInformationDialogL( TInt aResourceId );

        static void ShowGlobalInformationDialogL( TInt aResourceId );

        static void ShowGlobalInformationDialogL( const TDesC& aMessage );
        
        static void ShowGlobalErrorDialogL( const TDesC& aMessage );

        static void ShowGlobalErrorDialogL( TInt aResourceId );
        
        static void ShowGlobalErrorModalDialogL( const TDesC& aMessage );
        
        static void ShowGlobalErrorModalDialogL( TInt aResourceId );

        static TBool SelectAddressDialogL( CDesCArray* aAddressArray,
                                           TDes& aSelectedAddress );

        static TBool ShowTextQueryDialogL( const TDesC& aPrompt, TDes& aValue );

        static TBool ShowTextQueryDialogL( TInt aPromptResourceId,
                                           TDes& aValue );

    };



#endif // MUSDIALOGUTIL_H
