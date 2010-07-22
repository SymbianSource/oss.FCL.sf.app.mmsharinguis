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
*  Version     : %version: 20 % << Don't touch! Updated by Synergy at check-out.
*
*/


#ifndef MUSSETTINGS_INL
#define MUSSETTINGS_INL

#include "mussettings.h"
#include "mussettingskeys.h"
#include "muslogger.h"
#include "mussipprofileutils.h"

#include <centralrepository.h>

const TInt32 KMusDisableAVC = 0x0fffffff;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline TInt MultimediaSharingSettings::GetPropertyValueL( TUint32 aKey )
    {
    TInt value( KErrNotFound );

    CRepository* repository = CRepository::NewLC(
                                MusSettingsKeys::KRepositoryUid );

    User::LeaveIfError(repository->Get( aKey, value ));

    CleanupStack::PopAndDestroy( repository );

    return value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetPropertyValueL( TUint32 aKey,
                                                          TInt aValue )
    {
    CRepository* repository = CRepository::NewLC(
                                MusSettingsKeys::KRepositoryUid );

    User::LeaveIfError(repository->Set( aKey, aValue ));

    CleanupStack::PopAndDestroy( repository );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline HBufC16* MultimediaSharingSettings::GetPropertyValueLC( TUint32 aKey )
    {
    TBuf16<NCentralRepositoryConstants::KMaxUnicodeStringLength> value;

    CRepository* repository = CRepository::NewLC(
                                MusSettingsKeys::KRepositoryUid );

    User::LeaveIfError( repository->Get( aKey, value ) );

    CleanupStack::PopAndDestroy( repository );

    return value.AllocLC();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetPropertyValueL( 
                                            TUint32 aKey,
                                            const TDesC16& aValue )
    {
    CRepository* repository = CRepository::NewLC(
                                MusSettingsKeys::KRepositoryUid );

    User::LeaveIfError(repository->Set( aKey, aValue ));

    CleanupStack::PopAndDestroy( repository );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TActivation
    MultimediaSharingSettings::ActivationSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::ActivationSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KActivation );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::ActivationSettingL( %d )",
              value )
    return ( MusSettingsKeys::TActivation ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetActivationSettingL(
                MusSettingsKeys::TActivation aActivationSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetActivationSettingL( %d )",
              ( TInt ) aActivationSetting )
    SetPropertyValueL( MusSettingsKeys::KActivation, aActivationSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetActivationSettingL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TOperatorVariant
    MultimediaSharingSettings::OperatorVariantSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::OperatorVariantSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KOperatorVariant );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::OperatorVariantSettingL( %d )",
              value )
    return ( MusSettingsKeys::TOperatorVariant ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetOperatorVariantSettingL(
                MusSettingsKeys::TOperatorVariant aVariantSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetOperatorVariantSettingL( %d )",
              ( TInt ) aVariantSetting )
    SetPropertyValueL( MusSettingsKeys::KOperatorVariant, ( TInt ) aVariantSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetOperatorVariantSettingL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TAuditoryNotification
    MultimediaSharingSettings::AuditoryNotificationSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::AuditoryNotificationSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KAuditoryNotification );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::AuditoryNotificationSettingL( %d )",
              value )
    return ( MusSettingsKeys::TAuditoryNotification ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetAuditoryNotificationSettingL(
            MusSettingsKeys::TAuditoryNotification aNotificationSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetAuditoryNotificationSettingL( %d )",
              ( TInt ) aNotificationSetting )
    SetPropertyValueL( MusSettingsKeys::KAuditoryNotification, ( TInt ) aNotificationSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetAuditoryNotificationSettingL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TPopupNotification
    MultimediaSharingSettings::PopupNotificationSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::PopupNotificationSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KPopupNotification );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::PopupNotificationSettingL( %d )",
              value )
    return ( MusSettingsKeys::TPopupNotification ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetPopupNotificationSettingL(
                MusSettingsKeys::TPopupNotification aNotificationSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetPopupNotificationSettingL( %d )",
              ( TInt ) aNotificationSetting )
    SetPropertyValueL( MusSettingsKeys::KPopupNotification,
                       ( TInt ) aNotificationSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetPopupNotificationSettingL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TPopupNotificationType
    MultimediaSharingSettings::PopupNotificationTypeSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::PopupNotificationTypeSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KPopupNotificationType );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::PopupNotificationTypeSettingL( %d )",
              value )
    return ( MusSettingsKeys::TPopupNotificationType ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetPopupNotificationTypeSettingL(
            MusSettingsKeys::TPopupNotificationType aNotificationSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetActivationSettingL( %d )",
              ( TInt ) aNotificationSetting )
    SetPropertyValueL( MusSettingsKeys::KPopupNotificationType, ( TInt ) aNotificationSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetActivationSettingL()" )

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TEdgeDtmSupport
    MultimediaSharingSettings::EdgeDtmSupportSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::EdgeDtmSupportSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KEdgeDtmSupport );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::EdgeDtmSupportSettingL( %d )",
              value )
    return ( MusSettingsKeys::TEdgeDtmSupport ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetEdgeDtmSupportSettingL(
                MusSettingsKeys::TEdgeDtmSupport aSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetEdgeDtmSupportSettingL( %d )",
              ( TInt ) aSetting )
    SetPropertyValueL( MusSettingsKeys::KEdgeDtmSupport, ( TInt ) aSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetEdgeDtmSupportSettingL()" )

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TForceInternetSignaling
    MultimediaSharingSettings::ForceInternetSignalingSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::ForceInternetSignalingSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KForceInternetSignaling );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::ForceInternetSignalingSettingL( %d )",
              value )
    return ( MusSettingsKeys::TForceInternetSignaling ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetForceInternetSignalingSettingL(
                MusSettingsKeys::TForceInternetSignaling aSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetSessionSetupMethodSettingL( %d )",
              ( TInt ) aSetting )
    SetPropertyValueL( MusSettingsKeys::KForceInternetSignaling, ( TInt ) aSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetSessionSetupMethodSettingL()" )

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TAutoRecord
    MultimediaSharingSettings::AutoRecordSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::AutoRecordSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KAutoRecord );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::AutoRecordSettingL( %d )",
              value )
    return ( MusSettingsKeys::TAutoRecord ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetAutoRecordSettingL(
                MusSettingsKeys::TAutoRecord aSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetActivationSettingL( %d )",
              ( TInt ) aSetting )
    SetPropertyValueL( MusSettingsKeys::KAutoRecord, ( TInt ) aSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetActivationSettingL()" )

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline TInt MultimediaSharingSettings::VideoLocationSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::VideoLocationSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KVideoLocation );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::VideoLocationSettingL( %d )",
              value )
    return value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetVideoLocationSettingL(
                TInt aSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetVideoLocationSettingL( %d )",
              ( TInt ) aSetting )
    SetPropertyValueL( MusSettingsKeys::KVideoLocation, ( TInt ) aSetting );   
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetVideoLocationSettingL()" )

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline TInt MultimediaSharingSettings::SipProfileSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::SipProfileSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KSipProfileId );
    MUS_LOG1( "mus: [MUSSET]     <- MultimediaSharingSettings::SipProfileSettingL( %d )",
              value )
    return value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetSipProfileSettingL( TInt aSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetSipProfileSettingL( %d )",
              ( TInt ) aSetting )
              
    TInt oldProfileId = GetPropertyValueL( MusSettingsKeys::KSipProfileId );
    
    if( oldProfileId != KErrNotFound && oldProfileId != KErrNone )
        {
        MusSipProfileUtils::RemoveContactHeaderL( oldProfileId );
        }
    
    SetPropertyValueL( MusSettingsKeys::KSipProfileId, ( TInt ) aSetting );

    MusSipProfileUtils::InsertContactHeaderL( aSetting );

    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetSipProfileSettingL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TUiOrientation
    MultimediaSharingSettings::UiOrientationSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::UiOrientationSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KUiOrientation );
    MUS_LOG1( "mus: [MUSSET]  <- MultimediaSharingSettings::UiOrientationSettingL( %d )",
              value )
    return ( MusSettingsKeys::TUiOrientation ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetUiOrientationSettingL(
                MusSettingsKeys::TUiOrientation aOrientationSetting )
    {
    MUS_LOG1( "mus: [MUSSET]  -> MultimediaSharingSettings::SetActivationSettingL( %d )",
              ( TInt ) aOrientationSetting )
    SetPropertyValueL( MusSettingsKeys::KUiOrientation,
                       ( TInt ) aOrientationSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetActivationSettingL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TCapabilityQuery
    MultimediaSharingSettings::CapabilityQuerySettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::CapabilityQuerySettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KCapabilityQuery );
    MUS_LOG1( "mus: [MUSSET]  <- MultimediaSharingSettings::CapabilityQuerySettingL( %d )",
              value )
    return ( MusSettingsKeys::TCapabilityQuery ) value;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetCapabilityQuerySettingL(
                MusSettingsKeys::TCapabilityQuery aOrientationSetting )
    {
    MUS_LOG1( "mus: [MUSSET]     -> MultimediaSharingSettings::SetActivationSettingL( %d )",
              ( TInt ) aOrientationSetting )
    SetPropertyValueL( MusSettingsKeys::KCapabilityQuery,
                       ( TInt ) aOrientationSetting );
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetActivationSettingL()" )
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::ResourceAvailability(
                                       MusSettingsKeys::TAvailability &aCamera,
                                       MusSettingsKeys::TAvailability &aKeypad)
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::ResourceAvailability()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KProductModeVariation );
    // count 3 bit ,bit->100 = 4 (int) for camera
    aCamera = (value&4) ? MusSettingsKeys::EAvailabilityDynamic
                        : MusSettingsKeys::EAvailabilityStatic;
    // count 6 bit ,bit->100000 = 32 (int) for keypad
    aKeypad = (value&32) ? MusSettingsKeys::EAvailabilityDynamic
                         : MusSettingsKeys::EAvailabilityStatic;  
                           
    MUS_LOG2( "mus: [MUSSET]  <- MultimediaSharingSettings::ResourceAvailability \
             Camera = %d, Keypad = %d", aCamera,aKeypad)    
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::ResourceUsability(
                                       MusSettingsKeys::TUsability &aCamera,
                                       MusSettingsKeys::TUsability &aKeypad,
                                       MusSettingsKeys::TUsability &aVideoplayer)
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::ResourceUsability()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KProductModeVariation );
    MUS_LOG1( "mus: [MUSSET] product mode variation cenrep Key value = %d",value )      
    // Mask bit 1&2 with 11=3   
    TInt tempValue = value&3 ;    
    aCamera = (tempValue < 3) ? ( MusSettingsKeys::TUsability )tempValue 
    						  : MusSettingsKeys::EUsabilityAlways;
    
    // Left shift 3 bits and mask bit 1&2 with 11=3 so we get bits for keypad
    value = value >> 3;    
    tempValue = value&3 ;    
    aKeypad = ( tempValue < 3 ) ? ( MusSettingsKeys::TUsability ) tempValue 
                                : MusSettingsKeys::EUsabilityAlways;
    
    // Left shift again 3 bits mask bit 1&2 with 11=3,so we get bits for videoplayer    
    value = value >> 3;
    tempValue = value&3 ;    
    aVideoplayer = ( tempValue < 3 ) ? ( MusSettingsKeys::TUsability )tempValue 
                                     : MusSettingsKeys::EUsabilityAlways;            
                                 
    MUS_LOG3( "mus: [MUSSET] Camera = %d, Keypad = %d,Videoplayer = %d",\
                                aCamera,aKeypad,aVideoplayer ) 
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::ResourceUsability()" )   
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline HBufC8* MultimediaSharingSettings::EncoderConfigInfoLC()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::EncoderConfigInfoLC()" )
    
    TBuf8<NCentralRepositoryConstants::KMaxBinaryLength> keys;
    
    CRepository* repository = CRepository::NewLC(
                                MusSettingsKeys::KRepositoryUid );

    User::LeaveIfError( repository->Get( 
                                MusSettingsKeys::KEncoderConfigurationInfo, 
                                keys ) );

    CleanupStack::PopAndDestroy( repository );
    
    MUS_LOG_TDESC8( "mus: [MUSSET]        Keys: ", keys ) 
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::EncoderConfigInfoLC()" )
    
    return keys.AllocLC();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline void MultimediaSharingSettings::SetEncoderConfigInfoL( 
                                                const TDesC8& aConfigKeys )
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::SetEncoderConfigInfoL()" )

    CRepository* repository = CRepository::NewLC(
                                        MusSettingsKeys::KRepositoryUid );

    User::LeaveIfError( repository->Set( 
                                MusSettingsKeys::KEncoderConfigurationInfo,
                                aConfigKeys ) );

    CleanupStack::PopAndDestroy( repository );
    
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::SetEncoderConfigInfoL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline TUid MultimediaSharingSettings::EncodingDeviceL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::EncodingDeviceL()" )
    
    TInt value = GetPropertyValueL( MusSettingsKeys::KEncodingDevice );
    
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::EncodingDeviceL()")
    
    return TUid::Uid( value );
    }

