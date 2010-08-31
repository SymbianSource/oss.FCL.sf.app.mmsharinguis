/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The observer class for monitoring SIP profiles.
*
*/


#ifndef MMUSTESTDRAWER_H
#define MMUSTESTDRAWER_H

#include <e32base.h>
#include <w32std.h>

class CMusTestDrawer : public CBase,MDirectScreenAccess
    {
public:
    CMusTestDrawer* NewL();
    CMusTestDrawer* NewLC();    
    ~CMusTestDrawer();
    void DrawL( const TRect& aRect);
    virtual void Restart(RDirectScreenAccess::TTerminationReasons aReason);
private:
    CMusTestDrawer();
    void ConstructL();
    
    };

#endif // MMUSTESTDRAWER_H
