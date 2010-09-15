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



#ifndef MUSUIBACKGROUNDVIEWCONTAINER_H
#define MUSUIBACKGROUNDVIEWCONTAINER_H

#include <coecntrl.h>                // Control base class

class MAknsControlContext;
class CMusUiEventController;

/**
 * This class handles ordinal position two plain window.
 *
 *
 * @lib musui.exe
 */
class CMusUiBackgroundViewContainer : public CCoeControl,
                                      public MCoeControlObserver
    {
public:  // Constructors and destructor

    static CMusUiBackgroundViewContainer* NewL( 
        const TRect& aRect, 
        TInt aOrdinalPosition );
    
    ~CMusUiBackgroundViewContainer();


public:

    /**
     * Refreshes the view.  Called after the mode has been changed
     * from portrait to landscape or vice versa.
     */
    void RefreshView();

    /**
     * Sets the windows group ordinal position which is supplied
     * at construction phase
     */ 
    void SetOrdinalPosition();
    
    /**
     * Sets the windows group ordinal position for this container
     * @aOrdianlPosition : Ordinal Position
     */    
    void SetOrdinalPosition( TInt aOrdianlPosition);
    
    /**
     * Increase/decrease the windows group ordinal position which is supplied
     * at construction phase based on aUp.
     * @aUp : ETrue to increment else EFalse.
     */ 
    void UpdateOrdinalPositionValue( TBool aUp );
    
     /**
     * Sets the event controller
     */ 
    void SetUiEventController( CMusUiEventController* aController );
    
private: // Construction
    
    CMusUiBackgroundViewContainer( TInt aOrdinalPosition );
    
    void ConstructL( const TRect& aRect );
    
private: // from base class CCoeControl

   /**
    * From CoeControl,SizeChanged.
    */
    void SizeChanged();
    
    /**
     * Handles a change to the application's resources which are shared
     * across the environment.
     *
     * @param aType The type of resources that have changed.
     */
    void HandleResourceChange( TInt aType );

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
     * Gets called by the application framework when drawing is
     * needed.
     *
     * @param aRect The rectangle that needs to be drawn.
     */
    void Draw( const TRect& aRect ) const;

    /**
    * From MCoeControlObserver
    * Acts upon changes in the hosted control's state.
    *
    * @param    aControl    The control changing its state
    * @param    aEventType  The type of control event
    */
    void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);

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
     *
     */
    TTypeUid::Ptr MopSupplyObject( TTypeUid aId );


private: // data

    /**  Background skin control context */
    MAknsControlContext* iBackGround;

    /**
    * Created window group identifier
    */
    TInt iGroupIdentifier;

    /**
    * Ordinal Postion for windows group
    */
    TInt iOrdinalPosition;

    /**
    * Event controller
    */    
    CMusUiEventController* iController;

    };


#endif // MUSUILIVESHARINGVIEWCONTAINER_H

// end of file
