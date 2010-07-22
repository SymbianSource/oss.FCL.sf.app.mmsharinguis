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


#include "centralrepository.h"
#include "mussettingskeys.h"
#include "CSipSseTestTls.h"


/// CenRep UIDs and keys
const TUid KCRUidSIPClientResolverConfig = { 0x10282EE7 };

const TUid KCRUidInCallVolume = {0x102828B1};
const TUint32 KTelIncallEarVolume                           = 0x00000001;
const TUint32 KTelIncallLoudspeakerVolume                   = 0x00000002;


// Values for static variables
MusSettingsKeys::TOperatorVariant CRepository::iOperatorVariant =
            MusSettingsKeys::EStandard;
TBool CRepository::iEmulateSawfishRepository = EFalse;

// STUB stuff
/*
void CCRepositoryBehaviorTls::OpenL()
	{
    CCRepositoryBehaviorTls* self = new (ELeave) CCRepositoryBehaviorTls();
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
	}

void CCRepositoryBehaviorTls::Close ()
	{
	CCRepositoryBehaviorTls* self = Storage();
    delete self;
    Dll::SetTls( NULL );
	}

	
CCRepositoryBehaviorTls* CCRepositoryBehaviorTls::Storage()
	{
	return static_cast<CCRepositoryBehaviorTls*>(Dll::Tls());
	}

void CCRepositoryBehaviorTls::Reset()
    {
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KActivation,
                            MusSettingsKeys::EAlwaysActive ) );
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KAuditoryNotification,
                            MusSettingsKeys::EAuditoryNotificationOff ) );
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KPopupNotification,
                            MusSettingsKeys::EPopupNotificationOff ) );
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KPopupNotificationType,
                            MusSettingsKeys::ENotificationTypeQueryUser ) );
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KEdgeDtmSupport,
                            MusSettingsKeys::EDtmModeNotAllowed ) );
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KForceInternetSignaling,
                            MusSettingsKeys::EFollowProfileConfiguration ) );
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KAutoRecord,
                            MusSettingsKeys::EAutoRecordOn ) );       
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KSipProfileId,
                            0  ) ); //default profile
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KUiOrientation,
                            MusSettingsKeys::EPortrait ) );                    
    User::LeaveIfError( Storage()->Set ( MusSettingsKeys::KCapabilityQuery,
                            MusSettingsKeys::ENoOptions ) );
    }
    
TInt CCRepositoryBehaviorTls::Set(TUint32 aKey, TInt aValue)
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
    
TInt CCRepositoryBehaviorTls::Get(TUint32 aKey, TInt& aValue)
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
    
CCRepositoryBehaviorTls::CCRepositoryBehaviorTls()
	{
	}
	
CCRepositoryBehaviorTls::~CCRepositoryBehaviorTls()
	{
	iKeys.Reset();
    iValues.Reset();
    }
*/	
   
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CRepository* CRepository::NewL( TUid aRepositoryUid )
    {
    CRepository* self = CRepository::NewLC( aRepositoryUid );
    CleanupStack::Pop( self );
    return self;
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
EXPORT_C CRepository* CRepository::NewLC( TUid aRepositoryUid )
    {
    
    CRepository* self = new( ELeave ) CRepository( aRepositoryUid );
    CleanupStack::PushL( self );

    // This UID cannot be used in normal variant
    if ( aRepositoryUid == KCRUidSIPClientResolverConfig &&
         !iEmulateSawfishRepository )
        {
        User::Leave( KErrNotFound );
        }
    
    // Add a few keys
    if ( self->iRepositoryUid == KCRUidInCallVolume )
        {
        User::LeaveIfError( self->Set ( KTelIncallEarVolume, 4 ) );
        User::LeaveIfError( self->Set ( KTelIncallLoudspeakerVolume, 4 ) );
        }
/*    else if ( self->iRepositoryUid == MusSettingsKeys::KRepositoryUid )
        {
        // Set default values for numeric values which are not handled by static
        // variables
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KActivation,
                            MusSettingsKeys::EAlwaysActive ) );
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KAuditoryNotification,
                            MusSettingsKeys::EAuditoryNotificationOff ) );
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KPopupNotification,
                            MusSettingsKeys::EPopupNotificationOff ) );
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KPopupNotificationType,
                            MusSettingsKeys::ENotificationTypeQueryUser ) );
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KEdgeDtmSupport,
                            MusSettingsKeys::EDtmModeNotAllowed ) );
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KForceInternetSignaling,
                            MusSettingsKeys::EFollowProfileConfiguration ) );
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KAutoRecord,
                            MusSettingsKeys::EAutoRecordOn ) );       
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KSipProfileId,
                            0  ) ); //default profile
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KUiOrientation,
                            MusSettingsKeys::EPortrait ) );                    
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KCapabilityQuery,
                            MusSettingsKeys::ENoOptions ) );
        // No variation keys defined                         
        }
  */      
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CRepository::~CRepository()
    {
/*    iKeys.Reset();
    iValues.Reset();
*/    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Create(TUint32 /*aKey*/, TInt /*aValue*/ )
    {
    return KErrNone;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Create(TUint32 /*aKey*/, const TDesC8& /*aValue*/)
    {
    return KErrNone;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Delete( TUint32 /*aPartialKey*/, 
                                   TUint32 /*aMask*/, 
                                   TUint32& /*aErrorKey*/ ) 
    {
    return KErrNone;
    }
     

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Get(TUint32 aKey, TInt& aValue)
    {
    TInt error = KErrNone;
    
    if ( iRepositoryUid == MusSettingsKeys::KRepositoryUid &&
        aKey == MusSettingsKeys::KOperatorVariant )
        {
        aValue = CRepository::iOperatorVariant;
        return KErrNone;
        }
        
    CSipSseTestTls* repository = CSipSseTestTls::Storage();
    
    if ( repository )
        {
        error = repository->Get( aKey, aValue );
        }
    else
        {
        error = KErrNotFound;
        }

    return error;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Get(TUint32 /*aKey*/, TDes8& /*aValue*/)
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Set(TUint32 /*aKey*/, const TDesC8& /*aValue*/)
    {
    return KErrNotSupported;
    }

  
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Get(TUint32 /*aKey*/, TDes& aValue)
    {
    _LIT( KSampleValue, "\\data\\videos\\");
    aValue.Append( KSampleValue() );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Set(TUint32 /*aKey*/, const TDesC& /*aValue*/)
    {
    return KErrNone;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Set(TUint32 aKey, TInt aValue)
    {
    // Setting of operator variant value
    if ( iRepositoryUid == MusSettingsKeys::KRepositoryUid &&
        aKey == MusSettingsKeys::KOperatorVariant )
        {
        CRepository::iOperatorVariant =
                ( MusSettingsKeys::TOperatorVariant ) aValue;
        return KErrNone;
        }
/*
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
  */
    TInt error = KErrNone;
    //CCRepositoryBehaviorTls* repository = CCRepositoryBehaviorTls::Storage();
    CSipSseTestTls* repository = CSipSseTestTls::Storage();
    if ( repository )
        {
        error = repository->Set( aKey, aValue );
        }
    return error;
    }
	

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::FindL( TUint32 /*aPartialKey*/, 
                                  TUint32 /*aMask*/,
		                          RArray<TUint32>& /*aFoundKeys*/ )
    {
    return KErrNone;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::FindEqL( TUint32 /*aPartialKey*/, 
                                    TUint32 /*aMask*/,
		                            const TDesC8& /*aValue*/, 
		                            RArray<TUint32>& /*aFoundKeys*/ )
    {
    return KErrNone;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::StartTransaction( TTransactionMode /*aMode*/ )
    {
    return KErrNone;
    }
    
    
/// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::CommitTransaction(TUint32& /*aKeyInfo*/)
    {
    return KErrNone;
    }
       


// ------- Functions that are not present in real CRepository but are ----------
// ------- defined here exclusively for stubbing purposes             ----------  
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CRepository::ResetKeysAndValues()
    {
    CSipSseTestTls* repository = CSipSseTestTls::Storage();
    if ( repository )
        {
        repository->Reset();
        }
    }
  
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CRepository::CRepository( TUid aRepositoryUid )
    :iRepositoryUid( aRepositoryUid )
    {
    }    


