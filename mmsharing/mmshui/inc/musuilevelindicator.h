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


#ifndef MUSUILEVELINDICATOR_H
#define MUSUILEVELINDICATOR_H

#include "musuidefinitions.h"
#include "musuiactivetimerobserver.h"

// SYSTEM
#include <AknsControlContext.h>
#include <AknUtils.h>
#include <coecntrl.h>

class MAknsSkinInstance;
class CGulIcon;
class MMusUiLevelIndicatorObserver;
class CAknSlider;

/** 
* Class for displaying icons in viewfinder view
*/
class CMusUiLevelIndicator : public CCoeControl,
                             public MCoeControlObserver,
                             public MMusUiActiveTimerObserver
    {


    public: // Constructors and destructor
        
        /**
        * Symbian OS two-phased constructor.
        * @param aController reference to CCcorController
        */
        static CMusUiLevelIndicator* NewL(
                                TAknOrientation aLayout, 
        						TInt aMinLevel, 
        						TInt aMaxLevel, 
                                CCoeControl* aParent,
                                const TRect& aRect,
        						MMusUiLevelIndicatorObserver* aObserver );
        						
        /**
        * Symbian OS two-phased constructor.
        * @param aController reference to CCcorController
        */
        static CMusUiLevelIndicator* NewLC(
                                TAknOrientation aLayout,
        						TInt aMinLevel, 
        						TInt aMaxLevel, 
                                CCoeControl* aParent,
                                const TRect& aRect,
        						MMusUiLevelIndicatorObserver* aObserver );

        /**
        * Destructor.
        */
        ~CMusUiLevelIndicator();
        

    public: // New functions

		/**
		*
		*/
		void SetLevelL( TMusUiIndicatorType aType, 
		                TInt aLevel, 
		                TBool aUseTimer );

		/**
		*
		*/
		TInt GetLevel(TMusUiIndicatorType aType);

		/**
		*
		*/
		void SetRange( TInt aMinLevel, TInt aMaxLevel, TBool aUseTimer);
		
		/**
		*
		*/
		void SetIndicatorType( TMusUiIndicatorType aType );
			
		
	private: // From CCoeControl

	    /**
	     * Handles the size change events.
	     *
	     */
	    virtual void SizeChanged();
	    
		virtual void Draw(const TRect& aRect) const;

        virtual TInt CountComponentControls() const;
        
        virtual CCoeControl* ComponentControl( TInt aIndex ) const;

        virtual void HandleResourceChange( TInt aType );


    private: // From CCoeControlObserver
    
        virtual void HandleControlEventL( CCoeControl* aControl,
                                          TCoeEvent aEventType );
        
        
    private: // From MMusUiActiveTimerObserver    
    
        virtual void TimerComplete( CMusUiActiveTimer* aTimer );


    private: // Constructors and destructor

        /**
        * C++ constructor
        */
        CMusUiLevelIndicator( TInt aMinLevel, 
        					  TInt aMaxLevel, 
        					  MMusUiLevelIndicatorObserver* aObserver );

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL( TAknOrientation aLayout, 
                         CCoeControl* aParent,
                         const TRect& aRect );


    private: // HELPERS

        void  ConstructImageL( CEikImage* aImage,
                               MAknsSkinInstance* aSkin,
                               const TAknsItemID& aId,
                               TInt aBitmapId,
                               TInt aMaskId,
                               TParse aParse );

        /**
        * Callback for disable level indicator.
        * @since 2.8
        * @return KErrNone
        */
        //static TInt DisableLevelIndicator( TAny* aThis );
		//void DisableTimer();        
		//void EnableTimer();


    private: //data

      
        TRect iParentRect;

        TRect iViewfinderRect;

		TInt iMinLevel;

		TInt iMaxLevel;
		
		TInt iZoomLevel;
		
		TInt iBrightnessLevel;
		
		TInt iDurationLevel;

        CMusUiActiveTimer* iDisableLevelTimer;       
      
		TInt iIndicatorType;
		
        CAknSlider* iSlider;

        CMusUiActiveTimer* iSliderEventTimer;

        /**
        * NOT OWN!!!
        */
		MMusUiLevelIndicatorObserver* iObserver;

    };


#endif

// End of File
