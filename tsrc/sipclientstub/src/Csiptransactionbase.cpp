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

#include "siptransactionbase.h"
#include "sipresponseelements.h"
#include "sipstrings.h"
#include "SipStrConsts.h"
#include "musunittesting.h"

CSIPTransactionBase::~CSIPTransactionBase()
	{
	delete iResponseElements;
	}

CSIPTransactionBase::CSIPTransactionBase( RStringF aType, TBool aIsCliTrx )
  : iType(aType),
    iIsClientTransaction(aIsCliTrx),
    iState(ETrying)
	{
	    
	if ( iIsClientTransaction && iType == SIPStrings::StringF( SipStrConsts::EInvite ) )
	    {
	    iState = ECalling;
	    }
	}

EXPORT_C TBool CSIPTransactionBase::operator==(const CSIPTransactionBase& aTransactionBase) const
	{
	return (this == &aTransactionBase);
	}

EXPORT_C CSIPTransactionBase::TState CSIPTransactionBase::StateL()
	{
	return iState;
	}

EXPORT_C RStringF CSIPTransactionBase::Type() const 
    {
    return iType;
    }

EXPORT_C TBool CSIPTransactionBase::IsSIPClientTransaction() const
    {
    return iIsClientTransaction;
    }

void CSIPTransactionBase::ChangeState( CSIPTransactionBase::TState aNextState )
    {
    iState = aNextState;
    } 
    
void CSIPTransactionBase::Detach(const MTransactionAssociation& /*aAssociation*/)
    {
    }

