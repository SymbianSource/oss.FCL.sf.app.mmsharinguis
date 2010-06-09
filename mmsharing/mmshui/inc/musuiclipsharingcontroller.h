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


#ifndef MUSUICLIPSHARINGCONTROLLER_H
#define MUSUICLIPSHARINGCONTROLLER_H

#include "musuisendcontroller.h"
#include "musengclipsessionobserver.h"
#include "musunittesting.h"
#include "musuiactivetimerobserver.h"

#include <e32base.h>
#include <badesca.h>

class MMusUiClipSharingObserver;
class CMusEngClipSession;
class CMusUiActiveTimer;

/**
 *
 *
 * @lib musui.exe
 */
class CMusUiClipSharingController : public CMusUiSendController,
                                    public MMusEngClipSessionObserver,
                                    public MMusUiActiveTimerObserver
    {

    MUS_UNITTEST( UT_CMusUiClipSharingController )

public:

    static CMusUiClipSharingController* NewL(
                            MMusUiEventObserver& aEventObserver,
                            MMusUiSharingObserver& aSharingObserver,
	                        MMusUiSendObserver& aSendObserver,
                            MMusUiClipSharingObserver& aClipObserver,
                            const TRect& aRect );

    virtual ~CMusUiClipSharingController();


private:

    CMusUiClipSharingController( MMusUiEventObserver& aEventObserver,
                                 MMusUiSharingObserver& aSharingObserver,
	                             MMusUiSendObserver& aSendObserver,
	                             MMusUiClipSharingObserver& aClipObserver );

    void ConstructL( const TRect& aRect );


public: // From CMusUiEventController:

    virtual void OfferToolbarEventL( TInt aCommand );

    virtual void HandleCommandL( TInt aCommand );

    virtual void HandleAsyncEventL( TMusUiAsyncEvent aEventId );

    virtual void DeleteEngineSession();
    

public: // from CMusUiSendController

    virtual CMusEngMceOutSession* EngineOutSession();
    
    /**
    * Handles status changes specific to sending side sharing types.
    * To be overridden in sibling classes for sharing type specific 
    * status changes. Calls base class version if status change is not
    * sending side specific.
    */
    virtual void HandleChangedStatusL( TMusAvailabilityStatus aStatus );

    virtual void InviteL( const TDesC& aRecipient );
    
    virtual void HandleSliderValueChangeL( TInt aNewLevel );
    
 
public: // new functions:    
  
    void SetFileNameL( const TDesC& aFileName );

    TBool ClipEnded();
    
    TBool ClipMutedL();
    
    TBool ClipContainsAudioL();
    
    void CancelTranscodingL();

    void StopWindingL();

    TBool ToolbarFFRevSelected();


private:    // new functions:

    void TranscodingProgressedL( TInt aPercentage );

    void TranscodingCompletedInitL();
    
    void TranscodingCompletedFinalizeL();
    
    const TDesC& FormattedDurationString( 
                            const TTimeIntervalSeconds& aDurationSeconds,
                            const TTimeIntervalSeconds& aPositionSeconds );
    

public: // From MMusEngClipSessionObserver

    virtual void EndOfClip();

    virtual void TranscodingNeeded(TBool aDueUnknownRemoteCapabilities);
        
    virtual void TranscodingProgressed( TInt aPercentage );
    
    virtual void TranscodingCompletedInit();
    
    virtual void TranscodingCompletedFinalize();
    
    virtual void TranscodingFailed();
    
    virtual void RewindFromEndL();
    

public: // from MMusEngOutSessionObserver

    virtual void SessionRecipientNotFound();            // 404


public: // from MMusEngSessionObserver

    virtual void SessionEstablished();

    virtual void SessionTerminated();

    virtual void SessionConnectionLost();

    virtual void SessionFailed();

    virtual void StreamIdle();

    virtual void StreamStreaming();

    virtual void SessionTimeChanged( const TTimeIntervalSeconds& aSeconds );

    virtual void InactivityTimeout();
   
    
public: // from MMusUiActiveTimerObserver

    void TimerComplete( CMusUiActiveTimer* aTimer );


private:    // new functions:

    void StreamStreamingL();
    
    void StreamIdleL();

    void SessionTimeChangedL( const TTimeIntervalSeconds& aSeconds );
    
    TBool DoFastForwardingL( TBool aUseWinding );
    
    TBool DoFastRewindingL( TBool aUseWinding );
    
    TBool DoInviteL();


private:

    MMusUiClipSharingObserver& iClipObserver;

    CMusEngClipSession* iSession;

    HBufC* iFileName;

    TRect iRect;

    TBool iClipEnded;

    TBool iTranscode;
    
    TBool iTranscodeDueUnknownRemoteCapas;
    
    TBool iTranscodingGoing;
    
    TBool iToolbarFFRevSelected;

    TBuf<11> iDurationStringBuffer;

    TBool iFastForward;

    TBool iFastRewind;
    
    TBool iFileFetchingOngoing;
    
    // timer initiating session close on expiring
    CMusUiActiveTimer* iCloseTimer;
    
    TBool iWasPlayingBeforeWinding;
    
    };

#endif // MUSUICLIPSHARINGCONTROLLER_H

// end of file
