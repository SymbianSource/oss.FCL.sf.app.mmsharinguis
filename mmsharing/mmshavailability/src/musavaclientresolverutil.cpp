/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Utility class to handle SIP client resolver cenrep table.
*
*/




#include "musavaclientresolverutil.h"
#include "musuid.hrh"
#include "muslogger.h"
#include <centralrepository.h>
#include <sipprofile.h>

// SipMXResolverImplUid
const TUid KSipMXResolverImplUid = { 0x10283140 };


// <copied values>

// The following constant is defined in sipprofile.h in Sawfish.
// Since the constant is not defined in all releases, it must be copied here.

/** 
* A unique Contact-header's user-part 
* generated automatically by the SIP implementation.
* The value is of type TDesC8.
* This parameter is read-only and 
* trying to set it will fail with KErrAccessDenied.
*/
const TUint32 KCopyOfKSIPContactHeaderUser = 15;

// The following Uids and keys are defined in file 
// sipclientresolverconfigcrkeys.h in Sawfish. Since the header is not present
// in all releases, the values must be defined here.

/// Defines configuration parameters used by SIP Client Resolver
const TUid KCRUidSIPClientResolverConfig = { 0x10282EE7 };

/// Mask for the less significant bits in setting keys.
/// These bits are reserved for field type information.
const TUint32 KSIPClientResolverFieldTypeMask = 0xf0000000;

/// Mask for the most significant bits in setting keys.
/// These bits are reserved for the actual key part.
const TUint32 KSIPClientResolverKeyMask = 0x0fffffff;

/** 
* Mask for username field.
* Field type is TDesC8.
*/
const TUint32 KSIPClientResolverUserNameMask = 0x10000000;

/** 
* Mask for plug-in implementation UID field.
* Field type is TInt32.
*/
const TUint32 KSIPClientResolverPluginUIDMask = 0x20000000;

/** 
* Mask for client UID field.
* Field type is TInt32.
*/
const TUint32 KSIPClientResolverClientUIDMask = 0x30000000;

