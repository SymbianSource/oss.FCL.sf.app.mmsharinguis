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



#ifndef MUSUILIVESHARINGVIEWCONTAINER_H
#define MUSUILIVESHARINGVIEWCONTAINER_H

#include "musuisendviewcontainer.h"
#include "musuilevelindicatorobserver.h"


/**
 *
 *
 *
 * @lib musui.exe
 */
class CMusUiLiveSharingViewContainer :	public CMusUiSendViewContainer
    {

public:  // constructors and destructor

    /**
     * Symbian second-phase constructor.
     *
     * @param aView         Parent view of container.
     * @param aRect         Frame rectangle for container.
     * @param aInputParams  Packaged data.
     */
    void ConstructL( CMusUiGeneralView* aView, 
                     const TRect& aRect );

    /**
     * Destructor.
     */
    ~CMusUiLiveSharingViewContainer();


public:	// new functions

    void SetZoomValues( TInt aMinZoom, TInt aMaxZoom );
    
    void SetBrightnessValues( TInt aMinBrightness, TInt aMaxBrightness );
    
    void SetZoomL( TInt aZoomLevel );
    
    void SetBrightnessL( TInt aBrightnessLevel );
    
    void SetZoomVisible( TBool aVisible );

    void SetBrightnessVisible( TBool aVisible );


private: // from base class CCoeControl
    
    /**
     * Returns the number of control contained by this class.
     *
     * @return           Number of controls contained
     */
    TInt CountComponentControls() const;

    /**
     * Returns handle to control pointed by aIndex.
     *
     * @param aIndex     Wanted control's index [0..n]
     * @return           Handle to wanted control
     */
    CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
     * Handles key events.
     *
     * @param aEvent  The event that occured.
     * @param aType   Type of key event: EEventKey, EEventKeyUp, or
     *                EEventKeyDown.
     * @return  The container's response to event: either
     *          EKeyWasNotConsumed or EKeyWasConsumed.
     */
    TKeyResponse OfferKeyEventL( const TKeyEvent& aEvent, TEventCode aType );

    /**
     * Handles pointer events. 
     *
     * @param aEvent  The event that occured.
     */
    virtual void HandlePointerEventL( const TPointerEvent& aPointerEvent );
    
protected: // data

    /**
    * ETrue if operator specific functionality is needed 
    */
    TBool iOperatorSpecificFunctionality;
    
private: // data
	
    };


#endif // MUSUILIVESHARINGVIEWCONTAINER_H

// end of file
