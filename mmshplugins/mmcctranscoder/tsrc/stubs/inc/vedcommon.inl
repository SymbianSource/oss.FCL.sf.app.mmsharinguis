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
* Description:
*
*/


#ifndef __VEDCOMMON_INL__
#define __VEDCOMMON_INL__


inline void TVedPanic::Panic(TInt aPanic)
    {
    _LIT(KVedPanicCategory, "VIDEO EDITOR ENG");

    User::Panic(KVedPanicCategory, aPanic);
    }

inline TVedDynamicLevelMark::TVedDynamicLevelMark(TTimeIntervalMicroSeconds aTime, TInt aLevel) 
    {
    iTime = aTime;
    if ( aLevel < -127 )    // level is TInt8 with 0.5 dB steps
        {
        iLevel = -127;
        }
    else if (aLevel > 127 )
        {
        iLevel = 127;
        }
    else
        {
        iLevel = TInt8(aLevel);
        }
    }
    
inline TVedDynamicLevelMark::TVedDynamicLevelMark(const TVedDynamicLevelMark& aMark) 
    {
    iTime = aMark.iTime;
    iLevel = aMark.iLevel;
    }

#endif //__VEDCOMMON_INL__
