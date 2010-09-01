/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef UT_MUSUIOBSERVERSSTUB_H
#define UT_MUSUIOBSERVERSSTUB_H


#include "musuieventobserver.h"


#include <e32base.h>
#include <aknappui.h>

/**
*
* @lib musengine.lib
* @since S60 v3.2
*/
class CMusUiEventObserverAdapter : public CBase,
                                   public MMusUiEventObserver
    {



    public:

        CMusUiEventObserverAdapter();

        ~CMusUiEventObserverAdapter();


    public: // MMusUiEventObserver

        void ActivateLocalMusViewL( TUid aViewId );
        void ActivateLocalMusViewL( TUid aViewId,
                                            TUid aCustomMessageId,
                                      const TDesC8& aCustomMessage );
        TBool ShowConfirmationQueryDialogL( const TDesC& aPrompt );
        void ShowInformationDialogL( const TDesC& aPrompt );
        void ActivateContactViewL( TContactItemId aId );
        void HandleError( const TInt aReason ); // All ready defined
        void HandleExitL();


    public: // Helper

        /**
        * ETrue if all boolean member variables are set to EFalse and all the
        * pointers are NULL
        */
        TBool IsReseted();

        /**
        * Resets all the boolean member variable values to EFalse and deletes
        * and nullifies all the pointer variables.
        */
        void Reset();

    public: // Data

        enum TMusUiEventObserverFunction
            {
            ENone,                          // default value

            // MMusUiEventObserver
            EActivateLocalMusViewL,
            EActivateLocalMusViewL2,
            EShowConfirmationQueryDialogL,
            EShowInformationDialogL,
            EActivateContactViewL,
            EHandleError,
            EHandleExitL,

            };


        TInt iHandleErrorReason;
        TContactItemId iActivatedContactId;

        TMusUiEventObserverFunction iCalledObserver;

        TBool iBoolReturn;
        TInt iErrorCode;

        TUid iViewId;
        
        HBufC*  iPrompt;
        

    };




#endif // UT_MUSSTUBS_H

