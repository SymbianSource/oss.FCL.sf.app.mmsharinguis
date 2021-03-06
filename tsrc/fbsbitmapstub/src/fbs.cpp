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


#include <fbs.h>


CFbsBitmap::CFbsBitmap()
	{
	
	}
	
	
CFbsBitmap::~CFbsBitmap()
	{
	
	}

TInt CFbsBitmap::Create( const TSize& /*aSizeInPixels*/, TDisplayMode /*aDispMode*/)
	{
    return KErrNone;
	}

TInt CFbsBitmap::Load(const TDesC& aFileName,
                      TInt32 aId,TBool aShareIfLoaded)
    {
    return KErrNone;
    }


TInt CFbsBitmap::Load(const TDesC& aFileName,
                      TInt32 aId,TBool aShareIfLoaded,
                      TUint aFileOffset)
    {
    return KErrNone;
    }
	
TInt CFbsBitmap::Load(RFile& aFile,TInt32 aId,TBool aShareIfLoaded)
    {
    return KErrNone;
    }
    
TInt CFbsBitmap::Load(RFile& aFile,TInt32 aId,TBool aShareIfLoaded,TUint aFileOffset)
    {
    return KErrNone;
    }
    
TInt CFbsBitmap::Resize(const TSize& aSizeInPixels)
    {
    value =  aSizeInPixels  ; 
    return KErrNone;
    }  
    
TSize CFbsBitmap::SizeInPixels() const
    {
    return value;
    } 	
    
