/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef UT_MUSCOMMONSTUBS_H
#define UT_MUSCOMMONSTUBS_H


#include "muscallmonitorobserver.h"


#include <e32base.h>
#include <etelmm.h>

/**
*
* @lib musengine.lib
* @since S60 v3.2
*/        
class CMusCommonObserverStub : public CBase, 
                               public MMusCallMonitorObserver
    {
    
    public:

        CMusCommonObserverStub();
            
        ~CMusCommonObserverStub();
        
        // From MMusCallMonitorObserver
        
        void CallConnectedL( const TDesC& aTelNumber, TBool aIsSipUri );
        void CallHoldL( const TDesC& aTelNumber, TBool aIsSipUri );
        void ConferenceCallL();
        void NoActiveCallL();
           
        
    public: // Helpers
    
        /**
        * ETrue if all boolean member variables are set to EFalse and all the
        * pointers are NULL
        */
        TBool IsReseted();
        
        /**
        * Resets all the boolean member variable values to EFalse and deletes
        * and nullifies all the pointer variables.
        */
        void Reset();

    public: // Data  
        
        TBool iCallConnectedCalled;
        TBool iCallHoldCalled;
        TBool iConferenceCallCalled;
        TBool iNoActiveCallCalled;
        TBool iIsSipUri;
                
        HBufC* iTelNumber; 
        RMobileCall::TMobileCallEvent iCallEvent;
        
        
    };



    
#endif // UT_MUSCOMMONSTUBS_H

