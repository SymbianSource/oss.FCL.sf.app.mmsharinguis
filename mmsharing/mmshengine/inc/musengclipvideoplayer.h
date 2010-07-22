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
* Description: 
*
*/


#ifndef MUSHENGCLIPVIDEOPLAYER_H
#define MUSHENGCLIPVIDEOPLAYER_H

// INCLUDES
#include "musenglocalvideoplayer.h"
#include "musunittesting.h"
#include <lcsourcefilecontrol.h>

// CLASS DECLARATION
class CMusEngClipVideoPlayer : 
    public CMusEngLocalVideoPlayer,
    public MLcSourceFileControl
    {
    MUS_UNITTEST( UT_CMusEngClipVideoPlayer )
    
    public: // Constructors and destructor
        
        static CMusEngClipVideoPlayer* NewL( 
            MMusEngDisplayHandler& aDisplayHandler,
            MLcAudioControl& aLcAudioControl );
        
        ~CMusEngClipVideoPlayer();
        
    public: // New functions
        
        TBool HasClipEnded();
        
        void SetBufferingPeriod( const TTimeIntervalMicroSeconds& aPeriod );
        
    public: // From MLcVideoPlayer
        
        TBool LcIsPlayingL();
        
        void LcPlayL();
        
        void LcPauseL();
        
        MLcSourceFileControl* LcSourceFileControl();
        
    public: // From MLcSourceFileControl      
        
        void SetLcFileNameL( const TFileName& aFileName );
        
        TFileName& LcFileName();

        void LcFastForwardL( TBool aUseFFWD );

        void LcFastRewindL( TBool aUseFRWD );
        
        TTimeIntervalSeconds LcFileDurationL();

        TTimeIntervalSeconds LcFilePositionL();

        void SetLcFilePositionL( const TTimeIntervalSeconds& aPosition );        
        
    private: // Constructors
        
        CMusEngClipVideoPlayer(
            MMusEngDisplayHandler& aDisplayHandler,
            MLcAudioControl& aLcAudioControl );       
        
    private: // Helper functions
        
        TBool IsProtectedFileL( const TDesC& aClipFile );
        
        TTimeIntervalMicroSeconds PositionMicroSecondsL( 
            TBool aActualPosition = EFalse );
        
        TTimeIntervalMicroSeconds GetVideoSinkRelativeFilePos( 
            const TTimeIntervalMicroSeconds& aActualPosition,
            const TTimeIntervalMicroSeconds& aDuration );        
        
    private: // Data
            
        TFileName iFileName;
        TTime iFFWDStartTime;
        TTime iFRWDStartTime;
        TTimeIntervalMicroSeconds iBufferingPeriod;
        TTimeIntervalMicroSeconds iDelayFileEndingPos;
        TTimeIntervalMicroSeconds iPreviousPos;
        TBool iRewindedToBeginning;
    };

#endif // MUSHENGCLIPVIDEOPLAYER_H

// End of file

