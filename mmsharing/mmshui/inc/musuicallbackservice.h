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


#ifndef MUSUICALLBACKSERVICE_H
#define MUSUICALLBACKSERVICE_H

#include "musuidefinitions.h"
#include <e32base.h>

class MMusUiCallbackObserver;


/**
 * Callback Service used to pass asynchronous events
 */
class CMusUiCallbackService : public CActive
    {

public: // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aCallbackObserver the observer that will receive the callbacks.
    * @return new instance of CMusUiCallbackService.
    */
    static CMusUiCallbackService* NewL( 
                            MMusUiCallbackObserver& aCallbackObserver );
    
    /**
    * Destructor.
    */
    ~CMusUiCallbackService();

private: // Constructors

    /**
    * C++ default constructor.
    * @param aCallbackObserver the observer that will receive the callbacks.
    */
    CMusUiCallbackService( MMusUiCallbackObserver& aCallbackObserver );
    
    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();


public: // New functions

    /**
    * Buffer a request to trigger an event.
    * @param aEventId    Id of the event
    */
    void AsyncEventL( TMusUiAsyncEvent aEventId );

    /**
    * Called when the notifier has been deactivated so resources can be 
    * freed and outstanding messages completed.
    */
    void Cancel();
    
    /**
    * Check what event is currently pending
    * @return EMusUiAsyncUninitialized is no events are pending
    */
    TMusUiAsyncEvent CurrentEvent() const;


public: // Functions from CActive base class
    
    /**
    * From CActive.  Called when a request completes.
    */
    void RunL();

    /**
    * From CActive.  Called when a leave occurres in RunL.
    * @param aError Symbian OS errorcode.
    * @return Error Code.
    */
    TInt RunError(TInt aError);
    
    /**
    * From CActive.  Called when a request is cancelled.
    */
    void DoCancel();  


private: // New functions

    /**
    * Activate the object.
    */
    void Activate();


private:    // Data

    /**  Handle to callback observer. */
    MMusUiCallbackObserver& iCallbackObserver;

    /**  Event ID to pass through. */
    TMusUiAsyncEvent iEventId;

    };

#endif // MUSUICALLBACKSERVICE_H
