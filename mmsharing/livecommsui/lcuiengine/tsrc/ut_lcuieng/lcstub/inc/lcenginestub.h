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

#ifndef LCENGINESTUB_H
#define LCENGINESTUB_H


extern void lcutStub_LcEngine_reset();
extern void lcutStub_LcEngine_setFailure( int failure );
extern int lcutStub_LcEngine_failure();
extern void lcutStub_LcEngine_setLcFeatureSupported( bool supported );

// INCLUDES
#include "lcengine.h"

// FORWARD DECLARATIONS
class CLcSession;
class MLcUiProvider;

class CLcEngineStub : public CLcEngine
    {
    public:
    
        static CLcEngineStub* NewL();
        ~CLcEngineStub();
    
    public: // From CLcEngine
    
        MLcSession& Session();
        
        TBool IsFeatureSupported( TLcFeature aLcFeature );
        
    private: // Constructors

        CLcEngineStub();
        void ConstructL();
       
    private: // New functions
        
        CLcSession* CreateMusSessionL( int aUseCase );
        
    public: // Data
        
        CLcSession* iSession;
    };
#endif // LCENGINESTUB_H

// end of file
