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
* Description:  
*
*/


#ifndef MUSUIEVENTCONTROLLER_H
#define MUSUIEVENTCONTROLLER_H

#include "musuipropertyobserver.h"
#include "musuicallbackobserver.h"
#include "musuibitmapdecoderobserver.h"
#include "musresourceproperties.h"
#include "musuidefinitions.h"
#include "musmanagercommon.h"
#include "musuimmcobserver.h"
#include "musunittesting.h"

#include "musengsessionobserver.h"
#include "musengaudioroutingobserver.h"
#include "musengvolumechangeobserver.h"

#include <aknappui.h>        // CAknAppUiBase::TAppUiOrientation

class CMusUiCallbackService;
class MMusUiEventObserver;
class CMusUiMmcMonitor;
class CMusUiPropertyWatch;
class CMusUiBitmapDecoder;
class CFbsBitmap;
class CMusEngMceSession;
class MMusUiSharingObserver;
class CMusUiActivityManager;
class CMusUiResourceHandler;

using namespace MultimediaSharing;


/**
 *
 *
 * @lib musui.exe
 */
class CMusUiEventController : public CBase, 
                              public MMusUiPropertyObserver, 
                              public MMusUiCallbackObserver,
                              public MMusUiMmcObserver,
                              public MMusEngSessionObserver,
                              public MMusEngAudioRoutingObserver,
                              public MMusEngVolumeChangeObserver
    {

public: // enumerations

    enum TMusUiShutdownState
            {
            EMusUiShutdownNotDefined,
            EMusUiShutdownStarted,
            EMusUiShutdownVideoSaved,
            EMusUiShutdownContactSavingQueried,
            EMusUiShutdownOver
            };    


public:

    /**
    * 
    */
	virtual ~CMusUiEventController();

	
protected:

    /**
    * 
    */
	void ConstructL();
	
    /**
    * 
    */
	CMusUiEventController( MMusUiEventObserver& aEventObserver,
	                       MMusUiSharingObserver& aSharingObserver );


public:	// From MMusUiPropertyObserver

    /**
    * Sets the property key value common to all sharing types.
    * To be overridden for specific key values.
    * CMusUiPropertyWatch calls MMusUiPropertyObserver::PropertyChanged,
    * when P&S key values change
    *
    * @param TUint aKey - The Property key
    * @param TInt aValue - The value of the Property key
    */
    virtual	void PropertyChanged( const TUint aKey, const TInt aValue );
	
    /**
    * 
    */
	virtual void HandlePropertyError( const TInt aReason );


public:	// From MMusUiCallbackObserver

    /**
    * Handles async events common to all session types.
    * Should be called if child classes are not able to handle the call.
    */
    virtual void HandleAsyncEventL( TMusUiAsyncEvent aEventId );

    /**
    * Error handler for HandleAsyncEventL.
    */
    virtual TInt HandleAsyncError( TInt aError );


public:	// From MMusUiMmcObserver

    /**
    * 
    */
    virtual void MmcRemoved();


public: // From 

    /**
    * Handles session establishment procedures common to all session types.
    */
    virtual void SessionEstablished();
    

public: // From MMusEngAudioRoutingObserver
    
    /**
    * Indicates that some other application has changed audio routing.
    *
    */
    virtual void AudioRoutingChanged( TBool aShowNote );
    
    /**
    * Before doing audio routing, permission is asked
    *
    */
    virtual TBool AudioRouteChangeAllowed() const;
    
public: // From MMusEngVolumeChangeObserver
    
    /**
    * Indicates that some other application has changed volume level.
    *
    */
    virtual void VolumeChanged( TInt aVolume, TBool aAudioRouteChanged );
    

public:	// New functions
	
	/**
    * 
    */
	void SetRect( const TRect& aRect );

    /**
    * 
    */
    const TDesC& MusContactName();
    
    /**
    * 
    */
	const TDesC& MusVideoCodecValue();

    /**
    * 
    */
	const TDesC& MusTelNumberValue();
    
    /**
    * 
    */
    const TDesC& SessionTimeFormatted( const TTimeIntervalSeconds& aSeconds );
    
    /**
    * 
    */
    void SaveContactQueryL();
    
    /**
    * 
    */
    void HandleError( const TInt aError );

    /**
    * Starts shutdown process
    */
    void HandleExitL( TBool aTerminateCall = EFalse );

    /**
    * 
    */
    void AsyncQueryDialogAcceptedL( TBool aAccepted );
    
    /**
    * 
    */
    TBool ExitOccured();
    
    /**
    *
    */
    TBool AudioRoutingCanBeChanged();
    
    /**
    * 
    */
    TBool IsLoudSpeakerEnabled();
    
    /**
    * 
    */
    TBool DeviceHasDedicatedVolumeKeys();
    
    /**
    * Queries flag for session state
    */
    TBool ConnectionEstablished() const;
    
    /**
    * Set session initialization state
    */
    void SetConnectionInitialized( TBool aConnectionInitialized );
    
    /**
    * Queries flag for session state
    */
    TBool ConnectionInitialized() const;
    
    /**
    * 
    */
    void EnableDisplayL( TBool aEnable );
    
    /**
    * 
    */
    void ChangeOrientationL( CAknAppUiBase::TAppUiOrientation aOrientation );
    
    /**
    * 
    */
    TBool ToolbarVolumeSelected();
    
    /**
    *
    */ 
    TBool IsMicMutedL();
    
    /**
    * Function to be overridden in sibling classes
    * @return EFalse by default
    */
    virtual TBool ManualAddressTyped() const;
    
    /**
    * 
    */
    virtual void HandleForegroundEventL( TBool aForeground );
    
    /**
    * 
    */
    virtual void OfferToolbarEventL( TInt aCommand );
    
    /**
    * 
    */
    virtual void HandleCommandL( TInt aCommand );

    /**
    * Returns a pointer to engine side session.
    */
    virtual CMusEngMceSession* EngineSession() = 0;
    
    /**
    * 
    */
    virtual const TDesC& TypedAddress() const = 0;
    
    /**
    * 
    */
    virtual void DeleteEngineSession() = 0;
      
    /**
    * 
    */
    TBool IsDisplayEnabledL();
    

protected:    // New functions

    /**
    * General functionality of exit procedure
    */
    virtual void ExitProcedureL( TBool aUserAcceptance );
    
    /**
    * Handles status changes common to all sharing types.
    * To be overridden in sibling classes for sharing type specific 
    * status changes.
    */
    virtual void HandleChangedStatusL( TMusAvailabilityStatus aStatus );    
    
    
protected:
        
    /**
    * 
    */
	MMusUiEventObserver& iEventObserver;
	
    /**
    *
    */
    MMusUiSharingObserver& iSharingObserver;
    
    /**
    * 
    */
	CMusUiCallbackService* iCallbackService;

    /**
    * Flag of the connection state
    */
	TBool iConnectionEstablished;
	
	/**
    * Flag of the connection state
    */
	TBool iConnectionInitialized;

    /**
    * 
    */
	TFileName iVideoFilePath;
	
    /**
    * 
    */
	TFileName iLocalizedVideoFilePath;
	
    /**
    * 
    */
	TFileName iVideoFileNamePlain;
	
	/**
    * 
    */
	TInt iSipProfileId;
	
	/**
    * 
    */
	TInt iContactId;
	
	/**
    * 
    */
	HBufC* iContactName;

    /**
    * 
    */
	HBufC* iTelNumber;
	
    /**
    * 
    */
	HBufC* iDialogPrompt;
    
    /**
    * 
    */
	HBufC* iVideoCodec;
	
    /**
    * 
    */
    TBool iForeground;

    /**
    * ETrue if operator specific functionality is needed 
    */
    TBool iOperatorSpecificFunctionality;
    
    /**
    *
    */
    CMusUiResourceHandler* iResourceHandler;
    
	/**
    * 
    */
    TMusUiShutdownState iShutdownState;
    
    
private:
	
    /**
    * 
    */
	CMusUiMmcMonitor* iMmcMonitor;
	
	/**
    * 
    */
    CMusUiActivityManager* iActivityManager;

    /**
    * 
    */
    CMusUiPropertyWatch* iStatusPropertyWatch; 

    /**
    * 
    */
    TBuf<7> iTimeBuffer;
    
    /**
     * Flag of the Volume item in the Toolbar
     */
    TBool iToolbarVolumeSelected;
    
    /**
    * Flag indicating whether call has ended
    */
    TBool iActiveCall;
    
    MUS_UNITTEST( UT_CMusUiEventController )
    MUS_UNITTEST( UT_CMusUiLiveSharingController )
    MUS_UNITTEST( UT_CMusUiClipSharingController )
    MUS_UNITTEST( UT_CMusUiReceiveController )
    };

#endif // MUSUIEVENTCONTROLLER_H

// end of file
