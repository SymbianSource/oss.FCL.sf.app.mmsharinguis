/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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


//  CLASS HEADER
#include "UT_CMusAvaCapability.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <sdpmediafield.h>
#include <sdpattributefield.h>


//  INTERNAL INCLUDES
#include "sipprofile.h"
#include "sipconnection.h"

#include "musavaCapabilityExchange.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavaoptionhandler.h"
#include "musavasettingsimp.h"
#include "musavaobserverimp.h"
#include "musavaterminal.h"
#include "MusAvaCapability.h"
#include "musavacapabilitytesthelper.h"
#include "CSipSseTestTls.h"
#include "musavacapabilitycontext.h"
#include "musavacapabilityqueryobserverimp.h"
#include "mussettings.h"

_LIT8( KTestTerminalId, "TestTerminalID" );

// CONSTRUCTION
UT_CMusAvaCapability* UT_CMusAvaCapability::NewL()
    {
    UT_CMusAvaCapability* self = UT_CMusAvaCapability::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaCapability* UT_CMusAvaCapability::NewLC()
    {
    UT_CMusAvaCapability* self = new( ELeave ) UT_CMusAvaCapability();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaCapability::~UT_CMusAvaCapability()
    {
    }

// Default constructor
UT_CMusAvaCapability::UT_CMusAvaCapability()
    {
    }

// Second phase construct
void UT_CMusAvaCapability::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaCapability::SetupL(  )
    {
    MSIPObserver* dummyObserver = NULL;
    CSIPProfileRegistry* dummyRegistry = NULL;
    MSIPConnectionObserver* dummyConnectionObserver = NULL;
    
    TUid uid = TUid::Uid( 1 );
    TUint32 iapId = 32;
    
    iSIP = CSIP::NewL( uid, *dummyObserver );
    iProfile = CSIPProfile::NewL( dummyRegistry );
    iSIPConnection = CSIPConnection::NewL( *iSIP, 
                                            iapId,
                                            *dummyConnectionObserver );
                               
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iOptionHandler = CMusAvaOptionHandler::NewL( *iAvailabilityObserver,
                                                 *iConcreteSettings );

    iExchange = iOptionHandler->iCapabilityExchange;
    
    iCapability = iOptionHandler->iSwisCapability;
    
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    iStorage->Set( MusSettingsKeys::KFastStartupMode, 
         MusSettingsKeys::EFastModeOff );
    }

void UT_CMusAvaCapability::Teardown(  )
    {
    delete iOptionHandler;
    delete iAvailabilityObserver;
    delete iConcreteSettings;
    if ( iStorage )
        {
        iStorage->Clear();
        CSipSseTestTls::Close();
        iStorage = NULL;
        }

    delete iSIPConnection;
    delete iProfile;
    delete iSIP;

    }

void UT_CMusAvaCapability::UT_CMusAvaCapability_NewLL(  )
    {
    
    EUNIT_ASSERT( &iCapability->Exchange() == iExchange );
    EUNIT_ASSERT( iCapability->Feature().DesC().Compare( KCapabilitySwisFeature ) == 0 );
    EUNIT_ASSERT( iCapability->Enabled() );
    iCapability->Enabled() = EFalse;
    EUNIT_ASSERT( !iCapability->Enabled() );
    
    CSIPAcceptContactHeader* acceptContact = NULL;
    
    // create accept contact with feature tag, check if accept-contact-header has 
    // feature tag defined in iCapability
    acceptContact = MusAvaCapabilityContext::AcceptContactHeaderLC( iCapability->Feature() );
    EUNIT_ASSERT( iCapability->Supports( *acceptContact ) );
    // check if feature tag is the "right one"
    EUNIT_ASSERT( iCapability->Feature().DesC().Compare( KCapabilitySwisFeature ) == 0 );
    
    // remove the feature tag from accept-contact header, add test-feature-tag
    // check that iCapability does not support the test feature tag
    // check also that iCapability feature tag is the 'right one'
    acceptContact->DeleteParam( iCapability->Feature() );
    RStringF newParam = MusAvaCapabilityContext::SIPStringL( KCapabilityTestDataFeature );
    CleanupClosePushL( newParam );
    acceptContact->SetParamL( newParam );
    CleanupStack::PopAndDestroy();//newParam
    EUNIT_ASSERT( !iCapability->Supports( *acceptContact ) );
    EUNIT_ASSERT( iCapability->Feature().DesC().Compare( KCapabilitySwisFeature ) == 0 );

    CleanupStack::PopAndDestroy( acceptContact );

    }

void UT_CMusAvaCapability::UT_CMusAvaCapability_DoPopulateResponseLL(  )
    {
    CSIPServerTransaction* options = NULL;
    
    iCapability->Enabled() = ETrue;
     
    options =  CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                             KCapabilityTestAddress_B,
                                             KCapabilitySwisFeature,
                                             KCapabilitySwisFeature,
                                             KCapabilityTestAcceptHeader,
                                             KCapabilityTestTerminalID_B );
    CleanupStack::PushL( options );                                         

    CSdpDocument* sdpContent = 
            CMusAvaCapability::ResponseContentLC( *options );

    RPointerArray<CSIPHeaderBase> responseHeaders;
    CSIPHeaderBase::PushLC( &responseHeaders );

    iCapability->PopulateResponseL( *options, responseHeaders, *sdpContent );

    EUNIT_ASSERT( responseHeaders.Count() == 1 );
    EUNIT_ASSERT( sdpContent->AttributeFields().Count() == 2 );
    EUNIT_ASSERT( sdpContent->MediaFields().Count() == 1 );
    EUNIT_ASSERT( sdpContent->MediaFields()[0]->FormatAttributeFields().Count() == 2 );

    CleanupStack::PopAndDestroy( &responseHeaders );
    CleanupStack::PopAndDestroy( sdpContent );
    CleanupStack::PopAndDestroy( options );

    }


void UT_CMusAvaCapability::Setup1L()
    {
    // save operator variant setting type to be able to set it back to original value in teardown
    iOpVariantSetting = MultimediaSharingSettings::OperatorVariantSettingL();
    iCapabQueryObserver = new ( ELeave ) CMusAvaCapabilityQueryObserverImp();
    iExchange = CMusAvaCapabilityExchange::NewL( KTestTerminalId, *iCapabQueryObserver );
    iCapability = CMusAvaCapability::NewL( *iExchange );
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    iStorage->Set( MusSettingsKeys::KFastStartupMode, 
         MusSettingsKeys::EFastModeOff );
    }

void UT_CMusAvaCapability::Teardown1(  )
    {
    if ( iStorage )
        {
        iStorage->Clear();
        CSipSseTestTls::Close();
        iStorage = NULL;
        }
    delete iCapability;
    delete iExchange;
    delete iCapabQueryObserver;     
    }   
    
    
void UT_CMusAvaCapability::UT_CMusAvaCapability_SupportsL()
    {
     
    CSIPAcceptContactHeader* contactHeader = CSIPAcceptContactHeader::NewLC();      
    
    RStringF str = MusAvaCapabilityContext::SDPStringL( KCapabilitySwisFeature );
    CleanupClosePushL( str );
    
    contactHeader->SetParamL( str );

    EUNIT_ASSERT( iCapability->Supports( *contactHeader ) );
    
    CleanupStack::PopAndDestroy( &str );
    CleanupStack::PopAndDestroy( contactHeader );
    
    }
    
    
void UT_CMusAvaCapability::UT_CMusAvaCapability_ResolveCodecsLL()
    {
    _LIT( KTest263, "H263-2000/90000");
    _LIT( KTest264, "H264/90000");
    
    // Find H.263 codec
    CSdpDocument* sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP );
    CDesCArrayFlat* codecs = iCapability->ResolveCodecsL( *sdpContent );
    CleanupStack::PushL( codecs );
    EUNIT_ASSERT( codecs->MdcaCount() == 1 );
    EUNIT_ASSERT_EQUALS( (*codecs)[0], KTest263 );
    CleanupStack::PopAndDestroy( codecs );
    CleanupStack::PopAndDestroy( sdpContent );
    
    // Find H.264 codec
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP264 );
    codecs = iCapability->ResolveCodecsL( *sdpContent );
    CleanupStack::PushL( codecs );
    EUNIT_ASSERT( codecs->MdcaCount() == 1 );
    EUNIT_ASSERT_EQUALS( (*codecs)[0], KTest264 );
    CleanupStack::PopAndDestroy( codecs );
    CleanupStack::PopAndDestroy( sdpContent );

    // Find H.263 or H.264 codec
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP264And263 );
    codecs = iCapability->ResolveCodecsL( *sdpContent );
    CleanupStack::PushL( codecs );
    EUNIT_ASSERT( codecs->MdcaCount() == 2 );
    EUNIT_ASSERT_EQUALS( (*codecs)[0], KTest264 );
    EUNIT_ASSERT_EQUALS( (*codecs)[1], KTest263 );
    CleanupStack::PopAndDestroy( codecs );
    CleanupStack::PopAndDestroy( sdpContent );
    
    // Do NOT find an invalid codec
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPWrongCodec );
    codecs = iCapability->ResolveCodecsL( *sdpContent );
    CleanupStack::PushL( codecs );
    EUNIT_ASSERT( codecs->MdcaCount() == 0 );
    CleanupStack::PopAndDestroy( codecs );
    CleanupStack::PopAndDestroy( sdpContent );
    
    }    

