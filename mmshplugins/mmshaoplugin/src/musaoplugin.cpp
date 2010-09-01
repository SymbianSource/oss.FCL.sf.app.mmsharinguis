/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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


#include "musresourceproperties.h"
#include "mussesseioninformationapi.h"
#include "mussettingskeys.h"
#include "mussessionproperties.h"
#include "mussettings.inl"
#include "musaoplugin.h"
#include "mustsypropertymonitor.h"
// Enable the below line if Kodiak Ptt has to be monitered
// #include "muspttcallmonitor.h"
#include "muspropertymonitor.h"
#include "mustsypropertymonitor.h"
#include "muslogger.h"

#include <AlwaysOnlineManagerCommon.h>
#include <AlwaysOnlineManagerClient.h>
#include <mmtsy_names.h>
#include <featmgr.h>

using namespace NMusSessionInformationApi;

// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusAoPlugin* CMusAoPlugin::NewL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusAoPlugin::NewL" )
    CMusAoPlugin* self = new (ELeave) CMusAoPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusAoPlugin::NewL" )
    return self;
    }

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusAoPlugin::CMusAoPlugin() :
    CAlwaysOnlineEComInterface()
    {     
    }

// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAoPlugin::ConstructL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusAoPlugin::ConstructL" )
	FeatureManager::InitializeLibL();
    TBool support = FeatureManager::FeatureSupported( KFeatureIdMultimediaSharing );
	FeatureManager::UnInitializeLib();
	if ( support )
		{
        DefinePropertiesL();
        User::LeaveIfError( iServer.Connect() );
        User::LeaveIfError( iPhone.Open( iServer, KMmTsyPhoneName() ) );        
        iTsyPropertyMonitor = CMusTsyPropertyMonitor::NewL(iPhone);    
        iPropertyMonitor = CMusPropertyMonitor::NewL();    
        // Enable the below line if Kodiak Ptt has to be monitered
        // iPttCallMonitor = CMusPttCallMonitor::NewL(*iLineMonitor); 
		}
    MUS_LOG( "mus: [MUSAO]  <- CMusAoPlugin::ConstructL" )
    }

// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAoPlugin::~CMusAoPlugin()
    {
    MUS_LOG( "-> CMusAoPlugin::~CMusAoPlugin" )
    // should never come here, because service should always be on
    delete iPropertyMonitor;
    delete iTsyPropertyMonitor;
    // Enable the below line if Kodiak Ptt has to be monitered
    // delete iPttCallMonitor;
    DeleteProperties();
    iPhone.Close();
    iServer.Close();
    MUS_LOG( "<- CMusAoPlugin::~CMusAoPlugin" )
    }


// -----------------------------------------------------------------------------
// CMusAoPlugin::DefineResourcePropertiesL()
// Note : Ignore Define Errors . Because if define fails for sure Set
//        will fail too . But there are some cases in Define Error should
//        be ignored like KErrAlreadyExists , KErrPersmissionDenied.So It
//        could be better if we dont care Define Error but care about Set Err.
// -----------------------------------------------------------------------------
//
void CMusAoPlugin::DefinePropertiesL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusAoPlugin::DefineResourceProperties" )
    // define resource properties
    // they need to be defined here to make them exist always
	// Read from cenrep key about the availability of camera and keypad
	// if the availability is dynamic then define a key else no.
	MusSettingsKeys::TAvailability camera = MusSettingsKeys::EAvailabilityStatic;
	MusSettingsKeys::TAvailability keypad = MusSettingsKeys::EAvailabilityStatic ;
    MultimediaSharingSettings::ResourceAvailability(camera,keypad);         
    if( camera == MusSettingsKeys::EAvailabilityDynamic )
    	{
        DefinePropertyL(NMusResourceApi::KCameraAvailability,
                        RProperty::EInt,( TInt ) NMusResourceApi::ENotAvailable);
    	}	
	if( keypad == MusSettingsKeys::EAvailabilityDynamic )
		{		
        DefinePropertyL(NMusResourceApi::KKeypadAvailability,
                        RProperty::EInt,( TInt ) NMusResourceApi::ENotAvailable);      	
		}    		    
    DefinePropertyL(NMusResourceApi::KCameraInformation,
                    RProperty::EInt,( TInt ) NMusResourceApi::EUsePrimaryCamera);   
    DefinePropertyL(NMusSessionInformationApi::KMusCallEvent,
                    RProperty::EInt,( TInt ) NMusSessionInformationApi::ENoCall); 
    DefinePropertyL(NMusSessionInformationApi::KMusCallCount,RProperty::EInt,0); 
    _LIT(KEmptyTelNumber,"");
    DefinePropertyL(NMusSessionInformationApi::KMusTelNumber,
                    RProperty::EText,KEmptyTelNumber); 
    DefinePropertyL(NMusSessionInformationApi::KMUSForbidden,
                 RProperty::EInt,( TInt ) NMusSessionInformationApi::EMUSAllowed); 
    DefinePropertyL(NMusSessionInformationApi::KMusCallDirection,
                RProperty::EInt,( TInt ) NMusSessionInformationApi::ENoDirection);  
    DefinePropertyL(NMusSessionInformationApi::KMUSPrivacy,
                RProperty::EInt,( TInt ) NMusSessionInformationApi::EPrivacyOff);  
    DefinePropertyL(KMusClirSetting,
                RProperty::EInt,( TInt ) ESendOwnNumber);  
                              
 
    MUS_LOG( "mus: [MUSAO]  <- CMusAoPlugin::DefineResourceProperties" )
    }

