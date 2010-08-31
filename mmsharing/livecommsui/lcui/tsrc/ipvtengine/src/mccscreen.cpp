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
#include <ecam.h>
#include <ecam/mcameradirectviewfinder.h>
#include <coecntrl.h>
#include <eikenv.h>
#include <eikappui.h>
#include "MccScreen.h"
#include "svptimer.h"
#include "musenglogger.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

_LIT(KMccWindowGroupName,"MccVideoWindow");

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccScreen::NewL
// -----------------------------------------------------------------------------
//
CMccScreen* CMccScreen::NewL( CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
    TInt aWindowOrdinalPriority,
    TBool aSecondary,
    RDrawableWindow* aParentWindow )
    {
    // Resolve screen type based on ecam capabilities, asssume direct vf
    // to be available if camera not present yet.
    CMccScreen* screen = NULL;
    TBool useDirectVf = EFalse; 
    
    if ( aCamera )
        {
        TCameraInfo info;
        aCamera->CameraInfo( info );
        useDirectVf = ( info.iOptionsSupported & TCameraInfo::EViewFinderDirectSupported );
        }
    
    if ( useDirectVf )
        {
        TRAPD( err, screen = CMccScreenDirect::NewL( 
                    aCamera, aPos, aArea, aIndex, 
                    aWindowOrdinalPosition, aWindowOrdinalPriority ) );
        // If creation failes for some reason, bitmap vf is tried to be
        // created as a fallback
        if ( err == KErrNoMemory )
            {
            User::Leave( err );
            }
        if ( err )
            {
            }
        }
    
    if ( !screen )
        {
        screen = CMccScreenBitmap::NewL( 
                    aCamera, aPos, aArea, aIndex, 
                    aWindowOrdinalPosition, aWindowOrdinalPriority, aSecondary, aParentWindow );
        }
    return screen;
    }

// -----------------------------------------------------------------------------
// CMccScreen::~CMccScreen
// -----------------------------------------------------------------------------
//
CMccScreen::~CMccScreen()
    {

    StopCamera();
    
    delete iGraphicsContext;
    delete iDev;
    delete iRw;
    delete iRwGroup;
    iRwSession.Close();
    
    }

// -----------------------------------------------------------------------------
// CMccScreen::StartL
// -----------------------------------------------------------------------------
//
void CMccScreen::StartL()
    {
    
    }

// -----------------------------------------------------------------------------
// CMccScreen::Stop
// -----------------------------------------------------------------------------
//
void CMccScreen::Stop()
    {
    
    }

