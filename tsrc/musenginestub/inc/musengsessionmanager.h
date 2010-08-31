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

#ifndef CMUSENGSESSIONMANAGER_H
#define CMUSENGSESSIONMANAGER_H

// INCLUDES
#include "lcengine.h"
#include "musmanagercommon.h"

// FORWARD DECLARATIONS
class MLcSession;
class CMusEngMceSession;
class MLcUiProvider;


class CMusEngSessionManager : 
    public CLcEngine
    {
    public:
    
        static CMusEngSessionManager* NewL( TAny* aInitParams );
        ~CMusEngSessionManager();
    
    public: // From CLcEngine
    
        MLcSession& Session();

    private: // Constructors

        CMusEngSessionManager( TLcEngineParams& aInitParams );
        void ConstructL();
       
    private: // New functions
        
        CMusEngMceSession* CreateMusSessionL( 
            MultimediaSharing::TMusUseCase aUseCase );
        
    private: // Data
        
        CMusEngMceSession* iSession;
        
    };  
    
#endif // CMUSENGSESSIONMANAGER_H

// end of file
