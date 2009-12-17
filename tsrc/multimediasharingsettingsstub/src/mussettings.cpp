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


#include "mussettings.h"
#include "mussettingskeys.h"



HBufC* MultimediaSharingSettings::iVideoFolder = NULL;

MusSettingsKeys::TAutoRecord MultimediaSharingSettings::iAutoRecord = MusSettingsKeys::TAutoRecord( 0 );
TInt MultimediaSharingSettings::iVideoLocation = 2; // phone memory C:


HBufC* MultimediaSharingSettings::VideoFolderSettingLC()
    {
    HBufC* res = iVideoFolder->AllocLC();
    return res;

    }


MusSettingsKeys::TAutoRecord MultimediaSharingSettings::AutoRecordSettingL()
    {
    return iAutoRecord;
    }


TInt MultimediaSharingSettings::VideoLocationSettingL()
    {
    return iVideoLocation;
    }
        
