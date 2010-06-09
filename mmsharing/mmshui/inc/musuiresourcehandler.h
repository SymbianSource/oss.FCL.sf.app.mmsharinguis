/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Handler for camera, keypad and video player resources
*
*/



#include "musuipropertyobserver.h"
#include "musresourceproperties.h"
#include "mussettingskeys.h"
#include "musmanagercommon.h"

#include <e32base.h>

class CMusUiPropertyWatch;
class MMusUiEventObserver;
class CAknWaitDialog;

/**
 * Encapsulates handling and observing of camera, keypad and video player
 * hardware and software resources.
 */
class CMusUiResourceHandler : public CBase,
                              public MMusUiPropertyObserver
    {
    
    public:
    
        virtual ~CMusUiResourceHandler();
        
        static CMusUiResourceHandler* NewL( MMusUiEventObserver& aObserver );
    
    public: // API
        
        TBool RequestKeypadL( TBool aMandatory = EFalse );
        
        TBool RequestCameraL( TBool aMandatory = EFalse );
        
        TBool RequestVideoPlayerL( TBool aMandatory = EFalse );
        
        void CheckInitialOrientationL( MultimediaSharing::TMusUseCase aUsecase );
        
        
    public:	// From MMusUiPropertyObserver

        /**
        * @param TUint aKey - The Property key
        * @param TInt aValue - The value of the Property key
        */
        virtual	void PropertyChanged( const TUint aKey, const TInt aValue );

        virtual void HandlePropertyError( const TInt aReason );
        
        
    private:
    
        CMusUiResourceHandler( MMusUiEventObserver& aObserver );
        
        void ConstructL();
    
    
    private: // Helpers
    
        TBool ShowChangeProductModeDialogL( const TDesC& aPrompt );
        
        TBool IsCurrentlyAvailable( NMusResourceApi::TAvailability aResourceAvailability,
        							MusSettingsKeys::TUsability aResourceUsability );
        
        
    private: // data
        
        /**
        * 
        */
        MMusUiEventObserver& iObserver;
    
        /**
        * 
        */
    	CMusUiPropertyWatch* iCameraResourceWatch;

        /**
        * 
        */
    	CMusUiPropertyWatch* iKeypadResourceWatch;
        
        /**
        * It holds the value for CameraAvailability.
        * If it is static available ( Bit set to 0 in Cenrep)
        * then it will be available always , if the bit set
        * to 1 ( dynamic ) then it should be monitored by property
        * listner and set the value accordingly.
        */
        NMusResourceApi::TAvailability iCameraAvailability;

        /**
        * It holds the value for KeypadAvailability.
        * If it is static available ( Bit set to 0 in Cenrep)
        * then it will be available always , if the bit set
        * to 1 ( dynamic ) then it should be monitored by property
        * listner and set the value accordingly.
        */
        NMusResourceApi::TAvailability iKeypadAvailability;

		/**
        * It holds the value for CameraUsability.
        * Intially read from cenrep and assigned
        * Refer mussettingskeys.h to know the bit value.
        */
        MusSettingsKeys::TUsability iCameraUsability;
        
        /**
        * It holds the value for KeypadUsability.
        * Intially read from cenrep and assigned
        * Refer mussettingskeys.h to know the bit value.
        */
        MusSettingsKeys::TUsability iKeypadUsability;
        
        /**
        * It holds the value for VideoplayerUsability.
        * Intially read from cenrep and assigned
        * Refer mussettingskeys.h to know the bit value.
        */
        MusSettingsKeys::TUsability iVideoplayerUsability;

        /**
        * 
        */
        TBool iCameraAvailibilityInProgress;
        
        /**
        * 
        */
        TBool iKeypadAvailibilityInProgress;
        
        /**
        * 
        */
        CAknWaitDialog* iChangeProductModeDialog;
        
        /**
        * 
        */
        HBufC* iDialogPrompt;
        
    };




