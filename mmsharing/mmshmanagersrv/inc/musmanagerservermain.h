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


#ifndef MUSMANAGERSERVERMAIN_H
#define MUSMANAGERSERVERMAIN_H


#include <e32base.h>

#include "musmanagerserverpanics.h"

/**
 *  This static class contains the very essential starting functions 
 *  of MusManagerServer
 *
 *  @since S60 v3.2
 */
class MusManagerServerMain
    {
public : 
    
    static TInt ThreadFunction (TAny* aNone);

private:

    MusManagerServerMain ();

    static void PanicServer (TMusManagerServerPanic aReason);
    static void ThreadFunctionL (RSemaphore& aSemaphore);
    };


#endif // ? MUSMANAGERSERVERMAIN_H
