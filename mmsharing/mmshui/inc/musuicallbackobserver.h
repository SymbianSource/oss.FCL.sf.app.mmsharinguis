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



#ifndef MUSUICALLBACKOBSERVER_H
#define MUSUICALLBACKOBSERVER_H


#include "musuidefinitions.h"


/**
 * MUS application's note request interface.
 */
class MMusUiCallbackObserver
    {

public: // new functions
    
    /**
     * Process asynchronous event.
     *
     * @param aEventId Id of the triggered event
     */
    virtual void HandleAsyncEventL( TMusUiAsyncEvent aEventId ) = 0;

    /**
     * Error handler for HandleAsyncEventL.
     *
     * @param aError is error to be handled.
     * @return KErrNone means handled value. All other values will lead to
     *         a panic.
     */
    virtual TInt HandleAsyncError( TInt aError ) = 0;
    
    };


#endif // MUSUICALLBACKOBSERVER_H
