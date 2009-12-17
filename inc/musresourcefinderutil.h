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
* Description:  The ECOM interface implementation.
*
*/


#ifndef MUSRESOURCEFINDER_H
#define MUSRESOURCEFINDER_H

#include "musunittesting.h"

#include <e32def.h>
#include <e32cmn.h>

// possible locations of compiled resource file

_LIT( KMusResourceRom, "z:\\resource\\" );
_LIT( KMusResourceRam, "c:\\resource\\" );

_LIT( KMusAppResourceRom, "z:\\resource\\apps\\" );
_LIT( KMusAppResourceRam, "c:\\resource\\apps\\" );



class MusResourceFinderUtil
    {
    MUS_UNITTEST( UT_MusResourceFinderUtil )
    
    public:
        /**
         * Returns the full path of resource file name from Resource Dir
         * Note : This function will leave with KErrNotFound 
         *        If the file is not found in Rom or Ram.     
         */
        static HBufC* ResourcePathL( const TDesC& aFileName );  

        /**
         * Returns the full path of resource file name from App Resource Dir
         * Note : This function will leave with KErrNotFound 
         *        If the file is not found in Rom or Ram.     
         */  
        static HBufC* AppResourcePathL( const TDesC& aFileName );
           
        /**
         * Checks the Existence of File
         * Returns ETrue if Exist else EFalse
         */  
        static TBool IsExistL( const TDesC& aFileNameWithPath ); 
    
    };
    
#include "musresourcefinderutil.inl"

#endif // MUSRESOURCEFINDER_H
