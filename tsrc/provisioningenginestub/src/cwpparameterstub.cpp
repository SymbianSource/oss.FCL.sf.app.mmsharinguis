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



#include "cwpparameter.h"
#include "mwpvisitor.h"


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
CWPParameter* CWPParameter::NewL()
    {
    CWPParameter* self = CWPParameter::NewL();
    CleanupStack::Pop();
    return self;
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//     
CWPParameter* CWPParameter::NewLC()
    {
    CWPParameter* self = new ( ELeave ) CWPParameter();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//     
CWPParameter::~CWPParameter()
    {
    delete iName;
    delete iValue;
    }
    

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//     
EXPORT_C void CWPParameter::SetID( TInt aId )
    {
    iParameterID = aId;
    }
    

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//     
EXPORT_C TInt CWPParameter::ID() const
    {
    return iParameterID;
    }

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//     
EXPORT_C void CWPParameter::SetNameL( const TDesC& aValue )
    {
    delete iName;
    iName = NULL;
    iName = aValue.AllocL();
    }

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//     
EXPORT_C const TDesC& CWPParameter::Name() const
    {
    if ( iName )
        {
        return *iName;
        }
    return KNullDesC;
    }

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//     
EXPORT_C void CWPParameter::SetValueL( const TDesC& aValue )
    {
    delete iValue;
    iValue = NULL;
    iValue = aValue.AllocL();
    }

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//     
EXPORT_C const TDesC& CWPParameter::Value() const
    {
    if ( iValue )
        {
        return *iValue;
        }
    return KNullDesC;
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void CWPParameter::CallVisitorL( MWPVisitor& aVisitor )
    {
    aVisitor.VisitL( *this );
    }
        

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
TInt CWPParameter::Type() const
    {
    return 0;
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void CWPParameter::ExternalizeL( RWriteStream& /*aStream*/ ) const
    {
    }
    
    
// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void CWPParameter::InternalizeL( RReadStream& /*aStream*/ )
    {
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
CWPParameter::CWPParameter()
    {
    }
    

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void CWPParameter::ConstructL()
    {
    }



