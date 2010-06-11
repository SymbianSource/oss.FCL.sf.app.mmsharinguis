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


#ifndef CMCEAUDIOSTREAM_H
#define CMCEAUDIOSTREAM_H

//  INCLUDES
#include <e32std.h>  		
#include <mcemediastream.h>

// CONSTANTS
const TMceMediaType KMceAudio = 1;

// FORWARD DECLARATIONS
class CMceAudioCodec;

// CLASS DECLARATION

/**
* Class for representing unidirectional audio stream.
*
* It defines the used audio codecs for the stream.
* In order to be complete structure, at least one codec is required.
*
*  @lib mceclient.lib
*/
class CMceAudioStream: public CMceMediaStream
	{
	public: // Constructors and destructor

	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceAudioStream* NewL();
	   
	    /**
	    * Two-phased constructor.
	    */
		IMPORT_C static CMceAudioStream* NewLC();
		
	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceAudioStream();

	public: // Functions

	    /**
	    * Returns the available codecs to the session.
	    * For CMEOutSession, array contains all codecs supported by
	    * terminal in default. For CMEInSession, array contains codecs
	    * that were offered by remote terminal, and supported by local terminal.
	    * @return codecs available for the session.
	    */
		IMPORT_C const RPointerArray<CMceAudioCodec>& Codecs();

	    /**
	    * Adds codec to the stream, ownership is transferred.
	    * @param aCodec codec added to the stream
	    */
		IMPORT_C void AddCodecL( CMceAudioCodec* aCodec );

	    /**
	    * Removes codec from the stream
	    * @param aCodec codec removed from stream
	    */
		IMPORT_C void RemoveCodecL( CMceAudioCodec& aCodec );

	    
	public: //from CMceMediaStream
		
	    /**
	    * Initializes 
	    * @param aParent the parent
	    */
	    void InitializeL( CMceSession& aParent );

	    /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    */
	   void InitializeL( CMceManager* aManager );
		

	private: //methods

	    /**
	     * C++ default constructor.
	     */
		CMceAudioStream();

	    /**
	    * Second-phase constructor.
	    */
		void ConstructL();
	    		

	private: // Owned data

	    /**
	     * codecs
	     */
		RPointerArray<CMceAudioCodec> iCodecs;

	};

#endif
