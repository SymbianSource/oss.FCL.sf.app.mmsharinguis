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


// INCLUDES
#include "musmanagerserverstarter.h"
#include "musmanageripccommon.h"
#include "muslogger.h"

const TInt KMusServerClosingWaitTimeoutInMicrosecs = 10000000; // 10 secs

// ----------------------------------------------------------------------------
// MusManagerServerStarter::Start
// ----------------------------------------------------------------------------
//
TInt MusManagerServerStarter::Start ()
    {
    MUS_LOG( "mus: [MUSCLI]  -> MusManagerServerStarter::Start()" );
    TInt err( KErrNone );

    if( !Started() )
        {
        RSemaphore semaphore;
        err = semaphore.CreateGlobal( KMusManagerServerSemaphoreName ,0 );
        if ( err != KErrNone )
            {
            MUS_LOG1( "mus: [MUSCLI]     <- MusManagerServerStarter::Start( %d )",
                      err );
            return err;
            }

        err = CreateServerProcess( semaphore );
        semaphore.Close ();
        }
    MUS_LOG( "mus: [MUSCLI]  <- MusManagerServerStarter::Start()" );
    return err;
    }

// ----------------------------------------------------------------------------
// MusManagerServerStarter::Started
// ----------------------------------------------------------------------------
//
TBool MusManagerServerStarter::Started()
    {
    MUS_LOG( "mus: [MUSCLI]  -> MusManagerServerStarter::Start()" );
    TFindServer findServer( KMusManagerServerName );
    TFullName name;
    if (findServer.Next(name) == KErrNone)
        {
        MUS_LOG( "mus: [MUSCLI]  <- MusManagerServerStarter::Start()" );
    
        // Server may be running but already doing destruction, report
        // it as not running in such case.
        RSemaphore closingSemaphore;
        TBool closingCurrently( 
            closingSemaphore.OpenGlobal( KMusManagerServerClosingSemaphoreName ) == KErrNone );
        closingSemaphore.Close();
        return !closingCurrently;
        }
    MUS_LOG( "mus: [MUSCLI]  <- MusManagerServerStarter::Start()" );
    return EFalse;
    }

// ----------------------------------------------------------------------------
// MusManagerServerStarter::CreateServerProcess
// ----------------------------------------------------------------------------
//
TInt MusManagerServerStarter::CreateServerProcess( RSemaphore& aSemaphore )
    {
    TInt err = KErrNone;
    
    RSemaphore closingSemaphore;
    if ( closingSemaphore.OpenGlobal( KMusManagerServerClosingSemaphoreName ) == KErrNone )
        {
        MUS_LOG( "mus: [MUSCLI]     CreateServerProcess, wait for server closing" );
        // Don't wait forever if server is somehow horribly jammed
        
        TInt waitErr = closingSemaphore.Wait( KMusServerClosingWaitTimeoutInMicrosecs );
        MUS_LOG1( "mus: [MUSCLI]    CreateServerProcess, waitErr( %d )",
                  waitErr );
        closingSemaphore.Close();
        if ( waitErr != KErrNone )
            {
            return waitErr;
            }
        }
    
    const TUidType serverUid( KNullUid, KNullUid, KServerUid3 );
    RProcess server;
    err = server.Create( KMusManagerServerName, KNullDesC() ,serverUid );
    if ( err )
        {
        return err;
        }
    server.Resume();
    aSemaphore.Wait();
    TInt exitReason = server.ExitReason();
    server.Close();
    return exitReason;
    }

//  End of File
