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


#include "mcecamerasource.h"
#include "mcesession.h"
#include "mcemediastream.h"
#include "mcemanager.h"


const TInt KMinDigitalZoom = 0;
const TInt KMinContrast = -100;
const TInt KMaxContrast = 100;
const TInt KMinBrightness = -100;
const TInt KMaxBrightness = 100; 


const TInt KInitialCameraIndex = 0;
const TInt KInitialZoomFactor = 1;
const TInt KInitialDigitalZoomFactor = 1;
const TInt KInitialContrast = 0;
const TInt KInitialBrightness = 0;
const TInt KInitialExposure = CCamera::EExposureAuto;
const TInt KInitialWhiteBalance = CCamera::EWBAuto;

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCameraSource* CMceCameraSource::NewL( CMceManager& aManager )
    {
    CMceCameraSource* self = CMceCameraSource::NewLC( aManager );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCameraSource* CMceCameraSource::NewLC( CMceManager& /*aManager*/ )
    {
    CMceCameraSource* self = new( ELeave )CMceCameraSource();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCameraSource* CMceCameraSource::NewL()
    {
    CMceCameraSource* self = CMceCameraSource::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCameraSource* CMceCameraSource::NewLC()
    {
    CMceCameraSource* self = new( ELeave )CMceCameraSource();
    CleanupStack::PushL( self );
    return self;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceCameraSource::~CMceCameraSource()
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCameraSource::EnableL()
    {
    CMceMediaSource::DoEnableL();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCameraSource::DisableL()
    { 
    CMceMediaSource::DoDisableL();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	    
EXPORT_C TInt CMceCameraSource::CamerasAvailable() const
    {
    return iCameraCount;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceCameraSource::SetCameraIndexL( TInt aCameraIndex )	  
    {
    __ASSERT_ALWAYS( aCameraIndex >= 0 &&
                     aCameraIndex < this->CamerasAvailable(),
                     User::Leave( KErrArgument ) );
                     
   
    iCameraIndex = aCameraIndex;         
    
    }          
     
       
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	     
EXPORT_C void CMceCameraSource::GetCameraInfo( TCameraInfo& aInfo ) const
    {
    aInfo = iCameraInfo;
    
    aInfo.iMaxZoom += iCameraIndex*5;
    aInfo.iMaxDigitalZoom += iCameraIndex*5;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetZoomFactorL( TInt aZoomFactor )
    {
    __ASSERT_ALWAYS( aZoomFactor >= iCameraInfo.iMinZoom &&
                     aZoomFactor <= iCameraInfo.iMaxZoom, 
                     User::Leave( KErrArgument ) );
    
    iZoomFactor = aZoomFactor;    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCameraSource::ZoomFactorL()
    {
    return iZoomFactor; 
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetDigitalZoomFactorL( TInt aDigitalZoomFactor )
    {
    __ASSERT_ALWAYS( aDigitalZoomFactor >= KMinDigitalZoom &&
                     aDigitalZoomFactor <= iCameraInfo.iMaxDigitalZoom, 
                     User::Leave( KErrArgument ) );
    
    iDigitalZoomFactor = aDigitalZoomFactor;
        
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCameraSource::DigitalZoomFactorL()
    {
    return iDigitalZoomFactor;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetContrastL( TInt aContrast )
    {
    
    __ASSERT_ALWAYS( ( iCameraInfo.iOptionsSupported & 
                       TCameraInfo::EContrastSupported ) ==
                     TCameraInfo::EContrastSupported,
                     User::Leave( KErrNotSupported ) );
                    
    __ASSERT_ALWAYS( (( aContrast >= KMinContrast &&
                        aContrast <= KMaxContrast ) ||
                        aContrast == CCamera::EContrastAuto ),
                     User::Leave( KErrArgument ) );
                      
                                           
    iContrast = aContrast;
  
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCameraSource::ContrastL()
    {
    return iContrast;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetBrightnessL( TInt aBrightness )
    {
    
    __ASSERT_ALWAYS( ( iCameraInfo.iOptionsSupported & 
                       TCameraInfo::EBrightnessSupported ) ==
                     TCameraInfo::EBrightnessSupported,
                     User::Leave( KErrNotSupported ) );
                   
    __ASSERT_ALWAYS( (( aBrightness >= KMinBrightness &&
                        aBrightness <= KMaxBrightness ) ||
                        aBrightness == CCamera::EBrightnessAuto ),
                     User::Leave( KErrArgument ) );
    
    iBrightness = aBrightness;
        
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CMceCameraSource::BrightnessL()
    {
    return iBrightness; 
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetExposureL( CCamera::TExposure aExposure )
    {
    
    __ASSERT_ALWAYS( ( iCameraInfo.iExposureModesSupported & aExposure ) ==
                     aExposure,
                     User::Leave( KErrNotSupported ) );
  
    iExposure = aExposure;
    
    }
        


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C CCamera::TExposure CMceCameraSource::ExposureL()
    {
    
    
    return static_cast<CCamera::TExposure>( iExposure );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C void CMceCameraSource::SetWhiteBalanceL( 
                                    CCamera::TWhiteBalance aWhiteBalance )
    {
    __ASSERT_ALWAYS( 
        ( iCameraInfo.iWhiteBalanceModesSupported & aWhiteBalance ) == 
        aWhiteBalance,
        User::Leave( KErrNotSupported ) );
   
    iWhiteBalance = aWhiteBalance;   
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C CCamera::TWhiteBalance CMceCameraSource::WhiteBalanceL()
    {
    
    return static_cast<CCamera::TWhiteBalance>( iWhiteBalance );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMceCameraSource::InitializeL( CMceManager* /*aManager*/ )
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMceCameraSource::CMceCameraSource()
    :CMceMediaSource(),
    iCameraIndex( 0 ),
    iZoomFactor( 1 ),
    iDigitalZoomFactor( 0 ),
    iContrast( 0 ),
    iBrightness( 0 ),
    iExposure( CCamera::EExposureAuto ),
    iWhiteBalance( CCamera::EWBAuto ),
    iCameraCount ( 2 )
    {
    iType = KMceCameraSource;
    iCameraInfo.iMinZoom = 1;
    iCameraInfo.iMaxZoom = 3;
    iCameraInfo.iMaxDigitalZoom = 10;
    }
