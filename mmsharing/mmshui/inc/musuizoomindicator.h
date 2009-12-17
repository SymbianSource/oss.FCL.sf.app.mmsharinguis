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
* Description:  Class for displaying icons in viewfinder view
*
*/


#ifndef CMUSUIZOOMINDICATORS_H
#define CMUSUIZOOMINDICATORS_H

// SYSTEM
#include <AknsControlContext.h>
#include <aknutils.h>
#include <coecntrl.h>

class MAknsSkinInstance;
class CGulIcon;
class MMusUiZoomIndicatorObserver;

/** 
* Class for displaying icons in viewfinder view
* @since 2.8
*/
class CMusUiZoomIndicator : public CCoeControl
    {


    public: // Constructors and destructor
        
        /**
        * Symbian OS two-phased constructor.
        * @since 2.8
        * @param aController reference to CCcorController
        */
        static CMusUiZoomIndicator* NewLC( 
        						TInt aZoomMin, 
        						TInt aZoomMax, 
        						MMusUiZoomIndicatorObserver* aObserver );

        /**
        * Symbian OS two-phased constructor.
        * @since 2.8
        * @param aController reference to CCcorController
        */
        static CMusUiZoomIndicator* NewL( 
        						TInt aZoomMin, 
        						TInt aZoomMax, 
        						MMusUiZoomIndicatorObserver* aObserver );

        /**
        * Destructor.
        */
        ~CMusUiZoomIndicator();

    public: // New functions

		/**
		*
		* @since 3.2
		*/
		void SetZoomLevel( TInt aZoomLevel );

		/**
		*
		* @since 3.2
		*/
		TInt GetZoomLevel();

		/**
		*
		* @since 3.2
		*/
		void SetZoomMin( TInt aZoomMin );
		
		/**
		*
		* @since 3.2
		*/
		void SetZoomMax( TInt aZoomMax );
		
		/**
		*
		* @since 3.2
		*/
		void SetFullRect( const TRect aRect  );
		
		

	private: // From CCoeConrol

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
		void Draw(const TRect& aRect) const;

        TInt CountComponentControls() const;
        
        CCoeControl* ComponentControl( TInt aIndex ) const;
        


    private: // Constructors and destructor

        /**
        * C++ constructor
        * @since 2.8
        * @param aController reference to CCcorController
        */
        CMusUiZoomIndicator( TInt aZoomMin, 
        					 TInt aZoomMax, 
        					 MMusUiZoomIndicatorObserver* aObserver );

        /**
        * Symbian 2nd phase constructor.
        * @since 2.8
        */
        void ConstructL();


    private: // HELPERS

        /**
        * 
        */
		void DrawIcons( const TRect& aRect );
       
        /**
        * 
        */
		void UpdateZoomLayout();

        void  ConstructImageL( CEikImage* aImage,
                               MAknsSkinInstance* aSkin,
                               const TAknsItemID& aId,
                               TInt aBitmapId,
                               TInt aMaskId );

        /**
        * Callback for disable zoom indicator.
        * @since 2.8
        * @return KErrNone
        */
        static TInt DisableZoomIndicator( TAny* aThis );

        /**
        * 
        */
		void DisableTimer();

        /**
        * 
        */
		void EnableTimer();


    private: //data

        /**
        * 
        */
        TRect iParentRect;

        /**
        * 
        */
        TRect iViewfinderRect;

        /**
        * 
        */
		TInt iZoomMin;

        /**
        * 
        */
		TInt iZoomMax;
		
        /**
        * 
        */
		TInt iZoomLevel;

        /**
        * 
        */
        CPeriodic* iDisableZoomTimer;   // owned

        /**
        * 
        */
        TRgb iIconColor;

        /**
        * 
        */
		CEikImage* iIconElevator;

        /**
        * 
        */
		CEikImage* iIconMin;

        /**
        * 
        */
		CEikImage* iIconMax;

        /**
        * 
        */
		CEikImage* iIconBg;

        /**
        * 
        */
		TInt iTopMargin;

        /**
        * 
        */
		TInt iBottomMargin;
		
        /**
        * 
        */
		TInt iScaleStep;

        /**
        * 
        */
		TInt iScaleY;
		
        /**
        * 
        */
		TInt iMiddle;
		
        /**
        * NOT OWN!!!
        */
		MMusUiZoomIndicatorObserver* iObserver; 

        TInt iElevatorWidth;
        
        TInt iElevatorHeight;
    
        TRect iFullRect;
    

    };


#endif

// End of File
