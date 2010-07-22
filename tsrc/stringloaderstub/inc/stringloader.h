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
#ifndef __STRINGLOADER__H_
#define __STRINGLOADER__H_

#include <e32base.h>

#include <bamdesca.h>

class StringLoader
    {
public:
    static HBufC* LoadL( TInt aResourceId, const TDesC& aString );

    static HBufC* LoadL( TInt aResourceId );

    static HBufC* LoadL( TInt aResourceId, const MDesCArray& aStrings );

    static HBufC* LoadLC( TInt aResourceId );

    static HBufC* LoadLC( TInt aResourceId, const TDesC& aString );

    static void Cleanup();

private:
    static HBufC* iReturnValue;
    };

#endif
