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
#include "mccdatareceiverobserver.h"

// FORWARD DECLARATIONS
class MLcSession;
class CMusEngMceSession;
class MLcUiProvider;
class CMccDataReceiver;
class CMccDataSender;

class CMusEngSessionManager : public CLcEngine, public MMccDataReceiverObserver
    {
    public:
    
        static CMusEngSessionManager* NewL();
        ~CMusEngSessionManager();

	void DataReceived( const TDataMessage& aData );
    
    public: // From CLcEngine
    
        MLcSession& Session();       
        
        TBool IsFeatureSupported( TLcFeature aLcFeature );
        
    private: // Constructors

        CMusEngSessionManager();
        void ConstructL();
               
    private: // Data
        
        CMusEngMceSession* iSession;

        CMccDataReceiver* iMsgReceiver;
        CMccDataSender* iMsgSender;

    };  
    
#endif // CMUSENGSESSIONMANAGER_H

// end of file
