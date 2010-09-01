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



#ifndef MUSUISENDVIEWCONTAINER_H
#define MUSUISENDVIEWCONTAINER_H


#include "musuiviewcontainer.h"
#include "musuilevelindicatorobserver.h"

#include <avkon.hrh>

class MAknsControlContext;
class CMusUiGeneralView;
class CAknWaitDialog;
class CMusUiLevelIndicator;
class CMusUiSendController;

/**
 *
 *
 */
class CMusUiSendViewContainer :	public CMusUiViewContainer,
                                public MMusUiLevelIndicatorObserver
    {

public:  // constructors and destructor

    /**
     * Destructor.
     */
    ~CMusUiSendViewContainer();

public: // new functions

    /**
     * Return pointer to view class. User needs to type cast to
     * appropriate concrete view class.
     *
     * @return handle to view
     */
    CMusUiGeneralView* MusUiView() const;
    
    /**
     * @param Pointer to instance of controller who is notified about changes in 
     *        indicator. NULL if notifications are to be suppressed.
     */
    void SetController( CMusUiSendController* aController );
    

protected: // constructors
    
    /**
     * Meant to be a base class, no instantiation
     */
    CMusUiSendViewContainer();

    /**
     * Symbian second-phase constructor.
     *
     * @param aView         Parent view of container.
     * @param aRect         Frame rectangle for container.
     * @param aInputParams  Packaged data.
     * @param aContainerVisible
     * @param aIndicatorVisible
     */
    void ConstructL( CMusUiGeneralView* aView, 
                     const TRect& aRect,
                     TAknOrientation aIndicatorLayout,
                     TBool aContainerVisible = EFalse,
                     TBool aIndicatorVisible = EFalse );


private: // from base class CCoeControl

    /**
     * Gets called by the application framework when drawing is
     * needed.
     *
     * @param aRect The rectangle that needs to be drawn.
     */
    virtual void Draw( const TRect& aRect ) const;

    virtual void SizeChanged();

    virtual TTypeUid::Ptr MopSupplyObject( TTypeUid aId );


private: // MMusUiLevelIndicatorObserver
	
	virtual void SetLevelIndicatorVisibility( TBool aVisible );
    
    virtual void IndicatorLevelChanged( TInt aNewLevel );
    	

protected: // data

    CMusUiLevelIndicator* iIndicator;


private: // data

    /**  Background skin control context. */
    MAknsControlContext* iBackGround;
    
    /**  Handle to the view that owns of this container.  Not own. */
    CMusUiGeneralView* iView;
    
    /**
    * Engine DSA area 
    */
    TRect iVideoRect;
    
    // Can be NULL
    CMusUiSendController* iSendController;
    
    };


#endif // MUSUISENDVIEWCONTAINER_H

// end of file
