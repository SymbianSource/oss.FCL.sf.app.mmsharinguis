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
* Description:  Monitor that observes the status of the current CS call
*
*/



#ifndef UT_MUSPTTCALLMONITOR_H
#define UT_MUSPTTCALLMONITOR_H

#include <digia/eunit/ceunittestsuiteclass.h>
#include "mmusptteventobserver.h"

class CMusPttCallMonitor;

/**
 * Monitor, which observes status of Kodiak PTT Call *
 * @lib ut_mushaoplugin.dll
 */
NONSHARABLE_CLASS( UT_CMusPTTCallMonitor )
    : public CEUnitTestSuiteClass,MMusPttEventObserver
    {

public: // constructors and destructor
     /**
     * Two-phased constructor. Leaves on failure.
     * @return The constructed CMusAoCallMonitorTest object.
     */
    static UT_CMusPTTCallMonitor* NewLC();

    /**
     * Destructor.
     */
    ~UT_CMusPTTCallMonitor();

private:

    /**
     * C++ constructor.
     */
    UT_CMusPTTCallMonitor();

    /**
     * Symbian 2nd-phase constructor.
     */
    void ConstructL();

    void PttCallEventOccuredL(TPSCTsyKodiakPtt& /*aVal*/)
        {
        
        };

public: 

    void UT_RunLL();
    
    void UT_IsPTTCallExistL();    

    void UT_DoCancelL();

private:

    void SetupL();
    
    void Teardown();      

private: // data

    CMusPttCallMonitor* iMonitor;
    
    EUNIT_DECLARE_TEST_TABLE; 
    };

#endif // UT_MUSPTTCALLMONITOR_H
