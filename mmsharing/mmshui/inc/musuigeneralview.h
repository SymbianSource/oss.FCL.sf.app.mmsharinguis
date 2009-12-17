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



#ifndef MUSUIGENERALVIEW_H
#define MUSUIGENERALVIEW_H

#include "musuisharingobserver.h"
#include "musuiactivetimerobserver.h"

#include <avkon.rsg>              // Avkon resource file
#include <aknview.h>              // Avkon base view class
#include <akntoolbarobserver.h>

class CMusUiAppUi;
class CMusUiEventController;
class CMusUiActivityManager;
class CMusUiViewContainer;
class CAknIncallBubble;
class CMusUiActiveTimer;
class CMusUiBackgroundViewContainer;

/**
 * The base class for the MUS application's view classes.
 * Encapsulates common methods for views.
 *
 * @lib musui.exe
 */
class CMusUiGeneralView : public CAknView,
                          public MAknToolbarObserver,
                          public MMusUiSharingObserver,
                          public MMusUiActiveTimerObserver
    {
    
public: // constructors and destructor

    enum TMusFgBgEventType
        {
        EMusFgBgEventNone,
        EMusFgEvent,
        EMusBgEvent
        };

    /**
    * EPOC default constructor.
    */
	void ConstructL();

    /**
     * Destructor.
     */
    virtual ~CMusUiGeneralView();

public: // new functions

    /**
     * @return the pointer to the application's AppUi (CMusUiAppUi).
     */
    CMusUiAppUi* MusAppUi() const;
    
    /**
     * Refreshes the view.  Called after the mode has been changed from portrait
     * to landscape or vice versa.  This is a virtual function that must be 
     * implemented by all concrete MUS UI views.
     */
    virtual void RefreshView() = 0;
    
    virtual void HandleToolbarCommandL( TInt aCommand ) = 0;
    
    virtual void HandleKeyUpEvent( TInt aKeyCode );
    
    virtual void SetToolbarVisibility( TBool aVisible );

    void AsyncQueryDialogAcceptedL( TBool aAccepted );

    TInt FocusedToolbarItem() const;
    
    virtual void LevelIndicatorDismissed() {};
    virtual void SetInitialFocusedItemL() = 0;
    
    TBool OperatorSpecificFunctionality() const;
    /**
    * Called when UI comes to foregound / goes to background.
    * CAknView version of HandleForegroundEventL is not used
    * at it generates lots of events. Instead, appui implements
    * foreground event callback funtion and forwards events to views
    * via this method.
    */
    virtual void DoHandleForegroundEventL( TBool aForeground ) = 0;  


public: // from base class MAknToolbarObserver
    
    virtual void DynInitToolbarL( TInt aResourceId, CAknToolbar* aToolbar );
    
    virtual void OfferToolbarEventL( TInt aCommand );
    

public: // from MMusUiSharingObserver

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
    virtual TBool ShowWaitDialogL( const TDesC& aPrompt, 
                                   TInt aTimerInSeconds = 0,
                                   TInt aExpirationMessageResourceId = 0 );
    
    /*
    * Dismisses sychronous wait dialog shown by calling ShowWaitDialogL.
    * @param aReturnValue determines return value of function 
    *        ShowWaitDialogL.
    */
    virtual void DismissWaitDialogL( TBool aReturnValue = ETrue );
    
    /*
    * Set call back service of wait dialog
    * @param aCallback A pointer of class MProgressDialogCallback
    *        which implemented DialogDismissedL 
    */
    virtual void SetWaitDialogCallbackL( MProgressDialogCallback* aCallback );
    
    virtual void ShowStatusPaneTitleL();

    virtual void EnableMiddleSoftkeyL( TInt aResourceId );

    virtual void ActivateVolumeControlL( TInt aValue );  
    
    virtual void HighlightSelectedToolbarItem( TInt aCommand );
    
    /**
    * General functionality for replacing a toolbar command
    * Function should be overridden for specific functionality
    */
    virtual void ReplaceToolbarCommand( TInt aOldCommand, 
                                        TInt aNewCommand,
                                        TBool aSetNewCommandFocused );
    
    /**
    * If toolbar is shown, audio routing button will be refreshed according to
    * current state of audio routing. 
    * In case there is no toolbar, middle softkey will be updated
    */
    virtual void RefreshAudioRoutingToolbarButton();
    
    virtual void UpdateSessionTime( const TDesC& aSessionTime );                                  
                                        
                                        
public: // from base class CAknView

    /**
     * Takes care of command handling.
     *
     * @param aCommand  The command that was received.
     */
    virtual void HandleCommandL( TInt aCommand );

    /**
     * From MEikMenuObserver, called by framework before constructing menupane.
     * Initializes menu items common to all sharing types.
     */
    virtual void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
    
public: // from MMusUiActiveTimerObserver    
    
    /**
     * Indicates that a certain amount of time elapsed
     */    
    virtual void TimerComplete( CMusUiActiveTimer* aTimer );


protected: // New functions

    virtual CMusUiEventController* EventController() const = 0;
    
    virtual CMusUiViewContainer& SharingContainer() const = 0;
    
    /**
    * Leaves with KErrNotFound if eventcontroller is not available
    */
    CMusUiEventController& EventControllerL() const;
    
    /**
     * Called when event occurs of type EEventFocusLost or
     * EEventFocusGained.
     *
     * @param aForeground ETrue if the application is in the foreground,
     *                    otherwise EFalse.
     * @param aExit indicates if the application should exit 
     */
    void HandleForegroundEventL( TBool aForeground, TBool aExit );    
    

protected:

    /**
     * Gets called by the framework when it's activating this view.
     *
     * @param aPrevViewId Previous view id
     * @param aCustomMessageId Custom message's id
     * @param aCustomMessage Custom message
     */
    void DoActivateL( const TVwsViewId& aPrevViewId,
                      TUid aCustomMessageId,
                      const TDesC8& aCustomMessage );

    /**
     * Gets called by the framework when it's deactivating this view.
     */
    void DoDeactivate();

protected:

   
    void CompleteForegroundEventL();
    void DoForegroundEventL();
    void DoBackgroundEventL( TBool aExit );
    void SetCurrentFgBgEvent( TMusFgBgEventType aEventType );
    TInt ToolbarPlaceHolderHeight() const;
    void UpdateBackgroundOrdinalPosition( TBool aUp );  

protected: // data

    /**
    * ETrue if operator specific functionality is needed 
    */
    TBool iOperatorSpecificFunctionality;
        
    TBool iToolbarItemSelected;
    
private: // data
    
    CAknIncallBubble* iIncallBubble;
    
    CMusUiActiveTimer* iSessionEndTimer;
    
    CMusUiActiveTimer* iFgBgTimer;
    
    TMusFgBgEventType iCurrentFgBgEvent;
    
    TBool iCurrentExitSetting;
    
    CMusUiBackgroundViewContainer* iBackgroundContainer; // skin background drawer
    
    };

// macro for NO_TOOLBAR case
#define MUS_NO_TOOLBAR ( iOperatorSpecificFunctionality && !(AknLayoutUtils::PenEnabled()))

#endif // MUSUIGENERALVIEW_H

// end of file
