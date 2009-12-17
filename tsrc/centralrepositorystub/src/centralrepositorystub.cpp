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


#include "mussettingskeys.h"
#include "centralrepository.h"
#include "e32property.h"
#include <badesca.h>

/// CenRep UIDs and keys
const TUid KCRUidSIPClientResolverConfig = { 0x10282EE7 };

const TUid KCRUidInCallVolume = {0x102828B1};
const TUint32 KTelIncallEarVolume                           = 0x00000001;
const TUint32 KTelIncallLoudspeakerVolume                   = 0x00000002;


// Values for static variables
MusSettingsKeys::TOperatorVariant CRepository::iOperatorVariant =
            MusSettingsKeys::EStandard;
TBool CRepository::iEmulateSawfishRepository = EFalse;
HBufC8* CRepository::iStaticAvcConfigKeys = NULL;
TBool CRepository::iStaticWriteAvcKeysToStaticData = EFalse;
TInt CRepository::iForceFailWithCode = KErrNone;
TInt CRepository::iStaticEncoderUid = 0;


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
    CRepository* self = new (ELeave) CRepository( aRepositoryUid );
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
    else if ( self->iRepositoryUid == MusSettingsKeys::KRepositoryUid )
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
                            0 /*default profile*/ ) );                    
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KUiOrientation,
                            MusSettingsKeys::EPortrait ) );                    
        User::LeaveIfError( 
                self->Set ( MusSettingsKeys::KCapabilityQuery,
                            MusSettingsKeys::ENoOptions ) );
        
        // No variation keys defined                         
        }
    
    self->iDesC8Values = new (ELeave) CDesC8ArrayFlat( 1 );        

    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CRepository::~CRepository()
    {
    iKeys.Reset();
    iValues.Reset();
    iDesC8Keys.Reset();
    delete iDesC8Values;   
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Create(TUint32 /*aKey*/, TInt /*aValue*/ )
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Create(TUint32 /*aKey*/, const TDesC8& /*aValue*/)
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Delete( TUint32 /*aPartialKey*/, 
                                   TUint32 /*aMask*/, 
                                   TUint32& /*aErrorKey*/ ) 
    {
    return KErrNotSupported;
    }
     

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Get(TUint32 aKey, TInt& aValue)
    {
    // fail simulation
    if ( iForceFailWithCode != KErrNone )
        {
        TInt returnValue = iForceFailWithCode;
        iForceFailWithCode = KErrNone;
        return returnValue;
        }
    
    // Setting of encoding device
    if ( iRepositoryUid == MusSettingsKeys::KRepositoryUid &&
         aKey == MusSettingsKeys::KEncodingDevice )
        {
        aValue = CRepository::iStaticEncoderUid;
        return KErrNone;
        }
            
    if ( iRepositoryUid == MusSettingsKeys::KRepositoryUid &&
        aKey == MusSettingsKeys::KOperatorVariant )
        {
        aValue = CRepository::iOperatorVariant;
        return KErrNone;
        }

    TInt err = RProperty::Get( MusSettingsKeys::KRepositoryUid, aKey, aValue );
    if ( err != KErrNone )
        {
        for ( TInt i = 0; i < iKeys.Count(); ++i )
            {
            if ( iKeys[i] == aKey )
                {
                aValue = iValues[i];
                return KErrNone;
                }
            }
        }
    
    return err;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Get(TUint32 aKey, TDes8& aValue )
    {
    // fail simulation
    if ( iForceFailWithCode != KErrNone )
        {
        TInt returnValue = iForceFailWithCode;
        iForceFailWithCode = KErrNone;
        return returnValue;
        }
    
    if ( aKey == MusSettingsKeys::KEncoderConfigurationInfo &&
         iStaticAvcConfigKeys )
        {
        // Use static data instead of member data
        aValue.Copy( *iStaticAvcConfigKeys );
        return KErrNone;
        }
        
    for ( TInt i = 0; i < iDesC8Keys.Count(); ++i )
        {
        if ( iDesC8Keys[i] == aKey )
            {
            TPtrC8 value = iDesC8Values->MdcaPoint( i ); 
            aValue.Copy( value );
            return KErrNone;
            }
        }
        
    return KErrNotFound;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CRepository::Set( TUint32 aKey, const TDesC8& aValue )
    {
    TBool error( EFalse );
    
    // fail simulation
    if ( iForceFailWithCode != KErrNone )
        {
        error = iForceFailWithCode;
        iForceFailWithCode = KErrNone;
        return error;
        }
        
    TBool keyFound( EFalse );
    
    // Replace possibly existing value
    for ( TInt i = 0; i < iDesC8Keys.Count(); ++i )
        {
        if ( iDesC8Keys[i] == aKey )
            {
            iDesC8Values->Delete( i );
            iDesC8Values->Compress();
            TRAP( error, iDesC8Values->InsertL( i, aValue ) );
            if ( error != KErrNone )
                {
                return error;
                }
                
            keyFound = ETrue;
            }
        }
    
    // If no existing value, add new one
    if ( !keyFound )
        {
        error = iDesC8Keys.Append( aKey );
        if ( error == KErrNone )
            {
            TRAP( error, iDesC8Values->AppendL( aValue ) )
            }
        }
    
    if ( iStaticWriteAvcKeysToStaticData &&
         aKey == MusSettingsKeys::KEncoderConfigurationInfo )
        {
        // Write also to static data
        TRAP( error, SetStubAvcConfigKeysL( aValue ) );
        }
        
    return error;
    }
	
  
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Get(TUint32 aKey, TDes& aValue)
    {
    // fail simulation
    if ( iForceFailWithCode != KErrNone )
        {
        TInt returnValue = iForceFailWithCode;
        iForceFailWithCode = KErrNone;
        return returnValue;
        }

    _LIT( KSampleValue, "\\data\\videos\\");
    TInt err = RProperty::Get( MusSettingsKeys::KRepositoryUid, aKey, aValue );
    if ( err != KErrNone )
        {
        aValue.Append( KSampleValue() );
        }
    return KErrNone;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Set(TUint32 /*aKey*/, const TDesC& /*aValue*/)
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::Set(TUint32 aKey, TInt aValue)
    {
    // fail simulation
    if ( iForceFailWithCode != KErrNone )
        {
        TInt returnValue = iForceFailWithCode;
        iForceFailWithCode = KErrNone;
        return returnValue;
        }
    
    // Setting of encoding device
    if ( iRepositoryUid == MusSettingsKeys::KRepositoryUid &&
         aKey == MusSettingsKeys::KEncodingDevice )
        {
        CRepository::iStaticEncoderUid = aValue;
        return KErrNone;
        }
    
    // Setting of operator variant value
    if ( iRepositoryUid == MusSettingsKeys::KRepositoryUid &&
         aKey == MusSettingsKeys::KOperatorVariant )
        {
        CRepository::iOperatorVariant =
                ( MusSettingsKeys::TOperatorVariant ) aValue;
        return KErrNone;
        }

    RProperty::Set( MusSettingsKeys::KRepositoryUid, aKey, aValue );

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


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::FindL( TUint32 /*aPartialKey*/, 
                                  TUint32 /*aMask*/,
		                          RArray<TUint32>& /*aFoundKeys*/ )
    {
    User::Leave( KErrNotSupported ); 
    return KErrNotSupported;
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
    User::Leave( KErrNotSupported ); 
    return KErrNotSupported;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::StartTransaction( TTransactionMode /*aMode*/ )
    {
    return KErrNotSupported;
    }
    
    
/// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRepository::CommitTransaction(TUint32& /*aKeyInfo*/)
    {
    return KErrNotSupported;
    }
       


// ------- Functions that are not present in real CRepository but are ----------
// ------- defined here exclusively for stubbing purposes             ----------  
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CRepository::ResetKeysAndValues()
    {
    iKeys.Reset();
    iValues.Reset();

    iDesC8Keys.Reset();
    iDesC8Values->Reset();
    
    iEmulateSawfishRepository = EFalse;

    delete iStaticAvcConfigKeys;
    iStaticAvcConfigKeys = NULL;    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CRepository::SetStubAvcConfigKeysL( const TDesC8& aConfigKeys )
    {
    HBufC8* newConfigKeys = aConfigKeys.AllocL();
    CRepository::DeleteStubAvcConfigKeys();
    iStaticAvcConfigKeys = newConfigKeys; 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

void CRepository::DeleteStubAvcConfigKeys()
    {
    delete iStaticAvcConfigKeys;
    iStaticAvcConfigKeys = NULL;
    }
  
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CRepository::CRepository( TUid aRepositoryUid )
    :iRepositoryUid( aRepositoryUid )
    {
    }    


