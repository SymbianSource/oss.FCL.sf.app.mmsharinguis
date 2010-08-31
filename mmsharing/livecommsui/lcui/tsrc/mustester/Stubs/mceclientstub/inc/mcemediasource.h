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


#ifndef CMCEMEDIASOURCE_H
#define CMCEMEDIASOURCE_H

// INCLUDES 		
#include <e32base.h> 	

// DATA TYPES
typedef TUint TMceSourceType;

// FORWARD DECLARATIONS
class RReadStream;
class RWriteStream;
class CMceComMediaSource;
class TMceSourceFactory;
class CMceMediaStream;
class TMceEvent;
class TMceMediaId;
class CMceManager;

// CLASS DECLARATION

/**
* Base class for the MCE media sources.
*
* Media sources represent the origin of data.
* CMceMediaSource puts it data through CMceMediaStream into
* CMceMediaSink.
*
* CMceMediaSource has a type that defines how it can connected to other
* sinks and streams.
*
* Enable and Disable operations to sinks are considered to be local,
* so they are not signalled to remote terminal.
* 
*  @lib mceclient.lib
*/
class CMceMediaSource: public CBase
	{
	public: // Destructor

		IMPORT_C ~CMceMediaSource();

	public: // Functions

	    /**
	    * Returns the type of the source.
	    * @return The type of the source.
	    */
		IMPORT_C TMceSourceType Type() const;

	    /**
	    * Enables streaming explicitly.
	    */
		virtual void EnableL() = 0; 

	    /**
	    * Disables streaming explicitly.
	    */
		virtual void DisableL() = 0;

	    /**
	    * Returns the state of the source.
	    * @return ETrue if enabled, EFalse if disabled.
	    */
		IMPORT_C TBool IsEnabled() const;
		

	    
	public: //internal
	
	    /**
	    * Initializes 
	    * @param aParent the parent
	    */
	    virtual void InitializeL( CMceMediaStream& aParent );

        /**
	    * Initializes 
	    * @param aManager the manager. Ownership is NOT transferred
	    */
	    virtual void InitializeL( CMceManager* aManager );    
	   	    
	    /**
	    * Sets and gets reference count describing to how many
	    * streams source is associated
	    * @return reference count reference
	    */
	    TInt& ReferenceCount();

	    	    
	protected:

	    /**
	     * C++ default constructor.
	     */
		CMceMediaSource();
		
	    /**
	    * Enables streaming explicitly.
	    */
		void DoEnableL();

	    /**
	    * Disables streaming explicitly.
	    */
		void DoDisableL();
		

	protected: // NOT owned data
		
	    /**
	     * stream
	     */
		RPointerArray<CMceMediaStream> iStreams;
	    
	
	public: // stub data
	
	    TBool iIsEnabled;
	
	    TMceSourceType iType;

        TInt iReferenceCount;

		};


#endif 
