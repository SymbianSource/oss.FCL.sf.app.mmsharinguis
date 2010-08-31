/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Timer
*
*/

#ifndef CMUSAVACAPABILITYTIMER_H
#define CMUSAVACAPABILITYTIMER_H

#include <e32base.h>

/**
*  Timer class. 
*
*  @lib 
*/
class CMusAvaCapabilityTimer : public CTimer
    {

public: // timer observer
    
    class MTimeOutNotify
        {
        public:
            virtual void OnExpiredL( TInt aStatus ) = 0;
        };


public: // constructors and destructor

    static CMusAvaCapabilityTimer* NewL( MTimeOutNotify& aNotify );
    ~CMusAvaCapabilityTimer();

protected: // From CTimer

    void RunL();

private:

    CMusAvaCapabilityTimer( const TInt aPriority, MTimeOutNotify& aNotify );

    void ConstructL();

private:

    MTimeOutNotify* iNotify;

    };

#endif  // CMUSAVACAPABILITYTIMER_H

// End of File
