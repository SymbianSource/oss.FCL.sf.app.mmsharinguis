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



#ifndef MUSUICLIPSHARINGVIEW_H
#define MUSUICLIPSHARINGVIEW_H


#include "musuisendview.h"     // Base view class
#include "musuiclipsharingobserver.h"

#include <aknview.h>

class CMusUiClipSharingViewContainer;
class CMusUiClipSharingController;
class CMusUiActivityManager;
class CMusUiBackgroundViewContainer;
class CAknToolbar;


/**
 *  The MUS application's clip sharing view.
 *
 *  @lib musui.exe
 */
class CMusUiClipSharingView : public CMusUiSendView,
                              public MMusUiClipSharingObserver
    {

public: // constructors and destructor

    /**
    * EPOC 2nd phase constructor.
    */
    void ConstructL();

    /**
     * Destructor.
     */
    virtual ~CMusUiClipSharingView();


public: // from base class CAknView

    /**
    * From CAknView returns Uid of View
    * @return TUid uid of the view
    */
    virtual TUid Id() const;

    // From MEikMenuObserver, Called by framework before constructing menupane
    virtual void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );


public: // from CMusUiGeneralView

    virtual void HandleToolbarCommandL( TInt aCommand );

    virtual void HandleKeyUpEvent( TInt aKeyCode );

    virtual void SetInitialFocusedItemL();

public: // from base class MAknToolbarObserver

    virtual void DynInitToolbarL( TInt aResourceId, CAknToolbar* aToolbar );


public: // from MMusUiClipSharingObserver

    virtual void RunFetchDialogL();

    virtual void ShowTranscodingProgressDialogL();

    virtual void UpdateClipTranscodingPercentageL( TInt aPercetage );

    virtual void CancelTranscodingDialogL();

    virtual void UpdateClipPositionL( TInt aPositionInSeconds,
                                      const TDesC& aFormattedPosition );

    virtual void SetDurationValue( TInt aDurationValue );

    virtual void SetPositionValueL( TInt aPositionValue );

    virtual void SetDurationVisible();

    virtual void ReplaceToolbarCommand( TInt aOldCommand, 
                                        TInt aNewCommand,
                                        TBool aSetNewCommandFocused );
    
    virtual void InvalidVideoFrame( TBool aInvalid = EFalse );
    
public: // new functions

    void RefreshView( TBool aLayoutChange = EFalse );
    
    void CancelTranscodingL();


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
    CMusUiClipSharingViewContainer* iContainer;

    CMusUiClipSharingController* iController;

    };

#endif // MUSUICLIPSHARINGVIEW_H

// end of file
