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


#ifndef MUSHENGRECEIVESESSIONOBSERVER_H
#define MUSHENGRECEIVESESSIONOBSERVER_H

// INCLUDES

#include "musengsessionobserver.h"

// CLASS DECLARATION

/**
*  
*
*  @lib ???.lib
*/
class MMusEngReceiveSessionObserver : public MMusEngSessionObserver
    {

    public:
    
        /**
        * 
        *
        * @since S60 v3.2
        */
        virtual void IncomingSession( const TDesC& aOriginator ) = 0;


        /**
        * Current receiving session is buffering received RTP.
        *
        * @since S60 v3.2
        */
        virtual void StreamBuffering() = 0;
    };

#endif