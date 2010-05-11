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



#ifndef MUSUIRECEIVEVIEW_H
#define MUSUIRECEIVEVIEW_H


#include "musuigeneralview.h"     // Base view class
#include "musuinavimediadecorator.h"
#include "musuireceiveobserver.h"
#include "musuidefinitions.h"

#include <aknview.h>


class CMusUiReceiveContainer;
class CMusUiReceiveController;
class CMusUiActivityManager;
class CMusUiBackgroundViewContainer;


/**
 *  The MUS application's live sharing view.
 *
 */
class CMusUiReceiveView : public CMusUiGeneralView,
                          public MMusUiReceiveObserver
    {
    
public: // constructors and destructor

    /**
    * EPOC 2nd phase constructor.
    */
	void ConstructL();

    /**
     * Destructor.
     */
    virtual ~CMusUiReceiveView();


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
    
    virtual void SetInitialFocusedItemL();
    
    virtual void SetToolbarVisibility( TBool aVisible );
    
    virtual void DoHandleForegroundEventL( TBool aForeground );    

public: // from base class MAknToolbarObserver

    virtual void DynInitToolbarL( TInt aResourceId, CAknToolbar* aToolbar );
    

public: // from MMusUiReceiveObserver

    virtual void ShowInvitationQueryL( const TDesC& aQuery );

    virtual void DismissInvitationQuery( TBool aDelete );
    
    virtual void SetFullScreenL( TBool aFullScreen );


public: // new functions

    void RefreshView( TBool aLayoutChange );
    

protected: // from MusUiGeneralView

    virtual CMusUiEventController* EventController() const;
    
    virtual CMusUiViewContainer& SharingContainer() const;
     

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
    CMusUiReceiveContainer* iContainer;
    
    CMusUiReceiveController* iController;
    
    HBufC* iAddress;
    
    TRect iOriginalRect;

    };

#endif // MUSUIRECEIVEVIEW_H

// end of file
