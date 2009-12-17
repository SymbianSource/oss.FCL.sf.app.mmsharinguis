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



#ifndef MUSUIACTIVITYMANGER_H
#define MUSUIACTIVITYMANGER_H

#include <e32base.h>


class CMusUiActivityManager : public CActive
    {

public:

    static CMusUiActivityManager* NewL( TInt aTimeout = 60 );

    ~CMusUiActivityManager();


protected:

    CMusUiActivityManager( TInt aTimeout );

    void ConstructL();


public:     // new functions:

    void SetTimeout(TInt aTimeout);

    void Start();

    void Reset();

 
protected: // from CActive

    void DoCancel();

    void RunL();


protected:

    RTimer iTimer;

    TInt iTimeout;

};

#endif // MUSUIACTIVITYMANGER_H

// end of file