void UT_CMusAvaCapability::UT_CMusAvaCapability_AttributeL()
    {
    
    CSdpDocument* sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP );        
    
    // give an attribute which can be found
    RStringF attribute = MusAvaCapabilityContext::SDPStringL( _L8("type") );
    CleanupClosePushL( attribute );
    EUNIT_ASSERT( iCapability->Attribute( attribute, *sdpContent ) );
    CleanupStack::PopAndDestroy( &attribute );     
    
    // give an attribute which can be not found
    attribute = MusAvaCapabilityContext::SDPStringL( _L8("xxx") );
    CleanupClosePushL( attribute );
    EUNIT_ASSERT( !iCapability->Attribute( attribute, *sdpContent ) );
    CleanupStack::PopAndDestroy( &attribute );         
    
    CleanupStack::PopAndDestroy( sdpContent );       
    }
    
    
void UT_CMusAvaCapability::UT_CMusAvaCapability_PopulateResponseL_OpSpecificL()    
    {
     
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EOperatorSpecific );
    iCapabQueryObserver->iCapabilitiesResolvedForCingular = ETrue;
     
    CSIPServerTransaction* options =  
            CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                                   KCapabilityTestAddress_B,
                                                   KCapabilitySwisFeature,
                                                   KCapabilitySwisFeature,
                                                   KCapabilityTestAcceptHeader,
                                                   KCapabilityTestTerminalID_B );
    CleanupStack::PushL( options );                                         

    CSdpDocument* sdpContent = 
            CMusAvaCapability::ResponseContentLC( *options );

    RPointerArray<CSIPHeaderBase> responseHeaders;
    CSIPHeaderBase::PushLC( &responseHeaders );

    iCapability->PopulateResponseL( *options, responseHeaders, *sdpContent );

    EUNIT_ASSERT( responseHeaders.Count() == 1 );
    HBufC8* headerValue = responseHeaders[0]->ToTextValueL();
    // header value contains just one field
    EUNIT_ASSERT_EQUALS( KErrNotFound, headerValue->Find(_L8(";") ) );
    delete headerValue;

    CleanupStack::PopAndDestroy( &responseHeaders );
    CleanupStack::PopAndDestroy( sdpContent );
    CleanupStack::PopAndDestroy( options );
    }


