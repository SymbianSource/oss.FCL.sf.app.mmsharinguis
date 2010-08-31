/*
* Copyright (c) 2005 - 2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSENGURIPARSER_H
#define MUSENGURIPARSER_H

//  INCLUDES

#include "musunittesting.h"
#include <e32cmn.h>

const TInt KMaxUriLength = 512;
_LIT8( KMusEngAtSign, "@" );
_LIT8( KMusEngPlusSign, "+" );
_LIT8( KMusEngSipPrefix, "sip:" );
_LIT8( KMusEngSipsPrefix, "sips:" );
_LIT8( KMusEngTelPrefix, "tel:" );

// CLASS DECLARATION

/**
* 
*/
class TMusEngUriParser
    {
    MUS_UNITTEST( UT_TMusEngUriParser ) 
    
    public:
    
        enum TMusEngUriType
            {
            ENotParsed = 0,
            ESip,
            ETel
            };
    
    public:
     
		/**
		* Default constructor
		*/
     	TMusEngUriParser( const TDesC16& aUri );
     	
     	/**
		* @returns Uri type
		*/
     	TMusEngUriType UriType();

		/**
		* @returns Parsed and validated 8-bit version of contained URI
		*          Ownership is transferred
		* @pre UriType() != ENotParsed
		* @leave KErrNotReady if precondition is not fulfilled
		*/
     	HBufC8* GetUri8L();
     	
     	/**
     	* @param aPrefix if ETrue, also sip: or tel:prefix is returned
		* @returns Parsed and validated 16-bit version of contained URI
		*          Ownership is transferred
		* @pre UriType() != ENotParsed
		* @leave KErrNotReady if precondition is not fulfilled
		*/
     	HBufC16* GetUri16L( TBool aPrefix );
     
     	/**
        * Parses and validates contained URI
        * @leave KErrCorrupt if URI is not valid SIP or TEL URI
        * @post UriType() != ENotParsed
        */
        void ParseUriL();
        
     	
    private:

        void HandleSipUriL();
        void HandleTelUriL();
        void HandleLocalTelUriL();
        
    private: // Data
        
		TBuf8<KMaxUriLength> iUri;
        TMusEngUriType iUriType; 
    };

#endif // MUSENGURIPARSER_H
