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



#ifndef MUSUIRECEIVECONTAINER_H
#define MUSUIRECEIVECONTAINER_H

#include "musuiviewcontainer.h"

class MAknsControlContext;
class CMusUiGeneralView;
class CMusUiInvitationQuery;
class MMusUiInvitationQueryObserver;
class CMusUiReceiveView;

/**
 *
 *
 */
class CMusUiReceiveContainer :	public CMusUiViewContainer
    {

public:  // constructors and destructor

    /**
     * Public constructor.
     */
    static CMusUiReceiveContainer* NewL( CMusUiReceiveView& aView,
                                  TRect aRect );

    /**
     * Destructor.
     */
    ~CMusUiReceiveContainer();


private: // hidden constructors

    /**
     * C++ Constructor
     */
    CMusUiReceiveContainer( CMusUiReceiveView& aView );

    /**
     * Symbian second-phase constructor.
     *
     * @param aView         Parent view of container.
     * @param aRect         Frame rectangle for container.
     * @param aInputParams  Packaged data.
     */
    void ConstructL( const TRect& aRect );

protected: // data

    /**
    * ETrue if operator specific functionality is needed 
    */
    TBool iOperatorSpecificFunctionality;    

public:	// new functions

    /**
     * Shows an invitation query.
     */
    void ShowInvitationQueryL( const TDesC& aContact,
                               MMusUiInvitationQueryObserver& aObserver );

    /**
     * 
     */
    void DismissInvitationQuery( TBool aDelete );
    
    /**
     * Return pointer to view class. User needs to type cast to
     * appropriate concrete view class.
     *
     * @return handle to view
     */
    CMusUiGeneralView* MusUiView() const;
 

    void SetFullScreen( TBool aFullScreen, TRect aRect );


private: // from base class CCoeControl

   /**
    * 
    */
    virtual void SizeChanged();
    
    /**
     * Returns the number of control contained by this class.
     *
     * @return           Number of controls contained
     */
    virtual TInt CountComponentControls() const;

    /**
     * Returns handle to control pointed by aIndex.
     *
     * @param aIndex     Wanted control's index [0..n]
     * @return           Handle to wanted control
     */
    virtual CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
     * Gets called by the application framework when drawing is
     * needed.
     *
     * @param aRect The rectangle that needs to be drawn.
     */
    virtual void Draw( const TRect& aRect ) const;

    /**
     * Handles key events.
     *
     * @param aEvent  The event that occured.
     * @param aType   Type of key event: EEventKey, EEventKeyUp, or
     *                EEventKeyDown.
     * @return  The container's response to event: either
     *          EKeyWasNotConsumed or EKeyWasConsumed.
     */
    virtual TKeyResponse OfferKeyEventL( const TKeyEvent& aEvent, 
                                         TEventCode aType );

    /**
     * Handles pointer events. Will return from full screen mode as a response
     * to whatever pointer event.
     *
     * @param aEvent  The event that occured.
     */
    virtual void HandlePointerEventL( const TPointerEvent& aPointerEvent );


private: // data

    /**  A dialog for invitation query (accept/reject). Own.*/
    CMusUiInvitationQuery* iInvitationQuery;

    /**  Background skin control context. */
    MAknsControlContext* iBackGround;
    
    /**  Handle to the view that owns of this container.  Not own. */
    CMusUiGeneralView& iView;
    
    TBool iFullScreen;
    
    };


#endif // MUSUIRECEIVECONTAINER_H

// end of file
