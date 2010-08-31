/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


// INCLUDES
#include "muscallmonitorbase.h"
#include "muscallstatusmonitor.h"

#include <e32std.h>


//
//CMusCallMonitorBase stub
//

CMusCallMonitorBase::~CMusCallMonitorBase()
    {
    }

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusCallMonitorBase::CMusCallMonitorBase(const RMobileCall& aCall, 
                                            MMusTsyPropertyObserver& aObserver,
                                            MMusCallStateObserver& aCallStateObserver ) 
        : CActive( EPriorityNormal ),iCall(aCall),iTsyObserver( aObserver ),iCallStateObserver( aCallStateObserver )
        
    {   
    }

// -----------------------------------------------------------------------------
// CMusCallMonitorBase::SetStateL( TInt aError )
// Sets the Call State to Property
// -----------------------------------------------------------------------------
//
void CMusCallMonitorBase::SetStateL(NMusSessionInformationApi::TMusCallEvent /*aVal*/)
    {
    }


// -----------------------------------------------------------------------------
// CMusCallMonitorBase::RunError( TInt aError )
// Implemented for CActive.
// Will be called automatically by framework when RunL Leaves.
// -----------------------------------------------------------------------------
//

TInt CMusCallMonitorBase::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMusCallMonitorBase::NotifyCallStateChanged()
// Notify TSYobserver that our call status or call event state changed
// -----------------------------------------------------------------------------
//
void CMusCallMonitorBase::NotifyCallStateChanged( NMusSessionInformationApi::TMusCallEvent /*aVal*/ )
    {
    }


// -----------------------------------------------------------------------------
// MusCallStatusMonitor
// -----------------------------------------------------------------------------
//

CMusCallStatusMonitor* CMusCallStatusMonitor::NewL(const RMobileCall&, MMusTsyPropertyObserver&, 
        MMusCallStateObserver& )
    {
    return NULL;
    }

//  End of File  