// </copied values>


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusAvaClientResolverUtil::CMusAvaClientResolverUtil()
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaClientResolverUtil::ConstructL()
    {
    iRepository = CRepository::NewL( KCRUidSIPClientResolverConfig );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusAvaClientResolverUtil* CMusAvaClientResolverUtil::NewL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::NewL()" )
    CMusAvaClientResolverUtil* self = CMusAvaClientResolverUtil::NewLC();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::NewL()" )
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusAvaClientResolverUtil* CMusAvaClientResolverUtil::NewLC()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::NewLC()" )
    CMusAvaClientResolverUtil* self = new( ELeave ) CMusAvaClientResolverUtil;
    CleanupStack::PushL( self );
    self->ConstructL();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::NewLC()" )
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusAvaClientResolverUtil::~CMusAvaClientResolverUtil()
    {
    MUS_LOG( "mus: [MUSAVA]  -> ~CMusAvaClientResolverUtil()" )
    delete iRepository;
    MUS_LOG( "mus: [MUSAVA]  <- ~CMusAvaClientResolverUtil()" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaClientResolverUtil::RegisterClientWithUserL( 
                                                CSIPProfile& aProfile ) const
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::RegisterClientWithUserL( profile )" )
    RBuf8 contactHeaderUser;
    contactHeaderUser.CleanupClosePushL();
    User::LeaveIfError( GetContactHeaderUser( aProfile, contactHeaderUser ) );
    
    RegisterClientWithUserL( TUid::Uid( KMusUiUid ), 
                             contactHeaderUser, 
                             KSipMXResolverImplUid );
    
    CleanupStack::PopAndDestroy( &contactHeaderUser );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::RegisterClientWithUserL( profile )" )
    }
    

// -----------------------------------------------------------------------------
// Add line to client resolver cenrep table
// -----------------------------------------------------------------------------
//
void CMusAvaClientResolverUtil::RegisterClientWithUserL(
    const TUid& aImplementation,
    const TDesC8& aProfileContactHeaderUser, 
    const TUid& aResolver ) const
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::RegisterClientWithUserL()" )
    
    if( aImplementation.iUid <= 0 || 
        aProfileContactHeaderUser.Length() <= 0 ||
        aResolver.iUid <= 0 )
        {
        User::Leave( KErrArgument );
        }

    // Remove all earlier instances of client&user combination
    UnRegisterClientWithUserL( aImplementation, aProfileContactHeaderUser );

    User::LeaveIfError( 
        iRepository->StartTransaction( CRepository::EConcurrentReadWriteTransaction ) );
    TUint32 newKey = 0;
    CreateNewKeyL( newKey );
    // Add the new row
    TInt uid = aImplementation.iUid;
    User::LeaveIfError( 
        iRepository->Create(newKey|KSIPClientResolverClientUIDMask, uid ) );    
    User::LeaveIfError( 
        iRepository->Create(newKey|KSIPClientResolverUserNameMask, aProfileContactHeaderUser ) );
    uid = aResolver.iUid;    
    User::LeaveIfError( 
        iRepository->Create(newKey|KSIPClientResolverPluginUIDMask, uid ) ); 

    // Commit the transaction
    User::LeaveIfError( iRepository->CommitTransaction( newKey ) );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::RegisterClientWithUserL()" )
    }


// -----------------------------------------------------------------------------
// Remove line from client resolver cenrep table
// -----------------------------------------------------------------------------
//
void CMusAvaClientResolverUtil::UnRegisterClientWithUserL( 
                                                CSIPProfile& aProfile ) const
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::UnRegisterClientWithUserL( profile )" )

    RBuf8 contactHeaderUser;
    contactHeaderUser.CleanupClosePushL();
    User::LeaveIfError( GetContactHeaderUser( aProfile, contactHeaderUser ) );
    
    UnRegisterClientWithUserL( TUid::Uid( KMusUiUid ), contactHeaderUser );
    
    CleanupStack::PopAndDestroy( &contactHeaderUser );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::UnRegisterClientWithUserL( profile )" )
    }
    

// -----------------------------------------------------------------------------
// Remove line from client resolver cenrep table
// -----------------------------------------------------------------------------
//
void CMusAvaClientResolverUtil::UnRegisterClientWithUserL(
    const TUid& aImplementation, 
    const TDesC8& aProfileContactHeaderUser ) const
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::UnRegisterClientWithUserL()" )

    if( aImplementation.iUid <= 0 || 
        aProfileContactHeaderUser.Length() <= 0 )
        {
        User::Leave( KErrArgument );
        }

    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    GetClientWithUserL( aImplementation, aProfileContactHeaderUser, keys );
    for ( TInt i = 0; i < keys.Count(); i++ )
        {
        TUint32 errorKey;
        User::LeaveIfError( 
            iRepository->StartTransaction( 
                CRepository::EConcurrentReadWriteTransaction ) );
        iRepository->Delete( keys[i], KSIPClientResolverKeyMask, errorKey );
        // Commit the transaction
        User::LeaveIfError( iRepository->CommitTransaction( errorKey ) );
        }
    CleanupStack::PopAndDestroy( &keys );
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::UnRegisterClientWithUserL()" )
    }


