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



#ifndef MUSUIACTIVETIMEROBSERVER_H
#define MUSUIACTIVETIMEROBSERVER_H

class CMusUiActiveTimer;

class MMusUiActiveTimerObserver
	{
	
public:

	virtual void TimerComplete( CMusUiActiveTimer* aTimer ) = 0;
	
	};

#endif // MUSUIACTIVETIMEROBSERVER_H
            
// end of file
