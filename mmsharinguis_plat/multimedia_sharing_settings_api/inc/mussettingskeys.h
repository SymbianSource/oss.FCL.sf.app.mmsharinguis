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


#ifndef MUSSETTINGSKEYS_H
#define MUSSETTINGSKEYS_H

#include <e32cmn.h>

/**
 *  A namespace container for MultimediaSharing Settings API constants.
 */
namespace MusSettingsKeys
    {
    /** Repository uid */
    const TUid KRepositoryUid = { 0x1028238B };

    /* Defines when MultimediaSharing is activated (always/in home networks/never).
       Modified from the Settings UI of MultimediaSharing. */
    const TInt KActivation( 0x01 );
    /**  Contains the possible values of KMusActivation key. */
    enum TActivation
        {
        EAlwaysActive,
        EActiveInHomeNetworks,
        ENever       
        };	
    
    /** Defines that should we act in a operator specific way. */
    const TInt KOperatorVariant( 0x2 );
    /** Contains the possible values of KOperatorVariant key. */
    enum TOperatorVariant
        {
        EStandard,
        EOperatorSpecific
        };
        
    /** Defines that should a beep be played when MS becomes available. 
    	Modified from the Settings UI of MultimediaSharing. */
    const TInt KAuditoryNotification( 0x03 );	
    /** Contains the possible values of KMusAvailabilityAuditoryNotification key. */
    enum TAuditoryNotification
        {
        EAuditoryNotificationOn,
        EAuditoryNotificationOff
        };

    /** Defines that should a popup dialog be shown when MS becomes available. 
     *   @deprecated
     */
    const TInt KPopupNotification( 0x04 );	
    /** Contains the possible values of KMusAvailabilityPopupNotification key.
     *  @deprecated
     */
    enum TPopupNotification
        {
        EPopupNotificationOn,
        EPopupNotificationOff
        };
    
    /** @deprecated */
    const TInt KPopupNotificationType( 0x05 );
    /** @deprecated */
    enum TPopupNotificationType
	    {
    	ENotificationTypeQueryUser,
    	ENotificationTypePopup
    	};
    
    /** Defines EDGE/DTM support. */
    const TInt KEdgeDtmSupport( 0x06 );	
    /** Contains the possible values of KMusEdgeDtmSupport key. */
    enum TEdgeDtmSupport
        {
        EDtmModeAllowed,
        EDtmModeNotAllowed
        };
    
    /** Defines that is short (IETF) or long (3GPP) way of initiating sessions used. */
    const TInt KForceInternetSignaling( 0x07 );	
    /** Contains the possible values of KMusSessionSetupMethod key. */
    enum TForceInternetSignaling
        {
        EForceInternetSignaling,
        EFollowProfileConfiguration
        };
    
    /**  
    	Modified from the Settings UI of MultimediaSharing. */
    const TInt KAutoRecord( 0x08 );	
    /** Contains the possible values of KAutoRecord key. */
    enum TAutoRecord
        {
        EAutoRecordOn,
        EAutoRecordOff
        };
    
    /** 
    	Modified from the Settings UI of MultimediaSharing. */
    const TInt KVideoLocation( 0x09 );	

    /** Defines the id of the SIP Profile to be used by MultimediaSharing. 
    	Modified from the Settings UI of MultimediaSharing. */
    /** Contains an integer value. */
    const TInt KSipProfileId( 0x0A );
    
    /** Defines the default orientation of the UI. */
    const TInt KUiOrientation( 0x0B );	
    /** Contains the possible values of KMusUiOrientation key. */
    enum TUiOrientation
        {
        EPortrait,
        ELandscape
        };
        
    /** Defines the type of SIP options. 
        Defines that how are SIP OPTIONS handled (3GPP / operator specific way?) */
    const TInt KCapabilityQuery( 0x0C );
    /** */
    enum TCapabilityQuery
        {
        ESequential,
        EParallel,
        ENoOptions
        };

    /** Defines the intial value for product mode variation key. 
      * The values should be constructed like below
      *
      * 7 = Most Significant Bit(MSB) and 0 = Least Significant Bit (LSB)
      * 76543210 <-- bits 7 and 6 for the video player, 
      *                   5, 4 and 3 for the keypad,
      *                   2, 1 and 0 for the camera 
      *
      * Bit 1 & 0 : Camera Usability
      *             00 - > Always 
      *             01 - > Only portrait
      *             10 - > Only lanscape
      * Bit 2     : Camera Availability
      *             0  - > Static ( For example no camera lens cover) 
      *             1  - > Dynamic ( For example camera with lens cover)      
      * Bit 3 & 4 : Keypad Usuability
      *             00 - > Always 
      *             01 - > Only portrait
      *             10 - > Only lanscape
      * Bit 5     : Keypad Availability
      *             0  - > Static ( For example keypad Not covered) 
      *             1  - > Dynamic ( For example keypad covered)
      * Bit 6 & 7 : Videoplayer Usability
      *             00 - > Always 
      *             01 - > Only portrait
      *             10 - > Only lanscape
      * Note :Videoplayer availability will be always.There is no static
      *        and dynamic availability in videoplayer.  
      *
      * Example : Bits set like 
      *           01110001 = 0x71 ( In hex format ) = 113 ( In integer format )
      *           It will be intrepreted and used like
      *             a)Videoplayer is usable only in portrait and
      *             b)Keypad availability is dynamic and if it is available
      *               the can be used only in landscape mode.
      *             c)Camera availability is static and can be used only in
      *               portrait mode.
      */
    const TInt KProductModeVariation( 0x0D );
    
    enum TAvailability
    	{
    	EAvailabilityStatic,
    	EAvailabilityDynamic
    	};

	enum TUsability
    	{
    	EUsabilityAlways,
    	EUsabilityPortrait,
    	EUsabilityLandscape
    	};

    /** Concatenation of encoder configuration information. Information has been
      * divided to tokens separated by a semicolon. Modified from the MusEngine.
      * Contains a 8-bit descriptor value. 
      */
    const TInt KEncoderConfigurationInfo( 0x0E );

    /**
      * Contains UID of the encoding device meant to be used. Read by MuS Engine
      */
    const TInt KEncodingDevice( 0x0F );

    /**
      * Defines whether multimediasharing is restricted to CS call type only.
      * Default value will be 0 ( EFalse ) means it is allowed in all call types
      * for example VOIP and Skype calls.
      */
    const TInt KAllowOnlyWithActiveCSCall( 0x10 );
    /** Contains the possible values of KAllowedCSOnly key. */
    enum TAllowedCSOnly
        {
        EAllowedAllCalls,
        EAllowedCSOnly        
        };

    /**
      * Defines whether multimediasharing is restricted to 3G bearer only.
      * Default value will be 0 ( EFalse ) means it is allowed in all bearer types
      * for example 3G,WLAN etc.
      */
    const TInt KAllowOnlyIn3GNetwork( 0x11 );
    /** Contains the possible values of KAllowed3GOnly key. */
    enum TAllowed3GOnly
        {
        EAllowedAllBearers,
        EAllowed3GOnly        
        };

    /**
     * Defines which and how cameras are used. The default value
     * is EUseCameraSwapping. To disable camera swapping and roll back
     * old behaviuor, value must be set to EUseOnlyMainCamera.
     * to the menu or to the toolbar.
     */
    const TInt KCameraUsage( 0x12 );

    /** Contains the possible values of KCameraUsage key. */
    enum TUseCamera
        {
        EUseOnlyMainCamera,
        EUseOnlySecondaryCamera,
        EUseCameraSwapping
        };

    /**
     * Defines whether multimediasharing is supporting 2-way video. The
     * default value is EOneWayVideo. 
     */
    const TInt KVideoDirection( 0x13 );
    
    /** Contains the possible values of KVideoDirection key. */
    enum TVideoDirection
        {
        EOneWayVideo,
        ETwoWayVideo
        };
    
    /**
     * Defines whether b=AS and b=TIAS SDP attributes are used. Zero value 
     * means that usage of TIAS is disabled, only AS will be used (hard coded
     * value). Positive value means AS in KBits per second. TIAS value will 
     * be set to AS multiplied by 1000.
     */
    const TInt KVideoBandwidth( 0x14 );
    
    /**
     * Defines whether multimediasharing is supporting fast startup mode
     */
    const TInt KFastStartupMode( 0x15 );
    
    /** Contains the possible values of KFastStartupMode key. */
    enum TFastMode
        {
        EFastModeOn,
        EFastModeOff,
        EFastModeDisabled
        };    
    }

    
#endif // MUSSETTINGSKEYS_H
  
