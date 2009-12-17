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
* Description:  The MUS application's UI class.
*
*/



#ifndef MUSUIVIEWCONTAINER_H
#define MUSUIVIEWCONTAINER_H


#include <coecntrl.h>                // Control base class
#include <AknProgressDialog.h>

class CAknWaitDialog;
class CMusUiGeneralView;

/**
 *
 *
 */
class CMusUiViewContainer :	public CCoeControl
    {

public:  // constructors and destructor

    /**
     * Destructor.
     */
    ~CMusUiViewContainer();

public: // new functions
	
	void CancelWaitTimer();

    /**
     * Dismisses the current active wait note, if any.
     */
    void DismissWaitDialogL( TBool aReturnValue = ETrue );

    /**
     * Shows sychronous wait dialog with softkey R_AVKON_SOFTKEYS_CANCEL.
     * @param aPrompt Prompt text shown in dialog
     * @param aTimerInSeconds Timer in seconds when dialog is dismissed
     *        automatically. Timer value 0 means that no timer is used.
     * @param aExpirationMessageResourceId Message to be loaded and shown if 
     *        timer expires. Value 0 means that no message is shown.
     * @return ETrue if wait note is dismissed by calling DismissWaitDialogL
     *         with param ETrue. EFalse if user pressed cancel, timer expires
     *         or if DismissWaitDialogL is called with param EFalse.
     */
    TBool RunWaitDialogL( const TDesC& aPrompt, 
                          TInt aTimerInSeconds = 0,
                          TInt aExpirationMessageResourceId = 0 );
    
    /**
     * Set call back service of wait dialog
     * @param aCallback A pointer of class MProgressDialogCallback
     *        which implemented DialogDismissedL 
     */
    void SetWaitDialogCallbackL( MProgressDialogCallback* aCallback );
    
    /**
    * Checks whether wait dialog is shown currently.
    * @return ETrue if wait dialog is show.
    */
    TBool WaitDialogShown() const;
    
    /**
    * Request that toolbar visibility is set once currently shown dialog 
    * is dismissed.
    * @param aToolbarHandler if NULL is passed, previous request is canceled.
    *        Ownership not transferred.
    */
    void RequestToolbarVisibilityOnceDialogDismissed( 
                                CMusUiGeneralView* aToolbarHandler );
    
protected: // constructors
    
    /**
     * Meant to be a base class, no instantiation
     */
    CMusUiViewContainer();

private: // new static functions

    /**
     * Tick handler, callback function directly called by periodic timer
     *
     * @param aObject Pointer passed to TCallback.
     * @return Value to indicate if callback function should be called again.
     */
    static TInt DoWaitNoteTimeout( TAny* aObject );


private: // data
    
    /**  Wait dialog to show the invite is sent. Not own. */
    CAknWaitDialog* iWaitDialog;
    
    /**  Periodic timer active object.  Owns. */
    CPeriodic* iWaitDialogTimer;
    
    /**
    * Indicates if wait note is dismissed using DismissWaitDialogL
    */
    TBool iInvitationWaitDialogDismissed;
    
    /**
    * If iInvitationWaitDialogDismissed is ETrue, this value should be returned
    * from RunWaitDialogL
    */
    TBool iWaitDialogReturnValueWhenDismissed;
    
    /**
    * Resource ID of message to be shown if timer expires
    */
    TInt iExpirationMessageResourceId;
    
    /**
    * Flag indicating whether wait dialog is currently shown.
    */
    TBool iShowingWaitDialog;
    
    /**
    * Not owned.
    */
    CMusUiGeneralView* iToolbarHandler;
    
    };


#endif // MUSUISENDVIEWCONTAINER_H

// end of file
