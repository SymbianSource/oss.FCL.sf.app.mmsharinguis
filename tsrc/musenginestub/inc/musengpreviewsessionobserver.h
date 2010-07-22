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


#ifndef MUSHENGPREVIEWSESSIONOBSERVER_H
#define MUSHENGPREVIEWSESSIONOBSERVER_H

#include "musengsessionobserver.h"


// CLASS DECLARATION

/**
*  An interface to be implemented by users of Multimedia Sharing Engine if they wish to receive
*  events from session state changes.
*
*  This observer is set using CreateNewPreviewSessionL method
*
*  @lib ???.lib
*/
class MMusEngPreviewSessionObserver : public MMusEngSessionObserver
    {
    };

#endif

