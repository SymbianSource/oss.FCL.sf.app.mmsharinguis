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
* Description:  Mus Application's Sharing Observer Interface
*
*/



#ifndef MUSUISHARINGOBSERVER_H
#define MUSUISHARINGOBSERVER_H


#include <e32cmn.h>
#include <AknProgressDialog.h>

class MMusUiSharingObserver
    {
	
    public:

        /*
        * Shows sychronous wait dialog with softkey R_AVKON_SOFTKEYS_CANCEL.
        * @param aPrompt Prompt text shown in dialog
        * @param aTimerInSeconds Timer in seconds when dialog is dismissed
        *        automatically. Timer value 0 means that no timer is used.
        * @param aExpirationMessageResourceId is ID of resource indicating
        *        the message to be shown if timer expires. Value 0 means that
        *        no message is shown.
        * @return EFalse if dialog is dismissed by user pressing cancel
        *         or by expiration of timer. ETrue if DismissWaitDialogL
        *         is called with parameter ETrue;
        */
        virtual TBool ShowWaitDialogL( 
                                    const TDesC& aPrompt, 
                                    TInt aTimerInSeconds = 0,
                                    TInt aExpirationMessageResourceId = 0 ) = 0;
        
        /*
        * Dismisses sychronous wait dialog shown by calling ShowWaitDialogL.
        * @param aReturnValue determines return value of function 
        *        ShowWaitDialogL.
        */
        virtual void DismissWaitDialogL( TBool aReturnValue = ETrue ) = 0;
        
        /**
        * Set call back service of wait dialog
        * @param aCallback A pointer of class MProgressDialogCallback
        *        which implemented DialogDismissedL 
        */
        virtual void SetWaitDialogCallbackL( MProgressDialogCallback* aCallback ) = 0;
        
        virtual void ShowStatusPaneTitleL() = 0;
        
        /**
        *
        */
        virtual void EnableMiddleSoftkeyL( TInt aResourceId ) = 0;   
	    
        virtual void ActivateVolumeControlL( TInt aValue, TBool aOnlyIfVisible = EFalse ) = 0;  
        
        virtual void HighlightSelectedToolbarItem( TInt aCommand ) = 0;
        
        virtual void ReplaceToolbarCommand( TInt aOldCommand, 
                                            TInt aNewCommand,
                                            TBool aSetNewCommandFocused ) = 0;
        
        virtual void RefreshAudioRoutingToolbarButton() = 0;
        
        virtual void UpdateSessionTime( const TDesC& aSessionTime ) = 0;
        
        virtual void DismissMenuBar() = 0;
                                                    
    };

#endif // MUSUISHARINGOBSERVER_H
            
// end of file
