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


#ifndef MCEVIDEOSTREAM_H
#define MCEVIDEOSTREAM_H

//  INCLUDES
#include <e32std.h>  		
#include "mcemediastream.h"

// CONSTANTS
const TMceMediaType KMceVideo = 2;

// FORWARD DECLARATIONS
class CMceVideoCodec;

// CLASS DECLARATION

/**
* Class for representing unidirectional video stream.
*
* It defines the used video codecs for the stream.
* In order to be complete structure, at least one codec is required.
*
* @lib mceclient.lib
*/
class CMceVideoStream : public CMceMediaStream
	{
	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceVideoStream* NewL();
	   
	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceVideoStream* NewLC();
		
	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceVideoStream();

	public: // Functions

	    /**
	    * Returns the codecs available to the session.
	    * For CMceOutSession, array contains all codecs supported by
	    * terminal in default. For CMceInSession, array contains codecs
	    * that were offered by remote terminal, and supported by local terminal.
	    * Codecs are in preference order in the array. 
	    * Codec in the first element of the array has highest preference.
	    * Adding, removing or replacing a codec or changing the preference
	    * of a codec causes the returned array to be in invalid state.
	    * @return codecs available for the session
	    */
		IMPORT_C const RPointerArray<CMceVideoCodec>& Codecs();

        /**
	    * Adds codec to the stream. The codec contains preference value
	    * which will define its preference when added to the stream.
	    * Preference of codecs with the same preference value is determined
	    * by order they were added to the stream. In case of bound streams,
	    * codec is also automatically added to the opposite direction stream.
	    * After adding a codec an array of codecs received with a call
        * to Codecs() is not valid. 
	    * @param aCodec codec added to the stream; ownership is transferred
	    */
		IMPORT_C void AddCodecL( CMceVideoCodec* aCodec );

	    /**
	    * Removes codec from the stream. In case of bound streams, codec is
	    * also automatically removed from the opposite direction stream.
	    * After removing a codec an array of codecs previously received with 
	    * a call to Codecs() is not valid. 
	    * @param aCodec codec removed from stream
	    */
		IMPORT_C void RemoveCodecL( CMceVideoCodec& aCodec );
		
		/**
	    * Replaces codecs of the stream. Preference values of codecs will define
	    * the preference order. In case of bound streams, codecs are
	    * also automatically added to the opposite direction stream.
	    * After replacing codecs an array of codecs received with a call
        * to Codecs() is not valid. 
	    * @param aCodecs array of codecs which will replace existing codecs; 
	    *        ownership is transferred
	    */
		IMPORT_C void ReplaceCodecsL( RPointerArray<CMceVideoCodec>* aCodecs );

	    /**
	    * Gets supported video codecs of the current stream configuration.
	    * Supported codecs for this video stream is affected by attached
	    * sinks and sources.
	    * @return supported video codecs of the current stream configuration
	    */
	    IMPORT_C const RPointerArray<const CMceVideoCodec>& 
	                    SupportedVideoCodecs() const;	
	                    
	    
	public: // From CMceMediaStream
		
	    /**
	    * Initializes the stream. 
	    * @param aParent the parent
	    */
	    void InitializeL( CMceSession& aParent );

	    /**
	    * Initializes the stream.
	    * @param aManager the manager; ownership is not transferred
	    */
	   void InitializeL( CMceManager* aManager );
	   
	
	protected: // From CMceMediaStream
	
	    /**
	    * Determine whether current combination of source and sinks
	    * is allowed for spesific stream type.
	    * @return ETrue is combination is allowed; otherwise EFalse
	    */
	    virtual TBool IsAllowedCombination();	

	private:

	    /**
	    * C++ default constructor.
	    */
		CMceVideoStream();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL();
	    		

	private: // Owned data

	    /**
	    * Codecs.
	    */
		RPointerArray<CMceVideoCodec> iCodecs;

        /**
	    * Supported video codecs.
	    */
	    RPointerArray<const CMceVideoCodec> iSupportedVideoCodecs;

    private: // Reserved for future use
    
        TAny* iReserved;       


    public: // Stub data	
    
	};

#endif