// -----------------------------------------------------------------------------
// CMccScreen::Draw
// -----------------------------------------------------------------------------
//
void CMccScreen::Draw( CFbsBitmap& /*aFrame*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CMccScreen::SetCamera
// -----------------------------------------------------------------------------
//
TInt CMccScreen::SetCamera( CCamera* aCamera )
    {
    if ( !aCamera )
        {
        StopCamera();
        }
       
    iCamera = aCamera; 
       
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMccScreen::StopCamera
// -----------------------------------------------------------------------------
//
void CMccScreen::StopCamera()
    {
    
    if ( iCamera )
        {
        iCamera->StopViewFinder();
        }
    
    }

// -----------------------------------------------------------------------------
// CMccScreen::VfStartPossible
// -----------------------------------------------------------------------------
//
TBool CMccScreen::VfStartPossible()
    {
    return ( iCamera && !iCamera->ViewFinderActive() && 
             iArea.iWidth > 0 &&
             iArea.iHeight > 0 );
    }

void CMccScreen::SetPrimaryRectL(TRect aRect)
    {
    MUS_LOG2( "mus: [ENGINE]  -> CMccScreen::SetPrimaryRectL() (%dx%d)", 
              aRect.Width(), aRect.Height() );
    MUS_LOG2( "mus: [ENGINE]  -> CMccScreen::SetPrimaryRectL() (%d,%d)", 
              aRect.iTl.iX, aRect.iTl.iY );
    iPrimaryRect = aRect;
    }
void CMccScreen::SetSecondaryRectL(TRect aRect)
    {
    MUS_LOG2( "mus: [ENGINE]  -> CMccScreen::SetSecondaryRectL() (%dx%d)", 
              aRect.Width(), aRect.Height() );
    MUS_LOG2( "mus: [ENGINE]  -> CMccScreen::SetSecondaryRectL() (%d,%d)", 
              aRect.iTl.iX, aRect.iTl.iY );
    iSecondaryRect = aRect;
    }

void CMccScreen::Update(TBool aIsForeground)
    {
        if ( aIsForeground ){
    
            iRwGroup->SetOrdinalPosition( iWindowOrdinalPosition, iWindowOrdinalPriority );
            iRw->SetOrdinalPosition( iWindowOrdinalPosition, iWindowOrdinalPriority );
            iRw->HandleTransparencyUpdate();
            iRwSession.Flush();
        } else {
            
        }
        
    }


// -----------------------------------------------------------------------------
// CMccScreen::ConstructL
// -----------------------------------------------------------------------------
//
void CMccScreen::ConstructL()
    {
    User::LeaveIfError( iRwSession.Connect() );
    	
    // Create window group

    iRwGroup = new (ELeave) RWindowGroup( iRwSession ) ;    
    
    RWindowGroup& rootWin = CCoeEnv::Static()->RootWin();
    rootWin.AllowProcessToCreateChildWindowGroups(TUid::Uid( 0x101F8681 ));
    TUint32 groupId = rootWin.WindowGroupId(); //iRwSession.GetFocusWindowGroup();
    TUint32 focusGroupId = iRwSession.GetFocusWindowGroup();
    //User::LeaveIfError( iRwGroup->ConstructChildApp( groupId, (TUint32)iRwGroup, EFalse ) );
    User::LeaveIfError( iRwGroup->Construct( (TUint32)iRwGroup, EFalse ) );
    
    TBuf<100> name;
    name.Append(KMccWindowGroupName);
    iRwGroup->SetName( name );
    
    iRwGroup->SetOrdinalPosition( iWindowOrdinalPosition, iWindowOrdinalPriority );
    //iRwGroup->SetOrdinalPriorityAdjust(1);
    iRwGroup->AutoForeground(EFalse);
    
    // Create screen device
    iDev = new (ELeave) CWsScreenDevice( iRwSession );
    iDev->Construct( iDeviceIndex ); 
    
    TInt currMode = iDev->CurrentScreenMode();
    MUS_LOG1( "mus: [ENGINE]  -> CMccScreen::ConstructL() mode: %d", currMode );                   
    TPixelsTwipsAndRotation sizeAndRotation;
    iDev->GetScreenModeSizeAndRotation( currMode, sizeAndRotation );
    
    MUS_LOG3( "mus: [ENGINE]  -> CMccScreen::ConstructL() (%dx%d) %d", 
    	sizeAndRotation.iPixelSize.iWidth, sizeAndRotation.iPixelSize.iHeight, sizeAndRotation.iRotation );
    
    // Create window
    iRw = new (ELeave) RWindow( iRwSession );
    
    User::LeaveIfError( iRw->Construct( *iRwGroup, (TUint32)iRw ) );


    iRw->SetPosition( iPosition );

    iRw->SetSize( iArea );    
    iRw->SetOrdinalPosition( iWindowOrdinalPosition, iWindowOrdinalPriority );
    iRw->EnableVisibilityChangeEvents();
    
    User::LeaveIfError( iDev->CreateContext( iGraphicsContext ) );
    
    iRw->Activate();
    iRwSession.Flush();
    }

// -----------------------------------------------------------------------------
// CMccScreen::CMccScreen
// -----------------------------------------------------------------------------
//
CMccScreen::CMccScreen( 
    TMccScreenType aScreenType,
    CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
    TInt aWindowOrdinalPriority,
    TBool aSecondary,
    RDrawableWindow* aParentWindow ) : 
    iCamera( aCamera ),
    iDeviceIndex( aIndex ), 
    iPosition( aPos ), 
    iArea( aArea ),
    iWindowOrdinalPosition( aWindowOrdinalPosition ),
    iWindowOrdinalPriority( aWindowOrdinalPriority ),
    iScreenType( aScreenType ),
    iSecondary(aSecondary),
    iParentWindow(aParentWindow)/*,
    iRwSession(CCoeEnv::Static()->WsSession()),
    iRwGroup(&CCoeEnv::Static()->RootWin()),
    iDev(CCoeEnv::Static()->ScreenDevice())*/
    {
    
    }
        

// -----------------------------------------------------------------------------
// CMccScreenBitmap::CMccScreenBitmap
// -----------------------------------------------------------------------------
//
CMccScreenBitmap* CMccScreenBitmap::NewL( 
    CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
	TInt aWindowOrdinalPriority,
	TBool aSecondary,
	RDrawableWindow* aParentWindow)
	{
	CMccScreenBitmap* self = new ( ELeave ) CMccScreenBitmap( aCamera,
                                                  aPos, 
	                                              aArea, 
	                                              aIndex,
	                                              aWindowOrdinalPosition,
	                                              aWindowOrdinalPriority,
	                                              aSecondary,
	                                              aParentWindow);
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );	
	return self;
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::CMccScreenBitmap
// -----------------------------------------------------------------------------
//
CMccScreenBitmap::CMccScreenBitmap( 
    CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
	TInt aWindowOrdinalPriority,
	TBool aSecondary,
	RDrawableWindow* aParentWindow) :
	CMccScreen( EBitmapScreen, aCamera, aPos, aArea, aIndex,
                aWindowOrdinalPosition, aWindowOrdinalPriority, aSecondary, aParentWindow ),
	iAborted( ETrue )
	{
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::CMccScreenBitmap
// -----------------------------------------------------------------------------
//
CMccScreenBitmap::~CMccScreenBitmap()
	{
	
	DetachFrame(); // Must do bitmap detach before releasing window resources
	delete iDirectScreenAccess;
    
    delete iPausedFrameData;
    delete iTimer;
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::ConstructL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::ConstructL()
	{
    iTimer = CSVPTimer::NewL( *this, 1 ); 
    
	CMccScreen::ConstructL();
	
	// Create direct screen access                                	
	//iDirectScreenAccess = CDirectScreenAccess::NewL( iRwSession, *iDev, *iRw, *this );   
	
	UpdateViewFinderArea( iArea );
	
	StartL(); // Simu HACK
	iTimer->SetTime(500, 1);
	}
    
// -----------------------------------------------------------------------------
// CMccScreenBitmap::StartL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::StartL()
	{
	iStopped = EFalse;
	
    if ( iCamera )
        {
        StartCameraL();
        }
    else if ( iPausedFrameData )
        {
           
        CFbsBitmap* frame = RestoreFrameLC();
        AttachFrameL( frame );
        CleanupStack::Pop( frame );
        }
    else
        {
        // NOP
        }
    
    StartDsaL();
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::StartCameraL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::StartCameraL()
    {  
    
    __ASSERT_ALWAYS( iCamera, User::Leave( KErrNotReady ) );
    
    // Camera may modify the size
    TSize vfBitmapSize = iArea;

    if ( VfStartPossible() )
        {
        iCamera->StartViewFinderBitmapsL( vfBitmapSize );
        }
    
     
    UpdateViewFinderArea( vfBitmapSize );
    
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::StartDsaL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::StartDsaL()
    {
    /*
    iAborted = EFalse;
    iDirectScreenAccess->Cancel();
    iDirectScreenAccess->StartL();
    iGc = iDirectScreenAccess->Gc();    
    iFbsDev = iDirectScreenAccess->ScreenDevice();
    iRegion = iDirectScreenAccess->DrawingRegion();          
    
    iGc->SetClippingRegion( iRegion );
    */
    // Fill with black areas which are not covered by viewfinder image.
    // If viewfinder image has not been yet received and drawn, fill whole area.
    //
      
 /*   if ( !iFirstImageDrawn )
        {
        // Whole area
        iGc->SetBrushColor( KRgbBlue );
        iGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
        TPoint pointTl( 0, 0 );
        TRect rect( pointTl, iArea );
        iGc->DrawRect( rect );
        }
    else
        {
        DrawBlackBorders();
        }
        
    if ( iAttachedFrame )
        {
        
        TSize viewFinderImageSize = iAttachedFrame->SizeInPixels();
        
        TPoint corner = UpdateViewFinderArea( viewFinderImageSize );
        
        iGc->BitBlt( corner, iAttachedFrame );
        }
        
    DoScreenDeviceUpdate();
    */
    //iGc->SetBrushStyle( CGraphicsContext::ENullBrush );
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::Stop
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::Stop()
	{
	// Don't do anything yet, wait for next frame and then stop camera
	iStopped = ETrue;
	}
	
// -----------------------------------------------------------------------------
// CMccScreenBitmap::Restart
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::Restart( RDirectScreenAccess::TTerminationReasons /*aReason*/ )
	{
	
	TRAP_IGNORE( StartDsaL() );
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::AbortNow
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::AbortNow( RDirectScreenAccess::TTerminationReasons /*aReason*/ )
	{
	iDirectScreenAccess->Cancel();
	
	iAborted = ETrue;
	}

// -----------------------------------------------------------------------------
// CMccScreenBitmap::Draw
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::Draw( CFbsBitmap& aFrame )
	{
	if ( iStopped )
	    {
	    
	    // Screen was waiting for freeze frame from camera, inform observer
	    // that camera is not needed anymore
	    StoreFrameDataL( aFrame );
	    CFbsBitmap* frame = RestoreFrameLC();
	    AttachFrameL( frame );
	    CleanupStack::Pop( frame );

	    StopCamera();
	    }
	else
	    {
        // When normal frame draw is requested, attached frame is not anymore drawn
        DetachFrame();
        
        DoDraw( aFrame );
	    }
	}

void CMccScreenBitmap::TimedOut( TInt aTimerId )
    {         
    iGraphicsContext->Activate(*iRw);
    iRw->Invalidate();
    iRw->BeginRedraw();
    TRgb color1(0,0,255,255);
    iGraphicsContext->Clear();
    iGraphicsContext->SetBrushColor( color1 );
    iGraphicsContext->SetBrushStyle( CGraphicsContext::ESolidBrush );
    iGraphicsContext->DrawRect( iPrimaryRect );
    TRgb color2(0,255,0,255);
    iGraphicsContext->SetBrushColor( color2 );
    iGraphicsContext->DrawRect( iSecondaryRect );
    iRw->EndRedraw();
    iGraphicsContext->Deactivate();
    iRwSession.Flush();
    
    /*
    if ( !iAborted && iGc ){
        iGc->SetBrushColor( KRgbBlue );
        iGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
        iGc->DrawRect( iPrimaryRect );
        iGc->SetBrushColor( KRgbGreen );
        iGc->DrawRect( iSecondaryRect );
        DoScreenDeviceUpdate();
        }
        */
    iTimer->SetTime(500, 1);
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::AttachFrameL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::AttachFrameL( CFbsBitmap* aFrame )
    {
    
    __ASSERT_ALWAYS( aFrame, User::Leave( KErrArgument ) );
    
    DetachFrame();
    
    iAttachedFrame = aFrame;
    
    DoDraw( *iAttachedFrame );

    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::DetachFrame
// -----------------------------------------------------------------------------
//		
void CMccScreenBitmap::DetachFrame()
    {	
    if ( iAttachedFrame )
        {
        iAttachedFrame->Reset();
        delete iAttachedFrame;
        }
        
    iAttachedFrame = NULL;
    }
    
// -----------------------------------------------------------------------------
// CMccScreenBitmap::UpdateViewFinderArea
// -----------------------------------------------------------------------------
//
TPoint CMccScreenBitmap::UpdateViewFinderArea( TSize aViewFinderImageSize )
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
// CMccScreenBitmap::DoDraw
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::DoDraw( CFbsBitmap& aFrame )
    {	
    if ( !iAborted && iGc )
        {
        iFirstImageDrawn = ETrue;
        	
       	TSize viewFinderImageSize = aFrame.SizeInPixels();
        
       	TBool imageSizeChanged = ( viewFinderImageSize != iViewFinderImageRect.Size() );
       	
    	TPoint corner = UpdateViewFinderArea( viewFinderImageSize );
    	
    	if ( imageSizeChanged )
    	    {
    	    // Size of bitmap changed suddenly, borders need to be redrawn
    	    DrawBlackBorders();
    	    }

        iGc->BitBlt( corner, &aFrame );
        
        DoScreenDeviceUpdate();
        }
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::DoScreenDeviceUpdate
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::DoScreenDeviceUpdate()
    {
    if ( iFbsDev )
        {
        iFbsDev->Update();
        }
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::StoreFrameDataL
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::StoreFrameDataL( CFbsBitmap& aFrame )
    {
    
    iPausedFrameSize = aFrame.SizeInPixels();
    iPausedFrameDisplayMode = aFrame.DisplayMode();
    TInt bitmapSizeInBytes = 
            CFbsBitmap::ScanLineLength( iPausedFrameSize.iWidth, iPausedFrameDisplayMode ) *
            iPausedFrameSize.iHeight;
    HBufC8* pausedFrameData = HBufC8::NewLC( bitmapSizeInBytes );
    aFrame.LockHeap();
    pausedFrameData->Des().Copy( (TUint8*)aFrame.DataAddress(), bitmapSizeInBytes );
    aFrame.UnlockHeap(); 
    delete iPausedFrameData;
    iPausedFrameData = pausedFrameData;
    CleanupStack::Pop( pausedFrameData );
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::RestoreFrameLC
// -----------------------------------------------------------------------------
//
CFbsBitmap* CMccScreenBitmap::RestoreFrameLC()
    {
    
    __ASSERT_ALWAYS( iPausedFrameData, User::Leave( KErrNotReady ) );
    
    CFbsBitmap* tempFrame = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( tempFrame );
    
    User::LeaveIfError( tempFrame->Create( iPausedFrameSize, iPausedFrameDisplayMode ) );
    TInt bitmapSizeInBytes = 
            CFbsBitmap::ScanLineLength( iPausedFrameSize.iWidth, iPausedFrameDisplayMode ) *
            iPausedFrameSize.iHeight;
   
    tempFrame->LockHeap();
    Mem::Copy( tempFrame->DataAddress(), 
               iPausedFrameData->Des().Ptr(), 
               bitmapSizeInBytes );
    tempFrame->UnlockHeap();
    
    
    return tempFrame;
    }

// -----------------------------------------------------------------------------
// CMccScreenBitmap::DrawBlackBorders
// -----------------------------------------------------------------------------
//
void CMccScreenBitmap::DrawBlackBorders()
    {
    iGc->SetBrushColor( KRgbBlue );
    iGc->SetBrushStyle( CGraphicsContext::ESolidBrush );
            
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
	
// -----------------------------------------------------------------------------
// CMccScreenDirect::NewL
// -----------------------------------------------------------------------------
//
CMccScreenDirect* CMccScreenDirect::NewL( CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
    TInt aWindowOrdinalPriority )
    {
    
    CMccScreenDirect* self = new ( ELeave ) CMccScreenDirect( aCamera,
                                                      aPos, 
                                                      aArea, 
                                                      aIndex,
                                                      aWindowOrdinalPosition,
                                                      aWindowOrdinalPriority );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );  
    
    return self;
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::~CMccScreenDirect
// -----------------------------------------------------------------------------
//
CMccScreenDirect::~CMccScreenDirect()
    {
    
    if ( iDirectViewFinder )
        {
        iDirectViewFinder->Release();
        iDirectViewFinder = NULL;
        }

    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::StartL
// -----------------------------------------------------------------------------
//
void CMccScreenDirect::StartL()
    {

    
    if ( iDirectViewFinder && iDirectViewFinder->ViewFinderState() == 
            CCamera::CCameraDirectViewFinder::EViewFinderPause )
        {
        
        iDirectViewFinder->ResumeViewFinderDirectL();
        }
    else if ( VfStartPossible() )
        {
       
        StartDirectViewFinderL();
        }
    else
        {
        // NOP
        }
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::Stop
// -----------------------------------------------------------------------------
//
void CMccScreenDirect::Stop()
    {
    
    if ( iDirectViewFinder )
        {
		// TBD: pausing disabled temporarily due problems in it
        //TRAP_IGNORE( iDirectViewFinder->PauseViewFinderDirectL() )
        }
    
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::SetCamera
// -----------------------------------------------------------------------------
//
TInt CMccScreenDirect::SetCamera( CCamera* aCamera )
    {
    CMccScreen::SetCamera( aCamera );
    
    return HandleDirectViewFinder();
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::ConstructL
// -----------------------------------------------------------------------------
//
void CMccScreenDirect::ConstructL()
    {
        
    CMccScreen::ConstructL();
    
    User::LeaveIfError( HandleDirectViewFinder() );
 
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::CMccScreenDirect
// -----------------------------------------------------------------------------
//
CMccScreenDirect::CMccScreenDirect( 
    CCamera* aCamera,
    TPoint aPos, 
    TSize aArea, 
    TInt aIndex, 
    TInt aWindowOrdinalPosition,
    TInt aWindowOrdinalPriority ) : 
    CMccScreen( EDirectScreen, aCamera, aPos, aArea, aIndex,
                aWindowOrdinalPosition, aWindowOrdinalPriority )
    {
    
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::StartDirectViewFinderL
// -----------------------------------------------------------------------------
//
void CMccScreenDirect::StartDirectViewFinderL()
    {
    if ( VfStartPossible() )
        {
        // Camera may modify the size
        TSize vfBitmapSize = iArea;
        
        TPoint point( 0, 0 );
        TRect vfRect( point, vfBitmapSize );
        iCamera->StartViewFinderDirectL( iRwSession, *iDev, *iRw, vfRect );
        }
    }

// -----------------------------------------------------------------------------
// CMccScreenDirect::HandleDirectViewFinder
// -----------------------------------------------------------------------------
//
TInt CMccScreenDirect::HandleDirectViewFinder()
    {
    TInt err( KErrNone );
    if ( iCamera )
        {
        if ( !iDirectViewFinder )
            {

            iDirectViewFinder = static_cast<MCameraDirectViewFinder*>(
                iCamera->CustomInterface( TUid::Uid( KECamMCameraDirectViewFinderUidValue ) ) );
            
            if ( !iDirectViewFinder )
                {
                err = KErrNotSupported;
                }
            }
        }
    else if ( iDirectViewFinder )
        {
        iDirectViewFinder->Release();
        iDirectViewFinder = NULL;
        }
    else
        {
        // NOP
        }
    return err; 
    }


// End of file

