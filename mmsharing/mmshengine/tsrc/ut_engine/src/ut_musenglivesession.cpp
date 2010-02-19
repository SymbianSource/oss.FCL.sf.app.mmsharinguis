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
#include "ut_musenglivesession.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
#include "musenglivesession.h"
#include "mussipprofilehandler.h"
#include "musengmceutils.h"
#include "mussettings.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <mceoutsession.h>
#include <mcestreambundle.h>
#include <mcevideostream.h>
#include <mcecamerasource.h>
#include <mcertpsink.h>
#include <mcefilesink.h>
#include <mceh263codec.h>
#include <mceavccodec.h>
#include <mceaudiocodec.h>


_LIT8( KMusAvcBitrateLevel1TestText, "TestTextForAvcBrL1Level" );
_LIT8( KMusAvcBitrateLevel1bTestText, "TestTextForAvcBrL1bLevel" );
_LIT8( KMusAvcBitrateLevel1_1TestText, "TestTextForAvcBrL1_1Level" );
_LIT8( KMusAvcBitrateLevel1_2TestText, "TestTextForAvcBrL1_2Level" );
_LIT8( KMusAvcBitrateLevel1_3TestText, "TestTextForAvcBrL1_3Level" );
_LIT8( KMusAvcBitrateLevel2TestText, "TestTextForAvcBrL2Level" );

_LIT8( KMusAvcBitrateLevel_1b_ConfigKey, "AvcBrL1b=TestTextForAvcBrL1bLevel;" );

_LIT8( KMusAvcBitrateLevel_1_1_ConfigKey, 
"AvcBrL1_1=TestTextForAvcBrL1_1Level;");

_LIT8( KMusAvcBitrateLevels_1_1_And_1b_ConfigKeys, 
"AvcBrL1_1=TestTextForAvcBrL1_1Level;\
AvcBrL1b=TestTextForAvcBrL1bLevel;" );

