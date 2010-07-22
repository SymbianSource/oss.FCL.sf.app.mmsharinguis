/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CLCENGINE_H
#define CLCENGINE_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MLcSession;

// CONSTANTS
/** Interface UID of this ECOM interface */
const TUid KLcEngineInterfaceUid = { 0x20021340 };

// CLASS DECLARATION
/**
* A base class for ECom plug-ins implementing
* the Live Comms engine plug-in API. 
*/
class CLcEngine : public CBase
    {
    public: // Enumerations

        /**
        * The list of plug-in configurable features.
        */
        enum TLcFeature
            {
            ELcShowInvitingNote,
            ELcShowWaitingNote,
            ELcShowAcceptQuery,
            ELcSendVideoQuery
            };

    public: // Constructors and destructor
    
        /**
        * Loads and creats an instance of the plug-in.
        * @param aEngineName used in finding the correct implementation.
        * Matched to the IMPLEMENTATION_INFO / default_data of the plug-in.
        * @return a new instance of CLcEngine, the ownership is transferred
        */
        inline static CLcEngine* NewL( const TDesC8& aEngineName );
    
        /**
        * Destructor
        */
        inline ~CLcEngine();

    public: // New pure virtual functions
    
        /**
        * Returns a reference to the session created by the plug-in.
        * @return the session
        */
        virtual MLcSession& Session() = 0;

    public: // New virtual functions    
        
        /**
        * Checks if a feature should be used with the particular engine.
        * @param aLcFeature the identifier of the feature
        * @return ETrue if the feature is supported, otherwise EFalse.
        */
        inline virtual TBool IsFeatureSupported( TLcFeature aLcFeature );

    protected: // Constructors

        inline CLcEngine();
    
    public: // Data

        TUid iInstanceKey;
    };

#include "lcengine.inl"    
    
#endif // CLCENGINE_H

// end of file
