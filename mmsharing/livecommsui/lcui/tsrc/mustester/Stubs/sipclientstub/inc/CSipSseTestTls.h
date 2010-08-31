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



#ifndef CSIPSSETESTTLS_H
#define CSIPSSETESTTLS_H

#include <e32base.h>
#include <bamdesca.h>
#include <stringpool.h>
#include <sipprofileregistryobserver.h>

#include <etelmm.h>
#include <Etel3rdParty.h>

class CSIPMessageElements;
class CSIPResponseElements;
class RStringF;
class CSdpDocument;
class CSsfInvitation;
class TRequestStatus;
class CConnMonEventBase;
class CSIPRequestElements;

//class CTelephony;
//class MSIPProfileRegistryObserver;

// STUB stuff

typedef TInt TSIPProfileBehavior;
const TSIPProfileBehavior KRegistryLeaveAtConstruct = 1;
const TSIPProfileBehavior KSIPProfileIdFetchFails = 2;
const TSIPProfileBehavior KSIPIapIdFetchFails = 3;
const TSIPProfileBehavior KSIPConnectionLeaveAtConstruct = 4;

class CSipSseTestTls : public CBase
	{

public:

    static void OpenL();
    
    static void Close();
    
    static CSipSseTestTls* Storage();
    
    ~CSipSseTestTls();
    
    void Reset();
    
    TInt Set(TUint32 aKey, TInt aValue);
    
    TInt Get(TUint32 aKey, TInt& aValue);
    
    void SetError( TInt aError );
    
    TInt Error() const;
    
    void Clear();
    
    void SetRequestStatus( TRequestStatus& iStatus );
    
    TBool RegisterStatus(); 
    
    void SetRegisterStatus( TBool aStatus );
    
    TBool RegisterSent(); 
    
    void StoreRegistry( MSIPProfileRegistryObserver& aRegistry );
                      
    //RCommMon       
    TInt ConnectionCount();
   
    void SetConnectionCount( TInt aCount );
    
    void ConnectionInfo( const TUint& aIndex, 
                         TUint& aConnectionId, 
                         TUint& aSubConnectionCount );
                     
    void SetConnectionInfo( TUint aConnectionId, 
                            TUint aSubConnectionCount );
                            
    void GetUintAttribute(  const TUint aConnectionId, 
                            const TUint aSubConnectionId, 
                            const TUint aAttribute, 
                            TUint& aValue, 
                            TRequestStatus& aStatus );
   
    void SetUintAttribute(  const TUint aConnectionId, 
                            const TUint aSubConnectionId, 
                            const TUint aAttribute, 
                            TUint aValue, 
                            TRequestStatus& aStatus );
                            
    void GetIntAttribute(  const TUint aConnectionId, 
                            const TUint aSubConnectionId, 
                            const TUint aAttribute, 
                            TInt& aValue, 
                            TRequestStatus& aStatus );
   
    void SetIntAttribute(  const TUint aConnectionId, 
                            const TUint aSubConnectionId, 
                            const TUint aAttribute, 
                            TInt aValue, 
                            TRequestStatus& aStatus );
                            
    void SetEvent(const CConnMonEventBase& aConnMonEvent );
    
    
    const CConnMonEventBase& Event();
    
    
    void SetRegistrationStatus( RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus );
    void SetRegistrationStatus( RMobilePhone::TMobilePhoneRegistrationStatus* aRegStatus );
    
    void RegistrationStatus( RMobilePhone::TMobilePhoneRegistrationStatus& aReqStatus );
   
    void SetPhoneNetworkModeStatus( RMobilePhone::TMobilePhoneNetworkMode aStatus );
    
    RMobilePhone::TMobilePhoneNetworkMode& PhoneNetworkModeStatus();
    
    void ClientRequest( CSIPRequestElements* aRequest );
    void ClientResponse( RStringF aMethod, CSIPResponseElements* aResponse );

private:

    CSipSseTestTls();

public://data
	
    // Stubs leave/return this error
    TInt iError;  
    static TBool iSipRegisterStatus;
    MSIPProfileRegistryObserver* iRegistry;
    //RCommMon 
    TInt iConnectionCount;
    //ConnectionInfo
    TUint iConnectionId; 
    TUint iSubConnectionCount;
    //UintAttribute
    TUint iUintAttributeValue;
    //UintAttribute
    TUint iIntAttributeValue;
    TRequestStatus iRequestStatus;
    CConnMonEventBase* iConnMonEventBase;
    
    RMobilePhone::TMobilePhoneRegistrationStatus iPhoneRegistrationStatus;
    RMobilePhone::TMobilePhoneRegistrationStatus* iPhoneRegistrationStatusPointer;
    RMobilePhone::TMobilePhoneNetworkMode iPhoneNetworkMode;

    CSIPRequestElements* iClientRequest;
    RStringF iResponseToRequest;
    CSIPResponseElements* iClientResponse;
    RArray<TUint32> iKeys;
    RArray<TInt> iValues;
    TSIPProfileBehavior iRegistryBehavior;
    TSIPProfileBehavior iProfileBehavior;    
    TSIPProfileBehavior iConnectionBehavior;
    };



#endif 

// End of File
