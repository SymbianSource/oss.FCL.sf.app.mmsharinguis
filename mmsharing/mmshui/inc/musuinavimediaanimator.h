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



#ifndef MUSUINAVIMEDIAANIMATOR_H
#define MUSUINAVIMEDIAANIMATOR_H

#include "musuidefinitions.h"
#include "musuinavimediaanimatorobserver.h"

#include <coecntrl.h>
#include <AknUtils.h>

class CAknIconArray;
class MAknsSkinInstance;
class CPeriodic;

/**
 * CMusUiNaviMediaDecorator
 *
 * @lib musui.exe
 */
class CMusUiNaviMediaAnimator : public CBase
    {

    public:  // constructors and destructor

        /**
         * Two-phased constructor.
         *
         */
        static CMusUiNaviMediaAnimator* NewL(
                 MMusUiNaviMediaAnimatorObserver& aObserver, TSize aImageSize );

        /**
         *
         *
         */
        virtual ~CMusUiNaviMediaAnimator();


    private: // constructors

        /**
         * C++ default constructor.
         *
         */
        CMusUiNaviMediaAnimator( 
                MMusUiNaviMediaAnimatorObserver& aObserver, TSize aImageSize );

        /**
         * By default Symbian second-phase constructor is private.
         *
         */
        void ConstructL();


    public: // API

        /**
         * Sets animation timer interval eq. speed of animation
         *
         * @param aAnimationInterval Milliseconds between image transitions
         */
        void SetInterval( const TInt aAnimationInterval );


    private: // Perioidic timer CallBack

        static TInt AnimationInterval( TAny* aThis );
	    

    private: // HELPERS

        /**
         * Layouts all images.  And also set size.
         *
         */
        void LoadImagesL();


        /**
         * Construct image from resourse file
         *
         */
        void ConstructImageL( MAknsSkinInstance* aSkin,
                              const TAknsItemID& aId,
                              TInt aBitmapId,
                              TInt aMaskId,
                              TParse parse );



    private: // DATA

        /**
        *
        */
        CAknIconArray* iBitmapArray;

        /**
        *
        */
        //CAknIconArray* iMaskArray;

        /**
        * resource file location ( eq. Z or C )
        */
        TBool iResourceRam;

        /**
        * Animation image index
        */
        TInt iIndex;

        /**
        *
        */
        MMusUiNaviMediaAnimatorObserver& iObserver;

        /**
        *
        */
        TInt iInterval;
        
        /**
        * Image size
        */
        TSize iSize;
        
        /**
        *
        */
        CPeriodic* iAnimationTimer;   // owned


    };

#endif // MUSUINAVIMEDIADECORATOR_H

// end of file
