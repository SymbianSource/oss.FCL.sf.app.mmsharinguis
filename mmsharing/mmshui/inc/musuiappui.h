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
* Description:  The MUS application's UI class.
*
*/


#ifndef MUSUIAPPUI_H
#define MUSUIAPPUI_H

#include "musuieventobserver.h"
#include "musuiqueryobserver.h"

#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknViewAppUi.h>
#include <remconcoreapitargetobserver.h>
#include <coecobs.h>

class CMusUiStatusPaneHandler;
class CMusUiActiveQueryDialog;
class CRemConInterfaceSelector;
class CRemConCoreApiTarget;

/**
 *  The MUS application's UI class.
 *
 *  @lib musui.exe
 */
class CMusUiAppUi : public CAknViewAppUi, 
                    public MMusUiEventObserver,
                    public MMusUiQueryObserver,
                    public MRemConCoreApiTargetObserver,
                    public MCoeControlObserver
    {

public:  // constructors and destructor

    /**
    * EPOC default constructor.
    */      
    void ConstructL( );

    /**
     * Destructor.
     */
    ~CMusUiAppUi();

public: // New functions

	CMusUiStatusPaneHandler* MusStatusPane();
    
    TBool ExitValue();
    

public: // from base class CEikAppUi

    void HandleWsEventL( const TWsEvent& aEvent, CCoeControl* aDestination );

    void HandleForegroundEventL( TBool aForeground );
    
public: // from MRemConCoreApiTargetObserver

    void MrccatoCommand( TRemConCoreApiOperationId aOperationId, 
                         TRemConCoreApiButtonAction aButtonAct );
                         

public:	// from MMusUiQueryObserver

    virtual void QueryAcceptedL( TBool aAccepted );

    /**
     * Error handler for QueryAcceptedL.
     *
     * @param aError is error to be handled.
     * @return KErrNone means handled value. All other values will lead to
     *         a panic.
     */
    virtual TInt HandleQueryError( TInt aError );


public:	// from MMusUiEventObserver

    virtual void ActivateLocalMusViewL( TUid aViewId );
	
	virtual void ActivateLocalMusViewL( TUid aViewId,
	                            TUid aCustomMessageId, 
	                            const TDesC8& aCustomMessage );

    virtual void ShowAsyncConfirmationQueryDialogL( const TDesC& aPrompt );

    virtual void SetToolbarVisibility( TBool aVisible );

    virtual void ShowNaviPaneIconL( TMusUiNaviMediaDecorator aIcon );

    virtual CEikonEnv* EikonEnv( ) const;
    
    virtual void TerminateCall();
    
    virtual CAknAppUiBase::TAppUiOrientation AppOrientation() const;

    /**
     * Switch the active view's orientation, if it's in the foreground,
     * from landscape to portrait, or vice versa.
     */
    virtual void SwitchOrientationL( TAppUiOrientation aAppOrientation );
    
    virtual void HandleError( const TInt aReason );

    virtual void HandleExit();
    
    virtual TBool OrientationCanBeChanged() const;
    
    virtual TInt GetUiVolumeValue();

private: // from base class CEikAppUi

    /**
     * Takes care of command handling.
     *
     * @param aCommand The command to be handled.
     */
    void HandleCommandL( TInt aCommand );

    /**
     * Handles a change to the application's resources which are shared
     * across the environment.
     *
     * @param aType The type of resources that have changed.
     */
    void HandleResourceChangeL( TInt aType );
    
    TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
    
    // Help context from CCoeAppUi
    CArrayFix<TCoeHelpContext>* HelpContextL() const;
    
private:
    
    //from MCoeControlObserver
    void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

private:
    
    void DoHandleForegroundEventL( TInt aEventType );
    
    void FindWindowGroupIdentifiersL();

    
private: // data

    /**  Owns. Handle to status pane handler */
    CMusUiStatusPaneHandler* iStatusPaneHandler;
	
	CMusUiActiveQueryDialog* iConfirmationQuery;
    
    CRemConInterfaceSelector* iInterfaceSelector;

    CRemConCoreApiTarget* iCoreTarget;

	TBool iExit;
	
	TInt iLastKeyCode;
	
	/**
    * ETrue if operator specific functionality is needed 
    */
    TBool iOperatorSpecificFunctionality;
    
    // Window group id of this application.
    TInt iThisApplicationWgId;
    
    // Window group id of eikon server.
    TInt iEikonServerWgId;
    
    // Window group id for AknCapServer
    TInt iAknCapServerWgId;
    
    // Window group id for Akn Notifier Server
    TInt iAknNfyServerWgId;
    
    TBool iForeground;

	
    };

#endif // MUSUIAPPUI_H

// end of file
