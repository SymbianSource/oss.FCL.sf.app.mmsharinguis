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
#include  "cenrepeditorview1.h"
#include  "cenrepeditorview1container.h" 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCenrepEditorView1::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CCenrepEditorView1::ConstructL()
    {
    BaseConstructL( R_CENREPEDITOR_VIEW1 );
    }

// ---------------------------------------------------------
// CCenrepEditorView1::~CCenrepEditorView1()
// destructor
// ---------------------------------------------------------
//
CCenrepEditorView1::~CCenrepEditorView1()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CCenrepEditorView1::Id()
//
// ---------------------------------------------------------
//
TUid CCenrepEditorView1::Id() const
    {
    return KViewId1;
    }

// ---------------------------------------------------------
// CCenrepEditorView1::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CCenrepEditorView1::HandleCommandL(TInt aCommand)
    {      
    
            
     
        
       	iContainer->StoreSettingsL();
 
 	   CPhoneResources * phoneResources = static_cast<CCenrepEditorAppUi*>(AppUi())->PhoneResouces();
        TRAPD(err,phoneResources->SaveL());
        
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
// CCenrepEditorView1::HandleClientRectChange()
// ---------------------------------------------------------
//
void CCenrepEditorView1::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }
    
CCenrepEditorView1Container * CCenrepEditorView1::Container()
{
	return iContainer;
}
// ---------------------------------------------------------
// CCenrepEditorView1::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CCenrepEditorView1::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    CCenrepEditorAppUi* appUI= static_cast<CCenrepEditorAppUi*>(iEikonEnv->EikAppUi());
    CPhoneResources* phoneResources=appUI->PhoneResouces();   
    if (!iContainer)
        {
        iContainer = new (ELeave) CCenrepEditorView1Container;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect(),R_PHONE_RESOURCE_DETAILS_SETTINGS_LIST, phoneResources);
        AppUi()->AddToViewStackL( *this, iContainer );        
        iContainer->MakeVisible( ETrue ); 
        }  
                 
    iContainer->DrawNow();
   }

// ---------------------------------------------------------
// CCenrepEditorView1::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CCenrepEditorView1::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }

// End of File

