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
* Description: 
*
*/



#include "ut_musaiwresourcefinder.h"
#include "musaiwresourcefinder.h"

#include <digia/eunit/eunitmacros.h>

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when
// EUnit is fixed.
#pragma warn_illtokenpasting off


_LIT( KMusAiwProviderResourceRom, "z:\\resource\\musaiwproviderui.rsc" );
_LIT( KMusAiwProviderResourceRam, "c:\\resource\\musaiwproviderui.rsc" );


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
UT_MusAiwResourceFinder* UT_MusAiwResourceFinder::NewL()
    {
    UT_MusAiwResourceFinder* self = new( ELeave ) UT_MusAiwResourceFinder;
    CleanupStack::Pop( self );
    return self;

    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
UT_MusAiwResourceFinder* UT_MusAiwResourceFinder::NewLC()
    {
    UT_MusAiwResourceFinder* self = new( ELeave ) UT_MusAiwResourceFinder;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
UT_MusAiwResourceFinder::~UT_MusAiwResourceFinder()
    {
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
UT_MusAiwResourceFinder::UT_MusAiwResourceFinder()
    {
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void UT_MusAiwResourceFinder::ConstructL()
    {
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void UT_MusAiwResourceFinder::SetupL()
    {
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void UT_MusAiwResourceFinder::Teardown()
    {
    }


// --- Test cases --------------------------------------------------------------


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void UT_MusAiwResourceFinder::TestL()
    {
    EUNIT_ASSERT( MusAiwResourceFinder::ResourceFilenameRom() ==
                  KMusAiwProviderResourceRom() )
    EUNIT_ASSERT( MusAiwResourceFinder::ResourceFilenameRam() ==
                  KMusAiwProviderResourceRam() )
    }



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_MusAiwResourceFinder,
    "MusAiwResourceFinder",
    "UT_MusAiwResourceFinder" )

EUNIT_TEST(
    "TestL",
    "MusAiwResourceFinder",
    "ResourceFilenameRam, ResourceFilenameRom ",
    "FUNCTIONALITY",
    SetupL, TestL, Teardown)

EUNIT_END_TEST_TABLE