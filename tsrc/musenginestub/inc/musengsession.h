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


#ifndef MUSENGSESSION_H
#define MUSENGSESSION_H


//SYSTEM INCLUDES
#include <e32base.h>



// CLASS DECLARATION

/**
* Base class for Multimedia Sharing Engine sessions, inherited by
* CMusEngMceSession.
*
* CMusEngSession encapsulates all common activities example using
* resources.
*
* @lib musengine.lib
*/
class CMusEngSession : public CBase
    {
   
    public:

        /**
        * Desctructor, finalize session
        *
        */
        ~CMusEngSession();



    protected:

        /**
        * Constructor, private cannot be instantiate
        *
        * @param aRect reserved drawing area for this session
        */
        CMusEngSession();


    protected:

        /**
        * second-phase constructor, called by those session specific 
        * constructors
        */
        void ConstructL();

    };

#endif //MUSENGSESSION_H
