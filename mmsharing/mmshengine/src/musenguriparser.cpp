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
* Description:  Parser for recipient's address
*
*/


#include "musenguriparser.h"
#include "mussettings.h"
#include "muslogger.h"

#include <uri8.h>
#include <charconv.h>
#include <utf.h>
#include <collate.h>

     
// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// UTF7 is not working, since it converts + as +-
// -----------------------------------------------------------------------------
//
TMusEngUriParser::TMusEngUriParser( const TDesC16& aUri )
    :iUriType( ENotParsed )
    {
    TBuf8<KMaxUriLength> buf;

    if ( CnvUtfConverter::ConvertFromUnicodeToUtf8( buf, aUri ) == 0 )
        {
        iUri = buf;
        }
    else
        {
        iUri = KNullDesC8();
        }
    }

// ----------------------------------------------------------------------------- 
//
// -----------------------------------------------------------------------------
//
TMusEngUriParser::TMusEngUriType TMusEngUriParser::UriType()
    {
    return iUriType;
    }

// ----------------------------------------------------------------------------- 
//
// -----------------------------------------------------------------------------
//
HBufC8* TMusEngUriParser::GetUri8L()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngUriParser::GetUri8L()" )
    
    __ASSERT_ALWAYS( UriType() != TMusEngUriParser::ENotParsed, 
                     User::Leave( KErrNotReady ) ); 
    
    HBufC8* uri8 = iUri.AllocL();
    
    MUS_LOG( "mus: [ENGINE]  <- TMusEngUriParser::GetUri8L()" )

    return uri8;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
HBufC16* TMusEngUriParser::GetUri16L( TBool aPrefix )
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngUriParser::GetUri16L()" )
    
    __ASSERT_ALWAYS( UriType() != TMusEngUriParser::ENotParsed, 
                     User::Leave( KErrNotReady ) ); 

    HBufC16* uri16 = NULL;
    
    if ( aPrefix )
        {
        uri16 = CnvUtfConverter::ConvertToUnicodeFromUtf8L( iUri );
        }
    else
        {
        uri16 = CnvUtfConverter::ConvertToUnicodeFromUtf8L( 
                                    iUri.Mid( KMusEngTelPrefix().Length() ) );
        }
    
    MUS_LOG( "mus: [ENGINE]  <- TMusEngUriParser::GetUri16L()" )
    
    return uri16;
    }

// ----------------------------------------------------------------------------- 
//
// -----------------------------------------------------------------------------
//
void TMusEngUriParser::ParseUriL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngUriParser::ParseUriL()" )
    
    if ( iUri.Find( KMusEngAtSign ) >= 0 ) // Sip uri
        {
        HandleSipUriL();
        }
    else if ( iUri.Find( KMusEngPlusSign ) >= 0 ) // Tel uri
        {
        HandleTelUriL();
        }
    else
        {
        // check for variant
        if ( MultimediaSharingSettings::OperatorVariantSettingL()
                != MusSettingsKeys::EOperatorSpecific )
            {// basic behaviour
            MUS_LOG( "mus: [ENGINE]  -> TMusEngUriParser::ParseUriL() \
                    normal behaviour" )
            User::Leave( KErrArgument );        
            }
        else
            { // variant behaviour(local tel uri)
            HandleLocalTelUriL();
            }
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void TMusEngUriParser::HandleSipUriL()
    {
    if ( iUri.FindF( KMusEngTelPrefix() ) == 0 )
        {
        // SIP uri with prefix "tel:"
        User::Leave( KErrCorrupt );
        }
    else if ( iUri.FindF( KMusEngSipsPrefix ) == 0 )
        {
        // Secure SIP uri with prefix
        iUri.Replace( 0, KMusEngSipsPrefix().Length(), KMusEngSipsPrefix() );
        }
    else if ( iUri.FindF( KMusEngSipPrefix() ) != 0 )
        {
        // SIP uri without prefix
        if ( iUri.Length() + KMusEngSipPrefix().Length() <=
             KMaxUriLength )
            {
            iUri.Insert( 0, KMusEngSipPrefix() );
            }
        else
            {
            User::Leave( KErrCorrupt );
            }
        }
    else
        {
        // SIP uri with prefix
        iUri.Replace( 0, KMusEngSipPrefix().Length(), KMusEngSipPrefix() );
        }

    TUriParser8 parser;

    User::LeaveIfError( parser.Parse( iUri ) );

    User::LeaveIfError( parser.Validate() );
    
    iUriType = TMusEngUriParser::ESip;
    }
   
// ----------------------------------------------------------------------------- 
//
// -----------------------------------------------------------------------------
//
void TMusEngUriParser::HandleTelUriL()
    {
    iUri.TrimAll();
    if ( iUri.FindF( KMusEngTelPrefix() ) != 0 )
        {
        // Tel uri without prefix
        if ( iUri.Length() + KMusEngTelPrefix().Length() <=
             KMaxUriLength )
            {
            iUri.Insert( 0, KMusEngTelPrefix() );
            }
        else
            {
            User::Leave( KErrCorrupt );
            }
        }
    else
        {
        //Tel uri with prefix
        iUri.Replace( 0, KMusEngTelPrefix().Length(), KMusEngTelPrefix() );
        }

    TInt index = KMusEngTelPrefix().Length() + KMusEngPlusSign().Length();

    while ( index < iUri.Length() )
        {
        TChar character = iUri[ index ];
        if ( character.IsAlpha() )
            {
            User::Leave( KErrCorrupt ); // letters will not be accepted
            }
        else if ( !character.IsDigit() )
            {
            iUri.Delete( index, 1 ); // extra characters will be removed
            }
        else
            {
            ++index; // Valid tel URI so far
            }
        }

    iUriType = TMusEngUriParser::ETel;
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void TMusEngUriParser::HandleLocalTelUriL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngUriParser::ParseUriL() \
            variant behaviour" )
    if ( iUri.FindF( KMusEngTelPrefix() ) != 0 )
        {
        // local tel uri without prefix
        if ( iUri.Length() + KMusEngTelPrefix().Length() <= KMaxUriLength )
            {
            iUri.Insert( 0, KMusEngTelPrefix() );
            }
        else
            {
            User::Leave( KErrCorrupt );
            }
        }
    else
        {
        //Tel uri with prefix
        iUri.Replace( 0, KMusEngTelPrefix().Length(), KMusEngTelPrefix() );
        }

    TInt index = KMusEngTelPrefix().Length();

    // check if all chars are digits
    while ( index < iUri.Length() )
        {
        TChar character = iUri[ index ];
        if ( !character.IsDigit() )
            {
            User::Leave( KErrCorrupt );
            }
        ++index;
        }
        
    iUriType = TMusEngUriParser::ETel;
    }

// End of file
