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
* Description:  Declares document for application.
*
*/

#ifndef AIWTESTDOCUMENT_H
#define AIWTESTDOCUMENT_H

// INCLUDES
#include <akndoc.h>
   
// CONSTANTS

// FORWARD DECLARATIONS
class CEikAppUi;

// CLASS DECLARATION

/**
*  CAiwTestDocument application class.
*/
class CAiwTestDocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CAiwTestDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CAiwTestDocument();

    public: // New functions
    public: // Functions from base classes
    protected:  // New functions
    protected:  // Functions from base classes

    private:
        /**
        * C++ default constructor.
        */
        CAiwTestDocument(CEikApplication& aApp);

        /**
        * By default Symbian 2nd phase constructor is private.
        */        
        void ConstructL();

    private:
        /**
        * From CEikDocument, create CAiwTestAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    };

#endif

// End of File

