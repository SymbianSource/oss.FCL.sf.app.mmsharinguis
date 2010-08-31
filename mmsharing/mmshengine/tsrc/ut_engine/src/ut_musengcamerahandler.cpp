/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


//  INTERNAL INCLUDES
#include "ut_musengcamerahandler.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
#include "musenglivesession.h"
#include "mussipprofilehandler.h"
#include "musengmceutils.h"
#include "mussettings.h"
#include "mussessionproperties.h"

//  SYSTEM INCLUDES
#include <eunitmacros.h>
#include <mceoutsession.h>
#include <mcestreambundle.h>
#include <mcevideostream.h>
#include <mcecamerasource.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcertpsource.h>

const TUint KSipProfileId( 1 );

//use step in different time period
const TInt KZoomBigStepCount = 15;
const TInt KZoomSmallStepCount = KZoomBigStepCount*2;
const TInt KZoomStepMinSize = 1;

// 0.4 second
const TInt64 KZoomFasterTime = 400000;//must bigger than 1/3 second

//insure that step is 1
#define USER_SLEEP  User::After(KZoomFasterTime)



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_TMusEngCameraHandler* UT_TMusEngCameraHandler::NewL()
    {
    UT_TMusEngCameraHandler* self = UT_TMusEngCameraHandler::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_TMusEngCameraHandler* UT_TMusEngCameraHandler::NewLC()
    {
    UT_TMusEngCameraHandler* self = new( ELeave ) UT_TMusEngCameraHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_TMusEngCameraHandler::~UT_TMusEngCameraHandler()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_TMusEngCameraHandler::UT_TMusEngCameraHandler()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::SetupL()
    {
    iManager = CMceManager::NewL( TUid::Null(), NULL );
    iObserver = new( ELeave ) CMusEngObserverStub;
    iObserver->iRoamingBetweenAPsAllowed = ETrue;
    iProfileHandler = CMusSipProfileHandler::NewL( *iObserver );
     
    iProfileHandler->CreateProfileL( KSipProfileId );
     
    iSession = CMceOutSession::NewL( *iManager,
                                     *iProfileHandler->Profile(),
                                      KTestRecipientSipUri8() );
       
       
    CMceVideoStream* videoOut = CMceVideoStream::NewLC();
    
    videoOut->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    videoOut->AddSinkL( CMceDisplaySink::NewLC( *iManager ) );
    CleanupStack::Pop();
    
    videoOut->SetSourceL( CMceCameraSource::NewLC( *iManager ) );
    CleanupStack::Pop();
    
    CMceVideoStream* videoIn  = CMceVideoStream::NewLC();
    
    CMceDisplaySink* receivingDisplay = CMceDisplaySink::NewLC( *iManager );
    videoIn->AddSinkL( receivingDisplay );
    CleanupStack::Pop();
    
    videoIn->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    videoOut->BindL( videoIn );
    CleanupStack::Pop( videoIn );
    
    iSession->AddStreamL( videoOut );
    CleanupStack::Pop( videoOut ); 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::Teardown()
    {
    delete iSession;
    iSession = NULL;
    iCameraHandler.SetSession( NULL );
    delete iProfileHandler;
    delete iObserver;
    delete iManager;
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_LcCameraCountL()
    {
    // Call without setting the MCE session
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.LcCameraCountL(), KErrNotReady )
    
    iCameraHandler.SetSession( iSession );
    CMceCameraSource* camera = 
        MusEngMceUtils::GetCameraL( *( iSession ) );
    iCameraHandler.iCameraUsage = MusSettingsKeys::EUseCameraSwapping;
    camera->iCameraCount = 2;
    camera->iCameraIndex = TMusEngCameraHandler::EBackCamera;
   
    // Two cameras
    EUNIT_ASSERT_EQUALS( 2, iCameraHandler.LcCameraCountL() )

    // Two cameras, swapping cameras not allowed, use only main camera
    iCameraHandler.iCameraUsage = MusSettingsKeys::EUseOnlyMainCamera;
    camera->iCameraCount = 2;
    EUNIT_ASSERT_EQUALS( 1, iCameraHandler.LcCameraCountL() )

    // Two cameras, swapping cameras not allowed, use only main camera
    iCameraHandler.iCameraUsage = MusSettingsKeys::EUseOnlySecondaryCamera;
    camera->iCameraCount = 2;
    EUNIT_ASSERT_EQUALS( 1, iCameraHandler.LcCameraCountL() )

    // Only one camera available
    iCameraHandler.iCameraUsage = MusSettingsKeys::EUseCameraSwapping;
    camera->iCameraCount = 1;
    EUNIT_ASSERT_EQUALS( 1, iCameraHandler.LcCameraCountL() ) 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_ToggleLcCameraL()
    {
    EUNIT_ASSERT_EQUALS( TInt( TMusEngCameraHandler::ECameraNotAvailable ), 
                         iCameraHandler.CurrentLcCameraIndex() )     
    
    iCameraHandler.SetSession( iSession );
    iCameraHandler.iCameraUsage = MusSettingsKeys::EUseCameraSwapping;
    iCameraHandler.iCurrentCamera = TMusEngCameraHandler::EBackCamera;
    
    // Switch from back to front
    iCameraHandler.ToggleLcCameraL();
    EUNIT_ASSERT_EQUALS( TInt( TMusEngCameraHandler::EFrontCamera ), 
                         iCameraHandler.CurrentLcCameraIndex() )
    
    // Switch from front to back
    iCameraHandler.ToggleLcCameraL();
    EUNIT_ASSERT_EQUALS( TInt( TMusEngCameraHandler::EBackCamera ), 
                         iCameraHandler.CurrentLcCameraIndex() )    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_MinLcZoomL()
    {
    // MCE Session has not been set
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.MinLcZoomL(), KErrNotReady )

    // MCE Session has been set
    iCameraHandler.SetSession( iSession );
    EUNIT_ASSERT_EQUALS( 
        iCameraHandler.iCameraInfo.iMinZoom,
        iCameraHandler.MinLcZoomL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_MaxLcZoomL()
    {
    // MCE Session has not been set
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.MaxLcZoomL(), KErrNotReady )

    // MCE Session has been set
    iCameraHandler.SetSession( iSession );
    EUNIT_ASSERT_EQUALS( 
        iCameraHandler.iCameraInfo.iMaxZoom + 
        iCameraHandler.iCameraInfo.iMaxDigitalZoom,
        iCameraHandler.MaxLcZoomL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_LcZoomValueL()
    {  
    // MCE Session has not been set
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.LcZoomValueL(), KErrNotReady )
    
    // Check that originally current and default zoom are the same
    iCameraHandler.SetSession( iSession );
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *( iSession ) );    
    
    // Zoom to maximum optical value
    while ( camera->iZoomFactor < iCameraHandler.iCameraInfo.iMaxZoom )
        {
        iCameraHandler.LcZoomInL();
        }   
    EUNIT_ASSERT_EQUALS( 
        iCameraHandler.iCameraInfo.iMaxZoom,
        iCameraHandler.LcZoomValueL() )   

    // Zoom using digital zoom
    iCameraHandler.LcZoomInL();
    EUNIT_ASSERT_EQUALS(
        iCameraHandler.iCameraInfo.iMaxZoom + camera->iDigitalZoomFactor,
        iCameraHandler.LcZoomValueL() )              
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_SetLcZoomValueL()
    {
    // MCE Session has not been set
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCameraHandler.SetLcZoomValueL( 1 ), 
        KErrNotReady )
    
    // Value above the maximum zoom
    iCameraHandler.SetSession( iSession );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCameraHandler.SetLcZoomValueL( iCameraHandler.MaxLcZoomL() + 1 ), 
        KErrArgument )   
    
    // Value below the minimum zoom
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCameraHandler.SetLcZoomValueL( iCameraHandler.MinLcZoomL() - 1 ), 
        KErrArgument )     
    
    // OK: set zoom factor
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );
    TInt zoomValue = iCameraHandler.iCameraInfo.iMaxZoom - 1;  
    iCameraHandler.SetLcZoomValueL( zoomValue );
    EUNIT_ASSERT_EQUALS( zoomValue, camera->ZoomFactorL() )     
    
    // OK: set digital zoom factor
    zoomValue = iCameraHandler.iCameraInfo.iMaxZoom + 1;
    iCameraHandler.SetLcZoomValueL( zoomValue );
    EUNIT_ASSERT_EQUALS( 1, camera->DigitalZoomFactorL() )  
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_LcZoomInL()
    {    
    // Check that zooming is not possible before invite
    TRAPD( error, iCameraHandler.LcZoomInL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    iCameraHandler.SetSession( iSession );
    
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *( iSession ) );
    
    TInt oldZoomFactor = camera->iZoomFactor;
    
    // Check that zoom factor can be increased
    USER_SLEEP;  //insure that small step is used
    iCameraHandler.LcZoomInL();
    EUNIT_ASSERT( camera->iZoomFactor == oldZoomFactor +
            iCameraHandler.iSmallZoomStep );
    
   //Check the "burst" mode (immediate second ZoomIn), big step should be used
    TInt nZoom = camera->iZoomFactor + camera->iDigitalZoomFactor;
    iCameraHandler.LcZoomInL();
    TInt nStep = camera->iZoomFactor + camera->iDigitalZoomFactor - nZoom;
    EUNIT_ASSERT( nStep == iCameraHandler.iBigZoomStep );

    // Zoom to maximum optical value
    while ( camera->iZoomFactor < iCameraHandler.iCameraInfo.iMaxZoom )
        {
        iCameraHandler.LcZoomInL();
        }
        
    TInt maxOpticalZoomFactor = camera->ZoomFactorL();
    
    // Check that digital zoom is used instead of optical
    iCameraHandler.LcZoomInL();
    EUNIT_ASSERT( camera->iZoomFactor == maxOpticalZoomFactor );
    EUNIT_ASSERT( camera->iDigitalZoomFactor > 0 );
    
    // Zoom to maximum digital value
    while ( camera->iDigitalZoomFactor < 
            iCameraHandler.iCameraInfo.iMaxDigitalZoom )
        {
        iCameraHandler.LcZoomInL();
        }
    
    TInt maxDigitalZoomFactor = camera->DigitalZoomFactorL();
    
    // Check that further zooming is impossible
    iCameraHandler.LcZoomInL();
    EUNIT_ASSERT( camera->iZoomFactor == maxOpticalZoomFactor );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == maxDigitalZoomFactor );   
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_LcZoomOutL()
    {
    // Check that zooming is not possible before invite
    TRAPD( error, iCameraHandler.LcZoomOutL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    iCameraHandler.SetSession( iSession );
    
    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iSession) );
    
    // Simulate maximum zoom 
    camera->iZoomFactor = iCameraHandler.iCameraInfo.iMaxZoom;
    camera->iDigitalZoomFactor = iCameraHandler.iCameraInfo.iMaxDigitalZoom;
    
    // Check that digital zoom factor can be decreased
    USER_SLEEP;  //insure that small step is used
    iCameraHandler.LcZoomOutL();
    EUNIT_ASSERT( camera->iZoomFactor == iCameraHandler.iCameraInfo.iMaxZoom );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 
                    iCameraHandler.iCameraInfo.iMaxDigitalZoom - 
                    iCameraHandler.iSmallZoomStep );
    
    //Check the "burst" mode (immediate second ZoomOut), big step should be used
    TInt nZoom = camera->iZoomFactor + camera->iDigitalZoomFactor;
    iCameraHandler.LcZoomOutL();
    TInt nStep;
    nStep = nZoom - (camera->iZoomFactor + camera->iDigitalZoomFactor );
    EUNIT_ASSERT( nStep == iCameraHandler.iBigZoomStep );
 
    // Simulate maximum zoom again
    camera->iZoomFactor = iCameraHandler.iCameraInfo.iMaxZoom;
    camera->iDigitalZoomFactor = iCameraHandler.iCameraInfo.iMaxDigitalZoom;
    
    // Zoom to minimum digital zoom
    while ( camera->iDigitalZoomFactor > 0 )
        {
        iCameraHandler.LcZoomOutL();
        }
    
    EUNIT_ASSERT( camera->iZoomFactor == iCameraHandler.iCameraInfo.iMaxZoom );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 0 );
    
    // Check that optical zoom is decreased instead of digital
    USER_SLEEP;  //insure that step is the small one
    iCameraHandler.LcZoomOutL();
    EUNIT_ASSERT( camera->iZoomFactor == 
                    iCameraHandler.iCameraInfo.iMaxZoom -
                    iCameraHandler.iSmallZoomStep );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 0 );
    
    // Zoom to minimum optical zoom
    while ( camera->iZoomFactor > iCameraHandler.iCameraInfo.iMinZoom )
        {
        iCameraHandler.LcZoomOutL();
        }
    
    // Check that further zooming is impossible
    iCameraHandler.LcZoomOutL();
    EUNIT_ASSERT( camera->iZoomFactor == iCameraHandler.iCameraInfo.iMinZoom );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 0 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_SetLcBrightnessL()
    {
    // Check that setting brightness is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iCameraHandler.SetLcBrightnessL( KTestSomeBrightness ),
        KErrNotReady )
    
    iCameraHandler.SetSession( iSession );
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *( iSession ) );
   
    camera->iCameraInfo.iOptionsSupported =
        camera->iCameraInfo.iOptionsSupported|TCameraInfo::EBrightnessSupported;

    // Check that originally current and default brightness are the same
    EUNIT_ASSERT_EQUALS( iCameraHandler.iDefaultBrightness,
                         iCameraHandler.LcBrightnessL() )

    // Test setting brightness
    iCameraHandler.SetLcBrightnessL( KTestSomeBrightness );
    EUNIT_ASSERT_EQUALS( KTestSomeBrightness, iCameraHandler.LcBrightnessL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_LcBrightnessL()
    {
    // Check that checking current brightness is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.LcBrightnessL(),
                                 KErrNotReady )
    
    // All the other stuff checked in tests of SetBrightnessL
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_MaxLcBrightnessL()
    {
    // MaxBrightness should return a constant
    EUNIT_ASSERT_EQUALS( KTestMaxBrightness, iCameraHandler.MaxLcBrightnessL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_MinLcBrightnessL()
    {
    // MinBrightness should return a constant

    EUNIT_ASSERT_EQUALS( KTestMinBrightness, iCameraHandler.MinLcBrightnessL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_IncreaseLcBrightnessL()
    {
    // Check that increasing brightness is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.IncreaseLcBrightnessL(),
                                 KErrNotReady )

    iCameraHandler.SetSession( iSession );
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *(iSession) );

    camera->iCameraInfo.iOptionsSupported =
        camera->iCameraInfo.iOptionsSupported|TCameraInfo::EBrightnessSupported;
    
    // Check that originally current and default brightness are the same
    EUNIT_ASSERT_EQUALS( iCameraHandler.iDefaultBrightness,
                         iCameraHandler.LcBrightnessL() )

    // Check that brightness can be increased...
    iCameraHandler.IncreaseLcBrightnessL();
    EUNIT_ASSERT_EQUALS( iCameraHandler.iDefaultBrightness + KTestBrightnessStepSize,
                         iCameraHandler.LcBrightnessL() )
                         
    // ... but only to maximum
    iCameraHandler.SetLcBrightnessL( KTestMaxBrightness );
    iCameraHandler.IncreaseLcBrightnessL();
    
    EUNIT_ASSERT_EQUALS( KTestMaxBrightness,
                         iCameraHandler.LcBrightnessL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_DecreaseLcBrightnessL()
    {
    // Check that decreasing brightness is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.DecreaseLcBrightnessL(),
                                 KErrNotReady )

    iCameraHandler.SetSession( iSession );
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *(iSession) );

    camera->iCameraInfo.iOptionsSupported =
        camera->iCameraInfo.iOptionsSupported|TCameraInfo::EBrightnessSupported;
    
    // Check that originally current and default brightness are the same
    EUNIT_ASSERT_EQUALS( iCameraHandler.iDefaultBrightness,
                         iCameraHandler.LcBrightnessL() )

    // Check that brightness can be decreased...
    iCameraHandler.DecreaseLcBrightnessL();

    EUNIT_ASSERT_EQUALS( 
        iCameraHandler.iDefaultBrightness - KTestBrightnessStepSize,
        iCameraHandler.LcBrightnessL() )
    
    // ... but only to minimum
    iCameraHandler.SetLcBrightnessL( KTestMinBrightness );
    iCameraHandler.DecreaseLcBrightnessL();
    EUNIT_ASSERT_EQUALS( KTestMinBrightness,
                         iCameraHandler.LcBrightnessL() )
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_TMusEngCameraHandler::UT_PlayL()
    {
    // Check that resuming is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.PlayL(), KErrNotReady )
   
    iCameraHandler.SetSession( iSession );

    iCameraHandler.PauseL();
 
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *(iSession) );    
    iCameraHandler.PlayL();
    EUNIT_ASSERT( camera->IsEnabled() )
 
    // Try to enable camera again, request should be ignored
    iCameraHandler.PlayL();
    EUNIT_ASSERT( camera->IsEnabled() )
    }  

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_TMusEngCameraHandler::UT_PauseL()
    {
    // Check that pausing is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.PauseL(), KErrNotReady )
    
    iCameraHandler.SetSession( iSession );
    
    iCameraHandler.PlayL();
    
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *(iSession) );
                    
    iCameraHandler.PauseL();

    EUNIT_ASSERT( !camera->IsEnabled() )
    
    // Try to disable camera again, request should be ignored
    iCameraHandler.PauseL();
    EUNIT_ASSERT( !camera->IsEnabled() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_TMusEngCameraHandler::UT_IsPlayingL()
    {
    // Try without a session 
    EUNIT_ASSERT_SPECIFIC_LEAVE( iCameraHandler.IsPlayingL(), KErrNotReady )
    
    // Normal cases
    iCameraHandler.SetSession( iSession );
    
    iCameraHandler.PlayL();
    EUNIT_ASSERT( iCameraHandler.IsPlayingL() )
    
    iCameraHandler.PauseL();
    EUNIT_ASSERT( !iCameraHandler.IsPlayingL() )
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_InitializeZoomStepSizeL()
    {
    //Check that zooming steps have defualt values before session establishment
    iCameraHandler.InitializeZoomStepSize();
    EUNIT_ASSERT( iCameraHandler.iSmallZoomStep == KZoomStepMinSize );
    EUNIT_ASSERT( iCameraHandler.iBigZoomStep == KZoomStepMinSize );
    
    iCameraHandler.SetSession( iSession );   
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *(iSession) );
    
    //Check that zooming step sizes have default value KZoomStepMinSize 
    //in case of zooming values range is smaller than KZoomBigStepCount
    iCameraHandler.iCameraInfo.iMinZoom = 0;
    iCameraHandler.iCameraInfo.iMaxZoom = 0;
    iCameraHandler.iCameraInfo.iMaxDigitalZoom = KZoomBigStepCount / 2;    
    iCameraHandler.InitializeZoomStepSize();
    EUNIT_ASSERT( iCameraHandler.iSmallZoomStep == KZoomStepMinSize );
    EUNIT_ASSERT( iCameraHandler.iBigZoomStep == KZoomStepMinSize );
    
    //Check that zooming step sizes have default value KZoomStepMinSize 
    //in case of zooming values range is bigger than KZoomBigStepCount,
    //but smaller than 2*KZoomBigStepCount (KZoomSmallStepCount)
    iCameraHandler.iCameraInfo.iMaxDigitalZoom = 2*KZoomBigStepCount - 1;
    iCameraHandler.InitializeZoomStepSize();
    EUNIT_ASSERT( iCameraHandler.iSmallZoomStep == KZoomStepMinSize );
    EUNIT_ASSERT( iCameraHandler.iBigZoomStep == KZoomStepMinSize );
 
    //Check zooming steps sizes in case of zooming values range is
    //bigger than 2*KZoomBigStepCount and smaller than 2*KZoomSmallStepCount
    iCameraHandler.iCameraInfo.iMinZoom = - KZoomBigStepCount;
    iCameraHandler.iCameraInfo.iMaxZoom = KZoomBigStepCount;
    iCameraHandler.iCameraInfo.iMaxDigitalZoom = KZoomBigStepCount;
    //zooming range is (-KZoomBigStepCount, 2*KZoomBigStepCount)
    iCameraHandler.InitializeZoomStepSize();
    EUNIT_ASSERT( iCameraHandler.iSmallZoomStep == KZoomStepMinSize );
    EUNIT_ASSERT( iCameraHandler.iBigZoomStep == 3 );

    //Check zooming steps sizes in case of zooming values range is
    //bigger than 2*KZoomSmallStepCount
    iCameraHandler.iCameraInfo.iMaxZoom = KZoomSmallStepCount;
    iCameraHandler.iCameraInfo.iMaxDigitalZoom = KZoomSmallStepCount;
    iCameraHandler.iCameraInfo.iMinZoom = - KZoomSmallStepCount;
    //zooming range is (-KZoomSmallStepCount, 2*KZoomSmallStepCount)
    iCameraHandler.InitializeZoomStepSize();
    EUNIT_ASSERT( iCameraHandler.iSmallZoomStep == 3 );
    EUNIT_ASSERT( iCameraHandler.iBigZoomStep == 6 );    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_ChangeActiveCameraL()
    {
    iCameraHandler.SetSession( iSession );
    
    CMceCameraSource* camera = 
        MusEngMceUtils::GetCameraL( *(iSession) );
    
    //Camera is not enabled
    camera->iIsEnabled = ETrue;
    camera->iCameraCount = 2;
    camera->iCameraIndex = 1;
    iCameraHandler.ChangeActiveCameraL( TMusEngCameraHandler::EFrontCamera );
    EUNIT_ASSERT(  camera->iCameraIndex == 1 );
    EUNIT_ASSERT(  camera->iIsEnabled == ETrue );
    
    //Camera is not enabled
    camera->iIsEnabled = EFalse;
    camera->iCameraCount = 2;
    camera->iCameraIndex = 1;
    iCameraHandler.ChangeActiveCameraL( TMusEngCameraHandler::EFrontCamera );
    EUNIT_ASSERT(  camera->iCameraIndex == 1 );
    EUNIT_ASSERT(  camera->iIsEnabled == EFalse );    

    //Check that iCameraInfo get updated after camera change
    EUNIT_PRINT(_L("Check that iCameraInfo get updated after camera change"));
    
    //Check that iCameraInfo get updated after camera change
    EUNIT_PRINT(_L("Check that iCameraInfo get updated after camera change"));
    camera->iIsEnabled = ETrue;
    camera->iCameraCount = 2;
    camera->iCameraIndex = 0;
    iCameraHandler.ChangeActiveCameraL( TMusEngCameraHandler::EBackCamera );
    EUNIT_ASSERT(  camera->iCameraIndex == 0 );
    TCameraInfo infoBack = iCameraHandler.iCameraInfo;
    iCameraHandler.ChangeActiveCameraL( TMusEngCameraHandler::EFrontCamera );
    EUNIT_ASSERT(  camera->iCameraIndex == 1 );
    TCameraInfo infoFront = iCameraHandler.iCameraInfo;
    
    EUNIT_ASSERT(  infoBack.iMaxZoom != infoFront.iMaxZoom );
    EUNIT_ASSERT(  infoBack.iMaxDigitalZoom != infoFront.iMaxDigitalZoom );
    camera->iIsEnabled = ETrue;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngCameraHandler::UT_ChangeCameraL()
    { 
    TRAPD( error, iCameraHandler.ChangeCameraL( TMusEngCameraHandler::EFrontCamera ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    iCameraHandler.SetSession( iSession );
    
    CMceCameraSource* camera = 
                            MusEngMceUtils::GetCameraL( *(iSession) );
    //Only one camera is supported 
    camera->iCameraCount = 0;
    camera->iCameraIndex = 0;
    
    iCameraHandler.ChangeCameraL( TMusEngCameraHandler::EFrontCamera );
    //EUNIT_ASSERT(  camera->iCameraIndex == 0 );
    
    // Front camera is supported.
    camera->iCameraCount = 2;
    camera->iCameraIndex = 1;
    iCameraHandler.ChangeCameraL( TMusEngCameraHandler::EFrontCamera );
    EUNIT_ASSERT(  camera->iCameraIndex == 1 );
    
    //No change
    camera->iCameraCount = 2;
    camera->iCameraIndex = 0;
    camera->iIsEnabled = EFalse;
    iCameraHandler.ChangeCameraL( TMusEngCameraHandler::EFrontCamera );
    EUNIT_ASSERT(  camera->iIsEnabled == EFalse );
    
    // Back camera is supported.
    camera->iCameraCount = 2;
    camera->iCameraIndex = 1;
    iCameraHandler.ChangeCameraL( TMusEngCameraHandler::EBackCamera );
    EUNIT_ASSERT(  camera->iCameraIndex == 0 );
        
    //No change
    camera->iCameraCount = 2;
    camera->iCameraIndex = 0;
    camera->iIsEnabled = EFalse;
    iCameraHandler.ChangeCameraL( TMusEngCameraHandler::EBackCamera );
    EUNIT_ASSERT(  camera->iIsEnabled == EFalse );
    
    // Next camera.
    camera->iCameraCount = 2;
    camera->iCameraIndex = 0;
    iCameraHandler.ChangeCameraL( TMusEngCameraHandler::ECameraNotAvailable );
    EUNIT_ASSERT(  camera->iCameraIndex == 1 );
    
    camera->iCameraCount = 2;
    camera->iCameraIndex = 1;
    iCameraHandler.ChangeCameraL( TMusEngCameraHandler::ECameraNotAvailable );
    EUNIT_ASSERT(  camera->iCameraIndex == 0 );    
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_TMusEngCameraHandler,
    "UT_TMusEngCameraHandler",
    "UNIT" )

EUNIT_TEST(
    "LcCameraCountL - test ",
    "TMusEngCameraHandler",
    "LcCameraCountL",
    "FUNCTIONALITY",
    SetupL, UT_LcCameraCountL, Teardown)

EUNIT_TEST(
    "ToggleLcCameraL - test ",
    "TMusEngCameraHandler",
    "ToggleLcCameraL",
    "FUNCTIONALITY",
    SetupL, UT_ToggleLcCameraL, Teardown)

EUNIT_TEST(
    "MinLcZoomL - test ",
    "TMusEngCameraHandler",
    "MinLcZoomL",
    "FUNCTIONALITY",
    SetupL, UT_MinLcZoomL, Teardown)
    
EUNIT_TEST(
    "MaxLcZoomL - test ",
    "TMusEngCameraHandler",
    "MaxLcZoomL",
    "FUNCTIONALITY",
    SetupL, UT_MaxLcZoomL, Teardown)

EUNIT_TEST(
    "LcZoomValueL - test ",
    "TMusEngCameraHandler",
    "LcZoomValueL",
    "FUNCTIONALITY",
    SetupL, UT_LcZoomValueL, Teardown)
    
EUNIT_TEST(
    "SetLcZoomValueL - test ",
    "TMusEngCameraHandler",
    "SetLcZoomValueL",
    "FUNCTIONALITY",
    SetupL, UT_SetLcZoomValueL, Teardown)     
    
EUNIT_TEST(
    "LcZoomInL - test ",
    "TMusEngCameraHandler",
    "LcZoomInL",
    "FUNCTIONALITY",
    SetupL, UT_LcZoomInL, Teardown)

EUNIT_TEST(
    "LcZoomOutL - test ",
    "TMusEngCameraHandler",
    "LcZoomOutL",
    "FUNCTIONALITY",
    SetupL, UT_LcZoomOutL, Teardown)

EUNIT_TEST(
    "SetLcBrightnessL - test ",
    "TMusEngCameraHandler",
    "SetLcBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_SetLcBrightnessL, Teardown)

EUNIT_TEST(
    "LcBrightnessL - test ",
    "TMusEngCameraHandler",
    "LcBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_LcBrightnessL, Teardown)

EUNIT_TEST(
    "MaxLcBrightnessL - test ",
    "TMusEngCameraHandler",
    "MaxLcBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_MaxLcBrightnessL, Teardown)

EUNIT_TEST(
    "MinLcBrightnessL - test ",
    "TMusEngCameraHandler",
    "MinLcBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_MinLcBrightnessL, Teardown)

EUNIT_TEST(
    "IncreaseLcBrightnessL - test ",
    "TMusEngCameraHandler",
    "IncreaseLcBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_IncreaseLcBrightnessL, Teardown)

EUNIT_TEST(
    "DecreaseLcBrightnessL - test ",
    "TMusEngCameraHandler",
    "DecreaseLcBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_DecreaseLcBrightnessL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "TMusEngCameraHandler",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_PlayL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "TMusEngCameraHandler",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_PauseL, Teardown)

EUNIT_TEST(
    "IsPlayingL - test ",
    "TMusEngCameraHandler",
    "IsPlayingL",
    "FUNCTIONALITY",
    SetupL, UT_IsPlayingL, Teardown)

EUNIT_TEST(
    "InitializeZoomStepSize - test ",
    "TMusEngCameraHandler",
    "InitializeZoomStepSize",
    "FUNCTIONALITY",
    SetupL, UT_InitializeZoomStepSizeL, Teardown)      

EUNIT_TEST(
    "ChangeCameraL - test ",
    "TMusEngCameraHandler",
    "ChangeCameraL",
    "FUNCTIONALITY",
    SetupL, UT_ChangeCameraL, Teardown)    
    
EUNIT_TEST(
    "UT_ChangeActiveCameraL - test ",
    "TMusEngCameraHandler",
    "DisableAndEnableBeforeChangingCamera",
    "FUNCTIONALITY",
    SetupL, UT_ChangeActiveCameraL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE


