/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Main plugin class
*
*/




#include <alwaysonlinemanagercommon.h>
#include <alwaysonlinemanagerclient.h>
#include <digia/eunit/eunitmacros.h>
    
#include "musaocallmonitor.h"
#include "musaoplugin.h"
#include "musaoplugintest.h"
#include "muslogger.h" 
#include "musmanager.h"

// --------------------------------------------------------------------------
// C++ constructor.
// --------------------------------------------------------------------------
// 
CMusAoPluginTest::CMusAoPluginTest()
    {
    }


// --------------------------------------------------------------------------
// Symbian two-phase constructor.
// --------------------------------------------------------------------------
// 
CMusAoPluginTest* CMusAoPluginTest::NewL()
    {
    CMusAoPluginTest* self = new (ELeave) CMusAoPluginTest();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// --------------------------------------------------------------------------
// Symbian second-phase constructor.
// --------------------------------------------------------------------------
// 
void CMusAoPluginTest::ConstructL()
    {
    }


// --------------------------------------------------------------------------
// C++ destructor.
// --------------------------------------------------------------------------
// 
CMusAoPluginTest::~CMusAoPluginTest()
    {
    }

// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// 
void CMusAoPluginTest::HandleServerCommandL()
    {
    iPlugin->HandleServerCommandL( EAOManagerPluginStart, NULL );
    iPlugin->HandleServerCommandL( EAOManagerPluginStop, NULL );
    iPlugin->HandleServerCommandL( EAOManagerNWOpsNotAllowed, NULL );
    iPlugin->HandleServerCommandL( EAOManagerNWOpsAllowed, NULL );
    iPlugin->HandleServerCommandL( EAOManagerStartedRoaming, NULL );
    iPlugin->HandleServerCommandL( EAOManagerStoppedRoaming, NULL );
    iPlugin->HandleServerCommandL( EAOManagerDiskSpaceAboveCritical, NULL );
    iPlugin->HandleServerCommandL( EAOManagerDiskSpaceBelowCritical, NULL );
    }


// --------------------------------------------------------------------------
// From base class MMusAoCallMonitorObserver.
// --------------------------------------------------------------------------
// 
void CMusAoPluginTest::LineStatusChange()
    {
    iPlugin->LineStatusChange( MMusAoCallMonitorObserver::ELineConnected );
    iPlugin->LineStatusChange( MMusAoCallMonitorObserver::ELineDisconnected );
    iPlugin->LineStatusChange( MMusAoCallMonitorObserver::EUnknown ); 
    }
    
void CMusAoPluginTest::SetupL(  )
    {
    iPlugin = CMusAoPlugin::NewL();
    }

void CMusAoPluginTest::Teardown(  )
    {
    delete iPlugin;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    CMusAoPluginTest,
    "CMusAoPluginTest",
    "CMusAoPluginTest" )

EUNIT_TEST(
    "CMusAoPluginTest",
    "CMusAoPluginTest",
    "HandleServerCommandL",
    "FUNCTIONALITY",
    SetupL, HandleServerCommandL, Teardown)

EUNIT_TEST(
    "CMusAoPluginTest",
    "CMusAoPluginTest",
    "LineStatusChange",
    "FUNCTIONALITY",
    SetupL, LineStatusChange, Teardown)


EUNIT_END_TEST_TABLE

