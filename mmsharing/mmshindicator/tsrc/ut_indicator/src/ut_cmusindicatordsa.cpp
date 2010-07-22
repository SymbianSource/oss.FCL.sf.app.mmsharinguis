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
* Description:  Implements unit tests for CMusIndicatorApi class.
*
*/



#include "ut_cmusindicatordsa.h"
#include "musindicatordsa.h"
#include "mustesthelp.h"
#include "e32property.h"

#include <digia/eunit/eunitmacros.h>
#include <AknGlobalMsgQuery.h>
#include <AknsDrawUtils.h>


// Next row is to disable warning emerging from EUnit code.
#pragma warn_illtokenpasting off

const TSize KOriginalSize = TSize( 176, 208 );
const TSize KDifferentSize = TSize( 208, 176 );

static CWindowGc* gc = NULL;
static TAny* cc = NULL;
static TTimeIntervalMicroSeconds32 delay = 0;
static TSize sizeInPixels = KOriginalSize;


// ======== stubs ========


void CCoeControl::CreateWindowL()
    {
    
    }

void CCoeControl::ActivateL()
    {
    
    }

void CCoeControl::DrawDeferred() const
    {
    }

CWindowGc& CCoeControl::SystemGc() const
    {
    return *gc;
    }

MWsClientClass::MWsClientClass()
    {
    }

CWindowGc::CWindowGc(CWsScreenDevice* /*aDevice*/ )
    {
    
    }

CWindowGc::~CWindowGc()
    {
    
    }

void CWindowGc::SetBrushStyle( TBrushStyle /*aBrushStyle*/ )
    {
    }

void CWindowGc::Clear( const TRect& /*aRect*/ )
    {
    }
    
void CWindowGc::UseFont( const CFont* /*aFont*/ )
    {
    }
    
void CWindowGc::SetPenStyle( TPenStyle /*aPenStyle*/ )
    {
    }

void CWindowGc::SetPenColor( const TRgb& /*aColor*/ )
    {
    }

void CWindowGc::SetPenSize( const TSize& /*aSize*/ )
    {
    }

void CWindowGc::DrawRoundRect( const TRect& /*aRect*/,const TSize& /*aEllipse*/ )
    {
    }

void CWindowGc::DrawText( const TDesC& /*aBuf*/,const TPoint& /*aPos*/ )
    {
    }
    
//Cannot stub due it is virtual method. This is not called
TSize CWsScreenDevice::SizeInPixels() const
    {
    return sizeInPixels;
    }

void CWindowGc::BitBltMasked(const TPoint& /*aPoint*/,const CFbsBitmap* /*aBitmap*/,
        const TRect& /*aSourceRect*/,const CFbsBitmap* /*aMaskBitmap*/,TBool /*aInvertMask*/)
    {
    }

MAknsControlContext* AknsDrawUtils::ControlContext( const MObjectProvider* /*aMop*/ )
    {
    return static_cast<MAknsControlContext*>( cc );
    }

TBool AknsDrawUtils::Background( MAknsSkinInstance* /*aInstance*/,
            MAknsControlContext* /*aContext*/, const CCoeControl* /*aControl*/,
            CWindowGc& /*aGc*/, const TRect& /*aRect*/, const TInt /*aDrawParam*/ )
    {
    return ETrue;
    }


void CPeriodic::Start(TTimeIntervalMicroSeconds32 aDelay,TTimeIntervalMicroSeconds32 /*anInterval*/,TCallBack /*aCallBack*/)
    {
    delay = aDelay;
    After(aDelay);
    
    }



// ======== MEMBER FUNCTIONS ========


