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


#ifndef MUSFACTROYSETTINGS_INL
#define MUSFACTORYSETTINGS_INL


#include "musfactorysettings.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "muslogger.h"

// -----------------------------------------------------------------------------
// Checks from the factory settings ( central repository )
// whether call type is supported.
// -----------------------------------------------------------------------------
//
inline TBool MusFactorySettings::IsSupportedL(
                                        const TPSCTsyCallType& aCallType)
    {    
    MusSettingsKeys::TAllowedCSOnly allowedCallType = 
                           MultimediaSharingSettings::AllowedCSOnlySettingL();
    if( allowedCallType == MusSettingsKeys::EAllowedCSOnly && 
                                           aCallType != EPSCTsyCallTypeCSVoice )
        {
        MUS_LOG( "mus: [MUSAO]  <-> MusFactorySettings::IsSupportedL Call : EFalse" )
        return EFalse;
        }
    MUS_LOG( "mus: [MUSAO]  <-> MusFactorySettings::IsSupportedL Call : ETrue" )
    return ETrue;
    }


// -----------------------------------------------------------------------------
// Checks from the factory settings ( central repository )
// whether network type is supported.
// -----------------------------------------------------------------------------
//
inline TBool MusFactorySettings::IsSupportedL( 
            const RMobilePhone::TMobilePhoneNetworkMode& aNetworkType)
    {
    MUS_LOG1( "mus: [MUSAO] -> MusFactorySettings::IsSupportedL Network :%d", aNetworkType )

   	TBool networkSupported = EFalse;
	TInt onlyIn3GNetwork = MusSettingsKeys::EAllowedAllBearers;	
	 // Check from Central Repository that do we Only support in 3G or All Networks. 
	 // if key is not found then behave like default ie AllowedAllBearers.    
    TRAP_IGNORE( onlyIn3GNetwork = MultimediaSharingSettings::Allowed3GOnlySettingL() ); 
	if( onlyIn3GNetwork == MusSettingsKeys::EAllowedAllBearers )
		{
		networkSupported = ETrue;	
		}		
	else
		{
		switch( aNetworkType )
			{
			case RMobilePhone::ENetworkModeWcdma:
			case RMobilePhone::ENetworkModeTdcdma:
			case RMobilePhone::ENetworkModeCdma95:
			case RMobilePhone::ENetworkModeCdma2000:
				{
				networkSupported = ETrue;
				break;
				}

			case RMobilePhone::ENetworkModeGsm:
				{
			     TInt edgeDtmSupport = KErrNone;
			     
			     // We Will throw an Activation Error for the Mandatory Settings if 
			     // they are not found. 

				edgeDtmSupport = MultimediaSharingSettings::EdgeDtmSupportSettingL();
                
        		if( edgeDtmSupport == MusSettingsKeys::EDtmModeAllowed )
        			{
        			networkSupported = ETrue;
        			}
        		else
        			{
        			networkSupported = EFalse;
        			}
        		break;	
				}
			}
		}
    MUS_LOG1( "mus: [MUSAO]  <- MusFactorySettings::IsSupportedL (%d)", networkSupported )
	
    return networkSupported;		
    }
    
    
#endif // MUSFACTROYSETTINGS_INL
