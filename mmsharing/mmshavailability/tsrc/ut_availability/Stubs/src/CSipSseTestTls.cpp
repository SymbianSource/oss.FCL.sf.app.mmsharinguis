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


#include "CSipSseTestTls.h"
#include "musavaobserver.h"
#include <sipstrings.h>
#include <SipStrConsts.h>
#include <sipprofileregistry.h>
#include <sipprofileregistryobserver.h>
#include "SipResponseElements.h"
#include "siprequestelements.h"
#include "mussettingskeys.h"


TBool CSipSseTestTls::iSipRegisterStatus = EFalse;


void CSipSseTestTls::OpenL ()
	{
    CSipSseTestTls* self = new (ELeave) CSipSseTestTls();
    self->ClientRequest( NULL );
    self->ClientResponse( SIPStrings::StringF( SipStrConsts::EEmpty ), NULL );
    Dll::SetTls( self );

    User::LeaveIfError( self->Set ( MusSettingsKeys::KActivation,
                            MusSettingsKeys::EAlwaysActive ) );
    User::LeaveIfError( self->Set ( MusSettingsKeys::KAuditoryNotification,
                            MusSettingsKeys::EAuditoryNotificationOff ) );
    User::LeaveIfError( self->Set ( MusSettingsKeys::KPopupNotification,
                            MusSettingsKeys::EPopupNotificationOff ) );
    User::LeaveIfError( self->Set ( MusSettingsKeys::KPopupNotificationType,
                            MusSettingsKeys::ENotificationTypeQueryUser ) );
    User::LeaveIfError( self->Set ( MusSettingsKeys::KEdgeDtmSupport,
                            MusSettingsKeys::EDtmModeNotAllowed ) );
    User::LeaveIfError( self->Set ( MusSettingsKeys::KForceInternetSignaling,
                            MusSettingsKeys::EFollowProfileConfiguration ) );
    User::LeaveIfError( self->Set ( MusSettingsKeys::KAutoRecord,
                            MusSettingsKeys::EAutoRecordOn ) );
    User::LeaveIfError( self->Set ( MusSettingsKeys::KSipProfileId,
                            0  ) ); //default profile
    User::LeaveIfError( self->Set ( MusSettingsKeys::KUiOrientation,
                            MusSettingsKeys::EPortrait ) );                    
    User::LeaveIfError( self->Set ( MusSettingsKeys::KCapabilityQuery,
                            MusSettingsKeys::ENoOptions ) );
    User::LeaveIfError( self->Set ( MusSettingsKeys::KAllowOnlyIn3GNetwork,
                            MusSettingsKeys::EAllowedAllBearers ) );
                            
	}

void CSipSseTestTls::Close ()
	{
	CSipSseTestTls* self = Storage();
    delete self;
    Dll::SetTls( NULL );
	}

	
CSipSseTestTls* CSipSseTestTls::Storage()
	{
	return static_cast<CSipSseTestTls*>(Dll::Tls());
	}

CSipSseTestTls::CSipSseTestTls()
    : iError( KErrNone )
	{
	}
	
CSipSseTestTls::~CSipSseTestTls()
	{
   	delete iClientRequest;
	delete iClientResponse;
	iKeys.Reset();
    iValues.Reset();
	}
    
