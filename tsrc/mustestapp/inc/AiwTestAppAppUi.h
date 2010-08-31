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
#include "musavaobserver.h"
#include "muscallmonitorobserver.h"

#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknappui.h>


// FORWARD DECLARATIONS
class CAiwTestContainer;
class CAiwServiceHandler;
class CMusManager;
class CMusAvaInterface;
class CMusCallMonitor;

// CLASS DECLARATION

/**
* Application UI class.
* Provides support for the following features:
* - EIKON control architecture
*/
class CAiwTestAppUi : public CAknAppUi, public MMusAvaObserver,
                      public MMusCallMonitorObserver
    {
    public: // Constructors and destructor
        /**
        * By default Symbian 2nd phase constructor is private.
        */    
        void ConstructL();

        /**
        * Destructor.
        */      
        virtual ~CAiwTestAppUi();
        
    public: // New functions
    public: // Functions from base classes

     void AvailabilityState( TAvailabilityState aState, 
                                              TAvailabilityStatus aStatus,
                                              TAvailabilityName aName );
    

    void StopComplete();


    void CallConnectedL( const TDesC& /*aTelNumber*/, TBool /*aIsSipUri*/ ) 
        {
        }
    
    void CallHold( const TDesC& /*aTelNumber*/, TBool /*aIsSipUri*/ )
        {
        }
    
    void ConferenceCallL()
        {
        }
    
    void NoActiveCallL()
        {
        }
    

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
        CAiwTestContainer* iAppContainer;  
        CAiwServiceHandler* iServiceHandler;        
        
        CMusManager* iManager;    
        CMusAvaInterface* iPlugin;
        TAvailabilityStatus iMinimumStatus;
        TAvailabilityStatus iMandatoryStatus;
        TAvailabilityStatus iExtensionStatus;
        
        CMusCallMonitor* iCallMonitor;
    };

#endif

// End of File
