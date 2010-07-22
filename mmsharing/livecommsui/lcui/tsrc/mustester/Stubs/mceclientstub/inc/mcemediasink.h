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


#ifndef CMCEMEDIASINK_H
#define CMCEMEDIASINK_H

// INCLUDES 		
#include <e32base.h> 		
#include <badesca.h>

// DATA TYPES
typedef TUint TMceSinkType;

// FORWARD DECLARATIONS
class TMceSinkFactory;
class CMceMediaStream;
class TMceIds;
class TMceEvent;
class TMceMediaId;
class CMceManager;


// CLASS DECLARATION

/**
* Base class for the MCE media sinks.
*
* Media sinks represent the destination of data.
* CMceMediaSink gets it data from CMceMediaSource through
* CMceMediaStream. 
*
* CMceMediaSink has a type that defines how it can connected to other
* sources and streams.
*
* Enable and Disable operations to sinks are considered to be local,
* so they are not signalled to remote terminal.
* 
*  @lib mceclient.lib
*/
class CMceMediaSink: public CBase
	{

	public: // Destructor

		IMPORT_C ~CMceMediaSink();

	public: // Functions

	    /**
	    * Returns the type of the sink.
	    * @return The type of the sink.
	    */
		IMPORT_C TMceSinkType Type() const;

	    /**
	    * Enables streaming explicitly.
	    */
		virtual void EnableL() = 0; 

	    /**
	    * Disables streaming explicitly.
	    */
		virtual void DisableL() = 0;

	    /**
	    * Returns the state of the sink.
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
	    * streams sink is associated
	    * @return reference count reference
	    */
	    TInt& ReferenceCount();
        
	protected:

	    /**
	     * C++ default constructor.
	     */
		CMceMediaSink();

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
		CMceMediaStream* iStream;
		
    public: // stub data
    
        TMceSinkType iType;
    
        TBool iIsEnabled;
	    
	    TInt iReferenceCount;
	    
		};


#endif
