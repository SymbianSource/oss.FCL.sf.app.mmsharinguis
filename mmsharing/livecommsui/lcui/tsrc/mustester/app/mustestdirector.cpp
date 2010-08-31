/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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


// USER
#include "mustestdirector.h"
#include <musmanagercommon.h>
#include "mussessionproperties.h"
#include "musresourceproperties.h"
#include "mussesseioninformationapi.h"
#include "mussettingskeys.h"
#include "mussettings.inl"

#include <e32property.h>
#include <e32base.h>
#include <e32err.h>
#include <QDebug>

_LIT(KTestTelNumber,"+358504869612");
_LIT( KTestCallProvider,"MultimediaSharing" );
_LIT( KTestRemoteSipAddress,"sip:siva@musworld.com" );
_LIT( KTestRemoteSipAddressProposal,"sip:wondering@nokia.com,sip:package@gone.com" );
_LIT( KTestContactName,"Michel Jackson" );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusTestDirector::CMusTestDirector() 
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

CMusTestDirector* CMusTestDirector::NewL()
    {
    CMusTestDirector* self = new( ELeave )CMusTestDirector();
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

void CMusTestDirector::ConstructL()
    {
    DefinePropertiesL();
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

CMusTestDirector::~CMusTestDirector() 
    {
    DeleteProperties();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

void CMusTestDirector::ConfigureLiveSharing( ) 
    {
    SetUseCase( MultimediaSharing::EMusLiveVideo );
    
    TInt error = RProperty::Set( NMusResourceApi::KCategoryUid, NMusSessionInformationApi::KMusTelNumber, KTestTelNumber);    
    MUS_LOG1("mus: [TEST]     return value = %d",error )
    
    error = RProperty::Set( NMusResourceApi::KCategoryUid, NMusSessionInformationApi::KMusCallDirection, NMusSessionInformationApi::ECallOrginated);    
    MUS_LOG1("mus: [TEST]     return value = %d",error )
    
    error = RProperty::Set( NMusResourceApi::KCategoryUid, NMusSessionInformationApi::KMUSCallProvider, KTestCallProvider);    
    MUS_LOG1("mus: [TEST]     return value = %d",error )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

void CMusTestDirector::ConfigureReceiveSharing( ) 
    {
    SetUseCase( MultimediaSharing::EMusReceive );
    
    TInt error = RProperty::Set( NMusResourceApi::KCategoryUid, NMusSessionInformationApi::KMusTelNumber, KTestTelNumber);    
    MUS_LOG1("mus: [TEST]     return value = %d",error )
    
    error = RProperty::Set( NMusResourceApi::KCategoryUid, NMusSessionInformationApi::KMusCallDirection, NMusSessionInformationApi::ECallOrginated);    
    MUS_LOG1("mus: [TEST]     return value = %d",error )
    
    error = RProperty::Set( NMusResourceApi::KCategoryUid, NMusSessionInformationApi::KMUSCallProvider, KTestCallProvider);    
    MUS_LOG1("mus: [TEST]     return value = %d",error )
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

void CMusTestDirector::SetUseCase( MultimediaSharing::TMusUseCase aUseCase ) 
    {
    TInt err = RProperty::Set( NMusSessionApi::KCategoryUid, NMusSessionApi::KUseCase, ( TInt )aUseCase);   
    }

// -----------------------------------------------------------------------------
// CMusAoPlugin::DefineResourcePropertiesL()
// Note : Ignore Define Errors . Because if define fails for sure Set
//        will fail too . But there are some cases in Define Error should
//        be ignored like KErrAlreadyExists , KErrPersmissionDenied.So It
//        could be better if we dont care Define Error but care about Set Err.
// -----------------------------------------------------------------------------
//
void CMusTestDirector::DefinePropertiesL()
    {
    MUS_LOG( "mus: [TEST]  -> CMusAoPlugin::DefineResourceProperties") ;
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
    
    DefinePropertyL(NMusSessionInformationApi::KMusTelNumber,
                    RProperty::EText,KTestTelNumber); 
    DefinePropertyL(NMusSessionInformationApi::KMUSForbidden,
                 RProperty::EInt,( TInt ) NMusSessionInformationApi::EMUSAllowed); 
    DefinePropertyL(NMusSessionInformationApi::KMusCallDirection,
                RProperty::EInt,( TInt ) NMusSessionInformationApi::ENoDirection);  
    
    DefinePropertyL(NMusSessionInformationApi::KMUSCallProvider,
                        RProperty::EText,KTestCallProvider);

    DefineSessionPropertyL(NMusSessionApi::KUseCase,
                            RProperty::EInt,MultimediaSharing::EMusLiveVideo);

    DefineSessionPropertyL(NMusSessionApi::KStatus,
                            RProperty::EInt,MultimediaSharing::EMultimediaSharingAvailable);

    DefineSessionPropertyL(NMusSessionApi::KTelNumber,
                            RProperty::EText,KTestTelNumber);

    DefineSessionPropertyL(NMusSessionApi::KRemoteSipAddress,
                            RProperty::EText,KTestRemoteSipAddress);
    
    DefineSessionPropertyL(NMusSessionApi::KRemoteSipAddressProposal,
                                RProperty::EText,KTestRemoteSipAddressProposal);
    
    DefineSessionPropertyL(NMusSessionApi::KContactId,
                                    RProperty::EInt,1);
    
    DefineSessionPropertyL(NMusSessionApi::KContactName,
                                        RProperty::EText,KTestContactName);
    
    DefineSessionPropertyL(NMusSessionApi::KSipProfileId,
                                            RProperty::EInt,1);
    
    CRepository* repository = CRepository::NewL(MusSettingsKeys::KRepositoryUid);
    TInt uid = 0;
    TBool exist = (repository->Get(MusSettingsKeys::KEncodingDevice,uid) == KErrNone);
    if( !exist )
        {
        TInt uid = 1234;
        repository->Create(MusSettingsKeys::KEncodingDevice,uid);
        TUint32 modifiedKeyCount(0);
        User::LeaveIfError(repository->CommitTransaction(modifiedKeyCount));
        }
   delete repository;
   
    MUS_LOG( "mus: [TEST]  <- CMusAoPlugin::DefineResourceProperties" )
    }

// -----------------------------------------------------------------------------
// CMusAoPlugin::DefineProperty()
// -----------------------------------------------------------------------------
//
void CMusTestDirector::DefineSessionPropertyL(TInt aKey,RProperty::TType aType,TInt aVal)
    {
    MUS_LOG( "mus: [TEST]  -> CMusTestDirector::DefineSessionPropertyL( intValue )" )
    
    TInt error = RProperty::Define( NMusSessionApi::KCategoryUid,
                                     aKey,
                                     aType );
    MUS_LOG2( "mus: [TEST]     Key=%d defining tried, return=%d", aKey, error )
    
    if ( error == KErrNone )
        {
        error = RProperty::Set( NMusSessionApi::KCategoryUid, aKey, aVal );
        MUS_LOG2( "mus: [TEST]     RProperty value %d set, return = %d",
                  aVal, error )
        User::LeaveIfError( error );
        }
    else
        {
        MUS_LOG( "mus: [TEST]     Defining failed, do not set value" )
        }
    
    MUS_LOG( "mus: [TEST]  <- CMusTestDirector::DefineSessionPropertyL( intValue )" )
    }

// -----------------------------------------------------------------------------
// CTESTPlugin::DefineProperty()
// -----------------------------------------------------------------------------
//
void CMusTestDirector::DefineSessionPropertyL( TInt aKey,
                                    RProperty::TType aType,
                                    const TDesC& aVal )
    {
    MUS_LOG( "mus: [TEST]  -> CTESTPlugin::DefineSessionPropertyL( DesCValue )" )
    
    TInt error = RProperty::Define( NMusSessionApi::KCategoryUid,
                                     aKey,
                                     aType );
    MUS_LOG2( "mus: [TEST]     Key=%d defining tried, return=%d", aKey, error )  
    
    if ( error == KErrNone )
        {
        error = RProperty::Set( NMusSessionApi::KCategoryUid, aKey, aVal);
        MUS_LOG_TDESC( "mus: [TEST]     Tried to set RProperty value: ", aVal )
        MUS_LOG1("mus: [TEST]     return value = %d",error )
        User::LeaveIfError( error );         
        }
    else
        {
        MUS_LOG( "mus: [TEST]     Defining failed, do not set value" )
        }
    
    MUS_LOG( "mus: [TEST]  <- CTESTPlugin::DefineSessionPropertyL( DesCValue )" )
    }

// -----------------------------------------------------------------------------
// CMusAoPlugin::DefineProperty()
// -----------------------------------------------------------------------------
//
void CMusTestDirector::DefinePropertyL(TInt aKey,RProperty::TType aType,TInt aVal)
    {
    MUS_LOG( "mus: [TEST]  -> CMusAoPlugin::DefinePropertyL( intValue )" )
    
    TInt error = RProperty::Define( NMusResourceApi::KCategoryUid,
                                     aKey,
                                     aType );
    MUS_LOG2( "mus: [TEST]     Key=%d defining tried, return=%d", aKey, error )
    
    if ( error == KErrNone )
        {
        error = RProperty::Set( NMusResourceApi::KCategoryUid, aKey, aVal );
        MUS_LOG2( "mus: [TEST]     RProperty value %d set, return = %d",
                  aVal, error )
        User::LeaveIfError( error );
        }
    else
        {
        MUS_LOG( "mus: [TEST]     Defining failed, do not set value" )
        }
    
    MUS_LOG( "mus: [TEST]  <- CTESTPlugin::DefinePropertyL( intValue )" )
    }

// -----------------------------------------------------------------------------
// CTESTPlugin::DefineProperty()
// -----------------------------------------------------------------------------
//
void CMusTestDirector::DefinePropertyL( TInt aKey,
                                    RProperty::TType aType,
                                    const TDesC& aVal )
    {
    MUS_LOG( "mus: [TEST]  -> CTESTPlugin::DefinePropertyL( DesCValue )" )
    
    TInt error = RProperty::Define( NMusResourceApi::KCategoryUid,
                                     aKey,
                                     aType );
    MUS_LOG2( "mus: [TEST]     Key=%d defining tried, return=%d", aKey, error )  
    
    if ( error == KErrNone )
        {
        error = RProperty::Set( NMusResourceApi::KCategoryUid, aKey, aVal);
        MUS_LOG_TDESC( "mus: [TEST]     Tried to set RProperty value: ", aVal )
        MUS_LOG1("mus: [TEST]     return value = %d",error )
        User::LeaveIfError( error );         
        }
    else
        {
        MUS_LOG( "mus: [TEST]     Defining failed, do not set value" )
        }
    
    MUS_LOG( "mus: [TEST]  <- CTESTPlugin::DefinePropertyL( DesCValue )" )
    }

// -----------------------------------------------------------------------------
// CTESTPlugin::DeleteSessionProperties()
// -----------------------------------------------------------------------------
//
void CMusTestDirector::DeleteSessionProperties()
    {
    TInt retVal = RProperty::Delete( NMusSessionApi::KCategoryUid,NMusSessionApi::KStatus);
    retVal = RProperty::Delete( NMusSessionApi::KCategoryUid,NMusSessionApi::KRemoteSipAddress);
    retVal = RProperty::Delete( NMusSessionApi::KCategoryUid,NMusSessionApi::KRemoteSipAddressProposal); 
    retVal = RProperty::Delete( NMusSessionApi::KCategoryUid,NMusSessionApi::KContactId); 
    retVal = RProperty::Delete( NMusSessionApi::KCategoryUid,NMusSessionApi::KContactName);
    }
// -----------------------------------------------------------------------------
// CTESTPlugin::DeleteResourceProperties()
// Note : There is no need of caring the return value in deleting
//        This will be called when AO Plugin destroyed which is rare to happen
//        Any unwanted deletion of AO Plugin should leave some PS Keys open.
//        But this is OK if we ignore KErrAlreadyExists while defining next time.
// -----------------------------------------------------------------------------
//
void CMusTestDirector::DeleteProperties()
    {
    MUS_LOG( "mus: [TEST]  -> CTESTPlugin::DeleteResourcePropertiesL" )
    // Delete resource properties
    // they need to be defined here to make them exist always
    DeleteProperty(NMusResourceApi::KCameraAvailability);
    DeleteProperty(NMusResourceApi::KKeypadAvailability);    
    DeleteProperty(NMusResourceApi::KCameraInformation);
    DeleteProperty(NMusSessionInformationApi::KMusCallEvent);
    DeleteProperty(NMusSessionInformationApi::KMusCallCount);
    DeleteProperty(NMusSessionInformationApi::KMusTelNumber);
    DeleteProperty(NMusSessionInformationApi::KMUSCallProvider);
    DeleteProperty(NMusSessionInformationApi::KMUSForbidden);
    DeleteProperty(NMusSessionInformationApi::KMusCallDirection);
    MUS_LOG( "mus: [TEST]  <- CTESTPlugin::DeleteResourcePropertiesL" )
    }

// -----------------------------------------------------------------------------
// CTESTPlugin::DeleteProperty()
// -----------------------------------------------------------------------------
//
void CMusTestDirector::DeleteProperty(TInt aKey)
    {
    TInt retVal = RProperty::Delete( NMusResourceApi::KCategoryUid,aKey); 
    MUS_LOG2( "mus: [TEST]     RProperty::Delete Type=%d return=%d",\
                                                           aKey, retVal )                           
    }
