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


#ifndef MUSMANAGERSERVER_H
#define MUSMANAGERSERVER_H

#include <e32base.h>
#include "musmanagerserverpanics.h"
#include "musmanagerservercoreobserver.h"
#include "musunittesting.h"

class CMusManagerServerCloseTimer;
class CMusManagerServerCore;

/**
 *  This static class contains the very essential starting functions 
 *  of MusManagerServer
 *
 *  @since S60 v3.2
 */
class CMusManagerServer 
    : public CPolicyServer,
    public MMusManagerServerCoreObserver
	{
    MUS_UNITTEST( UT_CMusManagerServerSession )
	
public : 
    
    // TODO: comments
    
    static CMusManagerServer* NewL();
    static CMusManagerServer* NewLC();
    ~CMusManagerServer();
    
   	void IncrementSessions();
	void DecrementSessions();
	
public: // from MMusManagerServerCoreObserver

    void StopServer();

    TUint SessionCount();
    
protected: // From CActive
    
	TInt RunError( TInt aError );

private: // From CServer2
    
    CSession2* NewSessionL( const TVersion &aVersion,
                           const RMessage2& aMessage ) const;

private: // Second phase constructors

    CMusManagerServer( TInt aPriority );
    void ConstructL();

private: // Data

    TUint iSessionCount;

    CMusManagerServerCloseTimer* iCloseTimer;

    CActiveScheduler*       iActiveScheduler;
    
    CMusManagerServerCore* iServerCore;
    
	};

#endif // MUSMANAGERSERVER_H
