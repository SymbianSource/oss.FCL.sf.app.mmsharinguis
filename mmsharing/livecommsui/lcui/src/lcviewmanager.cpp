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

//USER
#include "lcviewmanager.h"
#include "lcviewmanager_p.h"
#include "lclogger.h"



// -----------------------------------------------------------------------------
// LcViewManager::LcViewManager
// -----------------------------------------------------------------------------
//
LcViewManager::LcViewManager( const QString &engineName,
                              const QString &applicationDisplayName)
{
            
    d_ptr = new LcViewManagerPrivate( engineName, applicationDisplayName );
    
}


// -----------------------------------------------------------------------------
// LcViewManager::~LcViewManager
// -----------------------------------------------------------------------------
//
LcViewManager::~LcViewManager()
{
    delete d_ptr;
}


// -----------------------------------------------------------------------------
// LcViewManager::terminateSession
// -----------------------------------------------------------------------------
//
void LcViewManager::terminateSession()
{
    d_ptr->terminateSession();
}


// End of file

