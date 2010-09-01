/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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


#include "musuifileutil.h"
#include "muslogger.h" // debug logging
#include "musuidefinitions.h"
#include "musuidialogutil.h"
#include "mussettings.h"
#include <musui.rsg>

#include <f32file.h>
#include <StringLoader.h>
#include <AknGlobalNote.h>
#include <apparc.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool MusUiFileUtil::FileExists( const TDesC& aFullFileName )
    {
    MUS_LOG( "mus: [MUSUI ]  -> MusUiFileUtil::FileExists" );
    
    TBool fileExists( EFalse );
    RFs fs;    

    if ( fs.Connect() == KErrNone )
        {
        TUint att;
        fileExists = ( fs.Att( aFullFileName, att ) != KErrNotFound );
        fs.Close();
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- MusUiFileUtil::FileExists" );
    
    return fileExists;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiFileUtil::VideoFilePathExistsL( const TDesC& aVideoFilePath)
    {
    MUS_LOG( "mus: [MUSUI ]  -> MusUiFileUtil::VideoFilePathExistsL" );
    
    RFs fs;
    
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    TInt error = fs.MkDirAll( aVideoFilePath );
    CleanupStack::PopAndDestroy( &fs );
    
    MUS_LOG1( "mus: [MUSUI ]  <- MusUiFileUtil::VideoFilePathExistsL,%d",error );
    User::Leave(error);
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiFileUtil::CreateVideoFileNameL( const TDesC& aVideoFilePath, 
                                          TFileName& aFileName )
    {
    MUS_LOG( "mus: [MUSUI ]  -> MusUiFileUtil::CreateVideoFileNameL" );
    
    RFs fs;
    TParse parse;
    TFileName fullFileName;
    TUint att;
     
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    if ( fs.Att( aVideoFilePath, att ) == KErrNone )
        {
        if ( MultimediaSharingSettings::OperatorVariantSettingL() == 
             MusSettingsKeys::EStandard )
            {
            HBufC* temp = StringLoader::LoadLC( R_MUS_VIEW_SAVED_FILENAME );
            User::LeaveIfError( parse.Set( aVideoFilePath, temp, NULL ) );
            CleanupStack::PopAndDestroy( temp );
            fullFileName = parse.FullName();
            fullFileName.Append( KMusVideoFileNameExtension );
            User::LeaveIfError( 
                CApaApplication::GenerateFileName( fs, fullFileName ) );
            User::LeaveIfError( parse.Set( fullFileName, NULL, NULL ) );
            }
        else    // operator specific case
            {
            RBuf newName;
            newName.CreateL( KMaxFileName );
            CleanupClosePushL( newName );

            TEntry entry;
            TInt i = 1;
            // path + 'VS' + date + 'V' + counter + extension
            _LIT( KFormatStringOne, "%S%S%S%S%02d%S" );
            _LIT( KFormatStringTwo, "%S%S%S%S%d%S" );
            TBuf<16> format;
            format = KFormatStringOne;

            // get current date
            _LIT( KDateFormat, "%F%M%D%*Y" );
            TTime time;
            time.UniversalTime();
            TBuf<6> dateString;
            time.FormatL( dateString, KDateFormat );

            // generate a valid filename that doesn't already exist...
            do
                {
                TPtrC driveAndPath = aVideoFilePath;
                TPtrC date = dateString;  
                newName.Format( format, 
                                &driveAndPath, &KMusVideoFileNameBegining, 
                                &date, &KMusVideoFileNameVersion, 
                                i++, &KMusVideoFileNameExtension );

                if ( newName.Length() > KMaxFileName )
                    {
                    User::Leave( KErrOverflow );
                    }

                if ( i >= 100 )
                    {
                    format = KFormatStringTwo;
                    }        			
                }       // Continue until DoesNotExist
            while ( fs.Entry( newName, entry ) == KErrNone ); 

            fullFileName.Copy( newName );
            User::LeaveIfError( parse.Set( newName, NULL, NULL ) );

            CleanupStack::PopAndDestroy( &newName );            
            }
        
        RFile file;
        CleanupClosePushL( file );
        User::LeaveIfError( 
            file.Create( fs, fullFileName, EFileWrite | EFileShareExclusive ) );
        CleanupStack::PopAndDestroy( &file );
        }
   
    CleanupStack::PopAndDestroy( &fs );
    
    aFileName = parse.Name();
    
    MUS_LOG( "mus: [MUSUI ] < - MusUiFileUtil::CreateVideoFileNameL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiFileUtil::DeleteFileL( const TDesC& aFileName )
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    TInt error = fs.Delete( aFileName );
    if ( ( error != KErrNone ) && ( error != KErrNotFound ) )
        {
        User::Leave( error );
        }
    CleanupStack::PopAndDestroy( &fs );
    }


// end of file
