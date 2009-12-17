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



#ifndef UT_MUSUIOBSERVERSSTUB_H
#define UT_MUSUIOBSERVERSSTUB_H


#include "musuiclipsharingobserver.h"
#include "musuipropertyobserver.h"
#include "musuicallbackobserver.h"


#include <e32base.h>
#include <aknappui.h>


/**
*
* @lib musengine.lib
* @since S60 v3.2
*/
class CMusUiCallbackObserverAdapter : public CBase,
                            public MMusUiCallbackObserver
    {



    public:

        CMusUiCallbackObserverAdapter();

        ~CMusUiCallbackObserverAdapter();




        //virtual void HandleError( const TInt aReason ) = 0;

    public: // MMusUiCallbackObserver

        void HandleAsyncEventL( TMusUiAsyncEvent aEventId );


    public: // data

        enum TMusUiCallbackObserverFunction
            {
            ENone,                          // default value
            EHandleAsyncEventL
            };



        TMusUiCallbackObserverFunction iCalledObserver;
        TInt iErrorCode;

        TMusUiAsyncEvent iHandleAsyncEventId;

    };




#endif // UT_MUSSTUBS_H