// -----------------------------------------------------------------------------
// Returns the cenrep configured value for KAllowOnlyWithActiveCSCall key.
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TAllowedCSOnly
    MultimediaSharingSettings::AllowedCSOnlySettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::AllowedCSOnlySettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KAllowOnlyWithActiveCSCall );
    MUS_LOG1( "mus: [MUSSET]  <- MultimediaSharingSettings::AllowedCSOnlySettingL( %d )",
              value )
    return ( MusSettingsKeys::TAllowedCSOnly ) value;
    }
    
// -----------------------------------------------------------------------------
// Returns the cenrep configured value for KAllowOnlyIn3GNetwork key.
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TAllowed3GOnly
    MultimediaSharingSettings::Allowed3GOnlySettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::Allowed3GOnlySettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KAllowOnlyIn3GNetwork );
    MUS_LOG1( "mus: [MUSSET]  <- MultimediaSharingSettings::Allowed3GOnlySettingL( %d )",
              value )
    return ( MusSettingsKeys::TAllowed3GOnly ) value;
    }    


// -----------------------------------------------------------------------------
// Returns the cenrep configured value for KCameraUsage key.
// -----------------------------------------------------------------------------
//
inline MusSettingsKeys::TUseCamera 
              MultimediaSharingSettings::CameraUsageL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::CameraUsageL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KCameraUsage );
    MUS_LOG1( "mus: [MUSSET]  <- MultimediaSharingSettings::CameraUsageL( %d )",
              value )
    return ( MusSettingsKeys::TUseCamera ) value;
    }    


