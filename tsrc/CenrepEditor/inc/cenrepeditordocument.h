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
#include <e32cmn.h>
#include <BADESCA.H>
#include <BADESCA.H>
#include  "cenrepeditormodel.h"
   
// CONSTANTS

// FORWARD DECLARATIONS
class CEikAppUi;
class CPhoneResources;

// CLASS DECLARATION

/**
*  CCenrepEditorDocument application class.
*/
class CCenrepEditorDocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CCenrepEditorDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CCenrepEditorDocument();

    public: // New functions                        
        CPhoneResources& CenrepPhoneResouces();
    public: // Functions from base classes
    protected:  // New functions
    protected:  // Functions from base classes

    private:
        /**
        * C++ default constructor.
        */
        CCenrepEditorDocument(CEikApplication& aApp);

        /**
        * By default Symbian 2nd phase constructor is private.
        */        
        void ConstructL();

    private:
        /**
        * From CEikDocument, create CCenrepEditorAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
         
    };

#endif

// End of File

