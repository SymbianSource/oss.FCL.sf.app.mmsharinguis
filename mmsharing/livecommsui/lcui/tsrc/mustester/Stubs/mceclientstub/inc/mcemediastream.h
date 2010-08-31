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


#ifndef CMCEMEDIASTREAM_H
#define CMCEMEDIASTREAM_H

// INCLUDES
#include <e32base.h>

// DATA TYPES
typedef TUint TMceMediaType;
 
// FORWARD DECLARATIONS
class CMceComMediaStream;
class TMceFactory;
class TMceMediaStreamFactory;
class RWriteStream;
class RReadStream;
class CDesC8Array;
class CMceSession;
class TMceIds;
class TMceEvent;
class TMceMediaId;
class CMceManager;
class CMceSession;
class CMceMediaSource;
class CMceMediaSink;
class MMceComSerializationContext;
class MDesC8Array;

// CLASS DECLARATION

/**
* Base class for MCE media streams.
*
* MCE Media stream represents stream of data negotiated between two terminals.
* Streams may transfer any data.
*
* Enabling or disabling of a stream is not a local operation, but it requires
* always signalling. Thus, these operation require always to be completed by
* calling CMceSession::UpdateL() function.
*
*  @lib mceclient.lib
*/
class CMceMediaStream : public CBase
	{
    public: // Stream states

        enum TState
            {
            EUninitialized,       // Stream is created
            EInitialized,         // Stream is initialized
            EBuffering,           // Stream is buffering
            EIdle,                // Stream is not receiving RTP
            EStreaming,           // Stream is streaming
            EDisabled,            // Stream is explicitly disabled
            ENoResources,         // Stream has no needed resources to stream
            ETranscodingRequired, // Stream requires non-realtime transcoding
            ETranscoding          // Stream is transcoding in non-realtime
            };
	        
	public: // Destructor

	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceMediaStream();

	public: // Functions

        /**
        * Gets the state of the stream.
        * @return the current state of the stream
        */
        IMPORT_C CMceMediaStream::TState State() const;

	    /**
	    * Returns the type of the stream.
	    * @return The type of the stream.
	    */
		IMPORT_C TMceMediaType Type() const;

	    /**
		* Configures media stream by updating SDP media lines. 
		* In order to get complete update, whole session must be 
		* updated causing sending of re-invite.
	    * @param aMediaSDPLines, application specific SDP media lines for
		*		 the stream. Ownership is transferred.
	    */
		IMPORT_C void SetMediaAttributeLinesL( CDesC8Array* aMediaSDPLines );
		
		/**
		* Gets attribute lines of the media.
		* @return array of media attribute lines, owneship is transferred.
		*/
		IMPORT_C MDesC8Array* MediaAttributeLinesL();
		
		/** 
	    * Sets local media port
	    * @param aLocalMediaPort, local port for the media
	    */
	    IMPORT_C void SetLocalMediaPortL(TUint aLocalMediaPort); 
	    
	    /**
	    * Gets the local media port.
	    * @return Local port used for media.
	    */
	    IMPORT_C TUint LocalMediaPort() const;
	    
	    /**
	    * Gets the session this stream belongs to, if exists. The
	    * ownership is not transfered.
	    * @return Session object
	    */
		IMPORT_C CMceSession* Session() const;
	              

	public: // Local stream control

	    /**
	    * Enables streaming explicitly.
	    */
	    virtual void EnableL(); 

	    /**
	    * Disables streaming explicitly.
	    */
		virtual void DisableL(); 
		
		/**
	    * Returns the state of the stream.
	    * @return ETrue if enabled, EFalse if disabled.
	    */
    	IMPORT_C TBool IsEnabled() const;


    public: // Source and sink functions

        /**
        * Adds source to the stream. CMceMediaStream can only
        * have one source at a time. Ownership of passed source is transferred.
        * However, the same source can be also passed to other streams resulting
        * that streams share ownership of the source. Passing NULL source will
        * remove source from stream.
        * In some cases the source might contain a codec. The codec is derived
        * to the stream when such source is added to the stream. 
        * Derived codec will get automatically highest preference.
        * @param aSource source to be added to the stream; ownership is 
        *        transferred
        */
        IMPORT_C void SetSourceL( CMceMediaSource* aSource );

        /**
        * Adds sink to the stream. CMceMediaStream can have
        * many sinks at a time. Ownership of passed sink is transferred.
        * However, the same sink can be also passed to other streams resulting
        * that streams share ownership of the sink.
        * @param aSink sink to be added to the stream; ownership is transferred
        */
        IMPORT_C void AddSinkL( CMceMediaSink* aSink );

        /**
        * Gets source of the stream. Returns NULL if source is not set.
        * @return source of the stream; ownership is not transferred
        */
        IMPORT_C CMceMediaSource* Source() const;

        /**
        * Gets array of sinks of the stream.
        * @return array of sinks
        */
        IMPORT_C const RPointerArray<CMceMediaSink>& Sinks() const;

        /**
        * Removes sink from the stream.
        * @post aSink cannot be used anymore and contents of 
        *       previously returned sink array (CMceMediaStream::Sinks())
        *       are not anymore valid
        * @param aSink sink to be removed from the stream
        */
        IMPORT_C void RemoveSinkL( CMceMediaSink& aSink );

    public: // Binding opposite way stream

        /**
        * Binds opposite direction stream to this stream.
        * @param aStream another stream to be bind to the stream; 
        *        ownership is transferred
        */
        IMPORT_C void BindL( CMceMediaStream* aStream );

        /**
        * Gets opposite direction stream of this stream.
        * @return opposite direction stream
        */
        IMPORT_C CMceMediaStream& BoundStreamL() const;

        /**
        * Checks if opposite direction stream is set.
        * @return ETrue if opposite direction stream is set; 
        *         otherwise EFalse
        */
        IMPORT_C TBool BoundStream() const;


	public: //internal

	    /**
	    * Checks if this is binder (biding owner)
	    * @return ETrue if this is binder (biding owner)
	    */
		TBool Binder() const;

	    /**
	    * Initializes 
	    * @param aParent the parent
	    */
	    virtual void InitializeL( CMceSession& aParent );
	    
	    /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    */
	    virtual void InitializeL( CMceManager* aManager );
	    
	protected:

	    /**
	     * C++ default constructor.
	     */
		CMceMediaStream();

	private:

	    /**
	    * Handles event
	    * @param aEvent the event
	    * @return status, if event was consumed or not or object needs update
	    */
	    TInt HandleEvent( TMceEvent& aEvent );

	    /**
	    * one-way bind
	    * @param aStream another stream to be bind to the stream.
	    */
	    void DoBindL( CMceMediaStream* aStream );
	    
	    /**
	    * Deletes sink
	    * @param aIndex index of sink in the array
	    */
        void DeleteSink( TInt aIndex );
        
	    /**
	    * Deletes source
	    */
    	void DeleteSource();
	    

	protected: // NOT owned data
				
	    /**
	     * session
	     */
		CMceSession* iSession;
		
	protected: // owned data


	    /**
	     * linked stream. Owned if iLinkOwner is ETrue
	     */
	    CMceMediaStream* iLinkedStream;	    

    public: // stub data
    
        /**
	     * source
	     */
		CMceMediaSource* iSource;
		
		/**
	     * sinks
	     */
		RPointerArray<CMceMediaSink> iSinks;
    
        TState iState;
    
        TMceMediaType iType;
    
        TBool iLinkOwner;

        TBool iIsEnabled;
        
        TUint iLocalMediaPort;
        
        CDesC8Array* iMediaSDPLines;
	};


#endif