// -----------------------------------------------------------------------------
// Returns the cenrep configured value for KVideoDirection key.
// -----------------------------------------------------------------------------
inline MusSettingsKeys::TVideoDirection 
              MultimediaSharingSettings::VideoDirectionL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::VideoDirectionL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KVideoDirection );
    MUS_LOG1( "mus: [MUSSET]  <- MultimediaSharingSettings::VideoDirectionL( %d )",
              value )
    return ( MusSettingsKeys::TVideoDirection ) value;
    }
    
// -----------------------------------------------------------------------------
// Returns the cenrep configured value for KVideoBandwidth key.
// -----------------------------------------------------------------------------
inline TInt MultimediaSharingSettings::VideoBandwidthSettingL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::VideoBandwidthSettingL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KVideoBandwidth );
    MUS_LOG1( "mus: [MUSSET]  <- MultimediaSharingSettings::VideoBandwidthSettingL( %d )",
              value )
    return value;
    }

// -----------------------------------------------------------------------------
// Returns the cenrep configured value for KFastStartupMode key.
// -----------------------------------------------------------------------------
inline MusSettingsKeys::TFastMode 
              MultimediaSharingSettings::FastStartupModeL()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::FastStartupModeL()" )
    TInt value = GetPropertyValueL( MusSettingsKeys::KFastStartupMode );
    MUS_LOG1( "mus: [MUSSET]  <- MultimediaSharingSettings::FastStartupModeL( %d )",
              value )
    return ( MusSettingsKeys::TFastMode ) value;
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
inline TBool MultimediaSharingSettings::IsAvcDisabled()
    {
    MUS_LOG( "mus: [MUSSET]  -> MultimediaSharingSettings::IsAvcDisabled()" )
    TInt value = 0;
    TRAPD(err, value =GetPropertyValueL( MusSettingsKeys::KEncodingDevice ));    
    if( err==KErrNone && value == KMusDisableAVC )
        {
        MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::IsAvcDisabled() true")
        return ETrue;
        }
    MUS_LOG( "mus: [MUSSET]  <- MultimediaSharingSettings::IsAvcDisabled() false")
    return EFalse;
    }
    
#endif // MUSSETTINGS_INL
