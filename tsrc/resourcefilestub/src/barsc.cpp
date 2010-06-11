/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "barsc.h"
#include <musaiwprovider.rsg>

_LIT8( KMusOnHold, "On hold" ); 
_LIT8( KMusNetworkIncompatible, "Network incompatible" ); 
_LIT8( KMusConferenceCall, "Conference call" ); 
_LIT8( KMusRecipientIncapable, "Recipient incapable" ); 
_LIT8( KMusNoReg, "No registration" );
_LIT8( KMusGeneral, "Application error" );

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C RResourceFile::RResourceFile()
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C void RResourceFile::Close()
    {
    delete iBuffer;
    iIsWarning = EFalse ;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C void RResourceFile::OpenL(RFs& /*aFs*/,const TDesC& /*aName*/)
    {
    iBuffer = HBufC8::NewL( 10 ); // Reserve just to leak memory if not closed
    iIsWarning = ETrue ;
    }
    
   
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//   
EXPORT_C HBufC8* RResourceFile::AllocReadL( TInt aResourceId ) const
    {
    if ( !iBuffer )
        {
        User::Panic( _L("ResourceFile not opened"), KErrNotReady );
        }
    
    HBufC8* resourceText = NULL;
  
    // The following values concearn AIW Provider. If same values are used, make
    // diffrence with checking of resource file name.
    
    switch( aResourceId )
        {
        case R_MUSAIWPROVIDER_ON_HOLD:
            {
            resourceText = KMusOnHold().AllocL();            
            break;
            }
        case R_MUSAIWPROVIDER_NETWORK_INCOMPATIBLE:
            {
            resourceText = KMusNetworkIncompatible().AllocL();            
            break;
            }
        case R_MUSAIWPROVIDER_CONFERENCE_CALL:
            {
            resourceText = KMusConferenceCall().AllocL();            
            break;
            }
        case R_MUSAIWPROVIDER_RECIPIENT_INCAPABLE:
            {
            resourceText = KMusRecipientIncapable().AllocL();            
            break;
            }  
        case R_MUSAIWPROVIDER_NO_REG:
            {
            resourceText = KMusNoReg().AllocL();            
            break;
            }   
        default:
            {
            resourceText = KMusGeneral().AllocL();            
            break;
            }            
        }
    
    return resourceText;
    }
    
    
