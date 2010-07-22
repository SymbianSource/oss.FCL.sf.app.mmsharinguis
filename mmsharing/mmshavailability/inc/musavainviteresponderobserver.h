/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provide interface for the client requestin network availability.
*
*/


#ifndef __MUSAVAINVITERESPONDEROBSERVER_H__
#define __MUSAVAINVITERESPONDEROBSERVER_H__

#include "musavasettingsobserver.h"

#include <e32base.h>

class CMusAvaInviteResponder;

/**
 *  Responds to a non-desired invitation.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class MMusAvaInviteResponderObserver 
	{
public:

    virtual MMusAvaSettingsObserver::TApplicationState ApplicationState() = 0;
    
    virtual void InvitationAnsweredLD() = 0;
    };


#endif // __MUSAVAINVITERESPONDER_H__