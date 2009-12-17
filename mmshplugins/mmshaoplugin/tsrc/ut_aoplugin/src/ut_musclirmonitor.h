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
* Description:  Monitor that observes the calling line identity restriction
*                setting
*
*/



#ifndef UT_MUSCLIRMONITORTEST_H
#define UT_MUSCLIRMONITORTEST_H

#include <etelmm.h>
#include <digia/eunit/ceunittestsuiteclass.h>


class CMusClirMonitor;
class CMusAoPlugin;


NONSHARABLE_CLASS( UT_CMusClirMonitor )
    : public CEUnitTestSuiteClass
    {

public: // constructors and destructor

    /**
     * Destructor.
     */
    ~UT_CMusClirMonitor();

    /**
     * Two-phased constructor. Leaves on failure.
     * @return The constructed UT_CMusClirMonitor object.
     */
    static UT_CMusClirMonitor* NewLC();

private:

    /**
     * C++ constructor.
     */
    UT_CMusClirMonitor();

    /**
     * Symbian 2nd-phase constructor.
     */
    void ConstructL();

public: 

    void UT_ReadClir();
    
    void UT_RunL();
    
    void UT_RunLErrorsL();

    void UT_DoCancel();

    void UT_RunError();
    
    void UT_TranslateResult();

private:

    void SetupL();
    
    void Teardown();

private: // data

    // Owned
    CMusClirMonitor* iMonitor;

    EUNIT_DECLARE_TEST_TABLE;

    };

#endif // UT_MUSCLIRMONITORTEST_H