_LIT8( KMusAvcAllLevelsConcatenation,
"AvcBrL1=TestTextForAvcBrL1Level;\
AvcBrL1b=TestTextForAvcBrL1bLevel;\
AvcBrL1_1=TestTextForAvcBrL1_1Level;\
AvcBrL1_2=TestTextForAvcBrL1_2Level;\
AvcBrL1_3=TestTextForAvcBrL1_3Level;\
AvcBrL2=TestTextForAvcBrL2Level;" );



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
UT_CMusEngLiveSession* UT_CMusEngLiveSession::NewL()
    {
    UT_CMusEngLiveSession* self = UT_CMusEngLiveSession::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveSession* UT_CMusEngLiveSession::NewLC()
    {
    UT_CMusEngLiveSession* self = new( ELeave ) UT_CMusEngLiveSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveSession::~UT_CMusEngLiveSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveSession::UT_CMusEngLiveSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::SetupL()
    {
    iObserver = new( ELeave ) CMusEngObserverStub;
    
    iLiveSession = CMusEngLiveSession::NewL( TRect(0, 0, 100, 100),
                                             *iObserver,
                                             *iObserver,
                                             *iObserver );
                                             
    iRecordedLiveSession = CMusEngLiveSession::NewL( KTestVideoFileName(),
                                                     TRect(0, 0, 100, 100),
                                                     *iObserver,
                                                     *iObserver,
                                                     *iObserver );    

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::Teardown()
    {
    delete iLiveSession;
    delete iRecordedLiveSession;
    delete iObserver;
    
    // Delete static data from CenRep stub
    CRepository::iStaticWriteAvcKeysToStaticData = EFalse;
    CRepository::DeleteStubAvcConfigKeys();
    CRepository::iForceFailWithCode = KErrNone;
    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_NewLL()
    {
    EUNIT_ASSERT( iLiveSession );
    EUNIT_ASSERT( iLiveSession->iRecordedFile == KNullDesC() );     
    EUNIT_ASSERT( iRecordedLiveSession ); 
    EUNIT_ASSERT( iRecordedLiveSession->iRecordedFile == KTestVideoFileName() );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_CurrentZoomLL()
    {
    // Check that checking current zoom is not possible before invite
    TRAPD( error, iLiveSession->CurrentZoomL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );

    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
    
    // Check that originally current and default zoom are the same
    EUNIT_ASSERT( iLiveSession->iDefaultZoomFactor == 
                  iLiveSession->CurrentZoomL() );
    
    // Zoom to maximum optical value
    while ( camera->iZoomFactor < iLiveSession->iCameraInfo.iMaxZoom )
        {
        iLiveSession->ZoomInL();
        }
    
    EUNIT_ASSERT( iLiveSession->iCameraInfo.iMaxZoom == 
                  iLiveSession->CurrentZoomL() );    

    // Zoom using digital zoom
    
    iLiveSession->ZoomInL();
    
    EUNIT_ASSERT( iLiveSession->CurrentZoomL() == 
                    iLiveSession->iCameraInfo.iMaxZoom + 
                    camera->iDigitalZoomFactor )
                  
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_CMusEngLiveSession_SetZoomLL()
    {
    TRAPD( error, iLiveSession->SetZoomL(2) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    ESTABLISH_OUT_SESSION( iLiveSession );
    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
    EUNIT_ASSERT( camera->iZoomFactor == 1 );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 0 );
    TRAPD( err, iLiveSession->SetZoomL(0) );
    EUNIT_ASSERT( err == KErrArgument );
    iLiveSession->SetZoomL(2);
    EUNIT_ASSERT( camera->iZoomFactor == 2 );
    TRAPD( err1, iLiveSession->SetZoomL(14) );
    EUNIT_ASSERT( err1 == KErrArgument );
    iLiveSession->SetZoomL( 5 );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 2 );
    }
void UT_CMusEngLiveSession::UT_MaxZoomLL()
    {
    // Check that checking maximum zoom is not possible before invite
    TRAPD( error, iLiveSession->MaxZoomL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );

    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    EUNIT_ASSERT( iLiveSession->MaxZoomL() == 
                    iLiveSession->iCameraInfo.iMaxZoom + 
                    iLiveSession->iCameraInfo.iMaxDigitalZoom );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_ZoomInLL()
    {    
    // Check that zooming is not possible before invite
    TRAPD( error, iLiveSession->ZoomInL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
    
    // Check the default value
    EUNIT_ASSERT( iLiveSession->iDefaultZoomFactor == camera->iZoomFactor );

    // Check that zoom factor can be increased
    USER_SLEEP;  //insure that small step is used
    iLiveSession->ZoomInL();
    EUNIT_ASSERT( camera->iZoomFactor == iLiveSession->iDefaultZoomFactor +
            iLiveSession->iSmallZoomStep );
    
   //Check the "burst" mode (immediate second ZoomIn), big step should be used
    TInt nZoom = camera->iZoomFactor + camera->iDigitalZoomFactor;
    iLiveSession->ZoomInL();
    TInt nStep = camera->iZoomFactor + camera->iDigitalZoomFactor - nZoom;
    EUNIT_ASSERT( nStep == iLiveSession->iBigZoomStep );

    // Zoom to maximum optical value
    while ( camera->iZoomFactor < iLiveSession->iCameraInfo.iMaxZoom )
        {
        iLiveSession->ZoomInL();
        }
        
    TInt maxOpticalZoomFactor = camera->ZoomFactorL();
    
    // Check that digital zoom is used instead of optical
    iLiveSession->ZoomInL();
    EUNIT_ASSERT( camera->iZoomFactor == maxOpticalZoomFactor );
    EUNIT_ASSERT( camera->iDigitalZoomFactor > 0 );
    
    // Zoom to maximum digital value
    while ( camera->iDigitalZoomFactor < 
            iLiveSession->iCameraInfo.iMaxDigitalZoom )
        {
        iLiveSession->ZoomInL();
        }
    
    TInt maxDigitalZoomFactor = camera->DigitalZoomFactorL();
    
    // Check that further zooming is impossible
    iLiveSession->ZoomInL();
    EUNIT_ASSERT( camera->iZoomFactor == maxOpticalZoomFactor );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == maxDigitalZoomFactor );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_ZoomOutLL()
    {
    // Check that zooming is not possible before invite
    TRAPD( error, iLiveSession->ZoomOutL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
    
    // Check the default value
    EUNIT_ASSERT( iLiveSession->iDefaultZoomFactor == camera->iZoomFactor );
    
    // Simulate maximum zoom 
    camera->iZoomFactor = iLiveSession->iCameraInfo.iMaxZoom;
    camera->iDigitalZoomFactor = iLiveSession->iCameraInfo.iMaxDigitalZoom;
    
    // Check that digital zoom factor can be decreased
    USER_SLEEP;  //insure that small step is used
    iLiveSession->ZoomOutL();
    EUNIT_ASSERT( camera->iZoomFactor == iLiveSession->iCameraInfo.iMaxZoom );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 
                    iLiveSession->iCameraInfo.iMaxDigitalZoom - 
                    iLiveSession->iSmallZoomStep );
    
    //Check the "burst" mode (immediate second ZoomOut), big step should be used
    TInt nZoom = camera->iZoomFactor + camera->iDigitalZoomFactor;
    iLiveSession->ZoomOutL();
    TInt nStep;
    nStep = nZoom - (camera->iZoomFactor + camera->iDigitalZoomFactor );
    EUNIT_ASSERT( nStep == iLiveSession->iBigZoomStep );
 
    // Simulate maximum zoom again
    camera->iZoomFactor = iLiveSession->iCameraInfo.iMaxZoom;
    camera->iDigitalZoomFactor = iLiveSession->iCameraInfo.iMaxDigitalZoom;
    
    // Zoom to minimum digital zoom
    while ( camera->iDigitalZoomFactor > 0 )
        {
        iLiveSession->ZoomOutL();
        }
    
    EUNIT_ASSERT( camera->iZoomFactor == iLiveSession->iCameraInfo.iMaxZoom );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 0 );
    
    // Check that optical zoom is decreased instead of digital
    USER_SLEEP;  //insure that step is the small one
    iLiveSession->ZoomOutL();
    EUNIT_ASSERT( camera->iZoomFactor == 
                    iLiveSession->iCameraInfo.iMaxZoom -
                    iLiveSession->iSmallZoomStep );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 0 );
    
    // Zoom to minimum optical zoom
    while ( camera->iZoomFactor > iLiveSession->iCameraInfo.iMinZoom )
        {
        iLiveSession->ZoomOutL();
        }
    
    // Check that further zooming is impossible
    iLiveSession->ZoomOutL();
    EUNIT_ASSERT( camera->iZoomFactor == iLiveSession->iCameraInfo.iMinZoom );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 0 );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_ZoomDefaultLL()
    {
    // Check that zooming is not possible before invite
    TRAPD( error, iLiveSession->ZoomDefaultL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
    
    // Check the default value
    EUNIT_ASSERT( iLiveSession->iDefaultZoomFactor == camera->iZoomFactor );
    
    // increase zoom factor so much that optical zoom is in maximum value
    // and digital zoom is in use
    while ( camera->iDigitalZoomFactor == 0 )
        {
        iLiveSession->ZoomInL();
        }
    EUNIT_ASSERT( camera->iZoomFactor > iLiveSession->iDefaultZoomFactor );
    EUNIT_ASSERT( camera->iDigitalZoomFactor > 0 );
    
    // back to default
    iLiveSession->ZoomDefaultL();
    EUNIT_ASSERT( camera->iZoomFactor == iLiveSession->iDefaultZoomFactor  );
    EUNIT_ASSERT( camera->iDigitalZoomFactor == 0 );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_SetBrightnessLL()
    {
    // Check that setting brightness is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                        iLiveSession->SetBrightnessL( KTestSomeBrightness ),
                        KErrNotReady )
    
    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );

    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
   
    camera->iCameraInfo.iOptionsSupported =
    camera->iCameraInfo.iOptionsSupported | TCameraInfo::EBrightnessSupported;

    // Check that originally current and default brightness are the same
    EUNIT_ASSERT_EQUALS( iLiveSession->iDefaultBrightness,
                         iLiveSession->CurrentBrightnessL() )

    // Test setting brightness
    iLiveSession->SetBrightnessL( KTestSomeBrightness );

    EUNIT_ASSERT_EQUALS( KTestSomeBrightness,
                         iLiveSession->CurrentBrightnessL() )
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_CurrentBrightnessLL()
    {
    // Check that checking current brightness is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->CurrentBrightnessL(),
                                 KErrNotReady )
    
    // All the other stuff checked in tests of SetBrightnessL
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_MaxBrightnessLL()
    {
    // MaxBrightness should return a constant

    EUNIT_ASSERT_EQUALS( KTestMaxBrightness, iLiveSession->MaxBrightnessL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_MinBrightnessLL()
    {
    // MinBrightness should return a constant

    EUNIT_ASSERT_EQUALS( KTestMinBrightness, iLiveSession->MinBrightnessL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_IncreaseBrightnessLL()
    {
    // Check that increasing brightness is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->IncreaseBrightnessL(),
                                 KErrNotReady )

    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );

    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );

    camera->iCameraInfo.iOptionsSupported =
    camera->iCameraInfo.iOptionsSupported | TCameraInfo::EBrightnessSupported;
    
    // Check that originally current and default brightness are the same
    EUNIT_ASSERT_EQUALS( iLiveSession->iDefaultBrightness,
                         iLiveSession->CurrentBrightnessL() )

    // Check that brightness can be increased...
    iLiveSession->IncreaseBrightnessL();

    EUNIT_ASSERT_EQUALS( iLiveSession->iDefaultBrightness + KTestBrightnessStepSize,
                         iLiveSession->CurrentBrightnessL() )
                         
    // ... but only to maximum
    
    iLiveSession->SetBrightnessL( KTestMaxBrightness );
    
    iLiveSession->IncreaseBrightnessL();
    
    EUNIT_ASSERT_EQUALS( KTestMaxBrightness,
                         iLiveSession->CurrentBrightnessL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_DecreaseBrightnessLL()
    {
    // Check that decreasing brightness is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->DecreaseBrightnessL(),
                                 KErrNotReady )

    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );

    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );

    camera->iCameraInfo.iOptionsSupported =
    camera->iCameraInfo.iOptionsSupported | TCameraInfo::EBrightnessSupported;
    
    // Check that originally current and default brightness are the same
    EUNIT_ASSERT_EQUALS( iLiveSession->iDefaultBrightness,
                         iLiveSession->CurrentBrightnessL() )

    // Check that brightness can be decreased...
    iLiveSession->DecreaseBrightnessL();

    EUNIT_ASSERT_EQUALS( iLiveSession->iDefaultBrightness - KTestBrightnessStepSize,
                         iLiveSession->CurrentBrightnessL() )
    
    // ... but only to minimum
    
    iLiveSession->SetBrightnessL( KTestMinBrightness );
    
    iLiveSession->DecreaseBrightnessL();
    
    EUNIT_ASSERT_EQUALS( KTestMinBrightness,
                         iLiveSession->CurrentBrightnessL() )
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_BrightnessDefaultLL()
    {
    // Check that setting default brightness is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->BrightnessDefaultL(),
                                 KErrNotReady )

    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );

    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );

    camera->iCameraInfo.iOptionsSupported =
    camera->iCameraInfo.iOptionsSupported | TCameraInfo::EBrightnessSupported;
    
    // Check that originally current and default brightness are the same
    EUNIT_ASSERT_EQUALS( iLiveSession->iDefaultBrightness,
                         iLiveSession->CurrentBrightnessL() )

    // set brightness to max
    iLiveSession->SetBrightnessL( KTestMaxBrightness );
    EUNIT_ASSERT_NOT_EQUALS( iLiveSession->iDefaultBrightness,
                             iLiveSession->CurrentBrightnessL() )
    
    // set brightness to default and check it
    iLiveSession->BrightnessDefaultL();

    EUNIT_ASSERT_EQUALS( iLiveSession->iDefaultBrightness,
                         iLiveSession->CurrentBrightnessL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_BrightnessAutoLL()
    {
    // Check that setting brightness to auto is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->BrightnessAutoL(),
                                 KErrNotReady );

    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );

    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );

    camera->iCameraInfo.iOptionsSupported =
    camera->iCameraInfo.iOptionsSupported | TCameraInfo::EBrightnessSupported;
    
    // Check that originally brightness is not auto
    EUNIT_ASSERT_NOT_EQUALS( CCamera::EBrightnessAuto, 
                             iLiveSession->CurrentBrightnessL() )

    // check that brightness is set to auto
    iLiveSession->BrightnessAutoL();
    EUNIT_ASSERT_EQUALS( CCamera::EBrightnessAuto,
                         iLiveSession->CurrentBrightnessL() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_RecordL_and_IsRecordingLL()
    {
    // Check that recording is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iRecordedLiveSession->RecordL( ETrue ),
                                 KErrNotReady )
    
    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );
    ESTABLISH_OUT_SESSION( iRecordedLiveSession );
    
    // Check that recording is off by default
    EUNIT_ASSERT( !iRecordedLiveSession->IsRecording() );
    
    // Check that recording can be set on
    iRecordedLiveSession->RecordL( ETrue );
    EUNIT_ASSERT( iRecordedLiveSession->IsRecording() );
    
    // Check that recording  cannot be set on if recording stream does not exist
     EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->RecordL( ETrue ),
                                  KErrNotReady )
    
    // Check that setting on the recording twice does not harm
    iRecordedLiveSession->RecordL( ETrue );
    EUNIT_ASSERT( iRecordedLiveSession->IsRecording() );
    
    // Check that recording can be set off again
    iRecordedLiveSession->RecordL( EFalse );
    EUNIT_ASSERT( !iRecordedLiveSession->IsRecording() );
    
    // Check that setting off the recording twice does not harm
    iRecordedLiveSession->RecordL( EFalse );
    EUNIT_ASSERT( !iRecordedLiveSession->IsRecording() );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngLiveSession::UT_PlayLL()
    {
    // Check that resuming is not possible before invite
    TRAPD( error, iLiveSession->PlayL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    ESTABLISH_OUT_SESSION( iLiveSession );

    iLiveSession->PauseL();
    
    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
     
    iLiveSession->PlayL();

    EUNIT_ASSERT( camera->IsEnabled() );
 
    // Try to enable camera again, request should be ignored
    
    iLiveSession->PlayL();

    EUNIT_ASSERT( camera->IsEnabled() );
 
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngLiveSession::UT_PauseLL()
    {
    // Check that pausing is not possible before invite
    TRAPD( error, iLiveSession->PauseL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    iLiveSession->PlayL();
    
    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
                    
    iLiveSession->PauseL();

    EUNIT_ASSERT( !camera->IsEnabled() );
    
    // Try to disable camera again, request should be ignored
    
    iLiveSession->PauseL();

    EUNIT_ASSERT( !camera->IsEnabled() );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngLiveSession::UT_IsPlayingLL()
    {
    // Try without a session 
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->IsPlayingL(), KErrNotReady );
    
    // Normal cases
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    iLiveSession->PlayL();
    EUNIT_ASSERT( iLiveSession->IsPlayingL() )
    
    iLiveSession->PauseL();
    EUNIT_ASSERT( !iLiveSession->IsPlayingL() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_CompleteSessionStructureLL()
    {
    
    CMceStreamBundle* localBundle = 
                            CMceStreamBundle::NewLC( CMceStreamBundle::ELS );
    
    // Check that structure cannot be completed before creating the session
    TRAPD( error, iLiveSession->CompleteSessionStructureL( *localBundle ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Normal case
  
    CSIPProfile* profile = iLiveSession->iSipProfileHandler->Profile();
    
    iLiveSession->iSession = CMceOutSession::NewL( 
                                    *(iLiveSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() );
                             
    iLiveSession->CompleteSessionStructureL( *localBundle );
    
    EUNIT_ASSERT( localBundle->Streams().Count() == 0 )
    
    EUNIT_ASSERT( iLiveSession->iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Type() == KMceVideo );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Source() );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Source()->Type() ==
                  KMceCameraSource );
    EUNIT_ASSERT( !iLiveSession->IsPlayingL() )
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Sinks().Count() == 1 );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Sinks()[0]->Type() ==
                  KMceRTPSink );
    
    // Test that also recording stream is constructed if needed
    
    iRecordedLiveSession->iSession = CMceOutSession::NewL( 
                                            *(iLiveSession->iManager),
                                            *profile,
                                            KTestRecipientSipUri8() );
                                    
    iRecordedLiveSession->CompleteSessionStructureL( *localBundle );
    
    EUNIT_ASSERT( localBundle->Streams().Count() == 0 )
    
    CMceCameraSource* camera = 
            MusEngMceUtils::GetCameraL( *(iRecordedLiveSession->iSession) );
                    
    // Check that there is recorded stream and that source is same camera
    CMceMediaSink* file = NULL;
    const RPointerArray<CMceMediaStream>& streams = 
                                    iRecordedLiveSession->iSession->Streams();
                                        
    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->Type() == KMceVideo )
            {
            if ( streams[i]->Sinks()[0]->Type() == KMceFileSink )
                {
                file = streams[i]->Sinks()[0];
                }
            EUNIT_ASSERT( streams[i]->Source() == camera )
            }
        }
        
    EUNIT_ASSERT( file )
    
    CleanupStack::PopAndDestroy( localBundle );
                                   
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_HandleSessionStateChangedL()
    {
    // Make Repository empty, config keys must be written
    CRepository::SetStubAvcConfigKeysL( KNullDesC8() );
    
    // Invite    
    iLiveSession->InviteL( KTestRecipientSipUri );
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    
    // Force failure on CenRep
    CRepository::iForceFailWithCode = KErrNoMemory;
    
    // Simulate session state transition notification
    iLiveSession->HandleSessionStateChanged( 
                        *iLiveSession->iSession,
                        200, KNullDesC8() );

    EUNIT_ASSERT( iObserver->iSessionFailedCalled );
    
    // Normal case, something will be written to CenRep
    iRecordedLiveSession->InviteL( KTestRecipientSipUri );
    iRecordedLiveSession->iSession->iState = CMceSession::EEstablished;
    iRecordedLiveSession->HandleSessionStateChanged( 
                        *iRecordedLiveSession->iSession,
                        200, KNullDesC8() );
    
    HBufC8* info = MultimediaSharingSettings::EncoderConfigInfoLC();
    EUNIT_ASSERT_NOT_EQUALS( *info, KMusAvcBitrateLevel_1b_ConfigKey() )
    CleanupStack::PopAndDestroy( info );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_AdjustVideoCodecLL()
    {
    CRepository::SetStubAvcConfigKeysL( KNullDesC8() );
    
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );    
    CMceVideoCodec* codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    CMceVideoCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    CMceVideoCodec* codecUnknown = CMceH263Codec::NewLC( KNullDesC8() );
        
    iLiveSession->AdjustVideoCodecL( *codecH263 );
    iLiveSession->AdjustVideoCodecL( *codecH2632000 );    
    iLiveSession->AdjustVideoCodecL( *codecAvc );  
    iLiveSession->AdjustVideoCodecL( *codecUnknown );

    // Test that bit rates has been set to H263 & AVC codecs
    EUNIT_ASSERT( codecH263->iAllowedBitrates == KMceAllowedH263BitrateAll )
    EUNIT_ASSERT( codecH263->iMaxBitRate == KMceH263Level45Bitrate )
    EUNIT_ASSERT( codecH263->iBitrate == 80000 )
    
    EUNIT_ASSERT( codecH2632000->iAllowedBitrates == 
                  KMceAllowedH263BitrateAll )
    EUNIT_ASSERT( codecH2632000->iMaxBitRate == KMceH263Level45Bitrate )
    EUNIT_ASSERT( codecH2632000->iBitrate == 80000 )
    
    EUNIT_ASSERT( codecAvc->iAllowedBitrates == 
                                        KMceAvcCodecProfileIdBaseline | 
                                        KMceAvcCodecProfileIopConstraintSet | 
                                        KMceAvcBitrateLevel1b )
    EUNIT_ASSERT( iLiveSession->iStoreEncoderConfigInfo )


    // Test that bitrates has not been set to unknown codecs
    EUNIT_ASSERT( codecUnknown->iAllowedBitrates != 
                  KMceAllowedH263BitrateAll )
    EUNIT_ASSERT( codecUnknown->iMaxBitRate != KMceH263Level45Bitrate )
    EUNIT_ASSERT( codecUnknown->iBitrate != KMceH263Level10Bitrate )
    
    // Test that base class has been called to all codecs
    EUNIT_ASSERT( codecH263->iPayloadType == 96 )
    EUNIT_ASSERT( codecH2632000->iPayloadType == 96 )
    EUNIT_ASSERT( codecAvc->iPayloadType == 98 )
    EUNIT_ASSERT( codecUnknown->iPayloadType == 0 )
    
    CleanupStack::PopAndDestroy( codecUnknown );
    CleanupStack::PopAndDestroy( codecAvc );
    CleanupStack::PopAndDestroy( codecH2632000 );
    CleanupStack::PopAndDestroy( codecH263 );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_AdjustAudioCodecLL()
    {
    CMceAudioCodec* codec = 
                iLiveSession->iManager->SupportedAudioCodecs()[0]->CloneL();
    CleanupStack::PushL( codec );
    iLiveSession->AdjustAudioCodecL( *codec );
    
    // Does nothing so just test that base class has been called
    
    EUNIT_ASSERT( codec->iPayloadType == 97 )
    
    CleanupStack::PopAndDestroy( codec );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_DoCodecSelectionLL()
    {
    CMceVideoStream* stream = CMceVideoStream::NewLC();
    
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( _L8( "Foo" ) );
    stream->AddCodecL( codecH263 );
    CleanupStack::Pop( codecH263 );
        
    CMceVideoCodec* codecH2632000 = CMceH263Codec::NewLC( _L8( "Bar" ) );
    stream->AddCodecL( codecH2632000 );
    CleanupStack::Pop( codecH2632000 );
    
    CMceVideoCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    stream->AddCodecL( codecAvc );
    CleanupStack::Pop( codecAvc );
   
    // No assumptions so all the codecs are left to stream 
    iLiveSession->DoCodecSelectionL( *stream );    
    EUNIT_ASSERT_EQUALS( stream->Codecs().Count(), 3 )
    
    // Check that AVC codecs are removed if they are known to be not supported
    iLiveSession->SetSupportedVideoCodecListL( _L( "SomethingThatIsNotAVC" ) );
    iLiveSession->DoCodecSelectionL( *stream );
    EUNIT_ASSERT_EQUALS( stream->Codecs().Count(), 2 )
    EUNIT_ASSERT( stream->Codecs()[0]->SdpName() != KMceSDPNameH264() )
    EUNIT_ASSERT( stream->Codecs()[1]->SdpName() != KMceSDPNameH264() )
    
    CleanupStack::PopAndDestroy( stream );
    
    }
    

// -----------------------------------------------------------------------------
// Although all states are not meaningfull for LiveSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_StreamStateChangedL()
    {
    // Try without a session, nothing happens
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *videoStream );
    CleanupStack::PopAndDestroy( videoStream );
    
    // Simulate sending invite
    iLiveSession->InviteL( KTestRecipientSipUri() );
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled );
    iObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    }


// -----------------------------------------------------------------------------
// Although all states are not meaningfull for LiveSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_StreamStateChangedWithSourceL()
    {
    // Try without a session, nothing happens
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                    *videoStream, 
                    *CMceCameraSource::NewLC( *iLiveSession->iManager ) );
    CleanupStack::PopAndDestroy(); // camera
    CleanupStack::PopAndDestroy( videoStream );
    
    // Simulate sending invite
    iLiveSession->InviteL( KTestRecipientSipUri() );
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    CMceMediaSource* changedSource = changedStream->Source();
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );;
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled );
    iObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() );
    }
    

// -----------------------------------------------------------------------------
// Although all states are not meaningfull for LiveSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_StreamStateChangedWithSinkL()
    {    
    // Try without a session, nothing happens
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    iRecordedLiveSession->StreamStateChanged(
                *iLiveSession->iSession->Streams()[0],
                *iLiveSession->iSession->Streams()[0]->Sinks()[0] );
    
    // Try with session
    
    ESTABLISH_OUT_SESSION( iRecordedLiveSession )
    iRecordedLiveSession->RecordL( ETrue );
    
    // Test state CMceMediaStream::ENoResources
                    
    // Try without recording stream, nothing happens
    
    iLiveSession->StreamStateChanged(
                *iLiveSession->iSession->Streams()[0],
                *iLiveSession->iSession->Streams()[0]->Sinks()[0] );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled )
    iObserver->Reset();

    // Try again with recording stream, but indicate change in another stream,
    // nothing happens    
    iRecordedLiveSession->StreamStateChanged(
                *iLiveSession->iSession->Streams()[0],
                *iLiveSession->iSession->Streams()[0]->Sinks()[0] );
                
    // Try again with recording stream, but with stream state != ENoResources, 
    // nothing happens
    CMceVideoStream* recordingStream = 
        MusEngMceUtils::GetRecordingStream( *iRecordedLiveSession->iSession );
        
    EUNIT_ASSERT( recordingStream )
    EUNIT_ASSERT( recordingStream->State() != CMceMediaStream::ENoResources )
    
    iRecordedLiveSession->StreamStateChanged( *recordingStream,
                                              *recordingStream->Sinks()[0] );
    
    // Try again with recording stream and stream state == ENoResources,
    // but with enabled sink, nothing happens
        
    recordingStream->iState = CMceMediaStream::ENoResources;
    EUNIT_ASSERT( recordingStream->Sinks()[0]->iIsEnabled )
    
    iRecordedLiveSession->StreamStateChanged( *recordingStream,
                                              *recordingStream->Sinks()[0] );
    
    // Disk full case
    
    recordingStream->Sinks()[0]->iIsEnabled = EFalse;
    
    iRecordedLiveSession->StreamStateChanged( *recordingStream,
                                              *recordingStream->Sinks()[0] );
    
    EUNIT_ASSERT( iObserver->iDiskFullCalled )
    iObserver->Reset();
    
    
    // Test default stream state change behavior
    
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    CMceMediaSink* changedSink = changedStream->Sinks()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled );
    iObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    // This state has non-default meaning, tested before defaults
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_SetEncodingDeviceLL()
    {
    CMceAvcCodec* codec = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    
    // Generic error in CenRep, leave expected
    CRepository::iForceFailWithCode = KErrNotReady;
    EUNIT_ASSERT_LEAVE( iLiveSession->SetEncodingDeviceL( *codec ) )
    EUNIT_ASSERT_EQUALS( codec->iEncodingDecodingDevice, TUid::Uid( 0 ) )
    
    // No entry in CenRep, default value expected
    CRepository::iForceFailWithCode = KErrNotFound;
    const TUid KDefaultEncodingDevice( TUid::Uid( 0x20001C13 ) );
    iLiveSession->SetEncodingDeviceL( *codec );
    EUNIT_ASSERT_EQUALS( codec->iEncodingDecodingDevice, 
                         KDefaultEncodingDevice )
    
    // Entry in CenRep
    const TInt KSomeEncodingDevice( 0x20001C15 );
    CRepository::iStaticEncoderUid = KSomeEncodingDevice;
    iLiveSession->SetEncodingDeviceL( *codec );
    EUNIT_ASSERT_EQUALS( codec->iEncodingDecodingDevice, 
                         TUid::Uid( KSomeEncodingDevice ) )
                                             
    CleanupStack::PopAndDestroy( codec );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_SetCodecConfigKeyLL()
    {
    CMceAvcCodec* codec = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1b );

    // Generic error in CenRep, leave expected
    CRepository::iForceFailWithCode = KErrNotReady;
    EUNIT_ASSERT_LEAVE( iLiveSession->SetCodecConfigKeyL( *codec ) );
    EUNIT_ASSERT( !codec->iConfigKey )
    EUNIT_ASSERT( !iLiveSession->iStoreEncoderConfigInfo )
    CRepository::iForceFailWithCode = KErrNone;
    
    // No entry in CenRep, nothing happens
    iLiveSession->SetCodecConfigKeyL( *codec );
    EUNIT_ASSERT( !codec->iConfigKey )
    EUNIT_ASSERT( !iLiveSession->iStoreEncoderConfigInfo )
    
    // Empty entry in CenRep
    CRepository::SetStubAvcConfigKeysL( KNullDesC8() );
    iLiveSession->SetCodecConfigKeyL( *codec );
    EUNIT_ASSERT( !codec->iConfigKey )
    EUNIT_ASSERT( iLiveSession->iStoreEncoderConfigInfo )
    iLiveSession->iStoreEncoderConfigInfo = EFalse; // emulate original situation
    
    // Entry in CenRep
    CRepository::SetStubAvcConfigKeysL( KMusAvcBitrateLevel_1b_ConfigKey );
    iLiveSession->SetCodecConfigKeyL( *codec );
    EUNIT_ASSERT( codec->iConfigKey )
    HBufC8* key = codec->ConfigKeyL();
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1bTestText() )
    CleanupStack::PopAndDestroy( key );
    EUNIT_ASSERT( !iLiveSession->iStoreEncoderConfigInfo )
    
    CleanupStack::PopAndDestroy( codec );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_ReadCodecConfigKeyLL()
    {
    HBufC8* key( NULL );
    
    // Try with H263, leave expected
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );  
    EUNIT_ASSERT_LEAVE( key = iLiveSession->ReadCodecConfigKeyL( *codecH263 ) )
    CleanupStack::PopAndDestroy( codecH263 );
    
    // Try without an entry in CenRep, leave expected
    CMceVideoCodec* codec = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codec->SetAllowedBitrates( 0 | KMceAvcBitrateLevel1 );
    EUNIT_ASSERT_LEAVE( key = iLiveSession->ReadCodecConfigKeyL( *codec ) )

    // Create CenRep entry which does not include wanted bitrate, NULL value
    // expected
    CRepository::SetStubAvcConfigKeysL( KMusAvcBitrateLevel1bTestText() );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( !key )
    
    // Create entry with all AVC keys and try with all the values
    CRepository::SetStubAvcConfigKeysL( KMusAvcAllLevelsConcatenation() );                           
    
    // KMceAvcBitrateLevel1
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1TestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel1b
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1b );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1bTestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel1_1
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1_1 );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1_1TestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel1_2
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1_2 );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1_2TestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel1_3
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1_3 );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1_3TestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel2
    codec->SetAllowedBitrates( KMceAvcBitrateLevel2 );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel2TestText );
    CleanupStack::PopAndDestroy( key );
   
    CleanupStack::PopAndDestroy( codec );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_StoreEncoderConfigInfoLL()
    {
    CRepository::iStaticWriteAvcKeysToStaticData = ETrue;
    
    // Try with clean CenRep
    MultimediaSharingSettings::SetEncoderConfigInfoL( KNullDesC8() );   
    
    ESTABLISH_OUT_SESSION( iLiveSession ) // calls StoreEncoderConfigInfoL
    // If StoreEncoderConfigInfoL leaves with KErrNoMemory,
    // MMusEngSessionObserver::SessionFailed will be called.
    __ASSERT_ALWAYS( !iObserver->iSessionFailedCalled, 
                     User::Leave( KErrNoMemory ) );
                     
    HBufC8* keys = MultimediaSharingSettings::EncoderConfigInfoLC();    
    EUNIT_ASSERT_EQUALS( *keys, KMusAvcBitrateLevel_1b_ConfigKey() )
    CleanupStack::PopAndDestroy( keys );
    
    // Try without a clean CenRep 
    MultimediaSharingSettings::SetEncoderConfigInfoL(
                                        KMusAvcBitrateLevel_1_1_ConfigKey() );   
    
    ESTABLISH_OUT_SESSION( iRecordedLiveSession )
    // If StoreEncoderConfigInfoL leaves with KErrNoMemory,
    // MMusEngSessionObserver::SessionFailed will be called.
    __ASSERT_ALWAYS( !iObserver->iSessionFailedCalled, 
                     User::Leave( KErrNoMemory ) );
                     
    keys = MultimediaSharingSettings::EncoderConfigInfoLC();
    EUNIT_ASSERT_EQUALS( *keys, 
                         KMusAvcBitrateLevels_1_1_And_1b_ConfigKeys() )
    CleanupStack::PopAndDestroy( keys );
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_InitializeZoomStepSizeL()
    {
    //Check that zooming steps have defualt values before session establishment
    iLiveSession->InitializeZoomStepSize();
    EUNIT_ASSERT( iLiveSession->iSmallZoomStep == KZoomStepMinSize );
    EUNIT_ASSERT( iLiveSession->iBigZoomStep == KZoomStepMinSize );
    
    // simulate session establishment
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceCameraSource* camera = 
                        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
    
    //Check that zooming step sizes have default value KZoomStepMinSize 
    //in case of zooming values range is smaller than KZoomBigStepCount
    iLiveSession->iCameraInfo.iMinZoom = 0;
    iLiveSession->iCameraInfo.iMaxZoom = 0;
    iLiveSession->iCameraInfo.iMaxDigitalZoom = KZoomBigStepCount / 2;    
    iLiveSession->InitializeZoomStepSize();
    EUNIT_ASSERT( iLiveSession->iSmallZoomStep == KZoomStepMinSize );
    EUNIT_ASSERT( iLiveSession->iBigZoomStep == KZoomStepMinSize );
    
    
    //Check that zooming step sizes have default value KZoomStepMinSize 
    //in case of zooming values range is bigger than KZoomBigStepCount,
    //but smaller than 2*KZoomBigStepCount (KZoomSmallStepCount)
    iLiveSession->iCameraInfo.iMaxDigitalZoom = 2*KZoomBigStepCount - 1;
    iLiveSession->InitializeZoomStepSize();
    EUNIT_ASSERT( iLiveSession->iSmallZoomStep == KZoomStepMinSize );
    EUNIT_ASSERT( iLiveSession->iBigZoomStep == KZoomStepMinSize );

    
    //Check zooming steps sizes in case of zooming values range is
    //bigger than 2*KZoomBigStepCount and smaller than 2*KZoomSmallStepCount
    iLiveSession->iCameraInfo.iMinZoom = - KZoomBigStepCount;
    iLiveSession->iCameraInfo.iMaxZoom = KZoomBigStepCount;
    iLiveSession->iCameraInfo.iMaxDigitalZoom = KZoomBigStepCount;
    //zooming range is (-KZoomBigStepCount, 2*KZoomBigStepCount)
    iLiveSession->InitializeZoomStepSize();
    EUNIT_ASSERT( iLiveSession->iSmallZoomStep == KZoomStepMinSize );
    EUNIT_ASSERT( iLiveSession->iBigZoomStep == 3 );

    
    //Check zooming steps sizes in case of zooming values range is
    //bigger than 2*KZoomSmallStepCount
    iLiveSession->iCameraInfo.iMaxZoom = KZoomSmallStepCount;
    iLiveSession->iCameraInfo.iMaxDigitalZoom = KZoomSmallStepCount;
    iLiveSession->iCameraInfo.iMinZoom = - KZoomSmallStepCount;
    //zooming range is (-KZoomSmallStepCount, 2*KZoomSmallStepCount)
    iLiveSession->InitializeZoomStepSize();
    EUNIT_ASSERT( iLiveSession->iSmallZoomStep == 3 );
    EUNIT_ASSERT( iLiveSession->iBigZoomStep == 6 );    
    }
    

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngLiveSession,
    "UT_CMusEngLiveSesssion",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngLiveSession",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)

EUNIT_TEST(
    "CurrentZoomL - test ",
    "CMusEngLiveSession",
    "CurrentZoomL",
    "FUNCTIONALITY",
    SetupL, UT_CurrentZoomLL, Teardown)

EUNIT_TEST(
    "MaxZoomL - test ",
    "CMusEngLiveSession",
    "MaxZoomL",
    "FUNCTIONALITY",
    SetupL, UT_MaxZoomLL, Teardown)
    
EUNIT_TEST(
    "ZoomInL - test ",
    "CMusEngLiveSession",
    "ZoomInL",
    "FUNCTIONALITY",
    SetupL, UT_ZoomInLL, Teardown)

EUNIT_TEST(
    "ZoomOutL - test ",
    "CMusEngLiveSession",
    "ZoomOutL",
    "FUNCTIONALITY",
    SetupL, UT_ZoomOutLL, Teardown)

EUNIT_TEST(
    "ZoomDefaultL - test ",
    "CMusEngLiveSession",
    "ZoomDefaultL",
    "FUNCTIONALITY",
    SetupL, UT_ZoomDefaultLL, Teardown)

EUNIT_TEST(
    "SetBrightnessL - test ",
    "CMusEngLiveSession",
    "SetBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_SetBrightnessLL, Teardown)

EUNIT_TEST(
    "CurrentBrightnessL - test ",
    "CMusEngLiveSession",
    "CurrentBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_CurrentBrightnessLL, Teardown)

EUNIT_TEST(
    "MaxBrightnessL - test ",
    "CMusEngLiveSession",
    "MaxBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_MaxBrightnessLL, Teardown)

EUNIT_TEST(
    "MinBrightnessL - test ",
    "CMusEngLiveSession",
    "MinBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_MinBrightnessLL, Teardown)

EUNIT_TEST(
    "IncreaseBrightnessL - test ",
    "CMusEngLiveSession",
    "IncreaseBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_IncreaseBrightnessLL, Teardown)

EUNIT_TEST(
    "DecreaseBrightnessL - test ",
    "CMusEngLiveSession",
    "DecreaseBrightnessL",
    "FUNCTIONALITY",
    SetupL, UT_DecreaseBrightnessLL, Teardown)

EUNIT_TEST(
    "BrightnessDefaultL - test ",
    "CMusEngLiveSession",
    "BrightnessDefaultL",
    "FUNCTIONALITY",
    SetupL, UT_BrightnessDefaultLL, Teardown)

EUNIT_TEST(
    "BrightnessAutoL - test ",
    "CMusEngLiveSession",
    "BrightnessAutoL",
    "FUNCTIONALITY",
    SetupL, UT_BrightnessAutoLL, Teardown)

EUNIT_TEST(
    "RecordL_and_IsRecordingL - test ",
    "CMusEngLiveSession",
    "RecordL_and_IsRecordingL",
    "FUNCTIONALITY",
    SetupL, UT_RecordL_and_IsRecordingLL, Teardown)

EUNIT_TEST(
    "PlayL - test ",
    "CMusEngLiveSession",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMusEngLiveSession",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_PauseLL, Teardown)

EUNIT_TEST(
    "IsPlayingL - test ",
    "CMusEngLiveSession",
    "IsPlayingL",
    "FUNCTIONALITY",
    SetupL, UT_IsPlayingLL, Teardown)

EUNIT_TEST(
    "CompleteSessionStructureL - test ",
    "CMusEngLiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureLL, Teardown)

EUNIT_TEST(
    "HandleSessionStateChanged - test ",
    "CMusEngLiveSession",
    "HandleSessionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_HandleSessionStateChangedL, Teardown)

EUNIT_TEST(
    "AdjustVideoCodecL - test ",
    "CMusEngLiveSession",
    "AdjustVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustVideoCodecLL, Teardown)

EUNIT_TEST(
    "AdjustAudioCodecL - test ",
    "CMusEngLiveSession",
    "AdjustAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustAudioCodecLL, Teardown)

EUNIT_TEST(
    "DoCodecSelectionL- test ",
    "CMusEngLiveSession",
    "DoCodecSelectionL",
    "FUNCTIONALITY",
    SetupL, UT_DoCodecSelectionLL, Teardown)

EUNIT_TEST(
    "StreamStateChangedL() - test ",
    "CMusEngLiveSession",
    "StreamStateChangedL()",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedL, Teardown)

EUNIT_TEST(
    "StreamStateChangedL( src ) - test ",
    "CMusEngLiveSession",
    "StreamStateChangedL( src )",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedWithSourceL, Teardown)        

EUNIT_TEST(
    "StreamStateChangedL( sink ) - test ",
    "CMusEngLiveSession",
    "StreamStateChangedL( sink )",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedWithSinkL, Teardown)

EUNIT_TEST(
    "SetEncodingDeviceL - test ",
    "CMusEngLiveSession",
    "SetEncodingDeviceL",
    "FUNCTIONALITY",
    SetupL, UT_SetEncodingDeviceLL, Teardown)   

EUNIT_TEST(
    "SetCodecConfigKeyL - test ",
    "CMusEngLiveSession",
    "SetCodecConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_SetCodecConfigKeyLL, Teardown)    

EUNIT_TEST(
    "ReadCodecConfigKeyL - test ",
    "CMusEngLiveSession",
    "ReadCodecConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_ReadCodecConfigKeyLL, Teardown)        

EUNIT_TEST(
    "StoreEncoderConfigInfoL - test ",
    "CMusEngLiveSession",
    "StoreEncoderConfigInfoL",
    "FUNCTIONALITY",
    SetupL, UT_StoreEncoderConfigInfoLL, Teardown)      
    
EUNIT_TEST(
    "InitializeZoomStepSize - test ",
    "CMusEngLiveSession",
    "InitializeZoomStepSize",
    "FUNCTIONALITY",
    SetupL, UT_InitializeZoomStepSizeL, Teardown)      

      
EUNIT_END_TEST_TABLE

//  END OF FILE


