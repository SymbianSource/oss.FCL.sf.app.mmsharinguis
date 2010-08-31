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
#include "musunittesting.h"
#include "muspropertyobserver.h"
#include <lcengine.h>
#include <musmanagercommon.h>

// FORWARD DECLARATIONS
class MLcSession;
class CMusEngMceSession;
class MLcUiProvider;
class CMusPropertyWatch;


class CMusEngSessionManager : public CLcEngine, public MMusPropertyObserver
    {
    public:
    
        static CMusEngSessionManager* NewL();
        ~CMusEngSessionManager();
    
    public: // From CLcEngine
    
        MLcSession& Session();
       
        TBool IsFeatureSupported( TLcFeature aLcFeature );
       
    public: // From MMusPropertyObserver
        
        virtual void PropertyChanged( const TUint aKey, const TInt aValue );    
            
        virtual void HandlePropertyError( const TInt aReason );
            
    private: // Constructors

        CMusEngSessionManager();
        void ConstructL();
       
    private: // New functions
        
        CMusEngMceSession* CreateMusSessionL( 
            MultimediaSharing::TMusUseCase aUseCase );
        
    private: // Data
        
        CMusEngMceSession* iSession;
        
        CMusPropertyWatch* iUseCaseWatch;
        
        MultimediaSharing::TMusUseCase iCurrentUseCase;
        
    MUS_UNITTEST( UT_CMusEngSessionManager )
    };  
    
#endif // CMUSENGSESSIONMANAGER_H

// end of file
