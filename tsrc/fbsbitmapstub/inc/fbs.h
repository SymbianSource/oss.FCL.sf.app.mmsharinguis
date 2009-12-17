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

#ifndef __FBSBITMAP__
#define __FBSBITMAP__

#include <e32base.h>

enum TDisplayMode
    {
    ///No display mode
    ENoneCopy,
    ///Monochrome display mode (1 bpp)
    EGray2Copy,
    ///Four grayscales display mode (2 bpp)
    EGray4Copy,
    ///16 grayscales display mode (4 bpp)
    EGray16Copy,
    ///256 grayscales display mode (8 bpp)
    EGray256Copy,
    ///Low colour EGA 16 colour display mode (4 bpp)
    EColor16Copy,
    ///256 colour display mode (8 bpp)
    EColor256Copy,
    ///64,000 colour display mode (16 bpp)
    EColor64KCopy,
    ///True colour display mode (24 bpp)
    EColor16MCopy,
    ///(Not an actual display mode used for moving buffers containing bitmaps)
    ERgbCopy,
    ///4096 colour display (12 bpp).
    EColor4KCopy,
    EColor16MUCopy,
    ///Display mode with alpha (24bpp colour plus 8bpp alpha)
    EColor16MACopy,
    //Any new display mode should be insterted here!
    //There might be up to 255 display modes, so value of the last
    //inserted EColorXXX enum item should be less than 256 -
    //BC reasons!
    EColorLastCopy,
    };
    
class CFbsBitmap
	{


	public:
	
		CFbsBitmap();
		~CFbsBitmap();
	
	
	TInt Create( const TSize& aSizeInPixels, TDisplayMode aDispMode);
 
	TInt CFbsBitmap::Load(const TDesC& /*aFileName*/,
	                      TInt32 /*aId*/,TBool /*aShareIfLoaded*/);


	TInt CFbsBitmap::Load(const TDesC& /*aFileName*/,
	                      TInt32 /*aId*/,TBool /*aShareIfLoaded*/,
	                      TUint /*aFileOffset*/);
	

	
	};

#endif