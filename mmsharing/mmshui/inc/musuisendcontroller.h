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
* Description:  
*
*/


#ifndef MUSUISENDCONTROLLER_H
#define MUSUISENDCONTROLLER_H

#include "musuieventcontroller.h"
#include "musengoutsessionobserver.h"
#include "musunittesting.h"

#include "musengoutsessionobserver.h"
#include <AknProgressDialog.h>

class MMusUiSendObserver;
class CMusEngMceOutSession;
class CDirectoryLocalizer;

/**
 *
 *
 * @lib musui.exe
 */
class CMusUiSendController : public CMusUiEventController,
                             public MMusEngOutSessionObserver
    {

public:

	virtual ~CMusUiSendController();
	
	
public:	// From CMusUiEventController:
    
    /**
    * Returns a pointer to engine side session.
    */
    virtual CMusEngMceSession* EngineSession();
    
    /**
    *
    */
    virtual const TDesC& TypedAddress() const;
    
    /**
    *
    */
    virtual TBool ManualAddressTyped() const;
    
    /**
    * Makes out session specific preparations for engine session deletion.
    */
    virtual void DeleteEngineSession();
       

public:	// From MMusUiPropertyObserver

    virtual void HandlePropertyError( const TInt aReason );


public: // From MMusUiCallbackObserver

    /**
    * Handles async events common to outgoing session types.
    * Should be called if child classes are not able to handle the call.
    */
    virtual void HandleAsyncEventL( TMusUiAsyncEvent aEventId );

    
public:	// new functions:

    /**
    * Returns a pointer to engine side session.
    */
    virtual CMusEngMceOutSession* EngineOutSession() = 0;

    virtual void InviteL( const TDesC& aRecipient ) = 0;
    
    virtual void HandleSliderValueChangeL( TInt aNewLevel ) = 0;

    void StartInvitationL();

    void InviteL();
    
	void CancelInvitationL();
    
    virtual void HandleForegroundEventL( TBool aForeground );
   
    virtual void PlayL();
    
    virtual void PauseL();
    
    TBool IsPlayingL();
    

public: // from MMusEngSessionObserver
	
    virtual void SessionEstablished();


public: // from MMusEngOutSessionObserver
	
    virtual	void SessionRejected();                     // e.g. 603 DECLINE
	
	virtual void SessionBadRequest();                   // 400 
	
	virtual void SessionUnauthorized();                 // 401 
	
	virtual void SessionPaymentRequired();              // 402
	
	virtual void SessionRecipientNotFound();            // 404
	
	virtual void SessionProxyAuthenticationRequired();  // 407
	
	virtual void SessionRequestTimeOut();               // 408
	
	virtual void SessionUnsupportedMediaType();         // 415
	
    virtual void SessionBusyHere();                     // 486

	virtual void SessionRequestCancelled();             // 487
	
	virtual void SessionTemporarilyNotAvailable();      // 480    
 
  
protected: // Contructors

    void ConstructL();
    
	CMusUiSendController( MMusUiEventObserver& aEventObserver,
	                      MMusUiSharingObserver& aSharingObserver, 
	                      MMusUiSendObserver& aSendObserver );


protected: // from MusUiEventController    
    
    /**
    * Send specific functionality of exit procedure
    */
    virtual void ExitProcedureL( TBool aUserAcceptance );

    /**
    * Handles status changes specific to sending side sharing types.
    * To be overridden in sibling classes for sharing type specific 
    * status changes. Calls base class version if status change is not
    * sending side specific.
    */
    virtual void HandleChangedStatusL( TMusAvailabilityStatus aStatus ); 
    

protected:

    /**
    * 
    */
    const TDesC& VideoFileNameL();
    
    void ShowInvitingWaitDialogL();

    void DismissWaitDialog();
    

private: // Helpers

    void SplitL( const TDesC& aDes, const TDesC& aChar, CDesCArray* aArray );	

    void SetVideoPathValueL();

    void ShowVideoSavedDialogL();

    void DeleteVideoL();
    
	void DeleteFileL();
    
protected:  // Data

    /*
    * Interface for outgoing session specific callbacks
    */
    MMusUiSendObserver& iSendObserver;
    
	HBufC* iAddress;
	
	TBool iVideoToBeSaved;
	
	TBool iManualAddressTyped;
	
    TInt iTriedInvitations;
	
    /**
    * 
    */
    TBool iSipRegistrationPending;
	
	/**
    * 
    */
	HBufC* iRemoteSipAddress;

    /**
    * 
    */
	HBufC* iRemoteSipAddressProposal;

	
private: // Data

    /**
    * If this variable is ETrue, playback should be continued
    * when gaining focus. ETrue by default.
    */
    TBool iContinuePlayingWhenForeground;
    
   /**
    * 
    */
	HBufC* iVideoFileName;
	
    CDirectoryLocalizer* iLocalizer;
    
protected:   
    /**
    * The class is used as a call back service of the wait dialog  
    */
    class TWaitDialogCallback : public MProgressDialogCallback
    	{
    public:
    	TWaitDialogCallback( CMusUiSendController& aController );	
    	void DialogDismissedL( TInt /*aButtonId*/ );
    	        	    
    private:
    	CMusUiSendController& iController;
    	
    	}iWaitDialogCallback;

	/**
    * If this variable is ETrue, playback should be continued
    * when gaining focus. ETrue by default.
    */
    };

#endif // MUSUISENDCONTROLLER_H

// end of file
