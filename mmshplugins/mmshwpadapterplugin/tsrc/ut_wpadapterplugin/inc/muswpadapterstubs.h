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


#ifndef __MUSWPADAPTERSTUBS_H__
#define __MUSWPADAPTERSTUBS_H__

#include <mwppairvisitor.h>
#include <e32def.h>
#include <e32cmn.h>

class TPairVisitorStub : public MWPPairVisitor
	{
public:

	void VisitPairL(const TDesC16& /*aName*/, const TDesC16& /*aValue*/);
    
    };




_LIT( KMusSummaryTitle, "Video sharing" );

/**
 * Stub for a class WPAdapterUtil which is located in library ProvisioningEngine
 * Stub does not contain all the functions present in original version.
 */ 
class WPAdapterUtil
    {
    public: // New functions
 
        /**
        * Stubbed version returns a const value despite of params
        */
        static HBufC* ReadHBufCL( const TDesC& aDllPath, 
            const TDesC& aAdapterName, TInt aResourceId );

    };



#endif // __MUSWPADAPTERSTUBS_H__
