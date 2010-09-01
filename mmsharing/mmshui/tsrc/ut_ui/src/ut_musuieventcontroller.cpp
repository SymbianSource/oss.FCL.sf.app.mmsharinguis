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
#include "ut_musuieventcontroller.h"
#include "musuieventcontroller.h"
#include "musuilivesharingcontroller.h"

#include "musuilivesharingobserveradapter.h"
#include "musuieventobserveradapter.h"

#include "musresourceproperties.h"
#include "mussessionproperties.h"
#include "musuidefinitions.h"
#include "musenglivesession.h"
#include "mussettings.h"


#include <e32property.h>
#include <fbs.h>
#include <stringloader.h>

//#include "musestubs.h"
//#include "musengtestdefs.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
//#include <centralrepository.h>
//#include <mcedisplaysink.h>

using namespace NMusSessionApi;




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiEventController* UT_CMusUiEventController::NewL()
    {
    UT_CMusUiEventController* self = UT_CMusUiEventController::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiEventController* UT_CMusUiEventController::NewLC()
    {
    UT_CMusUiEventController* self = new( ELeave ) UT_CMusUiEventController();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiEventController::~UT_CMusUiEventController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusUiEventController::UT_CMusUiEventController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusUiEventController::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiEventController::SetupL()
    {
    _LIT( KPath, "\\private\\1000af5a\\" );
    iVideoFolder = KPath().AllocL();
    
    //MultimediaSharingSettings::iVideoFolder =  iVideoFolder;
    MultimediaSharingSettings::iAutoRecord = MusSettingsKeys::EAutoRecordOff;
    MultimediaSharingSettings::iVideoLocation = 2;

    _LIT( KReturn, "savedfilename" );
    _iReturnValue = KReturn().AllocL();
    StringLoader::iReturnValue = _iReturnValue;
    
    
    iEventObserver = new (ELeave) CMusUiEventObserverAdapter();
    EUNIT_ASSERT( iEventObserver );

    iLiveSharingObserverAdapter = new (ELeave) CMusUiLiveSharingObserverAdapter();
    EUNIT_ASSERT( iLiveSharingObserverAdapter );


    RProperty::iIntProperty = 0;
    RProperty::iErrorCode = KErrNone;


    iLiveSharingController = CMusUiLiveSharingController::NewL( iLiveSharingObserverAdapter, TRect( 0,0,100,100));
    iEventController = static_cast<CMusUiEventController*>
                        ( iLiveSharingController );

    EUNIT_ASSERT( iEventController );
    iEventController->SetEventObserver( iEventObserver );
    EUNIT_ASSERT( iEventController->iObserver == iEventObserver );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiEventController::Teardown()
    {
    delete iLiveSharingController;
    iEventController = NULL;
    delete iEventObserver;
    delete iLiveSharingObserverAdapter;
    
    delete iVideoFolder;
    iVideoFolder = NULL;
    //MultimediaSharingSettings::iVideoFolder =  NULL;
    
    delete _iReturnValue ;
    _iReturnValue  = NULL;
   // StringLoader::iReturnValue = NULL;
    //StringLoader::Cleanup();
    //MultimediaSharingSettings::Cleanup();
    
    }


// TEST CASES

void UT_CMusUiEventController::UT_CMusUiEventController_ResourcePropertyChangedL()
    {
    //  test for setting status
    
    // Camera availability    
    iEventController->iCameraAvailibilityInProgress = EFalse;
    iEventController->iCameraAvailable = TAvailability(1);
    iEventController->ResourcePropertyChanged( KCameraAvailability, 0 );
    EUNIT_ASSERT( iEventController->iCameraAvailable == 0 );
    EUNIT_ASSERT( iEventController->iCameraAvailibilityInProgress == EFalse );

    _LIT( KRes, "address" ); // Needed for fake, MUS_LOG leaves if null
    iLiveSharingController->iAddress = KRes().AllocL();
    iEventController->iCameraAvailibilityInProgress = ETrue;
    iEventController->iCameraAvailable = TAvailability(0);
    iEventController->ResourcePropertyChanged( KCameraAvailability, 1 );
    EUNIT_ASSERT( iEventController->iCameraAvailable == 1 );
    EUNIT_ASSERT( iEventController->iCameraAvailibilityInProgress == EFalse );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EShowWaitDialogL );
    

    // Keypad availablity
    iEventController->iKeypadAvailable = TAvailability(0);
    iEventController->ResourcePropertyChanged( KKeypadAvailability, 2 );
    EUNIT_ASSERT( iEventController->iKeypadAvailable == 2 );

    // Videoplayer availability
    iEventController->iVideoPlayerAvailable = TAvailability(0);
    iEventController->ResourcePropertyChanged( KVideoPlayerAvailability, 3 );
    EUNIT_ASSERT( iEventController->iVideoPlayerAvailable == 3 );

    //Other case
    iEventController->ResourcePropertyChanged( -99, 0 );
    EUNIT_ASSERT( iEventController->iCameraAvailable == 1 );
    EUNIT_ASSERT( iEventController->iKeypadAvailable == 2 );
    EUNIT_ASSERT( iEventController->iVideoPlayerAvailable == 3 );

    }


void UT_CMusUiEventController::UT_CMusUiEventController_SessionPropertyChangedL()
    {
    
    iEventController->SessionPropertyChanged( KStatus, ESipRegistrationPending );
    EUNIT_ASSERT( iEventController->iSipRegistrationPending );

    // TODO: Cannot test UpdateContactL it leaves CleanupStack::PopAndDestroy function
    // Need to do changes on code
    iEventController->SessionPropertyChanged( KContactId, 0 );
    
    }


void UT_CMusUiEventController::UT_CMusUiEventController_SessionPropertyChanged2L()
    {
    // TODO: Is this functions alive? 
    // EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_HandleErrorL()
    {
    
    iEventController->HandleError( KErrUnknown );
    EUNIT_ASSERT( iEventObserver->iCalledObserver == CMusUiEventObserverAdapter::EHandleError );
    EUNIT_ASSERT( iEventObserver->iHandleErrorReason == KErrUnknown );
    
    }

void UT_CMusUiEventController::UT_CMusUiEventController_UpdateBitmapL()
    {
    delete iEventController->iContactBitmap;
    iEventController->iContactBitmap = NULL;
    CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
    iEventController->UpdateBitmap( bitmap );
    EUNIT_ASSERT( iEventController->iContactBitmap == bitmap );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_SetEventObserverL()
    {
    // Tested in SetupL();
    EUNIT_ASSERT( ETrue );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_HandleForegroundEventLL()
    {
    // Initalization
    iLiveSharingController->iConnectionEstablished = ETrue; // Need for testing
    iLiveSharingObserverAdapter->iErrorCode = KErrNone;
    
    // Enable display
    iEventController->HandleForegroundEventL( ETrue );
    EUNIT_ASSERT( iLiveSharingController->iSession->iDisplayEnabled );
    EUNIT_ASSERT( iLiveSharingController->iSession->iPlaying );
    EUNIT_ASSERT( iEventController->iPlaying );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EShowNaviPaneIconL );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iIcon == EMusUiNaviIconPlay );

    // Disable display
    iEventController->HandleForegroundEventL( EFalse );
    EUNIT_ASSERT( !iLiveSharingController->iSession->iDisplayEnabled );
    EUNIT_ASSERT( !iLiveSharingController->iSession->iPlaying );
    EUNIT_ASSERT( !iEventController->iPlaying );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EShowNaviPaneIconL );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iIcon == EMusUiNaviIconPause );

    }


void UT_CMusUiEventController::UT_CMusUiEventController_SetCameraStatusL()
    {
    iEventController->iCameraAvailable = TAvailability( 0 );
    iEventController->SetCameraStatus( TAvailability( 2 ) );
    EUNIT_ASSERT( iEventController->iCameraAvailable == 2 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_SetKeypadStatusL()
    {
    iEventController->iKeypadAvailable = TAvailability(0);
    iEventController->SetKeypadStatus( TAvailability( 2  ) );
    EUNIT_ASSERT( iEventController->iKeypadAvailable == 2 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_SetVideoPlayerStatusL()
    {
    iEventController->iVideoPlayerAvailable = TAvailability(0);
    iEventController->SetVideoPlayerStatus( TAvailability( 3  ) );
    EUNIT_ASSERT( iEventController->iVideoPlayerAvailable == 3 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_SetCameraAvailabilityInProgressL()
    {
    iEventController->iCameraAvailibilityInProgress = 0;
    iEventController->SetCameraAvailabilityInProgress( 1 );
    EUNIT_ASSERT( iEventController->iCameraAvailibilityInProgress == 1 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_CameraAvailableL()
    {
    iEventController->iCameraAvailable = TAvailability( 2 );
    EUNIT_ASSERT( iEventController->CameraAvailable() == 2 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_KeypadAvailableL()
    {
    iEventController->iKeypadAvailable = TAvailability( 2 );
    EUNIT_ASSERT( iEventController->KeypadAvailable() == 2 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_VideoPlayerAvailableL()
    {
    iEventController->iVideoPlayerAvailable = TAvailability( 2 );
    EUNIT_ASSERT( iEventController->VideoPlayerAvailable() == 2 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_CameraAvailibilityInProgressL()
    {
    iEventController->iCameraAvailibilityInProgress = 1;
    EUNIT_ASSERT( iEventController->CameraAvailibilityInProgress() == 1 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_MusRemoteSipAddressValueL()
    {
    _LIT( KRes, "remote_address");
    delete iEventController->iRemoteSipAddress;
    iEventController->iRemoteSipAddress = KRes().AllocL();
    EUNIT_ASSERT(  iEventController->MusRemoteSipAddressValue().Compare( KRes ) == 0 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_MusProposalAddressValueL()
    {
    _LIT( KRes, "remote_address");
    delete iEventController->iRemoteSipAddressProposal;
    iEventController->iRemoteSipAddressProposal = KRes().AllocL();
    EUNIT_ASSERT(  iEventController->MusProposalAddressValue().Compare( KRes ) == 0 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_MusContactNameL()
    {
    _LIT( KRes, "remote_address");
    delete iEventController->iContactName;
    iEventController->iContactName = KRes().AllocL();
    EUNIT_ASSERT(  iEventController->MusContactName().Compare( KRes ) == 0 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_MusContactBitmapL()
    {
    delete iEventController->iContactBitmap;
    iEventController->iContactBitmap = new (ELeave) CFbsBitmap();
    EUNIT_ASSERT( iEventController->MusContactBitmap() == iEventController->iContactBitmap );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_MusVideoCodecValueL()
    {
    _LIT( KRes, "codec_value");
    delete iEventController->iVideoCodec;
    iEventController->iVideoCodec = KRes().AllocL();
    EUNIT_ASSERT(  iEventController->MusVideoCodecValue().Compare( KRes ) == 0 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_MusTelNumberValueL()
    {
    _LIT( KRes, "telnumber");
    delete iEventController->iTelNumber;
    iEventController->iTelNumber = KRes().AllocL();
    EUNIT_ASSERT(  iEventController->MusTelNumberValue().Compare( KRes ) == 0 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_SipProfileIdL()
    {
    iEventController->iSipProfileId = 99;
    EUNIT_ASSERT(  iEventController->SipProfileId() == 99 );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_SipRegistrationPendingL()
    {
    
    iEventController->iSipRegistrationPending = ETrue;
    EUNIT_ASSERT(  iEventController->SipRegistrationPending() );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_SessionTimeFormattedL()
    {
    
    // Localized time format like "00:00:00"
    
    //TLocale locale;
    //locale.TimeSeparator( 2 )
    
    //EUNIT_ASSERT(  iEventController->MusTelNumberValue().Compare( iEventController->iTelNumber->Des() ) );    
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_SaveContactL()
    {
    
    //iEventController->iContactId = TContactItemId( 1 );
    
    //iEventController->SaveContact();
    //EUNIT_ASSERT( iEventObserver->iActivatedContactId == TContactItemId( 1 ) );
    
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_HandleExitLL()
    {
    
    
    
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_InspectKeypadLL()
    {
    
    // Initalize 
    iLiveSharingObserverAdapter->iErrorCode = KErrNone;
    iEventObserver->iErrorCode = KErrNone;

    
    // Basic case cannot test, nothing changes
    // iEventController->iKeypadAvailable = EAvailable;    
    // iEventController->InspectKeypadL();
    
    
    // case EAvailableInLandscapeOrientation
    iEventController->iKeypadAvailable = EAvailableInLandscapeOrientation;    
    iEventController->InspectKeypadL();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EChangeOrientationL );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iOrientation == CAknAppUiBase::EAppUiOrientationLandscape );
    
    
    // case EAvailableInPortraitOrientation
    iEventController->iKeypadAvailable = EAvailableInPortraitOrientation;    
    iEventController->InspectKeypadL();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EChangeOrientationL );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iOrientation == CAknAppUiBase::EAppUiOrientationPortrait );

    // case ENotDefined
    iEventController->iKeypadAvailable = ENotDefined;    
    iEventController->InspectKeypadL();
    EUNIT_ASSERT( iEventObserver->iCalledObserver == CMusUiEventObserverAdapter::EHandleExitL );

    // case ENotAvailable
    iEventController->iKeypadAvailable = ENotAvailable;    
    iEventController->InspectKeypadL();
    EUNIT_ASSERT( iEventObserver->iCalledObserver == CMusUiEventObserverAdapter::EHandleExitL );

   
    }


void UT_CMusUiEventController::UT_CMusUiEventController_UpdateContactLL()
    {
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_UpdateContactNameL()
    {
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_UpdateContactPictureLL()
    {
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiEventController::UT_CMusUiEventController_HandleChangedStatusLL()
    {
    EUNIT_ASSERT( EFalse );
    }



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusUiEventController,
    "UT_CMusUiEventController",
    "UNIT" )

EUNIT_TEST(
    "ResourcePropertyChangedL - test ",
    "CMusUiEventController",
    "ResourcePropertyChangedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_ResourcePropertyChangedL, Teardown)

EUNIT_TEST(
    "SessionPropertyChangedL - test ",
    "CMusUiEventController",
    "SessionPropertyChangedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SessionPropertyChangedL, Teardown)

EUNIT_TEST(
    "HandleError - test ",
    "CMusUiEventController",
    "HandleError",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_HandleErrorL, Teardown)

EUNIT_TEST(
    "UpdateBitmap - test ",
    "CMusUiEventController",
    "UpdateBitmap",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_UpdateBitmapL, Teardown)

EUNIT_TEST(
    "SetEventObserver - test ",
    "CMusUiEventController",
    "SetEventObserver",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SetEventObserverL, Teardown)

EUNIT_TEST(
    "HandleForegroundEventL - test ",
    "CMusUiEventController",
    "HandleForegroundEventL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_HandleForegroundEventLL, Teardown)

EUNIT_TEST(
    "SetCameraStatus - test ",
    "CMusUiEventController",
    "SetCameraStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SetCameraStatusL, Teardown)

EUNIT_TEST(
    "SetKeypadStatus - test ",
    "CMusUiEventController",
    "SetKeypadStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SetKeypadStatusL, Teardown)

EUNIT_TEST(
    "SetVideoPlayerStatus - test ",
    "CMusUiEventController",
    "SetVideoPlayerStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SetVideoPlayerStatusL, Teardown)

EUNIT_TEST(
    "SetCameraAvailabilityInProgress - test ",
    "CMusUiEventController",
    "SetCameraAvailabilityInProgress",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SetCameraAvailabilityInProgressL, Teardown)

EUNIT_TEST(
    "EnableLoudspeaker - test ",
    "CMusUiEventController",
    "EnableLoudspeaker",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SetCameraStatusL, Teardown)

EUNIT_TEST(
    "CameraAvailable - test ",
    "CMusUiEventController",
    "CameraAvailable",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_CameraAvailableL, Teardown)

EUNIT_TEST(
    "KeypadAvailable - test ",
    "CMusUiEventController",
    "KeypadAvailable",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_KeypadAvailableL, Teardown)

EUNIT_TEST(
    "VideoPlayerAvailable - test ",
    "CMusUiEventController",
    "VideoPlayerAvailable",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_VideoPlayerAvailableL, Teardown)

EUNIT_TEST(
    "CameraAvailibilityInProgress - test ",
    "CMusUiEventController",
    "CameraAvailibilityInProgress",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_CameraAvailibilityInProgressL, Teardown)

EUNIT_TEST(
    "MusRemoteSipAddressValue - test ",
    "CMusUiEventController",
    "MusRemoteSipAddressValue",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_MusRemoteSipAddressValueL, Teardown)

EUNIT_TEST(
    "MusProposalAddressValue - test ",
    "CMusUiEventController",
    "MusProposalAddressValue",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_MusProposalAddressValueL, Teardown)

EUNIT_TEST(
    "MusContactName - test ",
    "CMusUiEventController",
    "MusContactName",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_MusContactNameL, Teardown)

EUNIT_TEST(
    "EnableLoudspeakerL - test ",
    "CMusUiEventController",
    "EnableLoudspeakerL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SetCameraStatusL, Teardown)

EUNIT_TEST(
    "MusContactBitmap - test ",
    "CMusUiEventController",
    "MusContactBitmap",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_MusContactBitmapL, Teardown)

EUNIT_TEST(
    "MusVideoCodecValue - test ",
    "CMusUiEventController",
    "MusVideoCodecValue",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_MusVideoCodecValueL, Teardown)

EUNIT_TEST(
    "MusTelNumberValue - test ",
    "CMusUiEventController",
    "MusTelNumberValue",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_MusTelNumberValueL, Teardown)

EUNIT_TEST(
    "SipProfileId - test ",
    "CMusUiEventController",
    "SipProfileId",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SipProfileIdL, Teardown)

EUNIT_TEST(
    "SipRegistrationPending - test ",
    "CMusUiEventController",
    "SipRegistrationPending",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SipRegistrationPendingL, Teardown)

EUNIT_TEST(
    "SessionTimeFormatted - test ",
    "CMusUiEventController",
    "SessionTimeFormatted",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SessionTimeFormattedL, Teardown)

EUNIT_TEST(
    "EnableLoudspeakerL - test ",
    "CMusUiEventController",
    "EnableLoudspeakerL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SetCameraStatusL, Teardown)

EUNIT_TEST(
    "SaveContact - test ",
    "CMusUiEventController",
    "SaveContact",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SaveContactL, Teardown)

EUNIT_TEST(
    "SetCameraStatus - test ",
    "CMusUiEventController",
    "SetCameraStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_SetCameraStatusL, Teardown)

EUNIT_TEST(
    "HandleExitL - test ",
    "CMusUiEventController",
    "HandleExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_HandleExitLL, Teardown)


EUNIT_TEST(
    "InspectKeypadL - test ",
    "CMusUiEventController",
    "InspectKeypadL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_InspectKeypadLL, Teardown)

EUNIT_TEST(
    "UpdateContactL - test ",
    "CMusUiEventController",
    "UpdateContactL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_UpdateContactLL, Teardown)

EUNIT_TEST(
    "UpdateContactName - test ",
    "CMusUiEventController",
    "UpdateContactName",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_UpdateContactNameL, Teardown)

EUNIT_TEST(
    "UpdateContactPictureL - test ",
    "CMusUiEventController",
    "UpdateContactPictureL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_UpdateContactPictureLL, Teardown)

EUNIT_TEST(
    "HandleChangedStatusL - test ",
    "CMusUiEventController",
    "HandleChangedStatusL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiEventController_HandleChangedStatusLL, Teardown)

EUNIT_END_TEST_TABLE


//  END OF FILE
