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
* Description:  The MUS application's UI class.
*
*/


#include "musuisendview.h"
#include "musuiappui.h"
#include "musuisendcontroller.h"
#include "musuisendviewcontainer.h"
#include "musuidefinitions.h"
#include "musuidialogutil.h"
#include "muslogger.h" // debug logging

#include <musui.rsg>

#include <aknViewAppUi.h>


// -----------------------------------------------------------------------------
// EPOC two-phased constructor
// -----------------------------------------------------------------------------
//
void CMusUiSendView::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendView::ConstructL" );
    CMusUiGeneralView::ConstructL();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendView::ConstructL" );
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMusUiSendView::~CMusUiSendView()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendView::~CMusUiSendView" );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendView::~CMusUiSendView" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendView::ManualAddressEntryL( const TDesC& aAddress )
    {
    MUS_LOG_TDESC( "mus: [MUSUI ]    -> CMusUiSendView::ManualAddressEntryL: ",
                            aAddress );
     
    
     TBuf<KMusSipAddressMaxLength> address( aAddress );
    if ( SendController() && MusUiDialogUtil::ShowTextQueryDialogL( 
                                    R_MUS_TEXT_ENTER_ADDRESS_DIALOG_PROMPT, 
                                    address ) )
        {      
        SendController()->InviteL( address );
        }
    else
        {
        MusUiDialogUtil::ShowGlobalInformationDialogL( 
                            R_MUS_LIVE_SHARING_VIEW_NOTE_SHARING_ENDED );
        MusAppUi()->HandleExit();
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendView::ManualAddressEntryL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiEventController* CMusUiSendView::EventController() const
    {
    return SendController();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiViewContainer& CMusUiSendView::SharingContainer() const
    {
    return *SendContainer();
    }
        

// -----------------------------------------------------------------------------
// From CAknView, Gets called from framework when activating this view
// -----------------------------------------------------------------------------
//
void CMusUiSendView::DoActivateL( const TVwsViewId& aPrevViewId,
                                    TUid aCustomMessageId,
                                    const TDesC8& aCustomMessage )
    {
    // Base class activation first
    CMusUiGeneralView::DoActivateL( aPrevViewId,
                                    aCustomMessageId,
                                    aCustomMessage ); 
                                    
    }


// -----------------------------------------------------------------------------
// From AknView, Gets called from framework when deactivating this view
// -----------------------------------------------------------------------------
//
void CMusUiSendView::DoDeactivate()
    {
    CMusUiGeneralView::DoDeactivate(); // Base class deactivation first
    }
    
    
// -----------------------------------------------------------------------------
// Called when focus event occurs.
// -----------------------------------------------------------------------------
//
void CMusUiSendView::DoHandleForegroundEventL( TBool aForeground )
    {
    CMusUiGeneralView::HandleForegroundEventL( 
                                aForeground, 
                                iOperatorSpecificFunctionality &&
                                EventControllerL().ConnectionEstablished() &&
                                !EventControllerL().ExitOccured() );
    }    


// end of file
