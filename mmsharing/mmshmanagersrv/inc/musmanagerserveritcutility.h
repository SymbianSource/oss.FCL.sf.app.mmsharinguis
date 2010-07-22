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


#ifndef MUSMANAGERSERVERITCUTILITY_H
#define MUSMANAGERSERVERITCUTILITY_H

#include <e32base.h>

#include "musmanagercommon.h"
#include "musmanageripccommon.h"
#include "musmanagerserversessionobserver.h"

class RMessage2;
class TRequestStatus;

/**
 *  This static class contains the very essential starting functions 
 *  of MusManagerServer
 *
 *  @since S60 v3.2
 */
class MusManagerServerItcUtility 
	{
public: // new functions

    static void WriteAvailabilityL( 
        const RMessage2& aMessage, 
        TInt aAvailability );
        
    static void WriteAvailabilityReportL( 
        const RMessage2& aMessage, 
        TInt aAvailability );
    
    static TUid ReadChannelIdL( const RMessage2& aMessage );
    
    static TInt ReadUseCaseL( const RMessage2& aMessage );
    
    static TInt ReadCommandL( const RMessage2& aMessage );
    
	static TRequestStatus ReadRequestStatusL( const RMessage2& aMessage );
	
private:

    static void WriteL (
        TMusManagerIpcArguments aIndex,
        const RMessage2& aMessage,
        const TDesC8& aDes );
	};

#endif // MUSMANAGERSERVERITCUTILITY_H
