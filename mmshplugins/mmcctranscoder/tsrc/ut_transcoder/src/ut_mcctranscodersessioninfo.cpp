/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit test
*
*/



//  INTERNAL INCLUDES
#include "ut_mcctranscodersessioninfo.h"
#include "transcodersessioninfo.h"
#include <mmcccodecinformation.h>


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMccTranscoderSessionInfo* UT_CMccTranscoderSessionInfo::NewL()
    {
    UT_CMccTranscoderSessionInfo* self = UT_CMccTranscoderSessionInfo::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMccTranscoderSessionInfo* UT_CMccTranscoderSessionInfo::NewLC()
    {
    UT_CMccTranscoderSessionInfo* self = new( ELeave ) UT_CMccTranscoderSessionInfo();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMccTranscoderSessionInfo::~UT_CMccTranscoderSessionInfo()
    {
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::SetupL()
    {
    RFs fs;
    CleanupClosePushL( fs );
    iMovie = CVedMovie::NewL( &fs );
    CleanupStack::PopAndDestroy();//fs
    iSession = CMccTranscoderSessionInfo::NewLC();
    CleanupStack::Pop();

    CMccTranscoderAudioCodec* acodec = CMccTranscoderAudioCodec::NewL();
    iSession->SetAudioCodec( acodec );
    CMccTranscoderVideoCodec* vcodec = CMccTranscoderVideoCodec::NewL();
    iSession->SetVideoCodec( vcodec );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::Teardown()
    {
    delete iSession;
    delete iMovie;

    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_SetQualityL()
    {
    
    EUNIT_ASSERT( !iSession->iQuality );

    iSession->SetQuality( CVedMovie::EQualityAutomatic );
    EUNIT_ASSERT( iSession->Quality()  == CVedMovie::EQualityAutomatic );

    iSession->SetQuality( CVedMovie::EQualityMMSInteroperability );
    EUNIT_ASSERT( iSession->Quality()  == CVedMovie::EQualityMMSInteroperability );

    iSession->SetQuality( CVedMovie::EQualityResolutionCIF );
    EUNIT_ASSERT( iSession->Quality()  == CVedMovie::EQualityResolutionCIF );

    iSession->SetQuality( CVedMovie::EQualityLast );
    EUNIT_ASSERT( iSession->Quality()  == CVedMovie::EQualityLast );

    iSession->SetQuality( 589 );
    EUNIT_ASSERT( iSession->Quality()  == CVedMovie::EQualityLast );

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_CreateMovieLL()
    {

    EUNIT_ASSERT( !iSession->iMovie );

    iSession->CreateMovieL();
    EUNIT_ASSERT( &iSession->Movie() );

    iSession->CreateMovieL();
    EUNIT_ASSERT( &iSession->Movie() );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_SetDesFileLL()
    {
    
    _LIT( KTestFile, "C:\\testfile.3gp");
    
    EUNIT_ASSERT( iSession->iDesFile.Length() == 0 );
    EUNIT_ASSERT_LEAVE( iSession->SetDesFileL( KNullDesC ) );
    
    EUNIT_ASSERT( iSession->iDesFile.Length() == 0 );
    iSession->SetDesFileL( KTestFile );
    EUNIT_ASSERT( iSession->DesFile() == KTestFile() );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_SetVideoCodecL()
    {
    EUNIT_ASSERT( iSession->iVideoCodec );
    
    CMccTranscoderVideoCodec* codec = CMccTranscoderVideoCodec::NewL();
    iSession->SetVideoCodec( codec );
    EUNIT_ASSERT( &iSession->VideoCodec() == codec );
    
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_SetAudioCodecL()
    {
    EUNIT_ASSERT( iSession->iAudioCodec );
    
    CMccTranscoderAudioCodec* codec = CMccTranscoderAudioCodec::NewL();
    iSession->SetAudioCodec( codec );
    EUNIT_ASSERT( &iSession->AudioCodec() == codec );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_SetVideoTypeL()
    {

    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeUnrecognized );
    
    iSession->SetVideoType( KAVCSdpName, 0 );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeAVCBaselineProfile );

    iSession->SetVideoType( KH263SdpName, 0 );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeH263Profile0Level10 );

    iSession->SetVideoType( KH2631998SdpName, 0 );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeH263Profile0Level10 );

    iSession->SetVideoType( KH2632000SdpName, 0 );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeH263Profile0Level10 );

    iSession->SetVideoType( KH2632000SdpName, 64001 );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeH263Profile0Level45 );

    iSession->SetVideoType( KH2632000SdpName, 64001 );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeH263Profile0Level45 );

    iSession->VideoCodec().SetVideoType( EVedVideoTypeUnrecognized );
    iSession->SetVideoType( KH2632000SdpName, 128001 );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeUnrecognized );
    

    iSession->SetVideoType( KNullDesC8, 64001 );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeUnrecognized );

    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_SetAudioTypeL() 
    {
    
    EUNIT_ASSERT( iSession->AudioCodec().AudioType() == EVedAudioTypeUnrecognized );
    
    iSession->SetAudioType( KAACSdpName );
    EUNIT_ASSERT( iSession->AudioCodec().AudioType() == EVedAudioTypeAAC_LC );

    iSession->SetAudioType( KAMRSdpName );
    EUNIT_ASSERT( iSession->AudioCodec().AudioType() == EVedAudioTypeAMR );

    iSession->SetAudioType( KNullDesC8 );
    EUNIT_ASSERT( iSession->AudioCodec().AudioType() == EVedAudioTypeUnrecognized );

    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_CheckVideoCodecLL()
    {
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeUnrecognized );

    TMccTranscoderCodecInfo info;
    iSession->CheckVideoCodecL( info );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeUnrecognized );
    
    info.iMimeType = KAVCSdpName();
    iSession->CheckVideoCodecL( info );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeAVCBaselineProfile );

    delete iSession->iVideoCodec;
    iSession->iVideoCodec = NULL;
    iSession->CheckVideoCodecL( info );
    EUNIT_ASSERT( iSession->VideoCodec().VideoType() == EVedVideoTypeAVCBaselineProfile );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_CheckAudioCodecLL()
    {
    EUNIT_ASSERT( iSession->AudioCodec().AudioType() == EVedAudioTypeUnrecognized );

    TMccTranscoderCodecInfo info;
    iSession->CheckAudioCodecL( info );
    EUNIT_ASSERT( iSession->AudioCodec().AudioType() == EVedAudioTypeUnrecognized );
    
    info.iMimeType = KAACSdpName();
    iSession->CheckAudioCodecL( info );
    EUNIT_ASSERT( iSession->AudioCodec().AudioType() == EVedAudioTypeAAC_LC );

    delete iSession->iAudioCodec;
    iSession->iAudioCodec = NULL;
    iSession->CheckAudioCodecL( info );
    EUNIT_ASSERT( iSession->AudioCodec().AudioType() == EVedAudioTypeAAC_LC );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_GetPutputParametersL()
    {
    iSession->SetVideoType( KAVCSdpName, 0 );
    iSession->SetAudioType( KAACSdpName );
    
    TVedOutputParameters params;
    
    iSession->GetPutputParameters( params );
    EUNIT_ASSERT( params.iVideoType == EVedVideoTypeAVCBaselineProfile );
    EUNIT_ASSERT( params.iAudioType == EVedAudioTypeAAC_LC );
    
    
    // Video codec present, but no audio codec
    iSession->CreateMovieL();
    delete iSession->iAudioCodec;
    iSession->iAudioCodec = NULL;
    
     // Strange compatibility hack needed when using avc without audio
    iSession->iMovie->iAudioType = EVedAudioTypeNoAudio;
    params.iAudioBitrate = 0;
    iSession->GetPutputParameters( params );
    EUNIT_ASSERT( params.iVideoType == EVedVideoTypeAVCBaselineProfile );
    EUNIT_ASSERT( params.iAudioType == EVedAudioTypeNoAudio );
    EUNIT_ASSERT( params.iAudioBitrate == 16000 );
    
    // No hack needed for H263 codec without audio
    iSession->iVideoCodec->SetVideoType( EVedVideoTypeH263Profile0Level10 );
    params.iAudioBitrate = 0;
    iSession->GetPutputParameters( params );
    EUNIT_ASSERT( params.iVideoType == EVedVideoTypeH263Profile0Level10 );
    EUNIT_ASSERT( params.iAudioType == EVedAudioTypeNoAudio );
    EUNIT_ASSERT( params.iAudioBitrate == 0 );
    
    // Either of codecs are present
    iSession->iMovie->iAudioType = EVedAudioTypeAMR;
    delete iSession->iVideoCodec;
    iSession->iVideoCodec = NULL;
    
    iSession->GetPutputParameters( params );
    EUNIT_ASSERT( params.iSyncIntervalInPicture == 0 );
    
    delete iSession->iMovie;
    iSession->iMovie = NULL;
    
    iSession->GetPutputParameters( params );
    EUNIT_ASSERT( params.iSyncIntervalInPicture == 0 );
    
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_ProgressedL()
    {
    EUNIT_ASSERT( !iSession->iAudioNotPresent );
    
    EUNIT_ASSERT( iSession->Progressed( 20 ) == 20 );
    iSession->iAudioNotPresent = ETrue;
    EUNIT_ASSERT( iSession->Progressed( 50 ) == 50 * 2 );
    EUNIT_ASSERT( iSession->Progressed( 49 ) == 49 * 2 );
    EUNIT_ASSERT( iSession->Progressed( 75 ) == 75 );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_ClipAddedL()
    {
    EUNIT_ASSERT( !iSession->iAudioNotPresent );
    iSession->ClipAdded();
    EUNIT_ASSERT( !iSession->iAudioNotPresent );

    iSession->CreateMovieL();
    iSession->ClipAdded();

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
void UT_CMccTranscoderSessionInfo::UT_CMccTranscoderSessionInfo_QualityChangeNeededL()
    {
    iSession->SetQuality( CVedMovie::EQualityMMSInteroperability );
    EUNIT_ASSERT( !iSession->QualityChangeNeeded() );
    
    iSession->SetVideoCodec( NULL );
    EUNIT_ASSERT( iSession->QualityChangeNeeded() );

    
    iSession->SetQuality( CVedMovie::EQualityAutomatic );
    EUNIT_ASSERT( !iSession->QualityChangeNeeded() );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccTranscoderSessionInfo,
    "UT_CMccTranscoderSessionInfo",
    "UNIT" )

EUNIT_TEST(
    "SetQuality - test",
    "CMccTranscoderSessionInfo",
    "SetQuality",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_SetQualityL, Teardown)

EUNIT_TEST(
    "CreateMovieL - test",
    "CMccTranscoderSessionInfo",
    "CreateMovieL",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_CreateMovieLL, Teardown)

EUNIT_TEST(
    "SetDesFileL - test",
    "CMccTranscoderSessionInfo",
    "SetDesFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_SetDesFileLL, Teardown)

EUNIT_TEST(
    "SetVideoCodec - test",
    "CMccTranscoderSessionInfo",
    "SetVideoCodec",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_SetVideoCodecL, Teardown)

EUNIT_TEST(
    "SetAudioCodec - test",
    "CMccTranscoderSessionInfo",
    "SetAudioCodec",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_SetAudioCodecL, Teardown)

EUNIT_TEST(
    "SetVideoType - test",
    "CMccTranscoderSessionInfo",
    "SetVideoType",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_SetVideoTypeL, Teardown)

EUNIT_TEST(
    "SetAudioType - test",
    "CMccTranscoderSessionInfo",
    "SetAudioType",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_SetAudioTypeL, Teardown)

EUNIT_TEST(
    "CheckVideoCodecL - test",
    "CMccTranscoderSessionInfo",
    "CheckVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_CheckVideoCodecLL, Teardown)

EUNIT_TEST(
    "CheckAudioCodecL - test",
    "CMccTranscoderSessionInfo",
    "CheckAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_CheckAudioCodecLL, Teardown)

EUNIT_TEST(
    "GetPutputParameters - test",
    "CMccTranscoderSessionInfo",
    "GetPutputParameters",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_GetPutputParametersL, Teardown)

EUNIT_TEST(
    "Progressed - test",
    "CMccTranscoderSessionInfo",
    "Progressed",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_ProgressedL, Teardown)

EUNIT_TEST(
    "ClipAdded - test",
    "CMccTranscoderSessionInfo",
    "ClipAdded",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_ClipAddedL, Teardown)

EUNIT_TEST(
    "QualityChangeNeeded - test",
    "CMccTranscoderSessionInfo",
    "QualityChangeNeeded",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderSessionInfo_QualityChangeNeededL, Teardown)    


EUNIT_END_TEST_TABLE

//  END OF FILE


