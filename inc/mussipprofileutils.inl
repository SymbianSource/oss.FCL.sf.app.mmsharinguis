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
* Description:  ECOM interface default implementation. 
*
*/


#ifndef MUSSIPPROFILEUTILS_INL
#define MUSSIPPROFILEUTILS_INL

#include "musavasettings.h"
#include "muslogger.h"

#include <badesca.h>
#include <sipmanagedprofileregistry.h>
#include <sipmanagedprofile.h>
#include <utf.h>

_LIT8( KVsTag, "+g.3gpp.cs-voice" );

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline TInt MusSipProfileUtils::VsTagIndex( const MDesC8Array& aArray )
    {
    MUS_LOG( "mus: [MUSSET]  -> MusSipProfileUtils::VsTagIndex( )" )

    for( TInt i(0); i<aArray.MdcaCount(); i++ )
        {
        if( aArray.MdcaPoint( i ).Find( KVsTag ) == 0 )
            {   
            MUS_LOG1( "mus: [MUSSET]  <- MusSipProfileUtils::VsTagIndex( %d )",
                      i )

            return i;
            }
        }
    MUS_LOG( "mus: [MUSSET]  <- MusSipProfileUtils::VsTagIndex( KErrNotFound )" )
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline CDesC8ArrayFlat* MusSipProfileUtils::AddVsTagLC( const MDesC8Array& aArray )
    {
    CDesC8ArrayFlat* array 
                = new( ELeave ) CDesC8ArrayFlat( aArray.MdcaCount() + 1 );
    CleanupStack::PushL( array );
    
    for( TInt i(0); i<aArray.MdcaCount(); i++ )
        {
        array->AppendL( aArray.MdcaPoint( i ) );
        }
    array->AppendL( KVsTag() );
    
    return array;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline CDesC8ArrayFlat* MusSipProfileUtils::RemoveVsTagLC(
                                        const MDesC8Array& aArray,
                                        TInt aIndex )
    {
    CDesC8ArrayFlat* array 
                = new( ELeave ) CDesC8ArrayFlat( aArray.MdcaCount() + 1 );
    CleanupStack::PushL( array );
    
    for( TInt i(0); i<aArray.MdcaCount(); i++ )
        {
        if( aIndex != i )
            {
            array->AppendL( aArray.MdcaPoint( i ) );
            }
        }
    return array;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//

inline void MusSipProfileUtils::RemoveFeatureTagL( CSIPManagedProfile& aProfile )
    {
    //MDesC8Array* params;
    const MDesC8Array* params;
    
    aProfile.GetParameter( KSIPContactHeaderParams, params );
    
    MUS_LOG( "mus: [MUSSET]     Handling contact header params" )

    TInt index( KErrNotFound );
    if( params )
        {
        index = MusSipProfileUtils::VsTagIndex( *params );

        if( index != KErrNotFound )
            {
            MUS_LOG( "mus: [MUSSET]     No VS-specific contact header param set yet, setting it..." )
            const CDesC8ArrayFlat* newParams 
                                = RemoveVsTagLC( *params, index );
            aProfile.SetParameter( KSIPContactHeaderParams,
                                   *newParams );
            CleanupStack::PopAndDestroy();
            }
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline void MusSipProfileUtils::AddFeatureTagL( CSIPManagedProfile& aProfile )
    {
    const MDesC8Array* params;
    
    aProfile.GetParameter( KSIPContactHeaderParams, params );
    
    MUS_LOG( "mus: [MUSSET]     Handling contact header params" )

    TInt index( KErrNotFound );
    if( params )
        {
        index = MusSipProfileUtils::VsTagIndex( *params );

        if( index == KErrNotFound )
            {
            MUS_LOG( "mus: [MUSSET]     No VS-specific contact header param set yet, setting it..." )
            const CDesC8ArrayFlat* newParams 
                                = AddVsTagLC( *params );
            aProfile.SetParameter( KSIPContactHeaderParams,
                                   *newParams );
            CleanupStack::PopAndDestroy();
            }
        }

    if( !params )
        {
        MUS_LOG( "mus: [MUSSET]     No VS-specific contact header param set yet, setting it..." )
        CDesC8ArrayFlat* newParams 
                            = new( ELeave ) CDesC8ArrayFlat( 1 );
        CleanupStack::PushL( newParams );
        newParams->AppendL( KVsTag() );
        aProfile.SetParameter( KSIPContactHeaderParams,
                               *newParams );
        CleanupStack::PopAndDestroy();
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline void MusSipProfileUtils::InsertContactHeaderL( TInt aSipProfileId ) 
    {
    MUS_LOG1( "mus: [MUSSET]  -> MusSipProfileUtils::InsertContactHeaderL( %d )",
              aSipProfileId )
    MusSipProfileUtils* utils = new( ELeave ) MusSipProfileUtils();
    
    CleanupStack::PushL( utils );

    MUS_LOG( "mus: [MUSSET]     Opening SIP profile registry" )
    
    CSIPManagedProfileRegistry* registry 
            = CSIPManagedProfileRegistry::NewLC( *utils );

    MUS_LOG1( "mus: [MUSSET]     Looking for profile for id %d",
              aSipProfileId )
           
    CSIPManagedProfile* profile = NULL;
    
    TRAPD( err, profile = static_cast< CSIPManagedProfile* >( 
                    registry->ProfileL( aSipProfileId ) ) );
    if ( err == KErrNone )
        {
	    CleanupStack::PushL( profile );
		MUS_LOG( "mus: [MUSSET]     Corresponding profile found" )
	    AddFeatureTagL( *profile );
	    registry->SaveL( *profile );
  		CleanupStack::PopAndDestroy( profile );
        }
    CleanupStack::PopAndDestroy( registry );
    CleanupStack::PopAndDestroy( utils );

    MUS_LOG( "mus: [MUSSET]  <- MusSipProfileUtils::InsertContactHeaderL()" )
    }    	


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline void MusSipProfileUtils::RemoveContactHeaderL( TInt aSipProfileId ) 
    {   
    MUS_LOG1( "mus: [MUSSET]  -> MusSipProfileUtils::RemoveContactHeaderL( %d )",
              aSipProfileId )
    MusSipProfileUtils* utils = new( ELeave ) MusSipProfileUtils();
    
    CleanupStack::PushL( utils );

    CSIPManagedProfileRegistry* registry = 
            CSIPManagedProfileRegistry::NewLC( *utils );

    CSIPManagedProfile* profile = NULL;
    
    TRAPD( err,  profile = static_cast< CSIPManagedProfile* >
            ( registry->ProfileL( aSipProfileId ) ) ); 
    if ( err == KErrNone )
        {
        CleanupStack::PushL( profile );
        MUS_LOG( "mus: [MUSSET]     Corresponding profile found" )
        RemoveFeatureTagL( *profile );
        registry->SaveL( *profile );
        CleanupStack::PopAndDestroy( profile );
        }
    
    CleanupStack::PopAndDestroy( registry );
    CleanupStack::PopAndDestroy( utils );    
    MUS_LOG( "mus: [MUSSET]  <- MusSipProfileUtils::RemoveContactHeaderL()" )
    }    	

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline MusSipProfileUtils::MusSipProfileUtils()
    {
    
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
inline MusSipProfileUtils::~MusSipProfileUtils()
    {
    
    }

#endif // MUSSIPPROFILEUTILS_INL
