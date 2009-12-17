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
* Description:  Mus Applications Event Observer Interface
*
*/



#ifndef MUSUIRECEIVEOBSERVER_H
#define MUSUIRECEIVEOBSERVER_H

#include "musuidefinitions.h"

#include <aknappui.h>

class MMusUiReceiveObserver
	{

public:

    virtual void ShowInvitationQueryL( const TDesC& aQuery ) = 0;
    
    virtual void DismissInvitationQuery( TBool aDelete ) = 0;
	
    virtual void SetFullScreenL( TBool aFullScreen ) = 0;
	
	};

#endif // MUSUILIVESHARINGOBSERVER_H
            
// end of file
