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



#ifndef MUSUINAVIMEDIADECORATOR_H
#define MUSUINAVIMEDIADECORATOR_H

#include "musuidefinitions.h"
#include "musuinavimediaanimatorobserver.h"

#include <coecntrl.h>
#include <AknUtils.h>

class CAknIconArray;
class CMusUiNaviMediaAnimator;
class MAknsSkinInstance;

/**
 * CMusUiNaviMediaDecorator
 *
 * @lib musui.exe
 */
class CMusUiNaviMediaDecorator : public CCoeControl, 
                                 public MMusUiNaviMediaAnimatorObserver
    {

	public:  // constructors and destructor

	    /**
	     * Two-phased constructor.
	     *
	     */
	    static CMusUiNaviMediaDecorator* NewL();

	    /**
	     * 
	     *
	     */
	    virtual ~CMusUiNaviMediaDecorator();


	private: // constructors

	    /**
	     * C++ default constructor.
	     *
	     */
	    CMusUiNaviMediaDecorator();

	    /**
	     * By default Symbian second-phase constructor is private.
	     *
	     */
	    void ConstructL();


	public: // API

	    /**
	     * Updates left label (usually time string) in navi pane.
		 *
	     * @param aLabel New label
	     */
	    void SetLeftLabelL( const TDesC& aLabel );

	    /**
	     * Updates right label (usually time string) in navi pane.
		 *
	     * @param aLabel New label
	     */
	    void SetRightLabelL( const TDesC& aLabel );
	    
		/**
		* Selects icon show
		*
		*/
		void ShowIcon( TMusUiNaviMediaDecorator aIcon );


	protected: // from MMusUiNaviMediaAnimatorObserver
	
        void SetIcon( CGulIcon* aIcon );


	protected: // from CCoeControl

	    /**
	     * Handles the size change events.
	     *
	     */
	    void SizeChanged();

	    /**
	     * Draws the control.
	     *
	     * @param aRect The region of the control to be redrawn.
	     */
	    void Draw( const TRect& aRect ) const;


	private: // HELPERS 
	
	    /**
	     * Layouts the text depending on current situation.
	     *
	     */
		void DoLayoutText();


        void LoadImagesL();

        void  ConstructImageL( MAknsSkinInstance* aSkin,
                                                const TAknsItemID& aId,
                                                TInt aBitmapId,
                                                TInt aMaskId,
                                                TParse parse,
                                                TBool aIconHasOwnColor = ETrue);


	private: // DATA

	    /**
	    *
	    *
	    */
	    HBufC* iLeftLabel;

	    /**
	    *
	    *
	    */
	    HBufC* iRightLabel;


        CAknLayoutFont *iFont;

	    /**
	    *
	    *
	    */
	    TAknLayoutText iLeftTextLayout;

	    /**
	    *
	    *
	    */
	    TAknLayoutText iRightTextLayout;

	    /**
	    *
	    *
	    */
	    TAknLayoutRect iIconLayout;    
	    
	    /**
	    *
	    *
	    */
	    CAknIconArray* iIconArray;
	    
	    /**
	    *
	    *
	    */
	    TInt iMiddle;
	    
	    TInt iIconLeft;
	    
	    /**
	    *
	    *
	    */
	    TSize iLeftTextSize;

	    /**
	    *
	    *
	    */
	    TSize iRightTextSize;
	    
	    /**
	    *
	    *
	    */
	    CGulIcon* iIcon;
	    
	    /**
	    * resource file location ( eq. Z or C )
	    */
        TBool iResourceRam;
	    
    
        CMusUiNaviMediaAnimator* iAnimation;
        
        TSize iImageSize;
    
    };

#endif // MUSUINAVIMEDIADECORATOR_H

// end of file
