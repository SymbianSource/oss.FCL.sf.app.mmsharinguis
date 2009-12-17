/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Main plugin class
*
*/


#include "ut_musaoplugin.h"   
#include "muscallmonitor.h"
#include "musaoplugin.h"
#include "mustsypropertymonitor.h"
#include "musresourceproperties.h"
#include "mussesseioninformationapi.h"
#include "mussettings.inl"


#include <alwaysonlinemanagercommon.h>
#include <alwaysonlinemanagerclient.h>
#include <digia/eunit/eunitmacros.h>
#include "FeatMgr.h"



#pragma warn_illtokenpasting off

_LIT( KEmptyTelNumber, "");


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
// 
UT_CMusAoPlugin* UT_CMusAoPlugin::NewLC()
    {
    UT_CMusAoPlugin* self = new (ELeave) UT_CMusAoPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();    
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
// 
UT_CMusAoPlugin::~UT_CMusAoPlugin()
    {
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
// 
UT_CMusAoPlugin::UT_CMusAoPlugin()
    : CEUnitTestSuiteClass()
    {
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
// 
void UT_CMusAoPlugin::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusAoPlugin::SetupL(  )
    {
    TInt keyValue = 0;    
    MultimediaSharingSettings::SetPropertyValueL(
                            MusSettingsKeys::KProductModeVariation,keyValue);                               
    iPlugin = CMusAoPlugin::NewL();   
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusAoPlugin::Teardown(  )
    {
    delete iPlugin;
    iPlugin = NULL ;
    PropertyHelper::Close();
    
    }
	
	
//// -----------------------------------------------------------------------------
////
//// -----------------------------------------------------------------------------
//// 
void UT_CMusAoPlugin::NewLL()
	{
	
    TInt res = KErrNone;
    TInt ret = KErrNone;
	FeatureManager::MultimediaSharingNotSupported();
	iPlugin->DeleteProperties();
    delete iPlugin;
    iPlugin = NULL ;
	iPlugin = CMusAoPlugin::NewL(); 
	EUNIT_ASSERT( iPlugin );
    EUNIT_ASSERT( iPlugin->iTsyPropertyMonitor == NULL);
    EUNIT_ASSERT( iPlugin->iPropertyMonitor == NULL);
    // camera information property
    ret = RProperty::Get( NMusResourceApi::KCategoryUid,
            NMusResourceApi::KCameraInformation,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    ret = KErrNone;

    // call event
    ret = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallEvent,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    ret = KErrNone;
    
    // phone number
    TBuf<15> telnumber; // TODO change to des
    User::LeaveIfError( RProperty::Get(
                            NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMusTelNumber,
                            telnumber ) );
    EUNIT_ASSERT_EQUALS( telnumber.Compare( KEmptyTelNumber ), KErrNone )
    
    // Mus allowed
    ret = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMUSForbidden,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    ret = KErrNone;

    // Call direction
    ret = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallDirection,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    FeatureManager::MultimediaSharingSupported();
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusAoPlugin::DefinePropertiesLL()
    {
/* Test 1 */  
    /* Set Product Mode variation key value to
       00 - VideoPlayer always available
       0-Keypad available dynamic
       00-Keypad Usuable in portrait
       0-Camera available static
       00-Usuable Always
       00000000 = 0 (Int)
     */
    TInt keyValue = 0;    
    MultimediaSharingSettings::SetPropertyValueL(
    						MusSettingsKeys::KProductModeVariation,keyValue);     						    
    iPlugin->DefinePropertiesL();
    // It should not configure any PS keys for camera
    // Check the stub whether define is called.   
    TInt err = RProperty::Get( NMusResourceApi::KCategoryUid,
                    NMusResourceApi::KCameraAvailability,
                    keyValue );
    EUNIT_ASSERT(keyValue == KErrNotFound );
    // Keypad availability is static . 
    // Check the stub whether define is called. 
    err = RProperty::Get( NMusResourceApi::KCategoryUid,
                    NMusResourceApi::KKeypadAvailability,
                    keyValue );
                    
    EUNIT_ASSERT(keyValue == KErrNotFound );  
    
    iPlugin->DeleteProperties();
    
    
/* Test 2 */
    
    /* Set Product Mode variation key value to
       11 - VideoPlayer always available
       1-Keypad available dynamic
       11-Keypad Usuable in portrait
       1-Camera available static
       11-Usuable Always
       11111111 = 255 (Int)
     */
	keyValue = 255;
    MultimediaSharingSettings::SetPropertyValueL(
    						MusSettingsKeys::KProductModeVariation,keyValue);
    PropertyHelper::SetErrorCode(KErrNone);    	    
    iPlugin->DefinePropertiesL();						
    // It should configure any PS keys for Keypad
    // Check the stub whether define is called.     
    err = RProperty::Get( NMusResourceApi::KCategoryUid,
                    NMusResourceApi::KKeypadAvailability,
                    keyValue );    
    EUNIT_ASSERT(keyValue ==NMusResourceApi::ENotAvailable );
    
    // camera availability is dynamic . 
    // so it must define a property
    // Check the stub whether define is called.     
    err = RProperty::Get( NMusResourceApi::KCategoryUid,
                    NMusResourceApi::KCameraAvailability,
                    keyValue );                    
    EUNIT_ASSERT(keyValue == NMusResourceApi::ENotAvailable ); 	
    iPlugin->DeleteProperties();
    
 /* Test 3 */

    iPlugin->DefinePropertiesL();                       

    // camera information property
    TInt res = RProperty::Get( NMusResourceApi::KCategoryUid,
                                        NMusResourceApi::KCameraInformation,
                                        keyValue );
    EUNIT_ASSERT_EQUALS( keyValue, NMusResourceApi::EUsePrimaryCamera )    
    
    // call event
    RProperty::Get(NMusSessionInformationApi::KCategoryUid,
                   NMusSessionInformationApi::KMusCallEvent,
                                keyValue );
    EUNIT_ASSERT_EQUALS( keyValue, NMusSessionInformationApi::ENoCall )    
    
    // phone number
    TBuf<15> telnumber; // TODO change to des
    User::LeaveIfError( RProperty::Get(
                            NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMusTelNumber,
                            telnumber ) );
    EUNIT_ASSERT_EQUALS( telnumber.Compare( KEmptyTelNumber ), KErrNone )

    // Mus allowed
    RProperty::Get(NMusSessionInformationApi::KCategoryUid,
                   NMusSessionInformationApi::KMUSForbidden,
                   keyValue ) ;
    EUNIT_ASSERT_EQUALS( keyValue, NMusSessionInformationApi::EMUSAllowed )    

    // Call direction
    RProperty::Get(NMusSessionInformationApi::KCategoryUid,
                   NMusSessionInformationApi::KMusCallDirection,
                   keyValue ) ;
    EUNIT_ASSERT_EQUALS( keyValue, NMusSessionInformationApi::ENoDirection )    
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusAoPlugin::DefinePropertyL_IntPropertyL()
    {
    // Normal succcesfull case
    const TInt KTestAvailability( 0x10282300 ); // Imaginary test value
    const TInt KTestValue = 50; // Imaginary test value
    iPlugin->DefinePropertyL( KTestAvailability, RProperty::EInt, KTestValue );
    
    TInt keyValueInPS( 0 );
    User::LeaveIfError( RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                                        KTestAvailability,
                                        keyValueInPS ) );
    EUNIT_ASSERT_EQUALS( keyValueInPS, KTestValue )    
    
    // When trying to define existing property, we are not supposed to 
    // overwrite already existing value
    const TInt KTestValue100 = 100; // Imaginary test value
    PropertyHelper::SetErrorCode( KErrAlreadyExists );
    iPlugin->DefinePropertyL( KTestAvailability, 
                              RProperty::EInt, 
                              KTestValue100 );
    PropertyHelper::SetErrorCode( KErrNone );
    
    User::LeaveIfError( RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                    KTestAvailability,
                    keyValueInPS ) );
    EUNIT_ASSERT_EQUALS( keyValueInPS, KTestValue ) // Value not changed
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusAoPlugin::DefinePropertyL_DesCPropertyL()
    {
    // Normal succcesfull case
    const TInt KTestAvailability( 0x10282300 ); // Imaginary test value
    _LIT( KTestValue, "TestValue" ); // Imaginary test value
    iPlugin->DefinePropertyL( KTestAvailability, RProperty::EText, KTestValue );
    
    TBuf<100> keyValueInPS( KNullDesC );
    User::LeaveIfError( RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                                        KTestAvailability,
                                        keyValueInPS ) );
    EUNIT_ASSERT_EQUALS( keyValueInPS, KTestValue )    
    
    // When trying to define existing property, we are not supposed to 
    // overwrite already existing value
    _LIT( KNewTestValue, "NewTestValue" ); // Imaginary test value
    PropertyHelper::SetErrorCode( KErrAlreadyExists );
    iPlugin->DefinePropertyL( KTestAvailability, 
                              RProperty::EText, 
                              KNewTestValue );
    PropertyHelper::SetErrorCode( KErrNone );
    
    User::LeaveIfError( RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                    KTestAvailability,
                    keyValueInPS ) );
    EUNIT_ASSERT_EQUALS( keyValueInPS, KTestValue ) // Value not changed
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusAoPlugin::DeletePropertiesL()
    {    
    // Initialize     
    iPlugin->DefinePropertiesL();
    // and delete all
    iPlugin->DeleteProperties();
    
    TInt res = KErrNone;
    TInt ret = KErrNone;
    // camera availability
    ret = RProperty::Get( NMusResourceApi::KCategoryUid,
                          NMusResourceApi::KCameraAvailability,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    ret = KErrNone;


    // keybad availability
    ret = RProperty::Get( NMusResourceApi::KCategoryUid,
                          NMusResourceApi::KKeypadAvailability,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    ret = KErrNone;
    
    // videoplayer availability
    ret = RProperty::Get( NMusResourceApi::KCategoryUid,
                          NMusResourceApi::KVideoPlayerAvailability,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    ret = KErrNone;

    // camera information property
    ret = RProperty::Get( NMusResourceApi::KCategoryUid,
                          NMusResourceApi::KCameraInformation,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    ret = KErrNone;
    
    // call event
    ret = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallEvent,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    ret = KErrNone;
    
    // phone number
    /* hard to provide stub impl
    TBuf<15> telnumber; // TODO change to des
    ret = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusTelNumber,
                          telnumber );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    */
    
    ret = KErrNone;

    // Mus allowed
    ret = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMUSForbidden,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    ret = KErrNone;

    // Call direction
    ret = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallDirection,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )
    
    // Call privacy
    ret = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMUSPrivacy,
                          res );
    EUNIT_ASSERT_EQUALS( ret, KErrNotFound )


    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusAoPlugin::UT_HandleServerCommandLL()
    {
    TAny* a = iPlugin->HandleServerCommandL( EAOManagerPluginStart, NULL );
    EUNIT_ASSERT( a )
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusAoPlugin,
    "UT_CMusAoPlugin",
    "UNIT")

EUNIT_TEST(
    "UT_NewLL",
    "CMusAoPlugin",
    "NewL",
    "FUNCTIONALITY",
    SetupL, NewLL, Teardown)
        
EUNIT_TEST(
    "DefinePropertiesLL",
    "CMusAoPlugin",
    "DefinePropertiesL",
    "FUNCTIONALITY",
    SetupL, DefinePropertiesLL, Teardown)

EUNIT_TEST(
    "DefinePropertyL (int param)",
    "CMusAoPlugin",
    "DefinePropertyL",
    "FUNCTIONALITY",
    SetupL, DefinePropertyL_IntPropertyL, Teardown)
    
EUNIT_TEST(
    "DefinePropertyL (desc param)",
    "CMusAoPlugin",
    "DefinePropertyL",
    "FUNCTIONALITY",
    SetupL, DefinePropertyL_DesCPropertyL, Teardown)
    
EUNIT_TEST(
    "DeleteProperties",
    "CMusAoPlugin",
    "DeleteProperties",
    "FUNCTIONALITY",
    SetupL, DeletePropertiesL, Teardown)
    
    

EUNIT_TEST(
    "HandleServerCommandL",
    "CMusAoPlugin",
    "HandleServerCommandL",
    "FUNCTIONALITY",
    SetupL, UT_HandleServerCommandLL, Teardown)


EUNIT_END_TEST_TABLE







