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
*  Name        : mussettingskeys.h

*  Version     : %version: 10 % << Don't touch! Updated by Synergy at check-out.
*
*/


#ifndef MUSSETTINGS_H
#define MUSSETTINGS_H

#include "mussettingskeys.h"

/**
 *  A namespace container for MultimediaSharing Settings API constants.
 */
class MultimediaSharingSettings
    {
public:
    
    /**
     * Returns the cenrep configured value for KFastStartupMode key.
     *
     * @return Current KVideoDirection Key value from cenrep.
     */
    static MusSettingsKeys::TFastMode FastStartupModeL();
    
    
    /**
     * Returns the cenrep configured value for KVideoBandwidth key.
     *
     * @return Current KVideoBandwidth Key value from cenrep.
     */
    static TInt VideoBandwidthSettingL();

    /**
     * Returns the cenrep configured value for KVideoDirection key.
     *
     * @return Current KVideoDirection Key value from cenrep.
     */
    static MusSettingsKeys::TVideoDirection VideoDirectionL();
    

    /**
     * Returns the cenrep configured value for KCameraUsage key.
     *
     * @return Current KCameraUsage Key value from cenrep.
     */
    static MusSettingsKeys::TUseCamera CameraUsageL();
    
    /**
     * Returns the cenrep configured value for AllowedCSOnly key.
     *
     * @return Current AllowedCSOnly Key value from cenrep.
     */
    static MusSettingsKeys::TAllowedCSOnly AllowedCSOnlySettingL();

    /**
     * Returns the cenrep configured value for Allowed3GOnly key.
     *
     * @return Current Allowed3GOnly Key value from cenrep.
     */
    static MusSettingsKeys::TAllowed3GOnly Allowed3GOnlySettingL();

    /**
     * Returns current MS activation setting.
     *
     * @return Current MS activation setting.
     */
    static MusSettingsKeys::TActivation ActivationSettingL();
    
    /**
     * Sets the MS activation setting
     *
     * @param
     */
    static void SetActivationSettingL( MusSettingsKeys::TActivation aActivationSetting );
    
    /**
     * Returns 
     *
     * @return
     */
    static MusSettingsKeys::TOperatorVariant
                MultimediaSharingSettings::OperatorVariantSettingL();

    /**
     * Sets 
     *
     * @param
     */
    static void MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::TOperatorVariant aVariantSetting );

    /**
     * Returns 
     *
     * @return
     */
    static MusSettingsKeys::TAuditoryNotification AuditoryNotificationSettingL();

    /**
     * Sets 
     *
     * @param
     */
    static void SetAuditoryNotificationSettingL( 
                MusSettingsKeys::TAuditoryNotification aNotificationSetting );

    /**
     * Returns 
     *
     * @return
     */
    static MusSettingsKeys::TPopupNotification PopupNotificationSettingL();

    /**
     * Sets 
     *
     * @param
     */
    static void SetPopupNotificationSettingL( 
                MusSettingsKeys::TPopupNotification aNotificationSetting );
          
    /**
     * Returns 
     *
     * @return
     */
    static MusSettingsKeys::TPopupNotificationType PopupNotificationTypeSettingL();
					
    /**
     * Sets 
     *
     * @param
     */
    static void SetPopupNotificationTypeSettingL( 
                MusSettingsKeys::TPopupNotificationType aNotificationSetting );

    /**
     * Returns 
     *
     * @return
     */
    static MusSettingsKeys::TEdgeDtmSupport EdgeDtmSupportSettingL();
    
    /**
     * Sets 
     *
     * @param
     */
    static void SetEdgeDtmSupportSettingL( 
            MusSettingsKeys::TEdgeDtmSupport aSetting );
    
    /**
     * Returns 
     *
     * @return
     */
    static MusSettingsKeys::TForceInternetSignaling ForceInternetSignalingSettingL();
    
    /**
     * Sets 
     *
     * @param
     */
    static void SetForceInternetSignalingSettingL( 
            MusSettingsKeys::TForceInternetSignaling aSetting );
    
    /**
     * Returns 
     *
     * @return
     */
    static MusSettingsKeys::TAutoRecord AutoRecordSettingL();
    
    /**
     * Sets 
     *
     * @param
     */
    static void SetAutoRecordSettingL( MusSettingsKeys::TAutoRecord aSetting );
    
    /**
     * Returns 
     *
     * @return
     */
    static TInt VideoLocationSettingL();
    
    /**
     * Sets 
     *
     * @param
     */
    static void SetVideoLocationSettingL( TInt aSetting );
    
    /**
     * Returns 
     *
     * @return
     */
    static TInt SipProfileSettingL();
    
    /**
     * Sets 
     *
     * @param
     */
    static void SetSipProfileSettingL( TInt aSetting );
    
    /**
     * Returns 
     *
     * @return
     */
    static MusSettingsKeys::TUiOrientation UiOrientationSettingL();
    
    /**
     * Sets 
     *
     * @param
     */
    static void SetUiOrientationSettingL( 
            MusSettingsKeys::TUiOrientation aOrientationSetting );

    /**
     * Returns 
     *
     * @return
     */
    static MusSettingsKeys::TCapabilityQuery CapabilityQuerySettingL();

    /**
     * Sets 
     *
     * @param
     */
    static void SetCapabilityQuerySettingL(
                    MusSettingsKeys::TCapabilityQuery aOrientationSetting );
                    
    /**
     * Returns resource availability which is read from cenrep.
     * 
     * @return
     */
    static void ResourceAvailability(MusSettingsKeys::TAvailability &aCamera,
                                     MusSettingsKeys::TAvailability &aKeypad);

	/**
     * Returns resource usuability which is read from cenrep.
     *
     * @return
     */
    static void ResourceUsability(MusSettingsKeys::TUsability &aCamera,
                                  MusSettingsKeys::TUsability &aKeypad,
                                  MusSettingsKeys::TUsability &aVideoplayer);

	/**
     * @return Concatenation of encoder configuration information.
     *         Information has been divided to tokens separated by a semicolon.
     *         Keys will read from cenrep. Ownership is transferred.
     */
    static HBufC8* EncoderConfigInfoLC();

    

    /**
     * @param A concatenation of encoder configuration information
     */
    static void SetEncoderConfigInfoL( const TDesC8& aConfigInfo );

    /**
     *
     */
    static TUid EncodingDeviceL();

    /**
     * For internal usage only. Use direct getters/setters for each setting. 
     *
     * @param
     * @return
     */
    static TInt GetPropertyValueL( TUint32 aKey );

    /**
     * For internal usage only. Use direct getters/setters for each setting. 
     *
     * @param
     * @return
     */
    static HBufC16* GetPropertyValueLC( TUint32 aKey );
                                            
     /**
     * For internal usage only. Use direct getters/setters for each setting. 
     *
     * @param
     * @param
     */
    static void SetPropertyValueL( TUint32 aKey, const TDesC& aValue );
                                            
     /**
     * For internal usage only. Use direct getters/setters for each setting. 
     *
     * @param
     * @param
     */
    static void SetPropertyValueL( TUint32 aKey, TInt aValue );
   
    /**
     * If Encoding device set to KMusDisableAVC (0x0fffffff) returns ETrue 
     * else otherwise.  
     * Means AVC will be disabled when encoding device cenrep configuration 
     * set to 0x0fffffff.
     */
    static TBool IsAvcDisabled();
	
    };
    
#include "mussettings.inl"

#endif // MUSSETTINGS_H
