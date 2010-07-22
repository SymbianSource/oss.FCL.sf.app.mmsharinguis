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


#ifndef MUSHENGMCESESSION_H
#define MUSHENGMCESESSION_H

// USER
#include "musengsession.h"
#include "lcsession.h"
#include "lcwindow.h"
#include "lcaudiocontrol.h"


// CLASS DECLARATION

/**
* Base class for Multimedia Sharing Engine MCE sessions, inherited by
* CMusEngLiveSession CMusEngClipSession and CMusEngReceiveSession.
*
* CMusEngMceSession encapsulates CMCESession and simplifies use of
* MCE Sessions to user interface. Also splits observers messy events
* to simplified function calls.
*
* @lib musengine.lib
*/
class CMusEngMceSession : public CMusEngSession,
                          public MLcSession,
                          public MLcWindow,
                          public MLcAudioControl
    {
    
    public: 
    
        /**
        * Destructor
        */
        ~CMusEngMceSession();


    public: // from MLcSession
        
        TLcSessionState LcSessionState() const;
    
        virtual void EstablishLcSessionL();
        
        void TerminateLcSessionL();
        
        virtual MLcVideoPlayer* RemoteVideoPlayer();
    
        virtual MLcVideoPlayer* LocalVideoPlayer();       
       
        virtual const TDesC& LocalDisplayName();
    
        virtual const TDesC& RemoteDisplayName();

        TInt SetParameter( TInt aId, TInt aValue );

        TInt ParameterValue( TInt aId );
        
        void UpdateLcSessionL();

        
    public: // from MLcWindow
        
        void EnableLcWindowL( TBool aEnable );
        
        TBool IsLcWindowEnabled();
        
        void SetLcWindowRectL( TRect aRect );
        
        TRect LcWindowRect();
        
        void SetLcWindowOrientationL( 
            TLcWindowOrientation aOrientation );
        
        TLcWindowOrientation LcWindowOrientationL();        
        
    public: // from MLcAudioControl
        
        TBool IsLcAudioMutedL();

        void MuteLcAudioL( TBool aMute );       

        TBool IsLcMicMutedL();    

        void MuteLcMicL( TBool aMute );
        
        TBool IsLcLoudspeakerEnabled();

        void EnableLcLoudspeakerL( TBool aEnabled );
        
        TBool IsEnablingLcLoudspeakerAllowed();
        
        TInt LcVolumeL();
     
        void SetLcVolumeL( TInt aValue );    
    
        void IncreaseLcVolumeL();
        
        void DecreaseLcVolumeL();        
        
    protected: // CONSTRUCTORS

        /**
        * Constructor
        *
        */
        CMusEngMceSession( const TRect& aRect );

        /**
        * Second-phase constructor
        *
        */
        void ConstructL();
        

    private:

        MLcWindow::TLcWindowOrientation iOrientation;
        TBool iMuted;
        TBool iMicMuted;
        TBool iLoudspeakerEnabled;
        TBool iEnablingLoudspeakerAllowed;
        TInt iVolume;
        TRect iRect;
        TBool iWindowEnabled;

    };
    
#endif //MUSHENGMCESESSION_H
