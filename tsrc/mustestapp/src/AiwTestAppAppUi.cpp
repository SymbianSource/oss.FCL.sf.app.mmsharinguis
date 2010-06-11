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
* Description:  
*
*/


// INCLUDE FILES
#include "aiwtestappappui.h"
#include "aiwtestappcontainer.h" 
#include "musmanager.h"
#include <mustestapp.rsg>
#include "aiwtestapp.hrh"
#include "musresourceproperties.h"
#include "musavainterface.h"
#include "muscallmonitor.h"

#include <e32property.h>
#include <flogger.h>
#include <e32math.h>

#include <avkon.hrh>
#include <aknutils.h>

#include <aknmessagequerydialog.h>
#include <aknquerydialog.h>

#include <aiwservicehandler.h>

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CAiwTestAppUi::ConstructL()
// 
// ----------------------------------------------------------
//
void CAiwTestAppUi::ConstructL()
    {
    BaseConstructL();

    iPlugin = NULL;

    // Creates a container object and adds it to the application's control stack.
    iAppContainer = new (ELeave) CAiwTestContainer;
    iAppContainer->SetMopParent( this );
    iAppContainer->ConstructL( ClientRect() );
    AddToStackL( iAppContainer );

    // Create service handler instance.    
    iServiceHandler = CAiwServiceHandler::NewL();
 
    // Attach menu service interests to AIW framework.
    iServiceHandler->AttachMenuL(R_AIWTEST_MENU, R_AIWTESTAPP_INTEREST);
    //iServiceHandler->AttachMenuL(R_UMASETTINGS_MAIN_MENU, R_UMASETTINGSUI_INTEREST);    
    
    // Attach base service interests to AIW framework.
    iServiceHandler->AttachL(R_AIWTESTAPP_BASEINTEREST);

    iManager = CMusManager::NewL();

          RProperty::Define( NMusResourceApi::KCategoryUid, 
                                         NMusResourceApi::KCameraAvailability, 
                                         RProperty::EInt); 

            // 2. keypad property
           RProperty::Define( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KKeypadAvailability, 
                               RProperty::EInt );

            // 3. video player property
           RProperty::Define( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KVideoPlayerAvailability, 
                               RProperty::EInt 
                               );

            RProperty::Set( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KCameraAvailability, 
                               ( TInt ) NMusResourceApi::EAvailable );

            // 2. keypad property
           RProperty::Set( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KKeypadAvailability, 
                               ( TInt ) NMusResourceApi::EAvailable );

            // 3. video player property
           RProperty::Set( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KVideoPlayerAvailability, 
                               ( TInt ) NMusResourceApi::EAvailable );

    // Next line is an example of how to use base service commands.
    //iServiceHandler->ExecuteServiceCmdL(4, iServiceHandler->InParamListL(),
    //    iServiceHandler->OutParamListL());
    }

// ----------------------------------------------------
// CAiwTestAppUi::~CAiwTestAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------
//
CAiwTestAppUi::~CAiwTestAppUi()
    {
    delete iServiceHandler; 
    
    if (iAppContainer)
        {
        RemoveFromStack( iAppContainer );
        delete iAppContainer;
        }
    
    if ( iPlugin )
        {
        iPlugin->StopL();
        delete iPlugin;
        }
    
   iManager->StopApplicationL();
    
    delete iManager;
    }

// ------------------------------------------------------------------------------
// CAiwTestAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
// This function is called by the EIKON framework just before it displays
// a menu pane. Its default implementation is empty, and by overriding it,
// the application can set the state of menu items dynamically according
// to the state of application data.
// ------------------------------------------------------------------------------
//
void CAiwTestAppUi::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
    if (iServiceHandler->HandleSubmenuL(*aMenuPane))
        {
        return;
        }
          
    if (iServiceHandler->IsAiwMenu(aResourceId))
        {
        TFileName filename(_L("image.jpg")); // This is just an axample of how to fill param list.
        TAiwVariant variant(filename);
        TAiwGenericParam param(EGenericParamFile, variant);
        CAiwGenericParamList& inList = iServiceHandler->InParamListL();
        inList.AppendL(param);  

        iServiceHandler->InitializeMenuPaneL(*aMenuPane, aResourceId, 5000, 
            iServiceHandler->InParamListL());        
        }    
    }

// ----------------------------------------------------
// CAiwTestAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent, 
//     TEventCode aType)
// Takes care of key event handling.
// ----------------------------------------------------
//
TKeyResponse CAiwTestAppUi::HandleKeyEventL(const TKeyEvent& /*aKeyEvent*/,
    TEventCode /*aType*/)
    {
    return EKeyWasNotConsumed;
    }

// ----------------------------------------------------
// CAiwTestAppUi::HandleCommandL(TInt aCommand)
// Takes care of command handling.
// ----------------------------------------------------
//
void CAiwTestAppUi::HandleCommandL(TInt aCommand)
    {
    switch ( aCommand )
        {
        case EAknSoftkeyBack:  // fallthrough
        case EEikCmdExit:
            {
            RDebug::Print(_L("This is a debug message - Exit"));
            Exit();
            break;
            }
        case EAiwTestAppCmdBATTest:
            {
            break;
            }
        case EAiwTestAppCmdCoreInfo:
            {
            break;
            }
        case EAiwTestAppCmdEngineInfo:
            {
            break;
            }
        case 1000:     // 'Testi' menu item.
            {

            iManager->ExamineAvailabilityL();
            }
            break;
           
            
           
        case 1001:
                {
       
                iManager->StartApplicationL( MultimediaSharing::EMusLiveVideo );
              
                }
            break;
        case 1002:
                {
                iPlugin = CMusAvaInterface::NewL();
                iPlugin->SetObserver( *this );
                
                iPlugin->StartL();
                //iCallMonitor = CMusCallMonitor::NewL( *this );
                //iCallMonitor->StartMonitoringL();
                
                
                
                }
            break;
        default:
            {

            
            CAiwGenericParamList& in = iServiceHandler->InParamListL();
            // in.AppendL(param);

            // Executes menu service command.
            iServiceHandler->ExecuteMenuCmdL(aCommand, in, iServiceHandler->OutParamListL());

            //CleanupStack::PopAndDestroy(2);
            }
            break;      
        }
    }

     void CAiwTestAppUi::AvailabilityState( TAvailabilityState aState, 
                                              TAvailabilityStatus aStatus,
                                              TAvailabilityName aName )
        {
        if( aState == EMusAvaMinimum )
            iMinimumStatus = aStatus;
        else if( aState == EMusAvaMandatory )
            iMandatoryStatus = aStatus;
        else if( aState == EMusAvaExtension )
            iExtensionStatus = aStatus;
        }
    

    void CAiwTestAppUi::StopComplete()
        {
        
        }


// End of File  
