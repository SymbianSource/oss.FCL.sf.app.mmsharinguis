/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "contactenginestub.h"

#include <cntitem.h>
#include <cntfield.h>
#include <cntdef.h>
#include <cntfldst.h>

static TInt iValue = KErrNone;
static TContactEngineCalledFunction iFunc = EContactEngineStubNone;
static TBool iFindField = EFalse;
static TBool iFindFieldInfo = ETrue;
static CContactTextField iTextField;

// -----------------------------------------------------------------------------
// CContactTextField
// -----------------------------------------------------------------------------
//
void CContactTextField::SetTextL(const TDesC& /*aText*/)
    {
    iFunc = EContactEngineStubSetText;
    }

CContactTextField::~CContactTextField()
    {
    
    }

void CContactTextField::InternalizeL(RReadStream& /*aStream*/)    
    {

    }
void CContactTextField::ExternalizeL(RWriteStream& /*aStream*/) const    
    {

    }
TStreamId CContactTextField::StoreL(CStreamStore& /*aStore*/) const    
    {
    return TStreamId( 0 );
    }

void CContactTextField::RestoreL(CStreamStore& /*aStore*/,RReadStream& /*aStream*/)    
    {

    }

TBool CContactTextField::IsFull() const    
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// ContactEngineStubHelper
// -----------------------------------------------------------------------------
//
void ContactEngineStubHelper::Reset()
    {
    iValue = KErrNone;
    iFunc = EContactEngineStubNone;
    iFindField = EFalse;
    iFindFieldInfo = ETrue;
    }

void ContactEngineStubHelper::SetErrorCode( TInt aVal )
    {
    iValue = aVal;
    }

TContactEngineCalledFunction ContactEngineStubHelper::GetCalledFunction()
    {
    return iFunc;
    }

void ContactEngineStubHelper::SetSIPFieldFound( TBool aFound )
    {
    iFindField = aFound;
    }

void ContactEngineStubHelper::SetSIPFieldInfoFound( TBool aFound )
    {
    iFindFieldInfo = aFound;
    }
