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


#ifndef MUSHENGCLIPSESSIONOBSERVER_H
#define MUSHENGCLIPSESSIONOBSERVER_H

// INCLUDES
#include <e32std.h>
#include "musengoutsessionobserver.h"

// CLASS DECLARATION

/**
*  An interface to be implemented by users of Multimedia Sharing Engine if they wish to receive
*  events from session state changes.
*
*  This observer is set using CMSSession::SetSessionObserver function.
*
*  @lib ???.lib
*/
class MMusEngClipSessionObserver : public MMusEngOutSessionObserver
    {
    public: 
    
        virtual void EndOfClip() = 0;        
    };

#endif