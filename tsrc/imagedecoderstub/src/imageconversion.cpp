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

#include "imageconversion.h"

CImageDecoder::CImageDecoder()
	{
	iFrameInfo.iFrameCoordsInPixels = TRect( 0,0, 100,100 );
	iFrameInfo.iFrameDisplayMode = EColor16MCopy;
	}

CImageDecoder::~CImageDecoder()
	{
	
	}
		
		
const TFrameInfo& CImageDecoder::FrameInfo( TInt aFrameNumber ) const
	{
	return iFrameInfo;
	}

		//FrameInfo();
		
		//iDecoder->FrameInfo().iFrameCoordsInPixels.Size(),
		//                     iDecoder->FrameInfo().iFrameDisplayMode );

void CImageDecoder::Convert( TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber )
	{
	
	}
		//iDecoder->Convert( &iStatus, *iBitmap, 0 );
		
void CImageDecoder::Cancel()
	{
	
	}
		//Cancel

CImageDecoder* CImageDecoder::DataNewL(RFs& aFs, const TDesC8& aSourceData ) //, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType , const TUid aDecoderUid);

	{
	
	}
