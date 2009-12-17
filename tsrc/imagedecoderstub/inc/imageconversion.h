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
#ifndef __IMAGECONVERSION__
#define __IMAGECONVERSION__


#include <fbs.h>
#include <f32file.h>
#include <e32base.h>

	

class CFbsBitmap;

class TFrameInfo 
	{
	public:
	TRect iFrameCoordsInPixels;
	TDisplayMode iFrameDisplayMode;
	};


class CImageDecoder 
	{
	
	public:
	
	    
		CImageDecoder();
		~CImageDecoder();
		
		
		const TFrameInfo& FrameInfo(TInt aFrameNumber = 0) const;

		//FrameInfo();
		
		//iDecoder->FrameInfo().iFrameCoordsInPixels.Size(),
		//                     iDecoder->FrameInfo().iFrameDisplayMode );

		void Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber = 0);
		//iDecoder->Convert( &iStatus, *iBitmap, 0 );
		
		void Cancel();
		//Cancel
		
		
		//CImageDecoder* DataNewL( RFs aFs, HBufC8 *iData );
		
		//IMPORT_C static CImageDecoder* DataNewL(RFs& aFs, const TDesC8& aSourceData, const TDesC8& aMIMEType, const TOptions aOptions = EOptionNone);
		static CImageDecoder* DataNewL(RFs& aFs, const TDesC8& aSourceData );//,  const TOptions aOptions = EOptionNone, const TUid aImageType  = KNullUid, const TUid aImageSubType = KNullUid, const TUid aDecoderUid = KNullUid);

        TFrameInfo iFrameInfo;
		
	};


#endif