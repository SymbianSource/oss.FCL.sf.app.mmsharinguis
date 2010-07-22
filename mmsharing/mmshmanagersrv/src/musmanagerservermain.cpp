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


#include "muslogger.h"
#include "musmanagerservermain.h"
#include "musmanagerserver.h"
#include "musmanageripccommon.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// MusManagerServerMain::ThreadFunction
// -----------------------------------------------------------------------------
//
TInt MusManagerServerMain::ThreadFunction ( TAny* /*aNone*/ )
	{
    TInt err = KErrNone;
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
	if (!cleanupStack) 
        {
        PanicServer( ECreateTrapCleanup );
        }
	RSemaphore semaphore;
	err = semaphore.OpenGlobal( KMusManagerServerSemaphoreName );
    if (err != KErrNone)
        {
        PanicServer( ESrvCreateServer );
        }
    TRAP( err, ThreadFunctionL( semaphore ) );
    if ( err != KErrNone )
        {
        semaphore.Signal();
        semaphore.Close();
        }

    delete cleanupStack;
    return err;
	}

// -----------------------------------------------------------------------------
// MusManagerServerMain::ThreadFunctionL
// -----------------------------------------------------------------------------
//
void MusManagerServerMain::ThreadFunctionL (RSemaphore& aSemaphore)
	{
	// Give a name to this thread
    User::LeaveIfError(User::RenameThread(KMusManagerServerName));

    // Construct server
    CMusManagerServer* server = CMusManagerServer::NewLC();

	// Server created ok
	aSemaphore.Signal();
	aSemaphore.Close();

	// Start handling requests
	CActiveScheduler::Start();

    // This will be executed after the active scheduler has been stopped:
    CleanupStack::PopAndDestroy(server); 
	}

// -----------------------------------------------------------------------------
// MusManagerServerMain::PanicServer
// -----------------------------------------------------------------------------
//
void MusManagerServerMain::PanicServer (TMusManagerServerPanic aPanic)
	{
    User::Panic( KMusManagerServer(), aPanic );
	}


// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Main function of the application executable.
// ---------------------------------------------------------------------------
//

GLDEF_C TInt E32Main()
    {
    return MusManagerServerMain::ThreadFunction( NULL );
    }
