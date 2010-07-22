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




#include <bitstd.h>
#include <w32std.h>
#include <gdi.h>

#include "MccScreen.h"
#include "muslogger.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

_LIT(KMccWindowGroupName,"32MccVideoWindow");

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
CMccScreen* CMccScreen::NewL( 
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
	TInt aWindowOrdinalPriority,
	TSize aViewFinderImageSize )
	{
	CMccScreen* self = new ( ELeave ) CMccScreen( aPos, 
	                                              aArea, 
	                                              aIndex,
	                                              aWindowOrdinalPosition,
	                                              aWindowOrdinalPriority );
	CleanupStack::PushL( self );
	self->ConstructL( aViewFinderImageSize );
	CleanupStack::Pop( self );	
	return self;
	}

// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
CMccScreen::CMccScreen( 
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
	TInt aWindowOrdinalPriority ) :
	iDeviceIndex( aIndex ), 
	iPosition( aPos ), 
	iArea( aArea ),
	iWindowOrdinalPosition( aWindowOrdinalPosition ),
	iWindowOrdinalPriority( aWindowOrdinalPriority ),
	iAborted( ETrue )
	{
	}

// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
CMccScreen::~CMccScreen()
	{
	MUS_LOG( "CMccScreen::~CMccScreen" )
	
	DetachFrame(); // Must do bitmap detach before releasing window resources
	delete iDirectScreenAccess;
	delete iDev;
	delete iRw;
    delete iRwGroup;
    iRwSession.Close();
    
	MUS_LOG( "CMccScreen::~CMccScreen, exit" )
	}

// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
void CMccScreen::ConstructL( TSize aViewFinderImageSize )
	{
	MUS_LOG( "CMccScreen::ConstructL" )
    User::LeaveIfError( iRwSession.Connect() );
	
	// Create window group
	TInt groupId = iRwSession.GetFocusWindowGroup();
    iRwGroup = new (ELeave) RWindowGroup( iRwSession ) ;	
	//TInt handle = iRwSession.GetWindowGroupHandle( groupId );

    MUS_LOG1( "CMccScreen::ConstructL, window ordinal pos", iWindowOrdinalPosition )
    MUS_LOG1( "CMccScreen::ConstructL, window ordinal priority", iWindowOrdinalPriority )
    
    iRwGroup->Construct( groupId, EFalse );
	iRwGroup->SetName( KMccWindowGroupName );
    
    iRwGroup->SetOrdinalPosition( iWindowOrdinalPosition, iWindowOrdinalPriority );
         
    // Create screen device
	iDev = new (ELeave) CWsScreenDevice( iRwSession );
	iDev->Construct( iDeviceIndex ); 
	
	TInt currMode = iDev->CurrentScreenMode();
	MUS_LOG1( "CMccScreen::ConstructL, screen device mode = %d", currMode )
	                   
	TPixelsTwipsAndRotation sizeAndRotation;
    iDev->GetScreenModeSizeAndRotation( currMode, sizeAndRotation );
	MUS_LOG2( "CMccScreen::ConstructL, screen device size  %d %d", 
	                   sizeAndRotation.iPixelSize.iWidth, 
	                   sizeAndRotation.iPixelSize.iHeight )

	MUS_LOG1( "CMccScreen::ConstructL, screen device orientation %d", 
	                   sizeAndRotation.iRotation )
	
	// Create window
	iRw = new (ELeave) RWindow( iRwSession );
	
	iRw->Construct( *iRwGroup, (TUint32) this + 2 );
    
    MUS_LOG2( "CMccScreen::ConstructL, window pos %d %d", iPosition.iX, iPosition.iY )
	MUS_LOG2( "CMccScreen::ConstructL, window size %d %d", iArea.iWidth, iArea.iHeight )

    iRw->SetPosition( iPosition );
	iRw->SetBackgroundColor( KRgbBlack );
    iRw->SetSize( iArea );    
    iRw->SetOrdinalPosition( iWindowOrdinalPosition );
    iRw->Activate();
    iRwSession.Flush();
	
	// Create direct screen access                                	
	iDirectScreenAccess = CDirectScreenAccess::NewL( iRwSession, *iDev, *iRw, *this );   
	
	UpdateViewFinderArea( aViewFinderImageSize );
	
	MUS_LOG2( "CMccScreen::ConstructL, viewfinder pos %d %d", 
	                   iViewFinderImageRect.iTl.iX, iViewFinderImageRect.iTl.iY )
	MUS_LOG2( "CMccScreen::ConstructL, viewfinder size %d %d", 
	                   iViewFinderImageRect.Width(), iViewFinderImageRect.Height() )
	
	MUS_LOG( "CMccScreen::ConstructL, exit" )
	}
    
// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
void CMccScreen::StartL()
	{
	MUS_LOG( "CMccScreen::StartL" )	
	
	iAborted = EFalse;
	
	iDirectScreenAccess->StartL();
	iGc = iDirectScreenAccess->Gc();	
	iFbsDev = iDirectScreenAccess->ScreenDevice();
	iRegion = iDirectScreenAccess->DrawingRegion();		

#ifdef __MCC_CONTROLLER	
	if ( iRegion && !iRegion->IsEmpty() )
	    {
        for ( TInt i = 0; i < iRegion->Count(); i++ )
            {
            const TRect& regionRect = (*iRegion)[ i ];
            MUS_LOG_INT2( "CMccScreen::StartL, rect tl", regionRect.iTl.iX, regionRect.iTl.iY )
            MUS_LOG_INT2( "CMccScreen::StartL, rect br", regionRect.iBr.iX, regionRect.iBr.iY )
            }
	    }
#endif	        

    iGc->SetClippingRegion( iRegion );
    
    // Fill with black areas which are not covered by viewfinder image.
    // If viewfinder image has not been yet received and drawn, fill whole area.
    //
    iGc->SetBrushColor( KRgbBlack );
    iGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    
    if ( !iFirstImageDrawn )
        {
        // Whole area
        TPoint pointTl( 0, 0 );
        TRect rect( pointTl, iArea );
        iGc->DrawRect( rect );
        }
    else
        {
        // Upper area
        TPoint pointTl( 0, 0 );
        TPoint pointBr( iArea.iWidth, iViewFinderImageRect.iTl.iY );
        TRect rect( pointTl, pointBr );
        iGc->DrawRect( rect );

        // Lower area
        pointTl.SetXY( 0, iViewFinderImageRect.iBr.iY );
        pointBr.SetXY( iArea.iWidth, iArea.iHeight );
        rect.SetRect( pointTl, pointBr );
        iGc->DrawRect( rect );
        
        // Left area
        pointTl.SetXY( 0, iViewFinderImageRect.iTl.iY );
        pointBr.SetXY( iViewFinderImageRect.iTl.iX, iViewFinderImageRect.iBr.iY );
        rect.SetRect( pointTl, pointBr );
        iGc->DrawRect( rect );
        
        // Right area
        pointTl.SetXY( iViewFinderImageRect.iBr.iX, iViewFinderImageRect.iTl.iY );
        pointBr.SetXY( iArea.iWidth, iViewFinderImageRect.iBr.iY );
        rect.SetRect( pointTl, pointBr );
        iGc->DrawRect( rect );
        }
        
    if ( iAttachedFrame )
        {
        MUS_LOG( "CMccScreen::StartL, draw attached frame" )
        
        TSize viewFinderImageSize = iAttachedFrame->SizeInPixels();
    	
    	TPoint corner = UpdateViewFinderArea( viewFinderImageSize );
        
        iGc->BitBlt( corner, iAttachedFrame );
        }
        
    DoScreenDeviceUpdate();
    
    iGc->SetBrushStyle( CGraphicsContext::ENullBrush );
        		
	MUS_LOG( "CMccScreen::StartL, exit" )
	}

// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
void CMccScreen::Stop()
	{
	MUS_LOG( "CMccScreen::Stop" )
	iDirectScreenAccess->Cancel();
	MUS_LOG( "CMccScreen::Stop, exit" )	
	}
	
// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
void CMccScreen::Restart( RDirectScreenAccess::TTerminationReasons /*aReason*/ )
	{
	MUS_LOG( "CMccScreen::Restart" )	
	
	TRAP_IGNORE( StartL() );
	}

// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
void CMccScreen::AbortNow( RDirectScreenAccess::TTerminationReasons /*aReason*/ )
	{
	MUS_LOG( "CMccScreen::AbortNow" )	
	iDirectScreenAccess->Cancel();
	
	iAborted = ETrue;
	}

// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
void CMccScreen::Draw( CFbsBitmap& aFrame )
	{
	// When frame draw is requested, attached frame is not anymore drawn
	DetachFrame();
	
	DoDraw( aFrame );
	}

// -----------------------------------------------------------------------------
// CMccScreen::AttachFrameL
// -----------------------------------------------------------------------------
//
void CMccScreen::AttachFrameL( CFbsBitmap* aFrame )
    {
    MUS_LOG( "CMccScreen::AttachFrameL" )	
    
    __ASSERT_ALWAYS( aFrame, User::Leave( KErrArgument ) );
    
    DetachFrame();
    
    iAttachedFrame = aFrame;
    
    DoDraw( *iAttachedFrame );

    MUS_LOG( "CMccScreen::AttachFrameL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccScreen::DetachFrame
// -----------------------------------------------------------------------------
//		
void CMccScreen::DetachFrame()
    {	
    if ( iAttachedFrame )
        {
        MUS_LOG( "CMccScreen::DetachFrame, detaching" )
        iAttachedFrame->Reset();
        delete iAttachedFrame;
        }
        
    iAttachedFrame = NULL;
    }
    
// -----------------------------------------------------------------------------
// CMccScreen::UpdateViewFinderArea
// -----------------------------------------------------------------------------
//
TPoint CMccScreen::UpdateViewFinderArea( TSize aViewFinderImageSize )
    {
    TPoint corner( 0, 0 );
	if ( aViewFinderImageSize.iWidth < iArea.iWidth )
	    {
	    // Divide the subtraction by two (i.e. do centering)
	    corner.iX = ( iArea.iWidth - aViewFinderImageSize.iWidth ) >> 1;
	    }
	    
    if ( aViewFinderImageSize.iHeight < iArea.iHeight )
	    {
	    // Divide the subtraction by two (i.e. do centering)
	    corner.iY = ( iArea.iHeight - aViewFinderImageSize.iHeight ) >> 1;
	    }
	    
	iViewFinderImageRect = TRect( corner, aViewFinderImageSize );
	
	return corner;
    }

// -----------------------------------------------------------------------------
// CMccScreen::DoDraw
// -----------------------------------------------------------------------------
//
void CMccScreen::DoDraw( CFbsBitmap& aFrame )
    {	
    if ( !iAborted && iGc )
        {
        iFirstImageDrawn = ETrue;
        	
       	TSize viewFinderImageSize = aFrame.SizeInPixels();
      
    	TPoint corner = UpdateViewFinderArea( viewFinderImageSize );

        iGc->BitBlt( corner, &aFrame );
        
        DoScreenDeviceUpdate();
        }
    }

// -----------------------------------------------------------------------------
// CMccScreen::DoScreenDeviceUpdate
// -----------------------------------------------------------------------------
//
void CMccScreen::DoScreenDeviceUpdate()
    {
    if ( iFbsDev )
        {
        iFbsDev->Update();
        }
    }
    
// End of file