// -----------------------------------------------------------------------------
// CMusAoPlugin::DefineProperty()
// -----------------------------------------------------------------------------
//
void CMusAoPlugin::DefinePropertyL(TInt aKey,RProperty::TType aType,TInt aVal)
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusAoPlugin::DefinePropertyL( intValue )" )
    
    TInt error = RProperty::Define( NMusResourceApi::KCategoryUid,
                                     aKey,
                                     aType );
    MUS_LOG2( "mus: [MUSAO]     Key=%d defining tried, return=%d", aKey, error )
    
    if ( error == KErrNone )
        {
        error = RProperty::Set( NMusResourceApi::KCategoryUid, aKey, aVal );
        MUS_LOG2( "mus: [MUSAO]     RProperty value %d set, return = %d",
                  aVal, error )
        User::LeaveIfError( error );
        }
    else
        {
        MUS_LOG( "mus: [MUSAO]     Defining failed, do not set value" )
        }
    
    MUS_LOG( "mus: [MUSAO]  <- CMusAoPlugin::DefinePropertyL( intValue )" )
    }

// -----------------------------------------------------------------------------
// CMusAoPlugin::DefineProperty()
// -----------------------------------------------------------------------------
//
void CMusAoPlugin::DefinePropertyL( TInt aKey,
                                    RProperty::TType aType,
                                    const TDesC& aVal )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusAoPlugin::DefinePropertyL( DesCValue )" )
    
    TInt error = RProperty::Define( NMusResourceApi::KCategoryUid,
                                     aKey,
                                     aType );
    MUS_LOG2( "mus: [MUSAO]     Key=%d defining tried, return=%d", aKey, error )  
    
    if ( error == KErrNone )
        {
        error = RProperty::Set( NMusResourceApi::KCategoryUid, aKey, aVal);
        MUS_LOG_TDESC( "mus: [MUSAO]     Tried to set RProperty value: ", aVal )
        MUS_LOG1("mus: [MUSAO]     return value = %d",error )
        User::LeaveIfError( error );         
        }
    else
        {
        MUS_LOG( "mus: [MUSAO]     Defining failed, do not set value" )
        }
    
    MUS_LOG( "mus: [MUSAO]  <- CMusAoPlugin::DefinePropertyL( DesCValue )" )
    }

// -----------------------------------------------------------------------------
// CMusAoPlugin::DeleteResourceProperties()
// Note : There is no need of caring the return value in deleting
//        This will be called when AO Plugin destroyed which is rare to happen
//        Any unwanted deletion of AO Plugin should leave some PS Keys open.
//        But this is OK if we ignore KErrAlreadyExists while defining next time.
// -----------------------------------------------------------------------------
//
void CMusAoPlugin::DeleteProperties()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusAoPlugin::DeleteResourcePropertiesL" )
    // Delete resource properties
    // they need to be defined here to make them exist always
    DeleteProperty(NMusResourceApi::KCameraAvailability);
    DeleteProperty(NMusResourceApi::KKeypadAvailability);    
    DeleteProperty(NMusResourceApi::KCameraInformation);
    DeleteProperty(NMusSessionInformationApi::KMusCallEvent);
    DeleteProperty(NMusSessionInformationApi::KMusCallCount);
    DeleteProperty(NMusSessionInformationApi::KMusTelNumber);
    DeleteProperty(NMusSessionInformationApi::KMUSForbidden);
    DeleteProperty(NMusSessionInformationApi::KMusCallDirection);
    DeleteProperty(NMusSessionInformationApi::KMUSPrivacy);
    DeleteProperty(KMusClirSetting);
    MUS_LOG( "mus: [MUSAO]  <- CMusAoPlugin::DeleteResourcePropertiesL" )
    }

// -----------------------------------------------------------------------------
// CMusAoPlugin::DeleteProperty()
// -----------------------------------------------------------------------------
//
void CMusAoPlugin::DeleteProperty(TInt aKey)
    {
    TInt retVal = RProperty::Delete( NMusResourceApi::KCategoryUid,aKey); 
    MUS_LOG2( "mus: [MUSAO]     RProperty::Delete Type=%d return=%d",\
                                                           aKey, retVal )                           
    }

// -----------------------------------------------------------------------------
// CMusAoPlugin::HandleServerCommandL
// CallBack from base class.
// -----------------------------------------------------------------------------
//
TAny* CMusAoPlugin::HandleServerCommandL( TInt /*aCommand*/,
                                                      TDesC8* /*aParameters*/  )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusAoPlugin::HandleServerCommandL" )
    MUS_LOG( "mus: [MUSAO]  <- CMusAoPlugin::HandleServerCommandL: KErrNone" )
    return &iError;;
    }


// End of file