void CSipSseTestTls::Reset()
    {
    Storage()->Set ( MusSettingsKeys::KActivation,
        MusSettingsKeys::EAlwaysActive );
    Storage()->Set ( MusSettingsKeys::KAuditoryNotification,
        MusSettingsKeys::EAuditoryNotificationOff );
    Storage()->Set ( MusSettingsKeys::KPopupNotification,
        MusSettingsKeys::EPopupNotificationOff );
    Storage()->Set ( MusSettingsKeys::KPopupNotificationType,
        MusSettingsKeys::ENotificationTypeQueryUser );
    Storage()->Set ( MusSettingsKeys::KEdgeDtmSupport,
        MusSettingsKeys::EDtmModeNotAllowed );
    Storage()->Set ( MusSettingsKeys::KForceInternetSignaling,
        MusSettingsKeys::EFollowProfileConfiguration );
    Storage()->Set ( MusSettingsKeys::KAutoRecord,
        MusSettingsKeys::EAutoRecordOn );
    Storage()->Set ( MusSettingsKeys::KSipProfileId,
        0  ); //default profile
    Storage()->Set ( MusSettingsKeys::KUiOrientation,
        MusSettingsKeys::EPortrait );
    Storage()->Set ( MusSettingsKeys::KCapabilityQuery,
        MusSettingsKeys::ENoOptions );
        
   	Storage()->Set ( MusSettingsKeys::KAllowOnlyIn3GNetwork,
	   	MusSettingsKeys::EAllowedAllBearers );       

    iRegistryBehavior = 0;
    iProfileBehavior = 0;
    iConnectionBehavior = 0;
    }
    
TInt CSipSseTestTls::Set(TUint32 aKey, TInt aValue)
    {
    // Setting of any other value
    for ( TInt i = 0; i < iKeys.Count(); ++i )
        {
        if ( iKeys[i] == aKey )
            {
            iValues[i] = aValue;
            return KErrNone;
            }
        }
    
    // No wise error handling implemented
    TInt error = iKeys.Append( aKey );
    if ( error == KErrNone )
        {
        error = iValues.Append( aValue );
        }
        
    return error;
    }
    
TInt CSipSseTestTls::Get(TUint32 aKey, TInt& aValue)
    {
    for ( TInt i = 0; i < iKeys.Count(); ++i )
        {
        if ( iKeys[i] == aKey )
            {
            aValue = iValues[i];
            return KErrNone;
            }
        }

    return KErrNotFound;
    }
    
void CSipSseTestTls::SetError( TInt aError )
    {
    iError = aError;
    }

TInt CSipSseTestTls::Error() const
    {
    return iError;
    }


void CSipSseTestTls::Clear()
    {
    iError = KErrNone;
    //iState = MMusAvaObserver::EMusAvaNotStateDefined; 
    iStatus = MMusAvaObserver::EMusAvaStatusNotExecuted;
    iName = MMusAvaObserver::EMusAvaNameNotDefined;
    iConnectionCount = 0;
    iSubConnectionCount = 0;
    iUintAttributeValue = 0;
    iConnectionId = 0; 
    iIntAttributeValue = 0;
    iRequestStatus = KErrNone;
    iPhoneRegistrationStatus = RMobilePhone::ERegistrationUnknown;
    iPhoneRegistrationStatusPointer = NULL;
    iPhoneNetworkMode = RMobilePhone::ENetworkModeUnknown;
    ClientRequest( NULL );
    //iResponseToRequest = SIPStrings::StringF( SipStrConsts::EEmpty );
    }

void CSipSseTestTls::SetRequestStatus( TRequestStatus& aStatus ) 
    {
    iRequestStatus = aStatus;
    }


TBool CSipSseTestTls::RegisterStatus() 
    {
    
    return iSipRegisterStatus; 
    }
void CSipSseTestTls::SetRegisterStatus( TBool aStatus )
    {
    iSipRegisterStatus = aStatus;
    }
TBool CSipSseTestTls::RegisterSent() 
    {
    if ( !iSipRegisterStatus )
        {
        iSipRegisterStatus = ETrue;
        }
    return iSipRegisterStatus; 
    }
    
void CSipSseTestTls::StoreRegistry( MSIPProfileRegistryObserver& aRegistry )
    {
    iRegistry = &aRegistry;
    }
    
void CSipSseTestTls::ProfileRegistryEventOccurred(TUint32 aProfileId, MSIPProfileRegistryObserver::TEvent aEvent )
    {
    iRegistry->ProfileRegistryEventOccurred( aProfileId,aEvent );
    }
/*	
void CSipSseTestTls::AvailabilityState( MMusAvaObserver::TAvailabilityState aState, 
                         MMusAvaObserver::TAvailabilityStatus aStatus,
                         MMusAvaObserver::TAvailabilityName aName )
	{
    iState = aState; 
    iStatus = aStatus;
    iName = aName;                      	
    }
  */  
