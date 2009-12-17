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
* Description:  Document class of the application.
*
*/


#ifndef MUSUIDOCUMENT_H
#define MUSUIDOCUMENT_H

#include <AknDoc.h> // Avkon document base class

class CEikAppUi;

/**
 * Document class of MUS application.
 *
 * @lib musui.exe
 */
class CMusUiDocument : public CAknDocument
    {

public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aApp Handle to Eikon's application class
    * @return a new instance of CMusUiDocument
    */
    static CMusUiDocument* NewL( CEikApplication& aApp );

    /**
    * Destructor.
    */
    virtual ~CMusUiDocument();

private:

    /**
    * Default constructor
    * @param aApp Handle to Eikon's application class
    */
    CMusUiDocument( CEikApplication& aApp );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // Functions from base classes

    /**
    * From CEikDocument, create CMusUiAppUi "App UI" object.
    * @return a new instance of Mus Applications AppUi class
    */
    CEikAppUi* CreateAppUiL();

private: // data


    };

#endif // CMUSUIDOCUMENT_H

// end of file
