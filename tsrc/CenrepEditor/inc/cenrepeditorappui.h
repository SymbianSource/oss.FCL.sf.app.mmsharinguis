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
* Description:  Declares UI class for application.
*
*/

#ifndef AIWTESTAPPUI_H
#define AIWTESTAPPUI_H

// INCLUDES
#include "cenrepeditormodel.h"

#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknappui.h>
#include <akntabgrp.h>
#include <aknnavide.h>
#include <aknviewappui.h>


// FORWARD DECLARATIONS
class CCenrepEditorContainer;
class CAiwServiceHandler;
class CMusManager;
class CMusAvaInterface;
class CPhoneResources;
class CMusApplication;

// CLASS DECLARATION

/**
* Application UI class.
* Provides support for the following features:
* - EIKON control architecture
*/
class CCenrepEditorAppUi : public CAknViewAppUi
    {
    public: // Constructors and destructor
        /**
        * By default Symbian 2nd phase constructor is private.
        */    
        void ConstructL();

        /**
        * Destructor.
        */      
        virtual ~CCenrepEditorAppUi();
        
    public: // New functions
        CPhoneResources* PhoneResouces();
        COtherResources* OtherResouces();
        CMusApplication* MusApplication();
        
    private: 
        /**
        * From MEikMenuObserver, dynamically initialises a menu pane.
        * @param aResourceId The menu's ID.
        * @param aMenuPane The in-memory representation of the menu pane.
        */
        void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

    private:
        /**
        * From CEikAppUi, takes care of command handling.
        * @param aCommand command to be handled
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CEikAppUi, handles key events.
        * @param aKeyEvent Event to handled.
        * @param aType Type of the key event. 
        * @return Response code (EKeyWasConsumed, EKeyWasNotConsumed). 
        */
        virtual TKeyResponse HandleKeyEventL(
            const TKeyEvent& aKeyEvent,TEventCode aType);
    
    private:                
        CAknNavigationControlContainer* iNaviPane;
        CAknTabGroup*                   iTabGroup;
        CAknNavigationDecorator*        iDecoratedTabGroup;   
    
    private:
        CPhoneResources* iPhoneResources;        
        COtherResources* iOtherResources;    
        CMusApplication* iMusApplication;           
    };

#endif

// End of File
