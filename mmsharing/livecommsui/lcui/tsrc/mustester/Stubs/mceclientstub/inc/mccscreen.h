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



#ifndef MCCSCREEN_H
#define MCCSCREEN_H

#include <w32std.h>

class CMccScreen : public CBase, 
                   public MDirectScreenAccess
	{

	public:

		// Construction & destruction
		static CMccScreen* NewL( TPoint aPos, 
                                 TSize aArea, 
                                 TInt aIndex, 
                                 TInt aWindowOrdinalPosition,
                                 TInt aWindowOrdinalPriority,
                                 TSize aViewFinderImageSize );

		~CMccScreen();
		
		void ConstructL( TSize aViewFinderImageSize );
	    
	public:

		void StartL();
		
		void Stop();
		
		void Draw( CFbsBitmap& aFrame );
		
		void AttachFrameL( CFbsBitmap* aFrame );
		
		void DetachFrame();
		
	private:

		CMccScreen( TPoint aPos, 
                    TSize aArea, 
                    TInt aIndex, 
                    TInt aWindowOrdinalPosition,
                    TInt aWindowOrdinalPriority );
		
		void Restart( RDirectScreenAccess::TTerminationReasons aReason );
		
		void AbortNow( RDirectScreenAccess::TTerminationReasons aReason );
		
		TPoint UpdateViewFinderArea( TSize aViewFinderImageSize );

        void DoDraw( CFbsBitmap& aFrame );
        
        void DoScreenDeviceUpdate();
        
	private:

	    RWsSession           iRwSession;
		RWindowGroup*        iRwGroup;
		RWindow*             iRw;
		CWsScreenDevice*     iDev;
		TInt 				 iDeviceIndex;
		CDirectScreenAccess* iDirectScreenAccess;
		CFbsBitGc* iGc;
		RRegion* iRegion;
		CFbsScreenDevice* iFbsDev;
		
		TPoint iPosition;
		TSize  iArea;
		
		TInt iWindowOrdinalPosition;
		TInt iWindowOrdinalPriority;
		
		TRect iViewFinderImageRect;	
		
		TBool iFirstImageDrawn;
		
		CFbsBitmap* iAttachedFrame;
		
		TBool iAborted;
		
    #ifdef TEST_EUNIT
    	friend class UT_CMccCameraHandler;
    #endif
    	
	};

#endif
