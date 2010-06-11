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
* Description:  ?Description
*
*/


#ifndef MUSRESOURCEPROPERTIES_H
#define MUSRESOURCEPROPERTIES_H

#include <e32cmn.h>

/**
 *  A namespace container for MultimediaSharing Resource API constants.
 *  This namespace has PS keys for dynamic resource setting which is used
 *  in multimediasharing. 
 *  
 *  For example :
 *          If a product program configures the cenrep value of
 *          MusSettingsKeys::KProductModeVariation ( refer mussettingskeys.h)
 *          bit#2 and bit#5 to 1 ( means dynamic availability ) then it is upto 
 *          product program to inform multimediasharing when camera and keypad 
 *          resources will be available.
 *
 *  In a practical usecase a product has camera slider and it must configure
 *  cenrep value of MusSettingsKeys::KProductModeVariation bit#2 to 1. Also when
 *  camera slider is open then it should call 
 *  
 *             RProperty::Set( NMusResourceApi::KCategoryUid,
 *                             NMusResourceApi::KCameraAvailability,
 *                             ( TInt ) NMusResourceApi::EAvailable );
 *
 *  When camera slider is closed , It must call
 *  
 *             RProperty::Set( NMusResourceApi::KCategoryUid,
 *                             NMusResourceApi::KCameraAvailability,
 *                             ( TInt ) NMusResourceApi::ENotAvailable );
 *     
 */
namespace NMusResourceApi
    {
    /**  The category of these P&S properties. */
#ifndef UNIT_TESTING
  const TUid KCategoryUid = { 0x101F85EE }; // UID3 of AO Server  
#else
  const TUid KCategoryUid = { 0x01700a21 }; // UID3 of EunitTest of AO
#endif

    /**  Contains the availability state of camera resource.
      *  It can have one of the values of TAvailability.
      */
    const TInt KCameraAvailability( 0x1028239B );
    
    /**  Contains the availability state of keypad resource.
      *  It can have one of the values of TAvailability.
      */
    const TInt KKeypadAvailability( 0x1028239C );
    
    /**  Contains the availability state of video resource.
      *  It can have one of the values of TAvailability.
      */
    const TInt KVideoPlayerAvailability( 0x1028239D );

    /**  Contains the possible values of availability properties. */
    enum TAvailability
        {        
        EAvailable = 1,        
        ENotAvailable
        };

    /**  Contains the identifier of the preferred camera.
      *  It can have one of the values of TCameraInformation
      */
    const TInt KCameraInformation( 0x1028239E );
    
    /**  Contains the possible values of camera properties. */
    enum TCameraInformation
        {
        EUsePrimaryCamera = 1,
        EUseSecondaryCamera
        };

    }

#endif // MUSRESOURCEPROPERTIES_H