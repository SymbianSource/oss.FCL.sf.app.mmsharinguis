/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit tests for CMusSoundPlayer class.
*
*/



#ifndef UT_CMUSSOUNDPLAYER_H
#define UT_CMUSSOUNDPLAYER_H


#include "mussoundplayer.h"
#include "mussoundplayerobserver.h"
#include <digia/eunit/ceunittestsuiteclass.h>
#include <e32def.h>


#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif


class CMusSoundPlayer;


/**
 *  Unit test class for CMusSoundPlayer class.
 *  Implements unit tests for CMusSoundPlayer class.
 */
NONSHARABLE_CLASS( UT_CMusSoundPlayer )
     : public CEUnitTestSuiteClass, MMusSoundPlayerObserver
    {
public:

    static UT_CMusSoundPlayer* NewL();
    static UT_CMusSoundPlayer* NewLC();

    /**
     * Destructor.
     */
    ~UT_CMusSoundPlayer();

// from base class MMusSoundPlayerObserver

    /**
     * From MMusSoundPlayerObserver.
     * Notifies of sound playing complete.
     *
     * @param aError Errorcode associated with playback.
     */
    virtual void PlaySoundComplete( TInt aError );

private:

    UT_CMusSoundPlayer();

    void ConstructL();

    /**
     * Sets up a new test.
     */
    void SetupL();

    /**
     * Tears down a test.
     */
    void Teardown();

    /**
     * Actual unit test methods.
     */
    void UT_CMusSoundPlayer_NewLL();
    void UT_CMusSoundPlayer_NewLCL();
    void UT_CMusSoundPlayer_PlayLL();
    void UT_CMusSoundPlayer_StopL();
    void UT_CMusSoundPlayer_MapcInitCompleteL();
    void UT_CMusSoundPlayer_MapcPlayCompleteL();
    void UT_CMusSoundPlayer_SoundFileNameLCL();
    
    /*
    * Integration test methods
    */
    void IT_CMusSoundPlayer_DeletePlayerWhenPlaying();
    

private: // data

    /**
     * Tested class.
     * Own.
     */
    CMusSoundPlayer* iSoundPlayer;
    
    TInt iPlaySoundCompleteError;
    
    EUNIT_DECLARE_TEST_TABLE;

    };

#endif // UT_CMUSSOUNDPLAYER_H
