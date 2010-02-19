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
* Description:  The application's application class.
*
*/


#ifndef MUSUIAPP_H
#define MUSUIAPP_H


#include <eikapp.h>


/**
 * Application class of the MUS application.
 *
 * @lib musui.exe
 */
class CMusUiApp : public CEikApplication
    {

private:

    /**
    * From CApaApplication, creates CMusUiDocument document object.
    * @return A pointer to the created document object.
    */
    CApaDocument* CreateDocumentL();

    /**
    * From CApaApplication, returns application's UID.
    * @return The value of KUidMusUi
    */
    TUid AppDllUid() const;

    };

#endif // MUSUIAPP_H

// end of file