void CSipSseTestTls::SetState( MMusAvaObserver::TAvailabilityStatus aStatus )
    {
    iStatus = aStatus;
    }
    
TInt CSipSseTestTls::ConnectionCount()
    {
    return iConnectionCount;
    }
void CSipSseTestTls::SetConnectionCount( TInt aCount )
    {
    iConnectionCount = aCount;
    }
    
void CSipSseTestTls::ConnectionInfo( const TUint& aIndex, 
                     TUint& aConnectionId, 
                     TUint& aSubConnectionCount )
    {
    aConnectionId = iConnectionId;
    aSubConnectionCount = iSubConnectionCount;
    }
                     
void CSipSseTestTls::SetConnectionInfo( TUint aConnectionId, 
                                        TUint aSubConnectionCount )
    {
    iConnectionId = aConnectionId;
    iSubConnectionCount = aSubConnectionCount;
    }
    
void CSipSseTestTls::GetUintAttribute(  const TUint aConnectionId, 
                                        const TUint aSubConnectionId, 
                                        const TUint aAttribute, 
                                        TUint& aValue, 
                                        TRequestStatus& aStatus )
    {
    aValue = iUintAttributeValue;
    aStatus = iRequestStatus;
    }
void CSipSseTestTls::SetUintAttribute(  TUint aConnectionId, 
                                        TUint aSubConnectionId, 
                                        TUint aAttribute, 
                                        TUint aValue, 
                                        TRequestStatus& aStatus )
    {
    iUintAttributeValue = aValue;
    iRequestStatus =  aStatus;
    }
    
void CSipSseTestTls::GetIntAttribute(  const TUint aConnectionId, 
                                        const TUint aSubConnectionId, 
                                        const TUint aAttribute, 
                                        TInt& aValue, 
                                        TRequestStatus& aStatus )
    {
    aValue = iIntAttributeValue;
    aStatus = iRequestStatus;
    }
void CSipSseTestTls::SetIntAttribute(  TUint aConnectionId, 
                                        TUint aSubConnectionId, 
                                        TUint aAttribute, 
                                        TInt aValue, 
                                        TRequestStatus& aStatus )
    {
    iIntAttributeValue = aValue;
    iRequestStatus =  aStatus;
    }
    
void CSipSseTestTls::SetEvent(const CConnMonEventBase& aConnMonEvent)
    {
    iConnMonEventBase = const_cast<CConnMonEventBase*>( &aConnMonEvent );
    }
    
const CConnMonEventBase& CSipSseTestTls::Event()
    {
    return *iConnMonEventBase;
    }
    
void CSipSseTestTls::SetRegistrationStatus( RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus )
    {
    iPhoneRegistrationStatus = aRegStatus;
    }
    
void CSipSseTestTls::SetRegistrationStatus( RMobilePhone::TMobilePhoneRegistrationStatus* aRegStatus )
    {
    iPhoneRegistrationStatusPointer = aRegStatus;
    }

void CSipSseTestTls::RegistrationStatus( RMobilePhone::TMobilePhoneRegistrationStatus& aReqStatus )
    {
    aReqStatus = iPhoneRegistrationStatus;
    }
    
void CSipSseTestTls::SetPhoneNetworkModeStatus( RMobilePhone::TMobilePhoneNetworkMode aStatus )
    {
    iPhoneNetworkMode = aStatus;
    }
    
RMobilePhone::TMobilePhoneNetworkMode& CSipSseTestTls::PhoneNetworkModeStatus()
    {
    return iPhoneNetworkMode;
    }
 
 void CSipSseTestTls::ClientRequest( CSIPRequestElements* aRequest )
    {
    delete iClientRequest;
    iClientRequest = aRequest;
    }
	
void CSipSseTestTls::ClientResponse( RStringF aMethod, CSIPResponseElements* aResponse )
    {
    iResponseToRequest = aMethod;
    delete iClientResponse;
    iClientResponse = aResponse;
    }

//end of file