void UT_CMusAvaCapability::UT_CMusAvaCapability_ResolveFastModeLL()
    {
    // Fast mode feature is disabled in cenrep, fast mode is present in SDP
    CSdpDocument* sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPFastMode );
    MusSettingsKeys::TFastMode mode = iCapability->ResolveFastModeL( *sdpContent );
    EUNIT_ASSERT( mode == MusSettingsKeys::EFastModeOff );
    CleanupStack::PopAndDestroy( sdpContent );
    
    // Fast mode feature is enabled in cenrep, fast mode is present in SDP
    iStorage->Set( MusSettingsKeys::KFastStartupMode, MusSettingsKeys::EFastModeOn );
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPFastMode );
    mode = iCapability->ResolveFastModeL( *sdpContent );
    EUNIT_ASSERT( mode == MusSettingsKeys::EFastModeOn );
    CleanupStack::PopAndDestroy( sdpContent );

    // Fast mode feature is enabled in cenrep, fast mode isn't present in SDP
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP );
    mode = iCapability->ResolveFastModeL( *sdpContent );
    EUNIT_ASSERT( mode == MusSettingsKeys::EFastModeOff );
    CleanupStack::PopAndDestroy( sdpContent );
    }

void UT_CMusAvaCapability::UT_CMusAvaCapability_AddFastModeLL()
    {
    // Fast mode feature is disabled in cenrep
    CSdpDocument* sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP );
    EUNIT_ASSERT( sdpContent->AttributeFields().Count() == 2 );
    iCapability->AddFastModeL( *sdpContent );
    EUNIT_ASSERT( sdpContent->AttributeFields().Count() == 2 );
    
    // Fast mode feature is enabled in cenrep, a=keywds:fastmode added
    iStorage->Set( MusSettingsKeys::KFastStartupMode, MusSettingsKeys::EFastModeOn );
    iCapability->AddFastModeL( *sdpContent );
    EUNIT_ASSERT( sdpContent->AttributeFields().Count() == 3 );
    const CSdpAttributeField* field = sdpContent->AttributeFields()[2];
    RStringF keywds = MusAvaCapabilityContext::SDPStringL( 
            SdpCodecStringConstants::EAttributeKeywds );
    EUNIT_ASSERT( field->Attribute() == keywds );
    EUNIT_ASSERT( field->Value().Compare( KCapabilitySDPAttributeFastMode ) == 0 );
    CleanupStack::PopAndDestroy( sdpContent );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusAvaCapability,
    "CMusAvaCapability",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaCapability",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaCapability_NewLL, Teardown)

