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
class MMusEngOutSessionObserver;
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


    public: // NEW API FUNCTIONS

        /**
        * Sends invite to specified address. Session establishment is done in 
        * pause mode meaning that no RTP will be sent to network as a 
        * consequence of established session. PlayL() can be called any time 
        * during session establishment or after that in order to allow 
        * RTP sending.
        * @leave KErrAlreadyExists if session establishment is already 
        *        in progress
        */
        IMPORT_C void InviteL( const TDesC& aRecipient );

        /**
        * Cancels Invite.
        * @pre Invite is sent
        */
        IMPORT_C void CancelInviteL( );
        
        /**
        * If codecs supported by recipient are known e.g. because of OPTIONS
        * query, they can be communicated to engine via this function
        * @param aVideoCodecs Comma separated list of supported codecs 
        */
        IMPORT_C void SetSupportedVideoCodecListL( const TDesC& aVideoCodecs );


    public: // VIRTUAL API
     
        /**
        * Resumes previously paused session. Exact behavior depends on 
        * exact session type.
        */
        virtual void PlayL() = 0;

        /**
        * Pauses session. Exact behavior depends on exact session type.
        */
        virtual void PauseL() = 0;

        /**
        * Tells whether session is paused or not
        * @pre Call of InviteL has been completed
        */
        virtual TBool IsPlayingL() = 0;


    protected: // Must be implemented in derived class

        /**
        * @param aLocalBundle All local streams are supposed to be added to
        *        this bundle
        */
        virtual void CompleteSessionStructureL( 
                                        CMceStreamBundle& aLocalBundle ) = 0;
    
    
    protected: // May be overridden in derived classes
         
        /**
        * Calls EstablishL-function of class CMceOutSession with feature tag 
        * in Accept-Contact header. Functions also adjusts stream and codec
        * settings by calling CMusEngMceSession::AdjustStreamsAndCodecsL.
        * Intention of the function is to provide subclasses means to set 
        * preconditions to establishment, e.g. succesfull transcoding.
        */
        virtual void EstablishSessionL();
        

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
        void AdjustVideoCodecL( CMceVideoCodec& aVideoCodec );
        
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
        CMusEngMceOutSession( const TRect& aRect,
                              MMusEngSessionObserver& aSessionObserver,
                              MMusEngOutSessionObserver& aOutSessionObserver );

        /**
        * Second-phase constructor
        */
        void ConstructL( TUint aSipProfileId );


    private:

        /**
        *
        */
        void CreateMceSessionStructureL();  
        
        /**
        * Add privacy sip header if call privacy is switched on else otherwise.
        */
        void AddPrivacyHeaderL( CDesC8Array& aHeaders );
        
        
    protected: // DATA

        /**
        * Callback reference to outsession observer interface.
        */
        MMusEngOutSessionObserver& iOutSessionObserver;
        
        /**
        * ETrue if operator specific behavior is expected
        */
        TBool iPrivate; 
        
        /**
        * ETrue if private number is turn on
        */
        TBool iPrivateNumber;
        
    
    protected: // DATA
    
        /**
        * Recipient of session to be constructed
        */
        HBufC8* iRecipient;
        
        HBufC8* iVideoCodecList;
        
        
    private:
    
        MUS_UNITTEST ( UT_CMusEngOutSession )

    };
#endif //MUSHENGMCESESSION_H