UT_CMusIndicatorDsa* UT_CMusIndicatorDsa::NewL()
    {
    UT_CMusIndicatorDsa* self = UT_CMusIndicatorDsa::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


UT_CMusIndicatorDsa* UT_CMusIndicatorDsa::NewLC()
    {
    UT_CMusIndicatorDsa* self = new( ELeave ) UT_CMusIndicatorDsa();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusIndicatorDsa::~UT_CMusIndicatorDsa()
    {
    }


UT_CMusIndicatorDsa::UT_CMusIndicatorDsa()
    {
    delete iIndicatorDsa;
    iIndicatorDsa = NULL;
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// ----------------------------------------------------------------------------
// Prepares a test for execution.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::SetupL()
    {
    /* NOTE: EUnit reports leak for any first test due AVKON resource reservation,
     * which is not released by AVKON at end of test. Resource reservation
     * happend in contruction of CMusIndicatorDsa
     */
    
    cc = this;
    iIndicatorDsa = CMusIndicatorDsa::NewL();
    if ( !gc )
        {
        gc = new (ELeave) CWindowGc( NULL );
        }
    delay = 0;
    sizeInPixels = KOriginalSize;
    
    }


// ----------------------------------------------------------------------------
// Finalizes a test by releasing used resources.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::Teardown()
    {
    delete iIndicatorDsa;
    iIndicatorDsa = NULL;
    delete gc;
    gc = NULL;
    cc = NULL;
    delay = 0;
    sizeInPixels = KOriginalSize;
    PropertyHelper::Close();
    
    }


// ======== UNIT TEST METHODS ========


// ----------------------------------------------------------------------------
// Asserts successful instantiation.
// ----------------------------------------------------------------------------
//

void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_NewLL()
    {
    EUNIT_ASSERT( iIndicatorDsa );
    }


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_DrawL()
    {
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );

    TRect rect;
    iIndicatorDsa->Draw( rect );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    
    cc = NULL;
    iIndicatorDsa->Draw( rect );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    
    /*size cannot be changed
    sizeInPixels = KDifferentSize;
    iIndicatorDsa->Draw( rect );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic->IsActive() );
    */
    
    }

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_InitializeL()
    {
    iIndicatorDsa->Initialize();
        
    
    EUNIT_ASSERT( iIndicatorDsa->iTextpos.iX );
    EUNIT_ASSERT( iIndicatorDsa->iTextpos.iY );
    
    }


// ----------------------------------------------------------------------------
// Asserts that load image is not doing anything if icon is already loaded
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_LoadImageLL()
    {
    CFbsBitmap* icon = iIndicatorDsa->iIcon;
    CFbsBitmap* iconMask = iIndicatorDsa->iIconMask;
    EUNIT_ASSERT( icon != NULL );
    EUNIT_ASSERT( iconMask != NULL );
    iIndicatorDsa->LoadImageL();
    
    EUNIT_ASSERT( icon == iIndicatorDsa->iIcon );
    EUNIT_ASSERT( iconMask == iIndicatorDsa->iIconMask );
    }


// ----------------------------------------------------------------------------
// Asserts successful loading of resource string.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_LoadResourceStringLL()
    {
    iIndicatorDsa->LoadResourceStringL();
    EUNIT_ASSERT( iIndicatorDsa->iText );
    EUNIT_ASSERT( iIndicatorDsa->iText->Length() );
    }



// ----------------------------------------------------------------------------
// Asserts that periodic timer is started after method call.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_StartPeriodicTimerL()
    {
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    EUNIT_ASSERT( delay.Int() == 0 );
    
    iIndicatorDsa->StartPeriodicTimer( iIndicatorDsa->iActivateCallBack );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic->IsActive() );
    EUNIT_ASSERT( delay.Int() == KMusIndicatorDelay );
    
    delay = 0;
    iIndicatorDsa->StartPeriodicTimer( iIndicatorDsa->iActivateCallBack );
    EUNIT_ASSERT( delay.Int() == 0 );
    
    iIndicatorDsa->iPeriodic->Cancel();
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    EUNIT_ASSERT( delay.Int() == 0 );
    
    iIndicatorDsa->StartPeriodicTimer( iIndicatorDsa->iRedrawCallBack );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic->IsActive() );
    EUNIT_ASSERT( delay.Int() == KMusRedrawDelay );
    
    }


