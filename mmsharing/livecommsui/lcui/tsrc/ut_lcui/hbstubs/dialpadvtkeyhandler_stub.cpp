/*!
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Dialpad key handler.
*
*/

#include <dialpadvtkeyhandler.h>
#include <hbmainwindow.h>
#include <dialpad.h>

#define EMERGENCY_NUMBER "112"

DialpadVtKeyHandler::DialpadVtKeyHandler(Dialpad *dialPad, HbMainWindow& mainWindow, QObject * parent)
	: mMainWindow( mainWindow ),
      mDialPad( dialPad )
{
	 Q_UNUSED(parent);
}

DialpadVtKeyHandler::~DialpadVtKeyHandler()
{
	
}

void DialpadVtKeyHandler::contentChanged( QString chrs )
{
	if( mDialPad ){
		if( chrs == EMERGENCY_NUMBER ){
			mDialPad->setCallButtonEnabled(true);
		}else{
			mDialPad->setCallButtonEnabled(false);
		}
	}		
}
