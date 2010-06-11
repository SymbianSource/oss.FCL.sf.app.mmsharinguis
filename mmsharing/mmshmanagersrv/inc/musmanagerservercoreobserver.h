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


#ifndef MUSMANAGERSERVERCOREOBSERVER_H
#define MUSMANAGERSERVERCOREOBSERVER_H

#include <e32base.h>

/**
 *  This abstract class contains the very essential stopping functions 
 *  of MusManagerServer
 *
 *  @since S60 v3.2
 */
class MMusManagerServerCoreObserver
	{
public: // server stopper function
    
    virtual void StopServer() = 0;

    virtual TUint SessionCount() = 0;

	};

#endif // MUSMANAGERSERVERCOREOBSERVER_H