// -----------------------------------------------------------------------------
// Resolve correct implementation UID for contact header
// -----------------------------------------------------------------------------
//    
void CMusAvaClientResolverUtil::GetImplementationUidWithUserL( 
	const TDesC8& aProfileContactHeaderUser,
    TUid& aImplementation ) const
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::GetImplementationUidWithUserL()" )

    RArray<TUint32> keys;    
    CleanupClosePushL( keys );
    // Find all rows where KSIPClientResolverUserNameMask is aProfileContactHeaderUser
    iRepository->FindEqL( KSIPClientResolverUserNameMask, 
                          KSIPClientResolverFieldTypeMask,
                          aProfileContactHeaderUser, 
                          keys );
    if (keys.Count() > 1)
        {
        //resolve correct Key
        }
    else if(keys.Count() == 1)
        {
        TInt implementationValue = KErrNotFound;
        // Get implmentation uid of found user name
        iRepository->Get( ( keys[0]^KSIPClientResolverUserNameMask ) |
        KSIPClientResolverClientUIDMask, implementationValue );

        User::LeaveIfError(implementationValue);
        aImplementation.iUid = implementationValue;
        }
    else
        {
        User::Leave(KErrNotFound);
        }

    CleanupStack::PopAndDestroy( &keys );	
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::GetImplementationUidWithUserL()" )
    }
    
    
// -----------------------------------------------------------------------------
// Find clients from resolver cenrep table
// -----------------------------------------------------------------------------
//
void CMusAvaClientResolverUtil::GetClientWithUserL(
    const TUid& aImplementation, 
    const TDesC8& aProfileContactHeaderUser, 
    RArray<TUint32>& aFoundKeys ) const
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::GetClientWithUserL()" )

    RArray<TUint32> keys;
    
    CleanupClosePushL( keys );
    // Find all rows where KSIPClientResolverUserNameMask is aProfileUserName
    iRepository->FindEqL( KSIPClientResolverUserNameMask, 
                            KSIPClientResolverFieldTypeMask,
                            aProfileContactHeaderUser, keys );
    for ( TInt i = 0; i < keys.Count(); i++ )
        {
        TInt implementationValue = KErrNotFound;
        // Get implmentation uid of found user name
        iRepository->Get( ( keys[i]^KSIPClientResolverUserNameMask ) |
            KSIPClientResolverClientUIDMask, implementationValue );
        // If implementation uid matches given uid, add row to found keys
        if ( implementationValue == aImplementation.iUid )
            {
            aFoundKeys.Append( keys[i]^KSIPClientResolverUserNameMask );
            }

        }
    CleanupStack::PopAndDestroy( &keys );
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::GetClientWithUserL()" )
    }
    
    
// -----------------------------------------------------------------------------
// Create a new key for the new row
// -----------------------------------------------------------------------------
//
void CMusAvaClientResolverUtil::CreateNewKeyL(
    TUint32& aNewKey ) const
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::CreateNewKeyL()" )

    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt err = KErrNone;
    
    TRAPD( leaveCode, err = iRepository->FindL( KSIPClientResolverUserNameMask, 
                                  KSIPClientResolverFieldTypeMask, 
                                  keys ) );
    if ( KErrNotFound == leaveCode )
        {
        err = KErrNotFound;
        leaveCode = KErrNone;
        }
    User::LeaveIfError( leaveCode );
    if ( ( KErrNotFound == err ) || ( keys.Count() == 0 ) )
        {
        aNewKey = 1;
        }
    else
        {
        User::LeaveIfError( err );
        // Find the biggest key and increment it by one
        keys.SortUnsigned();
        aNewKey = KSIPClientResolverUserNameMask^keys[ keys.Count() - 1 ] + 1;       
        }
    CleanupStack::PopAndDestroy( &keys );
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::CreateNewKeyL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusAvaClientResolverUtil::GetContactHeaderUser(
            CSIPProfile& aProfile,
            RBuf8& aContactHeaderUser ) const
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaClientResolverUtil::GetContactHeaderUser()" )

    TInt result( KErrNotFound );
    const TDesC8* contactHeaderUser( NULL );

    // Get SIP's Username    
    result = aProfile.GetParameter( KCopyOfKSIPContactHeaderUser, contactHeaderUser );
    
    if ( result == KErrNone && contactHeaderUser )
        {
        result = aContactHeaderUser.ReAlloc( contactHeaderUser->Length() );
        if ( KErrNone == result )
            {
            aContactHeaderUser.Copy( *contactHeaderUser );
            }
        }

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaClientResolverUtil::GetContactHeaderUser()" )

    return result;                
    }


