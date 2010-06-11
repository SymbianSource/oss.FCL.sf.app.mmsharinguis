/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __VEDMOVIE_H__
#define __VEDMOVIE_H__

#include <e32base.h>
#include <f32file.h>
#include "VedVideoClipInfo.h"
#include "VedAudioClipInfo.h"

/*
 *  Constant definitions.
 */

const TInt KVedClipIndexAll(-1);




class CVedMovie;
class TVedDynamicLevelMark;

/**
 * Observer for movie events. 
 * <p>
 * Note that every change operation that is made to a movie or the clips it consists of 
 * results in a maximum of one notification method called (that is, more than one 
 * notification method is never called as a result of a single change). For example,
 * changing the index of a clip results in the <code>NotifyVideoClipIndicesChanged()</code>
 * method being called once. The <code>NotifyVideoClipTimingsChanged()</code> method is not 
 * called even if the timings of several clips may have changed as a result. See the
 * descriptions of the notification methods for more detailed information.
 *
 * @see  CVedMovie
 */
class MVedMovieObserver 
    {
public:

    /**
     * Called to notify that a new video clip has been successfully
     * added to the movie. Note that the indices and the start and end times
     * of the video clips after the new clip have also changed as a result.
     * Note that the transitions may also have changed. 
     *
     * @param aMovie  movie
     * @param aIndex  index of video clip in movie
     */
    virtual void NotifyVideoClipAdded(CVedMovie& aMovie, TInt aIndex) = 0;

    /**
     * Called to notify that adding a new video clip to the movie has failed.
     *
     * Possible error codes:
     *  - <code>KErrNotFound</code> if there is no file with the specified name
     *    in the specified directory (but the directory exists)
     *  - <code>KErrPathNotFound</code> if the specified directory
     *    does not exist
     *  - <code>KErrUnknown</code> if the specified file is of unknown format
     *  - <code>KErrNotSupported</code> if the format of the file is recognized but
     *    adding it to the movie is not supported (e.g., it is of different resolution
     *    or format than the other clips)
     *
     * @param aMovie  movie
     * @param aError  one of the system wide error codes
     */
    virtual void NotifyVideoClipAddingFailed(CVedMovie& aMovie, TInt aError) = 0;

    /**
     * Called to notify that a video clip has been removed from the movie.
     * Note that the indices and the start and end times of the video clips after 
     * the removed clip have also changed as a result. Note that the 
     * transitions may also have changed.
     *
     * @param aMovie  movie
     * @param aIndex  index of the removed video clip
     */
    virtual void NotifyVideoClipRemoved(CVedMovie& aMovie, TInt aIndex) = 0;
    
    /**
     * Called to notify that a video clip has moved (that is, its index and 
     * start and end times have changed). Note that the indices and the start and
     * end times of the clips between the old and new indices have also changed 
     * as a result. Note that the transitions may also have changed.
     *
     * @param aMovie     movie
     * @param aOldIndex  old index of the moved clip
     * @param aNewIndex  new index of the moved clip
     */
    virtual void NotifyVideoClipIndicesChanged(CVedMovie& aMovie, TInt aOldIndex, 
                                               TInt aNewIndex) = 0;

    /**
     * Called to notify that the timings (that is, the cut in or cut out time or
     * the speed and consequently the end time, edited duration, and possibly audio
     * settings) of a video clip have changed (but the index of the clip has 
     * <em>not</em> changed). Note that the start and end times of the video clips 
     * after the changed clip have also changed.
     *
     * @param aMovie  movie
     * @param aClip   changed video clip
     */
    virtual void NotifyVideoClipTimingsChanged(CVedMovie& aMovie,
                                               TInt aIndex) = 0;

    /**
     * Called to notify that the color effect or a color tone of the existing effect
     * of a video clip has changed.
     *
     * @param aMovie  movie
     * @param aClip   changed video clip
     */
    virtual void NotifyVideoClipColorEffectChanged(CVedMovie& aMovie,
                                                   TInt aIndex) = 0;
    
    /**
     * Called to notify that the audio settings of a video clip have changed. 
     *
     * @param aMovie  movie
     * @param aClip   changed video clip
     */
    virtual void NotifyVideoClipAudioSettingsChanged(CVedMovie& aMovie,
                                                     TInt aIndex) = 0;

    /**
     * Called to notify that some generator-specific settings of 
     * a generated video clip have changed.
     *
     * @param aMovie  movie
     * @param aClip   changed video clip
     */
    virtual void NotifyVideoClipGeneratorSettingsChanged(CVedMovie& aMovie,
                                                         TInt aIndex) = 0;

    /**
     * Called to notify that the descriptive name of a clip has changed. 
     *
     * @param aMovie  movie
     * @param aIndex  changed video clip index
     */
    virtual void NotifyVideoClipDescriptiveNameChanged(CVedMovie& aMovie,
                                                                TInt aIndex) = 0;

    /**
     * Called to notify that the start transition effect of the movie
     * has changed (but no other changes have occurred).
     *
     * @param aMovie  movie
     */
    virtual void NotifyStartTransitionEffectChanged(CVedMovie& aMovie) = 0;

    /**
     * Called to notify that a middle transition effect has changed 
     * (but no other changes have occurred).
     *
     * @param aMovie  movie
     * @param aIndex  index of the changed middle transition effect
     */
    virtual void NotifyMiddleTransitionEffectChanged(CVedMovie& aMovie, 
                                                     TInt aIndex) = 0;

    /**
     * Called to notify that the end transition effect of the movie
     * has changed (but no other changes have occurred).
     *
     * @param aMovie  movie
     */
    virtual void NotifyEndTransitionEffectChanged(CVedMovie& aMovie) = 0;

    /**
     * Called to notify that a new audio clip has been successfully
     * added to the movie. Note that the indices of the audio clips
     * starting after the new clip have also changed as a result.
     *
     * @param aMovie  movie
     * @param aClip   new audio clip
     */
    virtual void NotifyAudioClipAdded(CVedMovie& aMovie, TInt aIndex) = 0;

    /**
     * Called to notify that adding a new audio clip to the movie has failed.
     *
     * Possible error codes:
     *  - <code>KErrNotFound</code> if there is no file with the specified name
     *    in the specified directory (but the directory exists)
     *  - <code>KErrPathNotFound</code> if the specified directory
     *    does not exist
     *  - <code>KErrUnknown</code> if the specified file is of unknown format
     *
     * @param aMovie  movie
     * @param aError  one of the system wide error codes
     */
    virtual void NotifyAudioClipAddingFailed(CVedMovie& aMovie, TInt aError) = 0;

    /**
     * Called to notify that an audio clip has been removed from the movie.
     * Note that the indices of the audio clips starting after the removed
     * clip have also changed as a result.
     *
     * @param aMovie  movie
     * @param aIndex  index of the removed audio clip
     */
    virtual void NotifyAudioClipRemoved(CVedMovie& aMovie, TInt aIndex) = 0;

    /**
     * Called to notify that an audio clip has moved (that is, its
     * index has changed). This may happen when the start time of the audio 
     * clip is changed. Note that the indices of the clips between the old and 
     * new indices have also changed as a result.
     *
     * @param aMovie     movie
     * @param aOldIndex  old index of the moved clip
     * @param aNewIndex  new index of the moved clip
     */
    virtual void NotifyAudioClipIndicesChanged(CVedMovie& aMovie, TInt aOldIndex, 
                                               TInt aNewIndex) = 0;

    /**
     * Called to notify that the timings (for example, the start time or
     * the duration) of an audio clip have changed (but the index of the
     * clip has <em>not</em> changed as a result).
     *
     * @param aMovie  movie
     * @param aClip   changed audio clip
     */
    virtual void NotifyAudioClipTimingsChanged(CVedMovie& aMovie,
                                               TInt aIndex) = 0;

    /**
     * Called to notify that the quality setting of the movie has been
     * changed.
     *
     * @param aMovie  movie
     */
    virtual void NotifyMovieQualityChanged(CVedMovie& aMovie) = 0;

    /**
     * Called to notify that the movie has been reseted.
     *
     * @param aMovie  movie
     */
    virtual void NotifyMovieReseted(CVedMovie& aMovie) = 0;
    
    /**
     * Called to notify that the output parameters have been changed
     *
     * @param aMovie  movie
     */
    virtual void NotifyMovieOutputParametersChanged(CVedMovie& aMovie) = 0;
    
    /**
     * Called to notify that a dynamic level mark has been inserted 
     * to an audio clip.
     *
     * @param aMovie       movie
     * @param aClipIndex   audio clip index
     * @param aMarkIndex   index of the inserted level mark
     */
    virtual void NotifyAudioClipDynamicLevelMarkInserted(CVedMovie& aMovie, 
                                                         TInt aClipIndex, 
                                                         TInt aMarkIndex) = 0;

    /**
     * Called to notify that a dynamic level mark has been inserted 
     * to an audio clip.
     *
     * @param aMovie       movie
     * @param aClipIndex   audio clip index
     * @param aMarkIndex   index of the inserted level mark
     */
    virtual void NotifyAudioClipDynamicLevelMarkRemoved(CVedMovie& aMovie, 
                                                        TInt aClipIndex, 
                                                        TInt aMarkIndex) = 0;

    /**
     * Called to notify that a dynamic level mark has been inserted 
     * to an audio clip.
     *
     * @param aMovie       movie
     * @param aClipIndex   audio clip index
     * @param aMarkIndex   index of the inserted level mark
     */
    virtual void NotifyVideoClipDynamicLevelMarkInserted(CVedMovie& aMovie, 
                                                         TInt aClipIndex, 
                                                         TInt aMarkIndex) = 0;

    /**
     * Called to notify that a dynamic level mark has been inserted 
     * to an audio clip.
     *
     * @param aMovie       movie
     * @param aClipIndex   audio clip index
     * @param aMarkIndex   index of the inserted level mark
     */
    virtual void NotifyVideoClipDynamicLevelMarkRemoved(CVedMovie& aMovie, 
                                                        TInt aClipIndex, 
                                                        TInt aMarkIndex) = 0;    
    };


