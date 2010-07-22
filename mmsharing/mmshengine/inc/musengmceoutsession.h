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


#ifndef MUSHENGMCEOUTSESSION_H
#define MUSHENGMCEOUTSESSION_H

// USER
#include "musengmcesession.h"
#include "musunittesting.h"

// FORWARD DECLARATIONS
class MLcUiProvider;
class CMceStreamBundle;

// CLASS DECLARATION
/**
* Class for handling MCE out sessions. Direct inherit classes are
* CMusEngLiveSession and CMusEngClipSession.
*
* @lib musengine.lib
*/
class CMusEngMceOutSession : public CMusEngMceSession
    {
    MUS_UNITTEST( UT_CMusEngOutSession )

    public:

        /**
        * Destructor
        */
        ~CMusEngMceOutSession();

    public: // From MLcSession
        
        const TDesC& RemoteDisplayName();
        
        void EstablishLcSessionL();
        
        void TerminateLcSessionL();        
        
    protected: // Must be implemented in derived class

        /**
        * @param aLocalBundle All local streams are supposed to be added to
        *        this bundle
        */
        virtual void CompleteSessionStructureL( 
                                        CMceStreamBundle& aLocalBundle ) = 0;
    
        virtual void AddDisplayL( CMceMediaStream& aStream );
    
    protected: // May be overridden in derived classes
         
        /**
        * Calls EstablishL-function of class CMceOutSession with feature tag 
        * in Accept-Contact header. Functions also adjusts stream and codec
        * settings by calling CMusEngMceSession::AdjustStreamsAndCodecsL.
        * Intention of the function is to provide subclasses means to set 
        * preconditions to establishment, e.g. succesfull transcoding.
        */
        virtual void EstablishSessionL();
        
        TBool IsH264Supported() const;

        /**
         *
         */        
        virtual void CreateMceSessionStructureL( TBool aForceSdpBandwidth =  
                                                  EFalse );

         
    protected: // helper function inherited from CMusEngMceSession

        /**
        * Function handling MCE session termination.
        */
        void HandleTermination( TInt aStatusCode, const TDesC8& aReasonPhrase );

        /**
        * Sets Multimediasharing specific video codec settings like video 
        * payload types. This functions gets called for every video codec in 
        * session. This function overrides function in base class and may 
        * be further overridden. Function calls also overridden version.
        */ 
        void AdjustVideoCodecL( CMceVideoCodec& aVideoCodec, 
                                TMceSourceType aSourceType );
        
        /**
        * Sets Multimediasharing specific audio codec settings like audio and
        * video payload types. This functions gets called for every audio 
        * codec in session.  This function overrides function in base class and
        * may be further overridden. Function calls also overridden version.
        */ 
        void AdjustAudioCodecL( CMceAudioCodec& aAudioCodec );
        

    protected:

        /**
        * Constructor
        */
        CMusEngMceOutSession();

        /**
        * Second-phase constructor
        */
        void ConstructL();

        
    private: // from MMusSipProfileUser, overrides base class definition
            
        virtual void ProfileRegistered();
 
        
     private:
        
        void DoInviteL( const TDesC& aRecipient = KNullDesC );
       
        HBufC* ResolveRecipientLC();  
        
        static TInt AsyncBrakeCompleted( TAny* aPtr );
        
        static TInt RegistrationTimerExpired( TAny* aPtr );

        static TInt InvitationResponseTimerExpired( TAny* aPtr );
        
        void InitRecipientNotFoundHandling();

        void HandleRecipientNotFound();
        
        TBool DoSyncRetryL();

        void SplitL( const TDesC& aDes, const TDesC& aChar, CDesCArray* aArray );
        
        HBufC* ReadDescPropertyL( TUint aKey );

        TBool IgnoreErrorNote();
        
        HBufC* RemoteAddressL() const;
        
        void InputRecipientL( TDes& aRecipientAddress );
    
    protected: // DATA
    
        HBufC* iRemoteDisplayName;
        HBufC8* iRecipient;      
        HBufC8* iVideoCodecList;
        TInt iTriedInvitations;
        HBufC* iRemoteSipAddressProposal;
        CDeltaTimer* iDeltaTimer;
        TCallBack iAsyncBrakeCallBack;
        TDeltaTimerEntry iAsyncBrakeEntry;
        TCallBack iRegistrationCallBack;
        TCallBack iInvitationResponseCallBack;
        TDeltaTimerEntry iRegistrationEntry;
        TDeltaTimerEntry iInvitationResponseEntry;
        TBool iRegistrationPending;
        TBool iAddressQueried;
        
    private:
    
        MUS_UNITTEST ( UT_CMusEngOutSession )
    };
#endif //MUSHENGMCESESSION_H
