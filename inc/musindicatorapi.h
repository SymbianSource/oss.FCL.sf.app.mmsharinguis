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
* Description:  Provides means for confirmation query and availability
                 indications.
*
*/



#ifndef C_MUSINDICATORAPI_H
#define C_MUSINDICATORAPI_H


#include "musindicatorobserver.h"
#include "musunittesting.h"
#include "mussoundplayerobserver.h"
#include <e32base.h>
#include <e32property.h>



class CAknGlobalMsgQuery;
class CMusSoundPlayer;


/**
 *  MusIndicatorApi provides means to indicate availability
 *  of videosharing to user.
 *  @lib musindicator.lib
 */
class CMusIndicatorApi : public CActive, MMusSoundPlayerObserver
    {

public:

    /**  Different notes */
    enum TVsPopupQuery
        {
        EVsRoamingActivationQuery /**
                                   * Displays a query prompting for
                                   * activation of VS when roaming.
                                   */
        };

    /**
     * Two-phased constructor.
     *
     * @param aObserver Reference to observer interface.
     * return Pointer to newly created instance of CMusIndicatorApi.
     */
    IMPORT_C static CMusIndicatorApi* NewL(
        MMusIndicatorObserver& aObserver );

    /**
     * Two-phased constructor.
     *
     * @param aObserver Reference to observer interface.
     * return Pointer to newly created instance of CMusIndicatorApi.
     */
    IMPORT_C static CMusIndicatorApi* NewLC(
        MMusIndicatorObserver& aObserver );
        

    /**
     * Two-phased constructor.
     *
     * return Pointer to newly created instance of CMusIndicatorApi.
     */
    IMPORT_C static CMusIndicatorApi* NewL();

    /**
     * Destructor.
     */
    ~CMusIndicatorApi();

    /**
     * Indicates availability of video sharing.
     */
    IMPORT_C void IndicateAvailabilityL();

protected:

// from base class CActive.

    /**
     * From CActive.
     * Handles completion of an outstanding asynchronous request.
     */
    virtual void RunL();

    /**
     * From CActive.
     * Cancels an outstanding asynchronous request.
     */
    virtual void DoCancel();

    /**
     * From CActive.
     * Handles a leave occurring in the request completion event handler RunL.
     *
     * @param aError Error code of occured leave.
     */
    virtual TInt RunError( TInt aError );

// from base class MMusSoundPlayerObserver.

    /**
     * From MMusSoundPlayerObserver.
     * Notifies of sound playing complete.
     *
     * @param aError Errorcode associated with playback.
     */
    virtual void PlaySoundComplete( TInt aError );

private:

    CMusIndicatorApi( MMusIndicatorObserver* aObserver );

    void ConstructL();

	 /**
     * Indicating availability of VS.
     *
     * @param aShown ETrue set indicator on and EFalse to Indicator off.
     */
	void Indicator( TBool aShown );
	
    /**
     * Plays indication tone.
     */
    void PlayToneL();

    void StartLiveSharingL();
    
        
private: // data


    /**
     * Sound player.
     * Own.
     */
    CMusSoundPlayer* iSoundPlayer;

    /**
     * Reference to observer interface.
     * Own.
     */
    MMusIndicatorObserver* iObserver;
    
    RProperty iProperty;
            
    MUS_UNITTEST( UT_CMusIndicatorApi )

    };

#endif // C_MUSINDICATORAPI_H
