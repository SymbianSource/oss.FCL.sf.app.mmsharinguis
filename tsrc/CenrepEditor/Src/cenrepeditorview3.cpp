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
#include  "cenrepeditorview3.h"
#include  "cenrepeditorview3container.h" 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CCenrepEditorView3::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CCenrepEditorView3::ConstructL()
    {
    BaseConstructL( R_CENREPEDITOR_VIEW3 );
    }

// ---------------------------------------------------------
// CCenrepEditorView3::~CCenrepEditorView3()
// destructor
// ---------------------------------------------------------
//
CCenrepEditorView3::~CCenrepEditorView3()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CCenrepEditorView3::Id()
//
// ---------------------------------------------------------
//
TUid CCenrepEditorView3::Id() const
    {
    return KViewId3;
    }

// ---------------------------------------------------------
// CCenrepEditorView3::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CCenrepEditorView3::HandleCommandL(TInt aCommand)
    {   
    switch ( aCommand )
        {
        case EAknSoftKeyStart: 
            {  
            iContainer->StoreSettingsL();
            CCenrepEditorAppUi* appUI= static_cast<CCenrepEditorAppUi*>(iEikonEnv->EikAppUi());  
            CMusApplication* musApplication=appUI->MusApplication();  
            if(!musApplication-> AvailabilityInfo())/*break*/; // Mus Not available to start.Dont care            
            TBool result=musApplication->Start(musApplication->iUseCase);
            if(result)
                {
                Cba()->SetCommandSetL(R_CENREPEDITOR_SOFTKEYS_STOP_EXIT);        
                Cba()->MakeVisible(ETrue);
                Cba()->DrawDeferred();   
                musApplication->MusUseCaseInfo(ETrue);
                }            
            musApplication->MusUseCaseInfo(ETrue);
            break;
            }
        case EAknSoftKeyStop: 
            {
            iContainer->StoreSettingsL();
            CCenrepEditorAppUi* appUI= static_cast<CCenrepEditorAppUi*>(iEikonEnv->EikAppUi());  
            CMusApplication* musApplication=appUI->MusApplication();                
            TBool result=musApplication->Stop();
            if(result)
                {
                Cba()->SetCommandSetL(R_CENREPEDITOR_SOFTKEYS_START_EXIT);        
                Cba()->MakeVisible(ETrue);
                Cba()->DrawDeferred();           
                musApplication->MusUseCaseInfo(EFalse);    
                }
             musApplication->MusUseCaseInfo(EFalse);
             break;
             }
        default:
            {
            
            
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }

// ---------------------------------------------------------
// CCenrepEditorView3::HandleClientRectChange()
// ---------------------------------------------------------
//
void CCenrepEditorView3::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CCenrepEditorView3::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CCenrepEditorView3::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {    
     CCenrepEditorAppUi* appUI= static_cast<CCenrepEditorAppUi*>(iEikonEnv->EikAppUi());  
     CMusApplication* musApplication=appUI->MusApplication();   
    if (!iContainer)
        {
        iContainer = new (ELeave) CCenrepEditorView3Container;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect(),R_CENREPEDITOR_START_STOP_SETTINGS_LIST,musApplication);
        AppUi()->AddToViewStackL( *this, iContainer );        
        iContainer->MakeVisible( ETrue ); 
        }  
                 
    iContainer->DrawNow();
   }

// ---------------------------------------------------------
// CCenrepEditorView3::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CCenrepEditorView3::DoDeactivate()
    {    

    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }


// End of File

