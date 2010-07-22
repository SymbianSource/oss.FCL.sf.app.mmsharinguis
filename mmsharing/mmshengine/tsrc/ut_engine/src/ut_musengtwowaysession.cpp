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
#include "ut_musengtwowaysession.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
#include "musengtwowaysession.h"
#include "mussipprofilehandler.h"
#include "musengmceutils.h"
#include "mussettings.h"
#include "mussessionproperties.h"
#include "musengremotevideoplayer.h"

//  SYSTEM INCLUDES
#include <eunitmacros.h>
#include <mceoutsession.h>
#include <mcestreambundle.h>
#include <mcevideostream.h>
#include <mcecamerasource.h>
#include <mcertpsink.h>
#include <mcefilesink.h>
#include <mceh263codec.h>
#include <mceavccodec.h>
#include <mceaudiocodec.h>
#include <mcedisplaysink.h>
#include <mcertpsource.h>


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


// NOTE!!!!!!!!!!
// There are intentianally many same cases as already in CMusEngLiveSession 
// as two-way session derives from livesession. These cases therefore
// ensure that live session changes are not breaking two-way session.


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngTwoWaySession* UT_CMusEngTwoWaySession::NewL()
    {
    UT_CMusEngTwoWaySession* self = UT_CMusEngTwoWaySession::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngTwoWaySession* UT_CMusEngTwoWaySession::NewLC()
    {
    UT_CMusEngTwoWaySession* self = new( ELeave ) UT_CMusEngTwoWaySession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngTwoWaySession::~UT_CMusEngTwoWaySession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngTwoWaySession::UT_CMusEngTwoWaySession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::SetupL()
    {
    iLcSessionObserver = new( ELeave )CLcSessionObserverStub;
    iLcUiProvider = new( ELeave )CLcUiProviderStub;    
    iLiveSession = CMusEngTwoWaySession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    

    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        KTestRecipientSipUri ) );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::Teardown()
    {
    delete iLiveSession;
    delete iLcSessionObserver;
    delete iLcUiProvider;
    PropertyHelper::Close();
    // Delete static data from CenRep stub
    CRepository::iStaticWriteAvcKeysToStaticData = EFalse;
    CRepository::DeleteStubAvcConfigKeys();
    CRepository::ResetStubGlobal();
    CRepository::iForceFailWithCode = KErrNone;
    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_NewLL()
    {
    EUNIT_ASSERT( iLiveSession )
    EUNIT_ASSERT( iLiveSession->iCameraHandler.iCameraUsage == MusSettingsKeys::EUseCameraSwapping );     
    EUNIT_ASSERT( iLiveSession->iMceManagerUid == TUid::Uid( KMusUiUid ) );

    delete iLiveSession;
    iLiveSession = NULL;
    CRepository::SetStubGlobal( MusSettingsKeys::KCameraUsage,
                                MusSettingsKeys::EUseOnlySecondaryCamera );

    iLiveSession = CMusEngTwoWaySession::NewL();   
    
    EUNIT_ASSERT( iLiveSession->iCameraHandler.iCameraUsage == MusSettingsKeys::EUseOnlySecondaryCamera );     
    
    delete iLiveSession;
    iLiveSession = NULL;
    CRepository::SetStubGlobal( MusSettingsKeys::KCameraUsage,
                                MusSettingsKeys::EUseOnlyMainCamera );

    iLiveSession = CMusEngTwoWaySession::NewL();  
    
    EUNIT_ASSERT_EQUALS( TInt( MusSettingsKeys::EUseOnlyMainCamera ),
                         TInt( iLiveSession->iCameraHandler.iCameraUsage ) )      
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_EstablishLcSessionLL()
    {
    CRepository::SetStubGlobal( MusSettingsKeys::KVideoBandwidth,
                                128 );
    iLiveSession->EstablishLcSessionL();
    
    EUNIT_ASSERT( iLiveSession->iSession );
    // Next assertion ensures that session structure is new
    EUNIT_ASSERT( iLiveSession->iSession->Streams().Count() > 0 );   
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0] ); 
    EUNIT_ASSERT( *(iLiveSession->iRecipient) == KTestRecipientSipUri8() );
    EUNIT_ASSERT( iLiveSession->iSession->State() == CMceSession::EOffering );
    
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders );
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->Count() == 2 );
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 0 ) ==
                  KMusEngAcceptContactHeader() );
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 1 ) ==
                  KMusAcceptHeader() );
    
    // Ensure there is no SDP lines at session level
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines );
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->Count() == 1 );
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->MdcaPoint( 0 ) ==
                  KMusEngSessionSdpLineXApplication() );
        
    // Ensure b=AS and b=TIAS attributes are taken at media level   
    MDesC8Array* mediaSdpLines = 
        iLiveSession->iSession->Streams()[0]->iMediaSDPLines;

    EUNIT_ASSERT( mediaSdpLines );
    EUNIT_ASSERT( mediaSdpLines->MdcaCount() == 2 );
    EUNIT_ASSERT( mediaSdpLines->MdcaPoint( 0 ).Find( 
                  KMusEngSessionSdpLineBandwidthLine() ) == 0 );
    EUNIT_ASSERT( mediaSdpLines->MdcaPoint( 1 ).Find( 
                  KMusEngSessionSdpLineTiasLine() ) == 0 ); 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_EnableDisplayLL()
    {
    TRAPD( error, iLiveSession->EnableDisplayL( ETrue ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceDisplaySink* display = 
                    MusEngMceUtils::GetDisplayL( *(iLiveSession->iSession) );
    
    CMceDisplaySink* receivingDisplay = 
                    MusEngMceUtils::GetReceivingDisplayL( *iLiveSession->iSession );
    
    // Disable
    iLiveSession->EnableDisplayL( EFalse );
    
    EUNIT_ASSERT( !display->iIsEnabled );
    EUNIT_ASSERT( !receivingDisplay->iIsEnabled );
    
    // Try to disable again, request should be ignored
    
    iLiveSession->EnableDisplayL( EFalse );
    
    EUNIT_ASSERT( !display->iIsEnabled );
    EUNIT_ASSERT( !receivingDisplay->iIsEnabled );

    // Enable
    
    iLiveSession->EnableDisplayL( ETrue );
    
    EUNIT_ASSERT( display->iIsEnabled );
    EUNIT_ASSERT( receivingDisplay->iIsEnabled );
    
    // Try to enable again, request should be ignored
    
    iLiveSession->EnableDisplayL( ETrue );
    
    EUNIT_ASSERT( display->iIsEnabled );
    EUNIT_ASSERT( receivingDisplay->iIsEnabled );
    
    // Display is active only if receiving and display is explicitly enabled
    EUNIT_ASSERT( !iLiveSession->IsDisplayActive() )
    iLiveSession->iReceiving = ETrue;
    EUNIT_ASSERT( iLiveSession->IsDisplayActive() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_SetOrientationLL()
    {
    // Try without session, fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->SetOrientationL( 
                                               CMusEngMceSession::EPortrait ), 
                                KErrNotReady )
    
    // Establish a session
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    CMceDisplaySink* display = 
                   MusEngMceUtils::GetDisplayL( *iLiveSession->iSession );
    
    CMceDisplaySink* receivingDisplay = 
                   MusEngMceUtils::GetReceivingDisplayL( *iLiveSession->iSession );
       
    
    // Check the initial state
    display->iRotation = CMceDisplaySink::ENone;
    receivingDisplay->iRotation = CMceDisplaySink::ENone;
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         MLcWindow::EPortrait )      
                        
    // Successfully change orientation
    
    iLiveSession->SetOrientationL( CMusEngMceSession::ELandscape );
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         MLcWindow::ELandscape )
    
    EUNIT_ASSERT_EQUALS( display->iRotation, CMceDisplaySink::EClockwise90Degree );
    EUNIT_ASSERT_EQUALS( receivingDisplay->iRotation, CMceDisplaySink::EClockwise90Degree );
    
    iLiveSession->SetOrientationL( CMusEngMceSession::EPortrait );
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         MLcWindow::EPortrait )          
    
    EUNIT_ASSERT_EQUALS( display->iRotation, CMceDisplaySink::ENone );
    EUNIT_ASSERT_EQUALS( receivingDisplay->iRotation, CMceDisplaySink::ENone );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_SetLocalRectLL()
    {
    // Does nothing real before session establishment
    TRect rect1( 100, 200, 300, 500 );
    iLiveSession->SetSecondaryRectL( rect1 ); 
    
    EUNIT_ASSERT( rect1 == iLiveSession->iLocalRect );
    
    // Local rect changing affects to viewfinder display sink not to receiving video
    //
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceDisplaySink* vfDisplay = 
                   MusEngMceUtils::GetDisplayL( *( iLiveSession->iSession ) );
    CMceDisplaySink* receivingDisplay = 
                   MusEngMceUtils::GetReceivingDisplayL( *( iLiveSession->iSession ) );

    EUNIT_ASSERT( vfDisplay->DisplayRectL() == iLiveSession->iLocalRect );
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() != iLiveSession->iLocalRect );
        
    TRect rect2( 200, 200, 300, 500 );
    iLiveSession->SetSecondaryRectL( rect2 ); 
    EUNIT_ASSERT( rect2 == iLiveSession->iLocalRect );
    EUNIT_ASSERT( vfDisplay->DisplayRectL() == iLiveSession->iLocalRect );
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() != iLiveSession->iLocalRect );
    
    // Terminate session and try again, rect must not be changed
    
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    
    TRect rect3( 300, 300, 400, 600 );
    
    iLiveSession->SetSecondaryRectL( rect3 ); 
    
    EUNIT_ASSERT( vfDisplay->DisplayRectL() != rect3 )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_SetRectsLL()
    {
    // Does nothing real before session establishment
    TRect localrect1( 100, 200, 300, 400 );
    TRect remoterect1( 100, 200, 300, 500 );
    iLiveSession->SetRectsL( remoterect1, localrect1 ); 
    
    EUNIT_ASSERT( localrect1 == iLiveSession->iLocalRect );
    EUNIT_ASSERT( remoterect1 == iLiveSession->iRect );
    
    // Local rect changing affects to viewfinder display sink and remote rect to
    // receiving video display sink
    //
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceDisplaySink* vfDisplay = 
                   MusEngMceUtils::GetDisplayL( *( iLiveSession->iSession ) );
    CMceDisplaySink* receivingDisplay = 
                   MusEngMceUtils::GetReceivingDisplayL( *( iLiveSession->iSession ) );

    TRect localrect2( 200, 200, 300, 400 );
    TRect remoterect2( 200, 200, 300, 500 );
    EUNIT_ASSERT( vfDisplay->DisplayRectL() != localrect2 );
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() != remoterect2 );

    iLiveSession->SetRectsL( remoterect2, localrect2 ); 
    EUNIT_ASSERT( localrect2 == iLiveSession->iLocalRect );
    EUNIT_ASSERT( remoterect2 == iLiveSession->iRect );
    EUNIT_ASSERT( vfDisplay->DisplayRectL() == iLiveSession->iLocalRect );
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() == iLiveSession->iRect );
    
    // Terminate session and try again, rect must not be changed
    
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    
    TRect localrect3( 300, 200, 300, 400 );
    TRect remoterect3( 300, 200, 300, 500 );
    
    iLiveSession->SetRectsL( remoterect3, localrect3 );
    
    EUNIT_ASSERT( vfDisplay->DisplayRectL() != localrect3 )
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() != remoterect3 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_EnableLcFileL()
    {
    // Recording is not supported for two-way session
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->EnableLcFileL( ETrue ),
                                 KErrNotSupported )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_CompleteSessionStructureLL()
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
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Sinks().Count() == 1 );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Sinks()[0]->Type() ==
                  KMceRTPSink );

    EUNIT_ASSERT( iLiveSession->iCameraHandler.iCurrentCamera ==
                        TMusEngCameraHandler::EBackCamera );
    
    CMceVideoStream* vidStream = static_cast<CMceVideoStream*>( iLiveSession->iSession->Streams()[0] );
    EUNIT_ASSERT( vidStream->BoundStream() );
    CMceVideoStream& inStream = static_cast<CMceVideoStream&>( vidStream->BoundStreamL() );
    EUNIT_ASSERT( inStream.Source() );
    EUNIT_ASSERT( inStream.Source()->Type() == KMceRTPSource );
    EUNIT_ASSERT( inStream.Sinks().Count() == 1 );
    EUNIT_ASSERT( inStream.Sinks()[0]->Type() == KMceDisplaySink );

    delete iLiveSession->iSession; 
    iLiveSession->iSession = NULL; 
    
    iLiveSession->iSession = CMceOutSession::NewL( 
                                    *(iLiveSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() );
    
    iLiveSession->iCameraHandler.iCameraUsage = MusSettingsKeys::EUseOnlySecondaryCamera;     
    iLiveSession->CompleteSessionStructureL( *localBundle );
    
    EUNIT_ASSERT( iLiveSession->iCameraHandler.iCurrentCamera ==
                        TMusEngCameraHandler::EFrontCamera );
    
    
    CleanupStack::PopAndDestroy( localBundle );                                
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_HandleSessionStateChangedL()
    {
    // Make Repository empty, config keys must be written
    CRepository::SetStubAvcConfigKeysL( KNullDesC8() );
    
    // Invite    
    iLiveSession->EstablishLcSessionL();
    iLiveSession->iSession->iState = CMceSession::EEstablished;  
    CRepository::iForceFailWithCode = KErrNone;
    
    // Simulate session state transition notification
    iLiveSession->HandleSessionStateChanged( 
                        *iLiveSession->iSession,
                        200, KNullDesC8() );
    
    HBufC8* info = MultimediaSharingSettings::EncoderConfigInfoLC();
    EUNIT_ASSERT_NOT_EQUALS( *info, KMusAvcBitrateLevel_1b_ConfigKey() )
    CleanupStack::PopAndDestroy( info );
    
    // sendrecv->recvonly downgrade terminates session
    iLcSessionObserver->Reset();
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    CMceVideoStream* inStream = 
        MusEngMceUtils::GetVideoInStreamL( *iLiveSession->iSession );    
    inStream->EnableL();
    CMceVideoStream* outStream = 
        MusEngMceUtils::GetVideoOutStreamL( *iLiveSession->iSession );
    outStream->DisableL();
    iLiveSession->HandleSessionStateChanged( 
        *iLiveSession->iSession, 200, KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    }   

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_AdjustVideoCodecLL()
    {
    CRepository::SetStubAvcConfigKeysL( KNullDesC8() );
    
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );    
    CMceVideoCodec* codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    CMceVideoCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    CMceVideoCodec* codecUnknown = CMceH263Codec::NewLC( KNullDesC8() );
        
    iLiveSession->AdjustVideoCodecL( *codecH263, KMceCameraSource );
    iLiveSession->AdjustVideoCodecL( *codecH2632000, KMceCameraSource );    
    iLiveSession->AdjustVideoCodecL( *codecAvc, KMceCameraSource );  
    iLiveSession->AdjustVideoCodecL( *codecUnknown, KMceCameraSource );

    // Test that bit rates and keepalive has been set to H263 & AVC codecs
    EUNIT_ASSERT( codecH263->iAllowedBitrates == KMceAllowedH263BitrateAll )
    EUNIT_ASSERT( codecH263->iMaxBitRate == KMceH263Level45Bitrate )
    EUNIT_ASSERT( codecH263->iBitrate == 80000 )
    EUNIT_ASSERT_EQUALS( codecH263->KeepAliveTimer(), 6 )
    EUNIT_ASSERT_EQUALS( codecH263->KeepAlivePayloadType(), 96 )
    EUNIT_ASSERT_EQUALS( codecH263->KeepAliveData(), KNullDesC8() )
    
    EUNIT_ASSERT( codecH2632000->iAllowedBitrates == 
                  KMceAllowedH263BitrateAll )
    EUNIT_ASSERT( codecH2632000->iMaxBitRate == KMceH263Level45Bitrate )
    EUNIT_ASSERT( codecH2632000->iBitrate == 80000 )
    EUNIT_ASSERT_EQUALS( codecH2632000->KeepAliveTimer(), 6 )
    EUNIT_ASSERT_EQUALS( codecH2632000->KeepAlivePayloadType(), 96 )
    EUNIT_ASSERT_EQUALS( codecH2632000->KeepAliveData(), KNullDesC8() )
    
    EUNIT_ASSERT( codecAvc->iAllowedBitrates == 
                                        KMceAvcCodecProfileIdBaseline | 
                                        KMceAvcCodecProfileIopConstraintSet | 
                                        KMceAvcBitrateLevel1b )
    EUNIT_ASSERT_EQUALS( codecAvc->KeepAliveTimer(), 6 )
    EUNIT_ASSERT_EQUALS( codecAvc->KeepAlivePayloadType(), 98 )
    EUNIT_ASSERT_EQUALS( codecAvc->KeepAliveData(), KNullDesC8() )
    EUNIT_ASSERT( iLiveSession->iStoreEncoderConfigInfo )


    // Test that bitrates and keepalive has not been set to unknown codecs
    EUNIT_ASSERT( codecUnknown->iAllowedBitrates != 
                  KMceAllowedH263BitrateAll )
    EUNIT_ASSERT( codecUnknown->iMaxBitRate != KMceH263Level45Bitrate )
    EUNIT_ASSERT( codecUnknown->iBitrate != KMceH263Level10Bitrate )
    EUNIT_ASSERT_EQUALS( codecUnknown->KeepAliveTimer(), 6 )
    EUNIT_ASSERT_EQUALS( codecUnknown->KeepAlivePayloadType(), 0 )
    EUNIT_ASSERT_EQUALS( codecUnknown->KeepAliveData(), KNullDesC8() )

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
void UT_CMusEngTwoWaySession::UT_AdjustAudioCodecLL()
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
void UT_CMusEngTwoWaySession::UT_DoCodecSelectionLL()
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
    delete iLiveSession->iVideoCodecList;
    iLiveSession->iVideoCodecList = NULL;
    iLiveSession->iVideoCodecList = _L8("SomethingThatIsNotAVC").AllocL();
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
void UT_CMusEngTwoWaySession::UT_StreamStateChangedL()
    {
    // Try without a session, nothing happens
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *videoStream );
    CleanupStack::PopAndDestroy( videoStream );
    
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EBuffering, outgoing stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
     
    // EBuffering, incoming stream is buffering
    EUNIT_ASSERT( !iLiveSession->iBuffered );
    CMceMediaStream* changedInStream = &iLiveSession->iSession->Streams()[0]->BoundStreamL();
    changedInStream->iState = CMceMediaStream::EBuffering;
    iLiveSession->StreamStateChanged( *changedInStream );
    EUNIT_ASSERT( iLiveSession->iBuffered );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EStreaming, outgoing stream is streaming, no effect on receiving information
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EStreaming, incoming stream is streaming, receiving information is updated and notified
    EUNIT_ASSERT( !iLiveSession->iReceiving );
    changedInStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->StreamStateChanged( *changedInStream );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    EUNIT_ASSERT( iLiveSession->iReceiving );
    iLcSessionObserver->Reset();
    
    // EStreaming, incoming stream is streaming, state is notified even if receiving information
    // says we are already receiving
    changedInStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->StreamStateChanged( *changedInStream );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    }

// -----------------------------------------------------------------------------
// Although all states are not meaningfull for LiveSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_StreamStateChangedWithSourceL()
    {
    // Try without a session, nothing happens
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                    *videoStream, 
                    *CMceCameraSource::NewLC( *iLiveSession->iManager ) );
    CleanupStack::PopAndDestroy(); // camera
    CleanupStack::PopAndDestroy( videoStream );
    
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    CMceMediaSource* changedSource = changedStream->Source();
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );;
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_SetEncodingDeviceLL()
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
void UT_CMusEngTwoWaySession::UT_SetCodecConfigKeyLL()
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
void UT_CMusEngTwoWaySession::UT_ReadCodecConfigKeyLL()
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
void UT_CMusEngTwoWaySession::UT_StoreEncoderConfigInfoLL()
    {
    CRepository::iStaticWriteAvcKeysToStaticData = ETrue;
    
    // Try with clean CenRep
    MultimediaSharingSettings::SetEncoderConfigInfoL( KNullDesC8() );   
    
    ESTABLISH_OUT_SESSION( iLiveSession ) // calls StoreEncoderConfigInfoL
    // If StoreEncoderConfigInfoL leaves with KErrNoMemory,
    // MMusEngSessionObserver::SessionFailed will be called.
    if ( iLcSessionObserver->iCalledFunction == CLcSessionObserverStub::ESessionFailed )
        {
        User::Leave( KErrNoMemory );
        }
                     
    HBufC8* keys = MultimediaSharingSettings::EncoderConfigInfoLC();    
    EUNIT_ASSERT_EQUALS( *keys, KMusAvcBitrateLevel_1b_ConfigKey() )
    CleanupStack::PopAndDestroy( keys );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_RemoteVideoPlayerL()
    {
    EUNIT_ASSERT( iLiveSession->RemoteVideoPlayer() == 
                  iLiveSession->iRemoteVideoPlayer )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWaySession::UT_InactivityTimeoutL()
    {
    // Not receiving, no effect
    iLiveSession->iReceiving = EFalse;
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
    iLiveSession->InactivityTimeout( *videoStream, *rtpSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // Receiving, state change is notified and
    // receiving and buffering status are cleared
    iLiveSession->iBuffered = ETrue;
    iLiveSession->iReceiving = ETrue;
    iLiveSession->InactivityTimeout( *videoStream, *rtpSource );
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iCalledFunction, 
                         CLcSessionObserverStub::EPlayerStateChanged );
    EUNIT_ASSERT( !iLiveSession->iReceiving );
    EUNIT_ASSERT( !iLiveSession->iBuffered );
    CleanupStack::PopAndDestroy( rtpSource );
    CleanupStack::PopAndDestroy( videoStream );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngTwoWaySession,
    "UT_CMusEngTwoWaySession",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngTwoWaySession",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)

EUNIT_TEST(
    "EstablishLcSessionL - test ",
    "CMusEngTwoWaySession",
    "EstablishLcSessionL",
    "FUNCTIONALITY",
    SetupL, UT_EstablishLcSessionLL, Teardown)

EUNIT_TEST(
    "EnableDisplayL - test ",
    "CMusEngTwoWaySession",
    "EnableDisplayL",
    "FUNCTIONALITY",
    SetupL, UT_EnableDisplayLL, Teardown)
    
EUNIT_TEST(
    "SetOrientationL - test ",
    "CMusEngTwoWaySession",
    "SetOrientationL",
    "FUNCTIONALITY",
    SetupL, UT_SetOrientationLL, Teardown)
    
EUNIT_TEST(
    "SetLocalRectL - test ",
    "CMusEngTwoWaySession",
    "SetLocalRectL",
    "FUNCTIONALITY",
    SetupL, UT_SetLocalRectLL, Teardown)
    
EUNIT_TEST(
    "SetRectsL - test ",
    "CMusEngTwoWaySession",
    "SetRectsL",
    "FUNCTIONALITY",
    SetupL, UT_SetRectsLL, Teardown)

EUNIT_TEST(
    "CompleteSessionStructureL - test ",
    "CMusEngTwoWaySession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureLL, Teardown)

EUNIT_TEST(
    "HandleSessionStateChanged - test ",
    "CMusEngTwoWaySession",
    "HandleSessionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_HandleSessionStateChangedL, Teardown)

EUNIT_TEST(
    "AdjustVideoCodecL - test ",
    "CMusEngTwoWaySession",
    "AdjustVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustVideoCodecLL, Teardown)

EUNIT_TEST(
    "AdjustAudioCodecL - test ",
    "CMusEngTwoWaySession",
    "AdjustAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustAudioCodecLL, Teardown)

EUNIT_TEST(
    "DoCodecSelectionL- test ",
    "CMusEngTwoWaySession",
    "DoCodecSelectionL",
    "FUNCTIONALITY",
    SetupL, UT_DoCodecSelectionLL, Teardown)

EUNIT_TEST(
    "StreamStateChangedL() - test ",
    "CMusEngTwoWaySession",
    "StreamStateChangedL()",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedL, Teardown)

EUNIT_TEST(
    "StreamStateChangedL( src ) - test ",
    "CMusEngTwoWaySession",
    "StreamStateChangedL( src )",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedWithSourceL, Teardown)        

EUNIT_TEST(
    "SetEncodingDeviceL - test ",
    "CMusEngTwoWaySession",
    "SetEncodingDeviceL",
    "FUNCTIONALITY",
    SetupL, UT_SetEncodingDeviceLL, Teardown)   

EUNIT_TEST(
    "SetCodecConfigKeyL - test ",
    "CMusEngTwoWaySession",
    "SetCodecConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_SetCodecConfigKeyLL, Teardown)    

EUNIT_TEST(
    "ReadCodecConfigKeyL - test ",
    "CMusEngTwoWaySession",
    "ReadCodecConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_ReadCodecConfigKeyLL, Teardown)        

EUNIT_TEST(
    "StoreEncoderConfigInfoL - test ",
    "CMusEngTwoWaySession",
    "StoreEncoderConfigInfoL",
    "FUNCTIONALITY",
    SetupL, UT_StoreEncoderConfigInfoLL, Teardown)    
    
EUNIT_TEST(
    "RemoteVideoPlayer - test ",
    "CMusEngTwoWaySession",
    "RemoteVideoPlayer",
    "FUNCTIONALITY",
    SetupL, UT_RemoteVideoPlayerL, Teardown)    
   
EUNIT_TEST(
    "EnableLcFileL - test ",
    "CMusEngTwoWaySession",
    "EnableLcFileL",
    "FUNCTIONALITY",
    SetupL, UT_EnableLcFileL, Teardown)    

EUNIT_TEST(
    "InactivityTimeout - test ",
    "CMusEngTwoWaySession",
    "InactivityTimeout",
    "FUNCTIONALITY",
    SetupL, UT_InactivityTimeoutL, Teardown)  
    
EUNIT_END_TEST_TABLE

//  END OF FILE


