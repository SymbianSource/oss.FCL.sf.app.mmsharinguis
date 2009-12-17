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
#include "ut_musenguriparser.h"
#include "musenguriparser.h"
#include "musengtestdefs.h"
#include "mussettingskeys.h"
#include "mussettings.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_TMusEngUriParser* UT_TMusEngUriParser::NewL()
    {
    UT_TMusEngUriParser* self = UT_TMusEngUriParser::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_TMusEngUriParser* UT_TMusEngUriParser::NewLC()
    {
    UT_TMusEngUriParser* self = new( ELeave ) UT_TMusEngUriParser();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_TMusEngUriParser::~UT_TMusEngUriParser()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_TMusEngUriParser::UT_TMusEngUriParser()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_TMusEngUriParser::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngUriParser::SetupL()
    {
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngUriParser::Teardown()
    {
    
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngUriParser::UT_TMusEngUriParser_TMusEngUriParserL()
    {
    TMusEngUriParser parser( KTestRecipientSipUri() );
    EUNIT_ASSERT( parser.iUri == KTestRecipientSipUri8()  )
    
    // Too long
    TBuf<520> longBuf;
    longBuf.Fill('a', 520);
    
    TMusEngUriParser parser2( longBuf );
    EUNIT_ASSERT( parser2.iUri == KNullDesC8() )
                     
    }


#include <uri8.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngUriParser::UT_TMusEngUriParser_GetUri8LL()
    {
    TMusEngUriParser parser( KTestRecipientSipUriNoSipPrefix() );
    
    // No parsing done, fails
    TRAPD( error, parser.GetUri8L() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Parse and test
    parser.ParseUriL();
    
    HBufC8* uri = parser.GetUri8L(); 
                              
    CleanupStack::PushL( uri );
    EUNIT_ASSERT( *uri == KTestRecipientSipUri8() );
    CleanupStack::PopAndDestroy( uri );   
    
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngUriParser::UT_TMusEngUriParser_GetUri16LL()
    {
    TMusEngUriParser parser( KTestRecipientSipUriNoSipPrefix() );
    
    // No parsing done, fails
    TRAPD( error, parser.GetUri16L( ETrue ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Parse and test
    parser.ParseUriL();
    
    HBufC16* uri = parser.GetUri16L( ETrue ); 
                              
    CleanupStack::PushL( uri );
    EUNIT_ASSERT( *uri == KTestRecipientSipUri() );
    CleanupStack::PopAndDestroy( uri );
    
    uri = parser.GetUri16L( EFalse ); 
                              
    CleanupStack::PushL( uri );
    EUNIT_ASSERT( *uri == KTestRecipientSipUriNoSipPrefix() );
    CleanupStack::PopAndDestroy( uri );
      
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_TMusEngUriParser::UT_TMusEngUriParser_ParseUriLL()
    {
    TMusEngUriParser parser( KTestRecipientSipUri() );
    
    // Succesful case, sip uri
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientSipUri8() );
    
    // Succesful case, sip uri with uppercase prefix
    parser.iUri = KTestRecipientSipUriPrefixUpperCase8();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientSipUri8() );
    
    // Succesful case, secure sip uri
    _LIT8( KTestRecipientSipsUriPrefix8, "sips:testrecipient@host.domain" );
    parser.iUri = KTestRecipientSipsUriPrefix8();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientSipsUriPrefix8() );
    
    // Succesful case, secure sip uri with uppercase prefix
    _LIT8( KTestRecipientSipsUriPrefixUpperCase8, "SIPS:testrecipient@host.domain" );
    parser.iUri = KTestRecipientSipsUriPrefixUpperCase8();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientSipsUriPrefix8() );
    
    // Succesful case, tel uri
    parser.iUri = KTestRecipientTelUri8();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientTelUri8() );
    
    // Succesful case, sip uri added
    parser.iUri = KTestRecipientSipUriNoSipPrefix8();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientSipUri8() );
    
    // Succesful case, tel uri added
    parser.iUri = KTestRecipientTelUriNoTelPrefix8();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientTelUri8() );
    
    // Succesfull case, valid number with separators
    _LIT8( KTestTelUriWithSomeWildChars, " +358 (1) [11-]{1}111 " );
    _LIT8( KTestTelUriWithPrefixAndSomeWildChars, "tel:+358 (1) [11-]{1}111" );
    
    parser.iUri = KTestTelUriWithSomeWildChars();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientTelUri8() );
    
    parser.iUri = KTestTelUriWithPrefixAndSomeWildChars();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientTelUri8() );
        
    // Unsuccessful case, random string
    parser.iUri = KTestRecipientRandomText8();
    TRAPD( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );
    
    // Unsuccessful case, tel prefix and at sign present
    parser.iUri = KTestRecipientTelUriWithAtSign8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );
    
    // Unsuccessful case, plus sign and non-numeric characters
    parser.iUri = KTestRecipientTelUriWithXCharAtEnd8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );
    
    parser.iUri = KTestRecipientTelUriWithXCharAtStart8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );
    
    // Unsuccesful case, name too long to be able to add sip prefix
    parser.iUri.Fill('a', 510);
    parser.iUri.Replace( 0, 
                         KTestRecipientSipUriNoSipPrefix8().Length(),
                         KTestRecipientSipUriNoSipPrefix8() );
    
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );
    
    // Unsuccesful case, name too long to be able to add tel prefix
    parser.iUri.Fill('1', 510);
    parser.iUri.Replace( 0, 
                         KTestRecipientTelUriNoTelPrefix8().Length(),
                         KTestRecipientTelUriNoTelPrefix8() );
    
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );
 
  // test cases for operator variant
    // set operator variant
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EOperatorSpecific );
    
    EUNIT_ASSERT_EQUALS( MusSettingsKeys::EOperatorSpecific, 
                   MultimediaSharingSettings::OperatorVariantSettingL() );

    //local number cases without tel prefix 
     // Successful case
    parser.iUri = KTestRecipientLocalTelUriNoTelPrefix8();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientLocalTelUri8() );

    // Unsuccesful case, uri too long to be able to add tel prefix
    parser.iUri = KTestRecipientLocalTelUriNoTelPrefix8();
    parser.iUri.Fill('1', 510);
    
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );
    
    // Unsuccessful case, crap uri
    parser.iUri = KTestRecipientLocalTelUriNoTelPrefixWithXCharAtEnd8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );

    // Unsuccessful case, crap uri
    parser.iUri = KTestRecipientLocalTelUriNoTelPrefixWithXCharAtStart8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );

    // Unsuccessful case, crap uri
    parser.iUri = KTestRecipientLocalTelUriNoTelPrefixWithXCharAtMiddle8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );

    //local number cases with tel prefix
    // Successful case
    parser.iUri = KTestRecipientLocalTelUri8();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientLocalTelUri8() );

    // Successful case, prefix uppercase
    parser.iUri = KTestRecipientLocalTelUriPrefixUpperCase8();
    parser.ParseUriL();
    EUNIT_ASSERT_EQUALS( parser.iUri, KTestRecipientLocalTelUri8() );

    // Unsuccessful case, crap uri
    parser.iUri = KTestRecipientLocalTelUriWithXCharAtEnd8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );

    // Unsuccessful case, crap uri
    parser.iUri = KTestRecipientLocalTelUriWithXCharAtStart8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );

    // Unsuccessful case, crap uri
    parser.iUri = KTestRecipientLocalTelUriWithXCharAtMiddle8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );

    // Unsuccessful case, crap uri
    parser.iUri = KTestRecipientLocalTelUriTelPrefixAtMiddle8();
    TRAP( error, parser.ParseUriL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT_EQUALS( error, KErrCorrupt );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_TMusEngUriParser,
    "UT_TMusEngUriParser",
    "UNIT" )

EUNIT_TEST(
    "TMusEngUriParser - test ",
    "TMusEngUriParser",
    "TMusEngUriParser",
    "FUNCTIONALITY",
    SetupL, UT_TMusEngUriParser_TMusEngUriParserL, Teardown)

EUNIT_TEST(
    "GetUri8L - test ",
    "TMusEngUriParser",
    "GetUri8L",
    "FUNCTIONALITY",
    SetupL, UT_TMusEngUriParser_GetUri8LL, Teardown)

EUNIT_TEST(
    "GetUri16L - test ",
    "TMusEngUriParser",
    "GetUri16L",
    "FUNCTIONALITY",
    SetupL, UT_TMusEngUriParser_GetUri16LL, Teardown)

EUNIT_TEST(
    "ParseUriL - test ",
    "TMusEngUriParser",
    "ParseUriL",
    "FUNCTIONALITY",
    SetupL, UT_TMusEngUriParser_ParseUriLL, Teardown)



EUNIT_END_TEST_TABLE

//  END OF FILE


