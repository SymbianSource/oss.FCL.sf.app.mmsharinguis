/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares main application class.
*
*/

#ifndef AIWTESTAPP_H
#define AIWTESTAPP_H

// INCLUDES
#include <aknapp.h>

// CONSTANTS
// UID of the application
const TUid KUidAiwTestApp = { 0x1028239B };

// CLASS DECLARATION

/**
* CAiwTestApp application class.
* Provides factory to create concrete document object.
* 
*/
class CAiwTestApp : public CAknApplication
    {
    public: // Functions from base classes

    private:
        /**
        * From CApaApplication, creates CAiwTestAppDocument document object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
        
        /**
        * From CApaApplication, returns application's UID (KUidAiwTestApp).
        * @return The value of KUidAiwTestApp.
        */
        TUid AppDllUid() const;
    };

#endif

// End of File

