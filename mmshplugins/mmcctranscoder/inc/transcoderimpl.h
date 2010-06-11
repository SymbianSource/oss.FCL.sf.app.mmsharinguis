/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MCC_TRANSCODER_IMPL_H
#define MCC_TRANSCODER_IMPL_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <vedmovie.h>

#include "mmccevents.h"
#include "mmcctranscoder.h"
#include "transcodersessioninfo.h"

// CLASS DECLARATION
class MMccEventHandler;


/**
* class CMccTranscoderImpl
*/ 
class CMccTranscoderImpl : public CBase,
                           public MVedMovieObserver,
                           public MVedMovieProcessingObserver
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static   CMccTranscoderImpl* NewL();

        /**
        * Destructor.
        */
        virtual  ~CMccTranscoderImpl();

    public:  // New functions

       void RegisterEventObserver( MMccTranscoderObserver& aObserver );
       
       void UnregisterEventObserver();

        void TranscodeFileL( TUint32& aSessionId, const TDesC8& aVal );
       /**
        * Cancels transcoding of media file.
        * @param aSessionId session identifier for transcoding
        * @return KErrNone if successful
        */
        void CancelTranscodeFileL( TUint32 aSessionId );

    public: // MVedMovieObserver functions
    
        void NotifyVideoClipAdded(CVedMovie& aMovie, TInt aIndex);
        
        void NotifyVideoClipAddingFailed(CVedMovie& aMovie, TInt aError);
        
        
        void NotifyVideoClipRemoved(CVedMovie& aMovie, TInt aIndex);
       
        void NotifyMovieQualityChanged(CVedMovie& aMovie);
        
        void NotifyMovieOutputParametersChanged(CVedMovie& aMovie);

        
        inline void NotifyVideoClipIndicesChanged(
            CVedMovie& aMovie, 
            TInt aOldIndex, 
            TInt aNewIndex);
        
                                           
        inline void NotifyVideoClipTimingsChanged(
            CVedMovie& aMovie,
            TInt       aIndex);
                                               
        inline void NotifyVideoClipColorEffectChanged(
            CVedMovie& aMovie,
            TInt       aIndex);
                                                      

        inline void NotifyVideoClipAudioSettingsChanged(
            CVedMovie& aMovie,
            TInt       aIndex);
                                                        
                                                 
        inline void NotifyVideoClipGeneratorSettingsChanged(
            CVedMovie& aMovie,
            TInt       aIndex);
                                                     
        inline void NotifyVideoClipDescriptiveNameChanged(
            CVedMovie& aMovie,
            TInt       aIndex);

        inline void NotifyStartTransitionEffectChanged(
            CVedMovie& aMovie);
        
        inline void NotifyMiddleTransitionEffectChanged(
            CVedMovie& aMovie, 
            TInt       aIndex);
                                                 
        inline void NotifyEndTransitionEffectChanged(
            CVedMovie& aMovie);
        
        inline void NotifyAudioClipAdded( 
            CVedMovie& aMovie, TInt aIndex );
        
        inline void NotifyAudioClipAddingFailed(
            CVedMovie& aMovie, TInt aError);
        
        inline void NotifyAudioClipRemoved(
            CVedMovie& aMovie, TInt aIndex);
        
        inline void NotifyAudioClipIndicesChanged(
            CVedMovie& aMovie, 
            TInt       aOldIndex, 
            TInt       aNewIndex);
                                                                  
        inline void NotifyAudioClipTimingsChanged(
            CVedMovie& aMovie, TInt aIndex);
                                        
        inline void NotifyMovieReseted(CVedMovie& aMovie);
        
        inline void NotifyAudioClipDynamicLevelMarkInserted(CVedMovie& aMovie, 
                                                         TInt aClipIndex, 
                                                         TInt aMarkIndex);

        inline void NotifyAudioClipDynamicLevelMarkRemoved(CVedMovie& aMovie, 
                                                        TInt aClipIndex, 
                                                        TInt aMarkIndex);

        inline void NotifyVideoClipDynamicLevelMarkInserted(CVedMovie& aMovie, 
                                                         TInt aClipIndex, 
                                                         TInt aMarkIndex);

        inline void NotifyVideoClipDynamicLevelMarkRemoved(CVedMovie& aMovie, 
                                                        TInt aClipIndex, 
                                                        TInt aMarkIndex);    
                                                       
         
    public: // MVedMovieProcessingObserver
    
        void NotifyMovieProcessingStartedL(CVedMovie& aMovie);
        
        void NotifyMovieProcessingProgressed( CVedMovie& aMovie, TInt aPercentage );                                                                                                                                                                                                                                                                                    
        
        void NotifyMovieProcessingCompleted(CVedMovie& aMovie, 
                                            TInt       aError);
        
    protected:  // Functions from base classes
    
    private:
        /**
        * C++ default constructor.
        */
        CMccTranscoderImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aSource    Source of the data for the payload format plugin
        */
        void ConstructL();
        
    private:
    
        /**
        * @param aSessionId find session by ID
        * @return KErrNotFound if can not found, otherwise 
        *         return session index.
        */
        TInt FindSession( TUint32 aSessionId );
        
       /**
        * @param aMovie find session by CVedMovie object
        * @return KErrNotFound if can not found, otherwise 
        *         return session index.
        */
        TInt FindSession( CVedMovie& aMovie );
        
        void RemoveSession( const TInt aIndex );
       
        void SendTranscodeEventToClient( TMccEventType aEventType,
                                         TInt aError,
                                         TUint32 aSessionId,
                                         TUint32 aData = 0 );

        void Cleanup();
        
        void InitiateCleanup();
        
        static TInt AsyncTimerExpired( TAny* aPtr );
        
        void SetOutputParameters( CVedMovie& aMovie );
        
    private: // data
    
        RPointerArray<CMccTranscoderSessionInfo> iSessionArray;
        
        MMccTranscoderObserver* iEventObserver;
        
        CDeltaTimer* iDeltaTimer;
        TCallBack iDeltaTimerCallBack;
		TDeltaTimerEntry iDeltaTimerEntry;
        
    private:    // Friend classes
    
    #ifdef EUNIT_TEST
       friend class UT_CMccTranscoderImpl;
    #endif
    };

#include "transcoderimpl.inl"

#endif // end of MCC_TRANSCODER_IMPL_H
