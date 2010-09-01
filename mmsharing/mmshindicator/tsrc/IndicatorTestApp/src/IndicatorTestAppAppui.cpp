/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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

#include <avkon.hrh>
#include <aknnotewrappers.h> 

#include "IndicatorTestApp.pan"
#include "IndicatorTestAppAppUi.h"
#include "IndicatorTestAppAppView.h"
#include "IndicatorTestApp.hrh"
#include "indicatortestapp.rsg"

#include "musindicatorapi.h"
#include "mussettings.h"

using namespace MusSettingsKeys;

// ConstructL is called by the application framework
void CIndicatorTestAppAppUi::ConstructL()
    {
    BaseConstructL();

    iAppView = CIndicatorTestAppAppView::NewL(ClientRect());    

    AddToStackL(iAppView);
    
    iIndicator = CMusIndicatorApi::NewL( *this );
    
    }

CIndicatorTestAppAppUi::CIndicatorTestAppAppUi()                              
    {
	// no implementation required
    }

CIndicatorTestAppAppUi::~CIndicatorTestAppAppUi()
    {
    if (iAppView)
        {
        RemoveFromStack(iAppView);
        delete iAppView;
        iAppView = NULL;
        }
        
    delete iIndicator;        
    }

// handle any menu commands
void CIndicatorTestAppAppUi::HandleCommandL(TInt aCommand)
    {
    switch(aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            Exit();
            break;

        case EIndicatorTestAppEnableOperator:
            {
            _LIT(message,"Hello!");
            CAknInformationNote* informationNote = new (ELeave) CAknInformationNote;
            informationNote->ExecuteLD(message);
            }
            break;
            
        case EIndicatorTestAppSharingAvailable:
            {
            break;
            }
        case EIndicatorTestAppSharingNotAvailable:
            {
            break;
            }
        default:
            //Panic(EIndicatorTestAppBasicUi);
            break;
        }


    iIndicator->IndicateAvailabilityL();
    }


void CIndicatorTestAppAppUi::StartLiveSharingL() 
    {
    // 
    TInt a;
    a = 0;
   // CRepository
    }

void CIndicatorTestAppAppUi::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
    if( aResourceId == R_INDICATORTESTAPP_MENU ) 
        {        
        if( MultimediaSharingSettings::AuditoryNotificationSettingL() == EAuditoryNotificationOn )
            {
            aMenuPane->DeleteMenuItem( EIndicatorTestAppNoAudiotoryNotification );
            }
        else
            {
            aMenuPane->DeleteMenuItem( EIndicatorTestAppAudiotoryNotification );
            }
        if( MultimediaSharingSettings::OperatorVariantSettingL() == EOperatorSpecific )
            {
            aMenuPane->DeleteMenuItem( EIndicatorTestAppDisableOperator );
            }
        else
            {
            aMenuPane->DeleteMenuItem( EIndicatorTestAppEnableOperator );
            }

        // Write some key for MainUI app
        aMenuPane->DeleteMenuItem( EIndicatorTestAppIconNotification );
        aMenuPane->DeleteMenuItem( EIndicatorTestAppNoIconNotification );
            
        }
    }
