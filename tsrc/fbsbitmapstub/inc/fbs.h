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

#ifndef __FBS_H__
#define __FBS_H__

#include <e32base.h>
#include <gdi.h>
#include <bitmap.h>
    
class CFbsBitmap
	{
	public:
	
		CFbsBitmap();
		~CFbsBitmap();
	
	
	TInt Create( const TSize& aSizeInPixels, TDisplayMode aDispMode);
 
TInt Load(const TDesC& aFileName,TInt32 aId=0,TBool aShareIfLoaded=ETrue);
	TInt Load(const TDesC& aFileName,TInt32 aId,TBool aShareIfLoaded,TUint aFileOffset);
	TInt Load(RFile& aFile,TInt32 aId=0,TBool aShareIfLoaded=ETrue);
	TInt Load(RFile& aFile,TInt32 aId,TBool aShareIfLoaded,TUint aFileOffset);		  
    
	TInt Resize(const TSize& aSizeInPixels);
	TSize SizeInPixels() const;
	TSize value;

	

	
	};

class CFbsFont
	{
	};

class RFbsSession
    {
    };

class CFbsTypefaceStore
    {
    };

#endif
