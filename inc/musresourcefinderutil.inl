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
* Description:  Utility Class for Rom / Ram file finder.
*
*/

#ifndef MUSRESOURCEFINDERUTIL_INL
#define MUSRESOURCEFINDERUTIL_INL

#include "musresourcefinderutil.h"
#include "muslogger.h"

#include <bautils.h>


// -----------------------------------------------------------------------------
// Returns the full path of filename of the resource file from Resource dir
// -----------------------------------------------------------------------------
//

inline HBufC* MusResourceFinderUtil::ResourcePathL( const TDesC& aFileName )
    {
    MUS_LOG( "[COMMON]  -> MusResourceFinderUtil::ResourcePathL" )    
    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    // here extra one chracter length is added because of some strange chineese
    // resource extension reason.
    HBufC* fileName = HBufC::NewLC(KMusResourceRam().Length() + 
                                                     aFileName.Length() + 1 );
    TPtr fileNamePtr = fileName -> Des();
    fileNamePtr.Copy(KMusResourceRam());		
    fileNamePtr.Append(aFileName);
   
    bool resourceFileFound = false;

    //check for resource and variant file in RAM
    
    if (!resourceFileFound)
        {
        if(fileName->Length()<KMaxFileName)
         {
         MUS_LOG( "[COMMON]     File Name Length is lesser than KMaxFileName")
         TFileName fName(*fileName);    
         MUS_LOG( "[COMMON]     Searching For Language Variant in Ram (C:)")
         BaflUtils::NearestLanguageFile(fs ,fName );
         CleanupStack::PopAndDestroy( fileName ); // fileName
         fileName = fName.AllocLC();
         }
        if ( BaflUtils::FileExists( fs, *fileName ) )
            {
              resourceFileFound = true;
              MUS_LOG( "[COMMON]     Resource file was found from Ram (C:)")
            }
        else
            {
              resourceFileFound = false;
              MUS_LOG( "[COMMON]     Resource file NOT FOUND from Ram (C:)")
            }
        }
    
    //if not found in RAM, check for resource and variant file in ROM
    if (!resourceFileFound)
        {
        fileNamePtr.Copy(KMusResourceRom());        
        fileNamePtr.Append(aFileName);
        if(fileName->Length()<KMaxFileName)
         {
         MUS_LOG( "[COMMON]     File Name Length is lesser than KMaxFileName")
         TFileName fName(*fileName);    
         MUS_LOG( "[COMMON]     Searching For Language Variant in Rom (Z:)")
         BaflUtils::NearestLanguageFile(fs ,fName );
         CleanupStack::PopAndDestroy( fileName ); // fileName
         fileName = fName.AllocLC();
         }
        if ( BaflUtils::FileExists( fs, *fileName ) )
            {
              resourceFileFound = true;
              MUS_LOG( "[COMMON]     Resource file was found from Rom (Z:)")
            }
        else
            {
              resourceFileFound = false;
              MUS_LOG( "[COMMON]     Resource file NOT FOUND from Rom (Z:)")
            }
        }   
    
    CleanupStack::Pop( fileName ); // fileName
    CleanupStack::PopAndDestroy(); // fs
    MUS_LOG( "[COMMON]  <- MusResourceFinderUtil::ResourcePathL" )
    return fileName;
    }
    

// -----------------------------------------------------------------------------
// Returns the full path of filename of the resource file from App Resource dir
// -----------------------------------------------------------------------------
//

inline HBufC* MusResourceFinderUtil::AppResourcePathL( const TDesC& aFileName )
    {
    MUS_LOG( "[COMMON]  -> MusResourceFinderUtil::AppResourcePathL" )    
    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    // here extra one chracter length is added because of some strange chineese
    // resource extension reason.
    HBufC* fileName = HBufC::NewLC(KMusAppResourceRam().Length() + 
                                                   aFileName.Length() + 1 );
    TPtr fileNamePtr = fileName -> Des();
    fileNamePtr.Copy(KMusAppResourceRam());		
    fileNamePtr.Append(aFileName);
    if( BaflUtils::FileExists( fs, *fileName ) )  // Check in Ram
        {
        MUS_LOG_TDESC( "[COMMON]     File Found in Ram (C:) -> " , aFileName )                
        }
    else
    	{
        fileNamePtr.Copy(KMusAppResourceRom());		
        fileNamePtr.Append(aFileName);
        if( BaflUtils::FileExists( fs, *fileName) )
          {
          MUS_LOG_TDESC( "[COMMON]     File Found in Rom (Z:) -> " , aFileName )     
          }    
        else
    	  {            
    	  MUS_LOG_TDESC( "[COMMON]     File Not Found -> " , aFileName )     
    	  MUS_LOG( "[COMMON]     Returning Rom File by default")
          // Return Rom path since that will be default in real product
          // if that doesnt exist , it is upto sub system to handle this
          // because some cases it will not be mandatory file   
    	  fileNamePtr.Copy(KMusAppResourceRom());		
          fileNamePtr.Append(aFileName);
    	  }
    	}
    if(fileName->Length()<KMaxFileName)
        {
        MUS_LOG( "[COMMON]     File Name Length is lesser than KMaxFileName")
        TFileName fName(*fileName);    
        MUS_LOG( "[COMMON]     Searching For Language Variant")
        BaflUtils::NearestLanguageFile(fs ,fName );
        CleanupStack::PopAndDestroy( fileName ); // fileName
        fileName = fName.AllocLC();
        }
    CleanupStack::Pop( fileName ); // fileName
    CleanupStack::PopAndDestroy(); // fs
    MUS_LOG_TDESC( "[COMMON]     Returning -> " , fileNamePtr )         
    MUS_LOG( "[COMMON]  <- MusResourceFinderUtil::AppResourcePathL" )
    return fileName;
    }
 
 
// -----------------------------------------------------------------------------
// Returns ETrue if File Exist else EFalse
// -----------------------------------------------------------------------------
//

inline TBool MusResourceFinderUtil::IsExistL( const TDesC& aFileNameWithPath )
    {
    MUS_LOG( "[COMMON]  -> MusResourceFinderUtil::IsExist" )    
    TBool exist = EFalse;
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    if( BaflUtils::FileExists( fs, aFileNameWithPath ) )  // Check file
        {
        MUS_LOG_TDESC( "[COMMON]     File Exists -> " , aFileNameWithPath )   
        exist = ETrue ;             
        }
    else
    	 {    	
    	 MUS_LOG_TDESC( "[COMMON]     File Not Found -> " , aFileNameWithPath )  
    	 exist = EFalse;       	 
    	 }    
    CleanupStack::PopAndDestroy(); // fs
    MUS_LOG( "[COMMON]  <- MusResourceFinderUtil::IsExist" )
    return exist;
    }

#endif // MUSRESOURCEFINDERUTIL_INL

// end of file
