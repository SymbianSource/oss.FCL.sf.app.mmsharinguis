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


#ifndef MUSHENGMCEOUTSESSION_H
#define MUSHENGMCEOUTSESSION_H

// USER
#include "musengmcesession.h"



// CLASS DECLARATION
/**
* Class for handling MCE out sessions. Direct inherit classes are
* CMusEngLiveSession and CMusEngClipSession.
*
* @lib musengine.lib
*/
class CMusEngMceOutSession : public CMusEngMceSession
    {
    
    public:

        /**
        * Destructor
        */
        ~CMusEngMceOutSession();


    protected:

        /**
        * Constructor
        */
        CMusEngMceOutSession( const TRect& aRect );

        /**
        * Second-phase constructor
        */
        void ConstructL();

    };
#endif //MUSHENGMCESESSION_H
