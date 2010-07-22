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

#ifndef CLCENGINE_INL
#define CLCENGINE_INL

// INCLUDES
#include <ecom/ecom.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcEngine* CLcEngine::NewL( const TDesC8& aEngineName )
    {
    TEComResolverParams resolverParams;
    resolverParams.SetDataType( aEngineName );      
    return reinterpret_cast< CLcEngine* >(
        REComSession::CreateImplementationL(
            KLcEngineInterfaceUid,
            _FOFF( CLcEngine, iInstanceKey ),
            resolverParams ) );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcEngine::CLcEngine()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcEngine::~CLcEngine()
    {
    REComSession::DestroyedImplementation( iInstanceKey );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
TBool CLcEngine::IsFeatureSupported( TLcFeature /*aLcFeature*/ )
    {
    return EFalse;
    }

#endif // CLCENGINE_INL

// End of File