// ----------------------------------------------------------------------------
// Asserts that periodic timer is stopped after method call.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_StopPeriodicTimerL()
    {
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    iIndicatorDsa->StopPeriodicTimer();
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    
    iIndicatorDsa->StartPeriodicTimer( iIndicatorDsa->iActivateCallBack );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic->IsActive() );
    
    iIndicatorDsa->StopPeriodicTimer();
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    
    }

// ----------------------------------------------------------------------------
// Asserts that periodic timer 
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_TryActivateL()
    {
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );

    EUNIT_ASSERT_EQUALS( iIndicatorDsa->TryActivate( iIndicatorDsa ), KErrNone );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );

    cc = NULL;

    // Through async callback
    EUNIT_ASSERT_EQUALS( iIndicatorDsa->TryActivate( iIndicatorDsa ), KErrNone );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic->IsActive() );
    
    EUNIT_ASSERT_EQUALS( iIndicatorDsa->TryActivate( NULL ), KErrArgument );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic->IsActive() );
    
    }

void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_DoHandleResourceChangeL()
    {
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );

    EUNIT_ASSERT_EQUALS( iIndicatorDsa->DoHandleResourceChange( iIndicatorDsa ), KErrNone );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );

    EUNIT_ASSERT_EQUALS( iIndicatorDsa->DoHandleResourceChange( NULL ), KErrArgument );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    
    }

void UT_CMusIndicatorDsa::UT_CMusIndicatorDsa_HandleResourceChangeL()
    {
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    iIndicatorDsa->StartPeriodicTimer( iIndicatorDsa->iRedrawCallBack );
    EUNIT_ASSERT( iIndicatorDsa->iPeriodic->IsActive() );
    
    iIndicatorDsa->iScreenSize = TSize();
    iIndicatorDsa->HandleResourceChange( KEikDynamicLayoutVariantSwitch );
    EUNIT_ASSERT( !iIndicatorDsa->iPeriodic->IsActive() );
    
    EUNIT_ASSERT( iIndicatorDsa->iScreenSize != TSize() );
    
    iIndicatorDsa->iScreenSize = TSize();
    iIndicatorDsa->HandleResourceChange( 0 );
    EUNIT_ASSERT( iIndicatorDsa->iScreenSize == TSize() );
    
    }



// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusIndicatorDsa,
    "UT_CMusIndicatorDsa",
    "UNIT" )

EUNIT_TEST(
    "NewL - test",
    "CMusIndicatorDsa",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_NewLL, Teardown )


EUNIT_TEST(
    "Draw - test",
    "CMusIndicatorDsa",
    "Draw",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_DrawL, Teardown )


EUNIT_TEST(
    "Initialize - test",
    "CMusIndicatorDsa",
    "Initialize",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_InitializeL, Teardown )

EUNIT_TEST(
    "LoadImageL - test",
    "CMusIndicatorDsa",
    "LoadImageL",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_LoadImageLL, Teardown )

EUNIT_TEST(
    "LoadResourceStringL - test",
    "CMusIndicatorDsa",
    "LoadResourceStringL",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_LoadResourceStringLL, Teardown )


EUNIT_TEST(
    "TryActivate - test",
    "CMusIndicatorDsa",
    "TryActivate",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_TryActivateL, Teardown )

EUNIT_TEST(
    "StartPeriodicTimer - test",
    "CMusIndicatorDsa",
    "StartPeriodicTimer",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_StartPeriodicTimerL, Teardown )

EUNIT_TEST(
    "StopPeriodicTimer - test",
    "CMusIndicatorDsa",
    "StopPeriodicTimer",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_StopPeriodicTimerL, Teardown )

EUNIT_TEST(
    "DoHandleResourceChange - test",
    "CMusIndicatorDsa",
    "DoHandleResourceChange",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_DoHandleResourceChangeL, Teardown )

EUNIT_TEST(
    "HandleResourceChange - test",
    "CMusIndicatorDsa",
    "HandleResourceChange",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorDsa_HandleResourceChangeL, Teardown )

    
EUNIT_END_TEST_TABLE
