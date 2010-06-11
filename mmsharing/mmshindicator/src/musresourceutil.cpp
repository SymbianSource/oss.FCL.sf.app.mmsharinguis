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
* Description:  Provides means to read resource strings from resource files.
*
*/



#include "musresourcefinderutil.h"
#include "muslogger.h"
#include "musresourceutil.h"
#include <f32file.h>
#include <barsc.h>


// ======== MEMBER FUNCTIONS ========


// -----------------------------------------------------------------------------
// Reads resource string from file.
// -----------------------------------------------------------------------------
//
HBufC16* MusResourceUtil::ReadResourceString16LC( TUint aId, const TDesC& aFilename )
    {
    HBufC8* tmp = ReadResourceString8LC( aId, aFilename );
    HBufC* result = ConvertResourceStringLC( *tmp );
    CleanupStack::Pop( result );
    CleanupStack::PopAndDestroy( tmp );
    CleanupStack::PushL( result );
    return result;
    }


// -----------------------------------------------------------------------------
// Reads resource string from file.
// -----------------------------------------------------------------------------
//
HBufC16* MusResourceUtil::ReadResourceString16L( TUint aId, const TDesC& aFilename )
    {
    HBufC* result = ReadResourceString16LC( aId, aFilename );
    CleanupStack::Pop( result );
    return result;
    }


// -----------------------------------------------------------------------------
// Reads resource string from file.
// -----------------------------------------------------------------------------
//
HBufC8* MusResourceUtil::ReadResourceString8LC( TUint aId, const TDesC& aFilename )
    {
    HBufC8* result = ReadResourceString8L( aId, aFilename );
    CleanupStack::PushL( result );
    return result;
    }


// -----------------------------------------------------------------------------
// Reads resource string from file.
// -----------------------------------------------------------------------------
//
HBufC8* MusResourceUtil::ReadResourceString8L( TUint aId, const TDesC& aFilename )
    {
    MUS_LOG( "mus: [MUSIND ] -> MusIndicatorApi::ReadResourceString8L" )

    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    HBufC* filenameBuf = ResourceFilenameLC( aFilename );

    TFileName filename( *filenameBuf );

    CleanupStack::PopAndDestroy( filenameBuf );

    RResourceFile resource;
    resource.OpenL( fs, filename );
    CleanupClosePushL( resource );
    resource.ConfirmSignatureL();
    HBufC8* resString;
    resString = resource.AllocReadL( aId );
    CleanupStack::PopAndDestroy(); // resource
    CleanupStack::PopAndDestroy(); // fs

    MUS_LOG( "mus: [MUSIND ] <- MusResourceUtil::ReadResourceString8L" )
    return resString;
    }


// -----------------------------------------------------------------------------
// Converts HBufC8 to HBufC.
// -----------------------------------------------------------------------------
//
HBufC* MusResourceUtil::ConvertResourceStringLC( const TDesC8& aText )
    {
    MUS_LOG( "mus: [MUSIND ] -> MusResourceUtil::ConvertResourceStringLC" )
    HBufC16* result = HBufC16::NewL( aText.Length() / 2 );
    result->Des().Copy( (TUint16*) aText.Ptr(), aText.Length() / 2 );

    // push dlgPrompt now to cleanup stack
    CleanupStack::PushL( result );

    MUS_LOG( "mus: [MUSIND ] <- MusResourceUtil::ConvertResourceStringLC" )
    return result;
    }


// -----------------------------------------------------------------------------
// Constructs name of resource file.
// -----------------------------------------------------------------------------
//
HBufC* MusResourceUtil::ResourceFilenameLC( const TDesC& aFilename )
    {
    MUS_LOG( "mus: [MUSIND ]  -> MusResourceUtil::ResourceFilenameLC" )
    HBufC* fileName = MusResourceFinderUtil::ResourcePathL( aFilename );
    CleanupStack::PushL( fileName );
    MUS_LOG( "mus: [MUSIND ]  <- MusResourceUtil::ResourceFilenameLC" )
    return fileName;
    }
