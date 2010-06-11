/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSMANAGERSERVERSTARTER_H
#define MUSMANAGERSERVERSTARTER_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
* This class starts client-server server part of MultimediaSharing.
* 
* @lib 
*/

class MusManagerServerStarter
    {
public: // New functions

    static TInt Start();

    static TBool Started();

private:

    static TInt CreateServerProcess(RSemaphore& aSemaphore);
    };

#endif // MUSMANAGERSERVERSTARTER_H

// End of File
