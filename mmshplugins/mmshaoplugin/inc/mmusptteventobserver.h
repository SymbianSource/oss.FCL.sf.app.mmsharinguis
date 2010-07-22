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
* Description:  The observer class for monitoring kodiak ptt call.
*
*/


#ifndef MMUSPTTEVENTOBSERVER_H
#define MMUSPTTEVENTOBSERVER_H

#include <ctsydomainpskeys.h>


#ifdef UNIT_TESTING // if unit testing

    const TUint KCTsyKodiakPtt = 1234;
    enum TPSCTsyKodiakPtt
        {
        EPSCTsyKodiakPttActive,
        EPSCTsyKodiakPttNotActive    
        };

#endif

/**
*  @publishedAll
*  @released
*
*  The observer class for monitoring Kodiak Ptt call events.
*/
class MMusPttEventObserver
    {
public: 

    /**
    * An event related to kodiak ptt call
    * @param aVal value of kodiak ptt call state.    
    **/
   #ifdef UNIT_TESTING // if unit testing
   virtual void PttCallEventOccuredL(TPSCTsyKodiakPtt& aVal) = 0; 
   #endif

    };

#endif // MMUSPTTEVENTOBSERVER_H

