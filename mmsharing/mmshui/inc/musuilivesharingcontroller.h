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


#ifndef MUSUILIVESHARINGCONTROLLER_H
#define MUSUILIVESHARINGCONTROLLER_H

#include "musuisendcontroller.h"
#include "musenglivesessionobserver.h"
#include "musunittesting.h"

#include <e32base.h>
#include <badesca.h>

class MMusUiLiveSharingObserver;
class CMusEngLiveSession;

/**
 *
 *
 * @lib musui.exe
 */
class CMusUiLiveSharingController : public CMusUiSendController,
                                    public MMusEngLiveSessionObserver
    {

    MUS_UNITTEST( UT_CMusUiLiveSharingController; )
    MUS_UNITTEST( UT_CMusUiEventController; )
    
public:

	static CMusUiLiveSharingController* NewL(
                        	MMusUiEventObserver& aEventObserver,
	                        MMusUiSharingObserver& aSharingObserver,
	                        MMusUiSendObserver& aSendObserver,
	                        MMusUiLiveSharingObserver& aLiveObserver,
	                        const TRect& aRect );

	virtual ~CMusUiLiveSharingController();


private:

	CMusUiLiveSharingController(
                        	MMusUiEventObserver& aEventObserver, 
                        	MMusUiSharingObserver& aSharingObserver,
	                        MMusUiSendObserver& aSendObserver,
	                        MMusUiLiveSharingObserver& aLiveObserver );
	
	void ConstructL( const TRect& aRect );
	
	
public:	// From CMusUiEventController:
    
    virtual void PlayL();
    
    virtual void PauseL();

    virtual void OfferToolbarEventL( TInt aCommand );

    virtual void HandleCommandL( TInt aCommand );

    virtual void DeleteEngineSession();
    

public: // from CMusUiSendController

    virtual CMusEngMceOutSession* EngineOutSession();
    
    virtual void InviteL( const TDesC& aRecipient );
    
    virtual void HandleSliderValueChangeL( TInt aNewLevel );
    
    
public:	// new functions
    
    TMusUiNaviMediaDecorator RecordIcon();
    
    TBool IsBrightnessSupported();

    TBool IsDiskFull();
        
    /**
     * Determines whether zoom adjustment is selected either from toolbar 
     * or menu 
     */
    TBool ZoomSelected();

    /**
     * Determines whether brightness adjustment is selected either from toolbar 
     * or menu 
     */
    TBool BrightnessSelected();

    void LevelIndicatorDismissed();
    TBool ToolbarZoomSelected();

    TBool ToolbarBrightnessSelected();
    
    void ResetToolbarSelected();


private:	// new functions:
    
	void ZoomInL();
	
	void ZoomOutL();
	
    void IncreaseBrightnessL();
    
    void DecreaseBrightnessL();

    void ZoomL();
    
    void BrightnessL();
    
    void HandleMiddleSoftkeyOkL();
public: // From MMusEngLiveSessionObserver

	void DiskFull();

	    
public:	// from MMusEngSessionObserver

    void SessionEstablished();

	void SessionTerminated();

    void SessionConnectionLost();
    
    void SessionFailed();
    
	void StreamIdle();
	
	void StreamStreaming();	

	void SessionTimeChanged( const TTimeIntervalSeconds& aSeconds );
	
    void InactivityTimeout();


private:

	MMusUiLiveSharingObserver& iLiveObserver;
	
	CMusEngLiveSession* iSession;
    
    TBool iToolbarZoomSelected;
    
    TBool iToolbarBrightnessSelected;

    TBool iZoomSelected;
    
    TBool iBrightnessSelected;
    
    TBool iDiskFull;
	
    };

#endif // MUSUILIVESHARINGCONTROLLER_H

// end of file
