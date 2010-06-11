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

#include "cwpcharacteristic.h"


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
CWPCharacteristic* CWPCharacteristic::NewL( TInt aType )
    {
    CWPCharacteristic* self = CWPCharacteristic::NewL( aType );
    CleanupStack::Pop();
    return self;
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//     
CWPCharacteristic* CWPCharacteristic::NewLC( TInt aType )
    {
    CWPCharacteristic* self = new ( ELeave ) CWPCharacteristic( aType );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
CWPCharacteristic::~CWPCharacteristic()
    {
    delete iName;
    iElements.ResetAndDestroy();
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
EXPORT_C void CWPCharacteristic::InsertL(MWPElement* aElement)
    {
    iElements.AppendL( aElement );
    }
    

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
EXPORT_C void CWPCharacteristic::SetNameL( const TDesC& aValue )
    {
    delete iName;
    iName = NULL;
    iName = aValue.AllocL();
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
EXPORT_C const TDesC& CWPCharacteristic::Name() const
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
EXPORT_C void CWPCharacteristic::AcceptL( MWPVisitor& aVisitor )
    {
    for( TInt i( 0 ); i < iElements.Count(); i++ )
        {
        iElements[i]->CallVisitorL( aVisitor );
        }
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void CWPCharacteristic::CallVisitorL( MWPVisitor& aVisitor )
    {
    aVisitor.VisitL( *this );
    }
        

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
TInt CWPCharacteristic::Type() const
    {
    return iType;
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void CWPCharacteristic::ExternalizeL( RWriteStream& /*aStream*/ ) const
    {
    }
    
    
// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void CWPCharacteristic::InternalizeL( RReadStream& /*aStream*/ )
    {
    }
        

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
CWPCharacteristic::CWPCharacteristic( TInt aType )
    :iType( aType )
    {
    }


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------
//
void CWPCharacteristic::ConstructL()
    {
    }





