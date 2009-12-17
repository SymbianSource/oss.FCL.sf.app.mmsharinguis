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
* Description:  project specification
*
*/



#include "muswpadapterstubs.h"

// -------- TPairVisitor -------------------------------------------------------

void TPairVisitorStub::VisitPairL( const TDesC16& /*aName*/, 
                                   const TDesC16& /*aValue*/) 
    {
    }



// -------- WPAdapterUtil ------------------------------------------------------

HBufC* WPAdapterUtil::ReadHBufCL( const TDesC& /*aDllPath*/, 
                                         const TDesC& /*aAdapterName*/, 
                                         TInt /*aResourceId*/ )
    {
    return KMusSummaryTitle().AllocL(); 
    }