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


#ifndef MUSSETTINGS_H
#define MUSSETTINGS_H

#include "mussettingskeys.h"

/**
 *  A namespace container for MultimediaSharing Settings API constants.
 *
 *  @since S60 v3.2
 */
class MultimediaSharingSettings
    {
public:

    static MusSettingsKeys::TOperatorVariant
                MultimediaSharingSettings::OperatorVariantSettingL();
                    
    static MusSettingsKeys::TAuditoryNotification AuditoryNotificationSettingL();                
				
		static MusSettingsKeys::TPopupNotification PopupNotificationSettingL();		
    static MusSettingsKeys::TAutoRecord AutoRecordSettingL();

    static TInt VideoLocationSettingL();

    static HBufC* VideoFolderSettingLC();

    static MusSettingsKeys::TVideoDirection VideoDirectionL();

    static HBufC* iVideoFolder;
    static MusSettingsKeys::TAutoRecord iAutoRecord;
    static TInt iVideoLocation;
    static MusSettingsKeys::TVideoDirection iVideoDirection;

    };


#endif // MUSSETTINGS_H
