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
* Description:
*
*/


// INCLUDE FILES
#include <aknmessagequerydialog.h>
#include <aknquerydialog.h>
#include <aknnotewrappers.h>
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <cenrepeditor.rsg>
#include "cenrepeditorappui.h"
#include  "cenrepeditordocument.h"
#include  "cenrepeditormodel.h"
#include  "cenrepeditorview2.h"
#include  "cenrepeditorview2container.h" 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCenrepEditorView2::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CCenrepEditorView2::ConstructL()
    {
    BaseConstructL( R_CENREPEDITOR_VIEW1 );
    }

// ---------------------------------------------------------
// CCenrepEditorView2::~CCenrepEditorView2()
// destructor
// ---------------------------------------------------------
//
CCenrepEditorView2::~CCenrepEditorView2()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CCenrepEditorView2::Id()
//
// ---------------------------------------------------------
//
TUid CCenrepEditorView2::Id() const
    {
    return KViewId2;
    }

// ---------------------------------------------------------
// CCenrepEditorView2::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CCenrepEditorView2::HandleCommandL(TInt aCommand)
    {   
 
 	iContainer->StoreSettingsL();
 
 	   COtherResources * otherResources = static_cast<CCenrepEditorAppUi*>(AppUi())->OtherResouces();
        TRAPD(err,otherResources->SaveL());
        
        TBuf<50> buf;
       
        if(err)               
            {
            CAknErrorNote *dlg = new CAknErrorNote(ETrue); 
            buf.Copy(_L("Unable to Save "));
            buf.AppendNum(err);
            dlg->ExecuteLD(buf);                
            } 
        else
            {
        	CAknInformationNote *dlg = new CAknInformationNote(ETrue); 
            buf.Copy(_L("Saved"));
       		dlg->ExecuteLD(buf);   
            }                  
 
         AppUi()->HandleCommandL( aCommand );
     
    }

// ---------------------------------------------------------
// CCenrepEditorView2::HandleClientRectChange()
// ---------------------------------------------------------
//
void CCenrepEditorView2::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CCenrepEditorView2::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CCenrepEditorView2::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    CCenrepEditorAppUi* appUI= static_cast<CCenrepEditorAppUi*>(iEikonEnv->EikAppUi());
    COtherResources* otherResources=appUI->OtherResouces();   
    if (!iContainer)
        {
        iContainer = new (ELeave) CCenrepEditorView2Container;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect(),R_OTHER_RESOURCE_DETAILS_SETTINGS_LIST, otherResources);
        AppUi()->AddToViewStackL( *this, iContainer );        
        iContainer->MakeVisible( ETrue ); 
        }  
                 
    iContainer->DrawNow();
   }

// ---------------------------------------------------------
// CCenrepEditorView2::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CCenrepEditorView2::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

// End of File