EUNIT_TEST(
    "DoPopulateResponseL - test ",
    "CMusAvaCapability",
    "DoPopulateResponseL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaCapability_DoPopulateResponseLL, Teardown)
    
EUNIT_TEST(
    "Supports - test ",
    "CMusAvaCapability",
    "Supports",
    "FUNCTIONALITY",
    Setup1L, UT_CMusAvaCapability_SupportsL, Teardown1 ) 
    
EUNIT_TEST(
    "ResolveCodecsL - test ",
    "CMusAvaCapability",
    "ResolveCodecsL",
    "FUNCTIONALITY",
    Setup1L, UT_CMusAvaCapability_ResolveCodecsLL, Teardown1 )     
    
EUNIT_TEST(
    "Attribute - test ",
    "CMusAvaCapability",
    "Attribute",
    "FUNCTIONALITY",
    Setup1L, UT_CMusAvaCapability_AttributeL, Teardown1 )     
    
EUNIT_TEST(
    "PopulateResponseL Operator Specific - test ",
    "CMusAvaCapability",
    "PopulateResponseL",
    "FUNCTIONALITY",
    Setup1L, UT_CMusAvaCapability_PopulateResponseL_OpSpecificL, Teardown1 )         
   
EUNIT_TEST(
    "ResolveFastModeL - test ",
    "CMusAvaCapability",
    "ResolveFastModeL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaCapability_ResolveFastModeLL, Teardown)
    
EUNIT_TEST(
    "AddFastModeL - test ",
    "CMusAvaCapability",
    "AddFastModeL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaCapability_AddFastModeLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
