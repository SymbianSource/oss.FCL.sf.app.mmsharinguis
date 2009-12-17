/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/


#ifndef MUSSIPPROFILEUSER_H
#define MUSSIPPROFILEUSER_H

// SYSTEM INCLUDE
#include <e32def.h>

/**
* Callbacks for asking permission to roam between access points
* and notifying client about registering event
* @lib musengine.lib
*/
class MMusSipProfileUser                            
    {    
    public:

        /**
        * Checks whether roaming between access points is allowed.
        * @return ETrue is roaming is allowed, otherwise EFalse.
        */
        virtual TBool IsRoamingBetweenAPsAllowed() = 0;        
    };


#endif // MUSSIPPROFILEUSER_H