/**
 * Observer for movie processing operations. 
 *
 * 
 * @see  CVedMovie
 */
class MVedMovieProcessingObserver
    {
public:
    /**
     * Called to notify that a new movie processing operation has been started. 
     *
     * @param aMovie  movie
     */
    virtual void NotifyMovieProcessingStartedL(CVedMovie& aMovie) = 0;

    /**
     * Called to inform about the current progress of the movie processing operation.
     *
     * @param aMovie       movie
     * @param aPercentage  percentage of the operation completed, must be 
     *                     in range 0..100
     */
    virtual void NotifyMovieProcessingProgressed(CVedMovie& aMovie, TInt aPercentage) = 0;

    /**
     * Called to notify that the movie processing operation has been completed. 
     * 
     * @param aMovie  movie
     * @param aError  error code why the operation was completed. 
     *                <code>KErrNone</code> if the operation was completed 
     *                successfully.
     */
    virtual void NotifyMovieProcessingCompleted(CVedMovie& aMovie, TInt aError) = 0;
    };


/**
 * Video movie, which consists of zero or more video clips and zero or more audio clips.
 * 
 * @see  CVedVideoClip
 * @see  CVedAudioClip
 */
class CVedMovie : public CBase
    {
public:

    /**
     * Enumeration for movie quality settings.
     */
    enum TVedMovieQuality
        {
        EQualityAutomatic = 0,
        EQualityMMSInteroperability,
        EQualityResolutionCIF,      // Obsolete, please use Medium/High instead
        EQualityResolutionQCIF,     // Obsolete, please use Medium/High instead
        EQualityResolutionMedium,
        EQualityResolutionHigh,        
        EQualityLast  // this should always be the last
        };


public:

    /* Constructors & destructor. */

    /**
     * Constructs a new empty CVedMovie object. May leave if no resources are available.
     * 
     * Possible leave codes:
     *  - <code>KErrNoMemory</code> if memory allocation fails
     *
     * @param aFs  file server session to use to lock the video and audio
     *             clip files of the new movie; or NULL to not to lock the files
     *
     * @return  pointer to a new CVedMovie instance
     */
    static CVedMovie* NewL(RFs* aFs);

    /**
     * Constructs a new empty CVedMovie object and leaves the object in the cleanup stack.
     * 
     * Possible leave codes:
     *  - <code>KErrNoMemory</code> if memory allocation fails
     * 
     * @param aFs  file server session to use to lock the video and audio
     *             clip files of the new movie; or NULL to not to lock the files
     *
     * @return  pointer to a new CVedMovie instance
     */
    static CVedMovie* NewLC(RFs* aFs);
    
    
    ~CVedMovie();


    /**
     * Sets the quality setting of this movie.
     *
     * @param aQuality  quality setting
     */
    virtual void SetQuality(TVedMovieQuality aQuality);


    /**
     * Returns the video type of this movie. 
     * 
     * @return  video type of this movie
     */
    virtual TVedVideoType VideoType() const;

    /**
     * Returns the resolution of this movie. Panics with 
     * <code>TVedPanic::EMovieEmpty</code> if there are no clips 
     * in the movie.
     * 
     * @return  resolution of the movie
     */
    virtual TSize Resolution() const;

    /**
     * Returns the audio type of the movie audio track.
     * 
     * @return  audio type of the movie audio track
     */
    virtual TVedAudioType AudioType() const;

    /**
     * Returns the audio sampling rate of the movie audio track.
     *
     * @return  audio sampling rate of the movie audio track.
     */
    virtual TInt AudioSamplingRate() const;

    /**
     * Returns the audio channel mode of the movie audio track.
     * 
     * @return  audio channel mode of the movie audio track.
     */
    virtual TVedAudioChannelMode AudioChannelMode() const;


    /** 
     * Inserts a video clip from the specified file to the specified index 
     * in this movie. The observers are notified when the clip has been added 
     * or adding clip has failed. Panics with <code>EMovieAddOperationAlreadyRunning</code> 
     * if another add video or audio clip operation is already running.
     * Panics with code <code>USER-130</code> if the clip index is invalid.
     * The file will be opened in EFileShareReadersOnly mode by default, 
     * and the same mode should be used by the client too if it need to open
     * the file at the same time.
     *  
     * Possible leave codes:
     *  - <code>KErrNoMemory</code> if memory allocation fails
     *
     * @param aFileName  file name of the clip to add
     * @param aIndex     index the clip should be inserted at
     */
    virtual void InsertVideoClipL(const TDesC& aFileName, TInt aIndex);



    /* Processing methods. */

    /**
     * Starts a video processing operation. This method is asynchronous and 
     * returns immediately. The processing will happen in the background and
     * the observer will be notified about the progress of the operation.
     * Processed data is written into the specified file. Panics with 
     * <code>TVedPanic::EMovieEmpty</code> if there are no clips 
     * in the movie. Note that calling <code>ProcessL</code> may cause
     * changes in the maximum frame rates of generated clips.
     * 
     * Possible leave codes:
     *  - <code>KErrNoMemory</code> if memory allocation fails
     *  - <code>KErrAccessDenied</code> if the file access is denied
     *  - <code>KErrDiskFull</code> if the disk is full
     *  - <code>KErrWrite</code> if not all data could be written
     *  - <code>KErrBadName</code> if the filename is bad
     *  - <code>KErrDirFull</code> if the directory is full
     * 
     * @param aObserver  observer to be notified of the processing status
     * @param aFileName  name of the file to be written
     */
    virtual void ProcessL(const TDesC& aFileName,
                           MVedMovieProcessingObserver& aObserver);

    /**
     * Cancels the current video processing operation. If there is no 
     * operation in progress, the function does nothing.
     */
    virtual void CancelProcessing();


    /* Observer methods. */

    /**
     * Registers a movie observer. Panics with panic code 
     * <code>EMovieObserverAlreadyRegistered</code> if the movie observer is 
     * already registered.
     *
     * @param aObserver  observer that will receive the events
     */
    virtual void RegisterMovieObserverL(MVedMovieObserver* aObserver);

    /**
     * Unregisters a movie observer.
     *
     * @param aObserver  observer to be unregistered
     */
    virtual void UnregisterMovieObserver(MVedMovieObserver* aObserver);


    /**
     * Returns the target bitrate of the movie audio track.
     * 
     * @return  target bitrate of the movie audio track.
     */
    virtual TInt AudioBitrate() const;

    /**
     * Returns the target framerate of the movie video track.
     * 
     * @return  target framerate of the movie video track.
     */
    virtual TReal VideoFrameRate() const;

    /**
     * Sets the output parameters for the movie. Leaves
     * with KErrNotSupported if a parameter is illegal,
     * e.g., target bitrate is too high for the given 
     * codec. Setting a integer parameter to zero indicates
     * that a default value will be used for that parameter.
     *
     * This method overrides the SetQuality method
     *
     * Possible leave codes:
     *  - <code>KErrNotSupported</code> if setting is not valid
     *
     * @param Output parameters
     */
    
    virtual void SetOutputParametersL(TVedOutputParameters& aOutputParams);

public: // Stub stuff

    void ConstructL();
    
    CVedMovie();

    enum TVedMovieCurrentCallbackType
        {
        ENotifyVideoClipAdded,
        ENotifyMovieQualityChanged,
        ENotifyMovieOutputParametersChanged,
        ENotifyMovieProcessingStartedL,
        ENotifyMovieProcessingProgressed,
        ENotifyMovieProcessingCompleted
        };
        
    void IssueCallback( TVedMovieCurrentCallbackType aCallback );
		
	static TInt AsyncTimerExpired(TAny* aPtr);
	
	void DoCurrentCallbackL();

    MVedMovieObserver* iObserver;
    MVedMovieProcessingObserver* iProcessObserver;
    
    CDeltaTimer* 				iDeltaTimer;
    TCallBack 					iDeltaTimerCallBack;
	TDeltaTimerEntry 			iDeltaTimerEntry;
	
	TVedMovieCurrentCallbackType iCurrentCallback;
	TInt iProgress;
	
	TVedMovieQuality iQuality;
	TVedOutputParameters iOutputParams;
	TVedAudioType iAudioType;
    
    };


#endif // __VEDMOVIE_H__

