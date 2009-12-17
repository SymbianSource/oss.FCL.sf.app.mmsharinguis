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


#ifndef MUSHENGOBSERVER_H
#define MUSHENGOBSERVER_H

// INCLUDES
//#include <e32std.h>

// CLASS DECLARATION

/**
*  An interface to be implemented by users of Multimedia Sharing Engine if they wish to receive
*  events from session state changes.
*
*  This observer is set using CMSSession::SetSessionObserver function.
*
*  @lib ???.lib
*/
class MMusEngObserver
    {


    protected:
        /**
        * Callback function to indicate that engine state was changed
        * @param aState, the new session state
        */
        virtual void EngineStateChangedL(/*const TMSEngState& aState*/) = 0;


    };

#endif