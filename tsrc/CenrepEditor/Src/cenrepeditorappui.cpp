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
#include "cenrepeditorappui.h"
#include "cenrepeditorview1.h"
#include "cenrepeditorview2.h"
#include "cenrepeditorview3.h"
#include <cenrepeditor.rsg>
#include "cenrepeditor.hrh"
#include <flogger.h>
#include <e32math.h>

#include <avkon.hrh>
#include <aknutils.h>

#include <aknmessagequerydialog.h>
#include <aknquerydialog.h>
#include <aknnotewrappers.h>
#include "musmanagercommon.h"
#include <aiwservicehandler.h>

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CCenrepEditorAppUi::ConstructL()
// 
// ----------------------------------------------------------
//
void CCenrepEditorAppUi::ConstructL()
    {    
    BaseConstructL(EAknEnableSkin);
    
    // Show tabs for main views from resources
    CEikStatusPane* sp = StatusPane();
    sp->MakeVisible(ETrue);
    // Fetch pointer to the default navi pane control
    iNaviPane = (CAknNavigationControlContainer*)sp->ControlL( 
        TUid::Uid(EEikStatusPaneUidNavi));
    iNaviPane->MakeVisible(ETrue);

    // Tabgroup has been read from resource and it were pushed to the navi pane. 
    // Get pointer to the navigation decorator with the ResourceDecorator() function. 
    // Application owns the decorator and it has responsibility to delete the object.
    iDecoratedTabGroup = iNaviPane->ResourceDecorator();
    iDecoratedTabGroup->MakeVisible(ETrue);
    if (iDecoratedTabGroup)
        {
        iTabGroup = (CAknTabGroup*) iDecoratedTabGroup->DecoratedControl();
        }    
    iPhoneResources =  CPhoneResources::NewL();  
    iPhoneResources->ReadL();      
      
   	CCenrepEditorView1*   view1 = new (ELeave) CCenrepEditorView1;
    CleanupStack::PushL( view1 );
    view1->ConstructL();
    AddViewL( view1 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view1
         
    iOtherResources =  COtherResources::NewL();    
    iOtherResources->ReadL(); 

    CCenrepEditorView2*	view2 = new (ELeave) CCenrepEditorView2;
    CleanupStack::PushL( view2 );
    view2->ConstructL();
    AddViewL( view2 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view1

    iMusApplication =  CMusApplication::NewL();    
    CCenrepEditorView3* view3 = new (ELeave) CCenrepEditorView3;
    CleanupStack::PushL( view3 );
    view3->ConstructL();
    AddViewL( view3 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view1       
    SetDefaultViewL(*view1);    
    iTabGroup->SetActiveTabByIndex( 0);

       
    }

// ----------------------------------------------------
// CCenrepEditorAppUi::~CCenrepEditorAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------
//
CCenrepEditorAppUi::~CCenrepEditorAppUi()
    {    
    delete iPhoneResources; 
    delete iOtherResources; 
    delete iMusApplication;    
    delete iDecoratedTabGroup;
    
    iDecoratedTabGroup=NULL;
    iNaviPane=NULL ; // not owned
    iTabGroup = NULL ; // not owned
    
    }

// ------------------------------------------------------------------------------
// CCenrepEditorAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
// This function is called by the EIKON framework just before it displays
// a menu pane. Its default implementation is empty, and by overriding it,
// the application can set the state of menu items dynamically according
// to the state of application data.
// ------------------------------------------------------------------------------
//
void CCenrepEditorAppUi::DynInitMenuPaneL(TInt /*aResourceId*/, CEikMenuPane* /*aMenuPane*/)
    {
   
    }

// ----------------------------------------------------
// CCenrepEditorAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent, 
//     TEventCode aType)
// Takes care of key event handling.
// ----------------------------------------------------
//
TKeyResponse CCenrepEditorAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,
    TEventCode /*aType*/)
    {
    if ( iTabGroup == NULL )
        {
        return EKeyWasNotConsumed;
        }

    TInt active = iTabGroup->ActiveTabIndex();    

    switch ( aKeyEvent.iCode )
        {
        case EKeyLeftArrow:
            if ( active == 1 )
                {                
                iTabGroup->SetActiveTabByIndex( 0);
                ActivateLocalViewL(KViewId1);                
                }
            else if ( active == 2 )
                {                
                iTabGroup->SetActiveTabByIndex( 1);
                ActivateLocalViewL(KViewId2);                
                }
            break;
        case EKeyRightArrow:
            if( active == 0 )
                {                
                iTabGroup->SetActiveTabByIndex( 1 );
                ActivateLocalViewL(KViewId2);                
                }
            else if( active == 1 )
                {                
                iTabGroup->SetActiveTabByIndex( 2 );
                ActivateLocalViewL(KViewId3);                
                }
            break;        
        default:
            return EKeyWasNotConsumed;
        }

    return EKeyWasConsumed;

    }

// ----------------------------------------------------
// CCenrepEditorAppUi::HandleCommandL(TInt aCommand)
// Takes care of command handling.
// ----------------------------------------------------
//
void CCenrepEditorAppUi::HandleCommandL(TInt aCommand)
    {
    switch ( aCommand )
        {
        case EAknSoftKeyExit:  // fallthrough
        case EEikCmdExit:
            {            
  
            Exit();
            break;
            }              
        default:
            break;
        }
    }

// ----------------------------------------------------
// CCenrepEditorAppUi::PhoneResouces()
// Takes care of command handling.
// ----------------------------------------------------
//
CPhoneResources* CCenrepEditorAppUi::PhoneResouces()
    {
    return iPhoneResources;
    }

// ----------------------------------------------------
// CCenrepEditorAppUi::OtherResouces()
// Takes care of command handling.
// ----------------------------------------------------
//
COtherResources* CCenrepEditorAppUi::OtherResouces()
    {
    return iOtherResources;
    }

// ----------------------------------------------------
// CCenrepEditorAppUi::MusApplication()
// Takes care of command handling.
// ----------------------------------------------------
//
CMusApplication* CCenrepEditorAppUi::MusApplication()
    {
    return iMusApplication;
    }


// End of File  
