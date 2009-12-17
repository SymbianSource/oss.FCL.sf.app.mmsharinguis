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



#ifndef MUSUISTATUSPANEHANDLER_H
#define MUSUISTATUSPANEHANDLER_H


#include "musuinavimediadecorator.h"

#include <akntabgrp.h>              // Tab group
#include <akntitle.h>               // Title pane
#include <akncontext.h>             // Context pane
#include <AknIndicatorContainer.h>  // Indicator container
#include <aknEditStateIndicator.h>
#include <aknnavide.h>              // CAknNavigationDecorator


class CAknNavigationControlContainer;
class CAknVolumePopup;

/**
 * Handles status, title and contextpane operations.
 *
 * @lib musui.exe
 */
class CMusUiStatusPaneHandler : public CBase
    {

public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CMusUiStatusPaneHandler* NewL( CEikonEnv& aEikonEnv );

    /**
    * Destructor.
    */
    virtual ~CMusUiStatusPaneHandler();

private:

    /**
    * C++ default constructor.
    */
    CMusUiStatusPaneHandler( CEikonEnv& aEikonEnv );

    /**
    * Symbian 2nd-phase constructor.
    */
    void ConstructL();


public: // New functions

    /**
    * Sets statuspane icons.
    *
    * @param aIconId ID of icon from .mbg file.
    * @param aIconMaskId ID of icon mask from .mbg file.
    */
    void SetStatusPaneIconsL( TInt32 aIconId, TInt32 aIconMaskId );

    /**
    * Sets statuspane icons.
    *
    */
    void SetStatusPaneIconsL( CFbsBitmap* aBitmap );

    /**
    * Hides navi-pane decorators and clears text
    */
    void ClearNaviPaneL();

    /**
    * Sets title-pane text
    *
    * @param aTitle Title text
    */
    void SetTitleL( const TDesC& aTitle );

    /**
    * Sets title-pane text
    *
    */
    void SetTitleFromResourceL( TInt aResourceId );

    /**
     * Get the current volume setting.
     *
     * @return Current volume.
     */
    TInt GetVolumeControlValue() const;

    /**
     * Activates volume control.
     *
     * @return Volume controller.
     */
    void ActivateVolumeControlL( TInt aVolume );

    /**
     * Activate media navi pane.
     */
    void ActivateNaviPaneMediaL();
    
    /**
     *
     */
    void ShowNaviPaneIconL( TMusUiNaviMediaDecorator aIcon );

    /**
     * Set text in media navi pane.
     *
     * @param aLabel New label of media navi pane control
     */
    void SetLeftLabelL( const TDesC& aLabel );

    /**
     * Set text in media navi pane.
     *
     * @param aLabel New label of media navi pane control
     */
    void SetRightLabelL( const TDesC& aLabel );

    /**
     * Create media navi pane.
     */
    CAknNavigationDecorator* CreateMediaNaviL();
    
    /**
     * Get Volume popup control pointer
     * @return pointer of VolumePopup control
     */
    CAknVolumePopup* GetVolumePopup() const;


private: // data

    CEikonEnv& iEikonEnvironment;

    // UI controls
    CAknNavigationControlContainer*     iNaviPane;
    CAknTitlePane*                      iTitlePane;
    CAknContextPane*                    iContextPane;
    CAknNavigationDecorator*            iNaviPaneText;
    CAknVolumePopup*                    iVolumePopup;
    // Navi pane media control
    CAknNavigationDecorator*            iMediaControl;

    // Volume level of the volume control
    TInt                                iVolume;

    };

#endif // MUSUISTATUSPANEHANDLER_H

// end of file
