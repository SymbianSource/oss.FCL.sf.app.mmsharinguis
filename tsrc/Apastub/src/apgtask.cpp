/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "apgtask.h"


// =============================================================================
// Stub variables for TApaTask
// =============================================================================

TBool TApaTask::iApplicationExist = EFalse;

TInt TApaTask::iApaTaskCalledFunction = 0;


// =============================================================================
// Stub functions for TApaTask
// =============================================================================

TApaTask::TApaTask(RWsSession& aWsSession)
    : iWsSession( aWsSession )
    {

    }

TBool TApaTask::Exists()
    {
    iApaTaskCalledFunction = EExists;
    return iApplicationExist;
    }

 void TApaTask::BringToForeground()
    {
    iApaTaskCalledFunction = EBringToForeground;
    }
 void TApaTask::SendToBackground()
    {
    iApaTaskCalledFunction = ESendToBackground;
    }
 void TApaTask::EndTask()
    {
    iApaTaskCalledFunction = EEndTask;
    }
void TApaTask::KillTask()
    {
    iApaTaskCalledFunction = EKillTask;
    }





// =============================================================================
// Stub functions for TApaTaskList
// =============================================================================

TInt TApaTaskList::iCalledFunction = 0;


TApaTaskList::TApaTaskList(RWsSession& aWsSession)
    : iWsSession (aWsSession)
    {
    }
    
    
TApaTask TApaTaskList::FindApp(const TDesC& /*aAppName*/ )
    {
    return TApaTask( iWsSession );
    }

TApaTask TApaTaskList::FindDoc(const TDesC& /*aDocName*/ )
    {
    return TApaTask( iWsSession );
    }

TApaTask TApaTaskList::FindByPos(TInt /*aPos*/ )
    {
    return TApaTask( iWsSession );
    }

TApaTask TApaTaskList::FindApp(TUid /*aAppUid*/ )
    {
    iCalledFunction = EFindApp;    
    return TApaTask( iWsSession );
    }
