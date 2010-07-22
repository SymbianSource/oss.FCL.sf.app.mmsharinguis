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
* Description:  This interface is used by
*                clients to get and availability setting for Video Charging
*
*/


#ifndef MUSSESSIONSIMULATOR_H_
#define MUSSESSIONSIMULATOR_H_

#include <e32base.h>
#include <mcemanager.h>

class MMceSessionObserver;

class MTimerNotify
    {
    public:
        virtual void TimerExpired(TAny* aTimer,TInt aError) = 0;
        };
 
 class CMusTestTimer: public CActive
        {
    public:
        static CMusTestTimer* NewL(const TInt aPriority,MTimerNotify& aNotify);
        ~CMusTestTimer();
    public:
        void At(const TTime& aTime);
        void After(TTimeIntervalMicroSeconds32 aInterval);
        void Inactivity(TTimeIntervalSeconds aSeconds);
    protected:
        void RunL();
        void DoCancel();
    private:
        CMusTestTimer(const TInt aPriority,MTimerNotify& aNotify);
        void ConstructL(void);
    private:
        RTimer                  iTimer;
        MTimerNotify&    iNotify;
        };

class CMusTestSessionSimulator : public MTimerNotify
    {
    
public: 
    static CMusTestSessionSimulator* NewL( CMceManager& aObserver );
    CMusTestSessionSimulator( CMceManager& aObserver );
    ~CMusTestSessionSimulator();
    void StartL();
    void Stop();
    TBool IsRunning();
private:
    void ConstructL();    
    void StateChangeL();
    void TimerExpired(TAny* aTimer,TInt aError);
    void SimulateReceiveSession();
private:
    CMceManager& iManager;
     
private :
    CMusTestTimer* iTimer;
    TBool ibIncoming;
    };

#endif /* MUSSESSIONSIMULATOR_H_ */
