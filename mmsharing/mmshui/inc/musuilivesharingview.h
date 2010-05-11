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



#ifndef MUSUILIVESHARINGVIEW_H
#define MUSUILIVESHARINGVIEW_H


#include "musuisendview.h"     // Base view class
#include "musuilivesharingobserver.h"

#include <aknview.h>

class CMusUiLiveSharingViewContainer;
class CMusUiLiveSharingController;
class CMusUiActivityManager;
class CMusUiBackgroundViewContainer;
class CAknToolbar;


/**
 *  The MUS application's live sharing view.
 *
 *  @lib musui.exe
 */
class CMusUiLiveSharingView : public CMusUiSendView,
                              public MMusUiLiveSharingObserver
    {

public: // constructors and destructor

    /**
    * EPOC 2nd phase constructor.
    */
    void ConstructL();

    /**
     * Destructor.
     */
    virtual ~CMusUiLiveSharingView();


public: // from base class CAknView

    /**
    * From CAknView returns Uid of View
    * @return TUid uid of the view
    */
    TUid Id() const;

    // From MEikMenuObserver, Called by framework before constructing menupane
    void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );


public: // from CMusUiGeneralView

    void HandleToolbarCommandL( TInt aCommand );
    
    virtual void SetInitialFocusedItemL();

    void RefreshAudioRoutingButton();
public: // from base class MAknToolbarObserver

    void DynInitToolbarL( TInt aResourceId, CAknToolbar* aToolbar );


public: // from MMusUiLiveSharingObserver

    void SetZoomValues( TInt aMinZoom, TInt aMaxZoom );

    void SetBrightnessValues( TInt aMinBrightness, TInt aMaxBrightness );

    void SetZoomL( TInt aZoomLevel );

    void SetBrightnessL( TInt aBrightnessLevel );

    void SetZoomVisible( TBool aVisible );

    void SetBrightnessVisible( TBool aVisible );

    void ReplaceToolbarCommand( TInt aOldCommand, 
                                TInt aNewCommand,
                                TBool aSetNewCommandFocused );


public: // new functions

    void RefreshView( TBool aLayoutChange );

    void DoRefreshView();

    void LevelIndicatorDismissed();
    
	void HandleKeyUpArrowL();
    
	void HandleKeyDownArrowL();

    /* In order to receive right oriented frames from camera when
     * orientation changed, it has to be recreated.Only way to do
     * from UI is to pause and play.
     *
     * aNewRect : New rectangle co-ordinates.
     *            This will be compared to old rectangle co-ordinates
     *            to find out whether orientation changed.
     */
    void RefreshCameraOrientationL(TRect& aNewRect);

    /* -----------------------------------------------------------------------------
     * When Live sharing paused , camera source will be unavilable, hence 
     * it is inappropriate to show the zoom and brightness
     * so hide those item. And unhide it again when unpaused.
     * 
     * @aIsPaused : ETrue if paused else EFalse
     */    
    void HideToolbarCommandForPause( TBool aIsPaused );
    
    void ResetToolBarSelected();

protected: // from CMusUiSendView

    virtual CMusUiSendController* SendController() const;

    virtual CMusUiSendViewContainer* SendContainer() const;


private:

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


private: // data

    /**  Container for all controls in this view.  Owns. */
    CMusUiLiveSharingViewContainer* iContainer;

    CMusUiLiveSharingController* iController;

    TBool iPauseSelected;
    
    };


#endif // MUSUILIVESHARINGVIEW_H

// end of file
