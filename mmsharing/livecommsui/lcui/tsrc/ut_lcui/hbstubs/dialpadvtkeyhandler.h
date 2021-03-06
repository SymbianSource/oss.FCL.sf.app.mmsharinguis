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

#ifndef DIALPADVTKEYHANDLER_H
#define DIALPADVTKEYHANDLER_H

#include <QObject>

class Dialpad;
class HbMainWindow;

/*!
    DialpadVtKeyHandler
    Class provides key handling for dialpad component.

    @code
    Dialpad *dialpad = new Dialpad();
    DialpadVtKeyHandler *keyhandler = new DialpadVtKeyHandler(dialpad, this);
    @endcode
    
*/
class DialpadVtKeyHandler : public QObject
{
    Q_OBJECT

public:
    explicit DialpadVtKeyHandler(Dialpad *dialPad, HbMainWindow& mainWindow, QObject *parent = 0);
    virtual ~DialpadVtKeyHandler();

private:
    HbMainWindow& mMainWindow;
    Dialpad* mDialPad; // not owned
    
public: // helper    	
	  void contentChanged( QString chrs );
	  
};

#endif // DIALPADVTKEYHANDLER_H
