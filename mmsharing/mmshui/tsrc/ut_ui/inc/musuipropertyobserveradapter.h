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

#ifndef __MUSUIPROPERTYOBSERVERADAPTER_H__
#define __MUSUIPROPERTYOBSERVERADAPTER_H__

#include "musuipropertyobserver.h"

class CMusUiPropertyObserverAdapter : public CBase,
                                      public MMusUiPropertyObserver

    {

    public: // MMusUiPropertyObserver

        CMusUiPropertyObserverAdapter();
        ~CMusUiPropertyObserverAdapter();

        void ResourcePropertyChanged( const TUint aKey, const TInt aValue );
        void SessionPropertyChanged( const TUint aKey, const TInt aValue );
        void HandleError( const TInt aReason );


        enum TMusUiPropertyObserverFunction
            {
            ENone,                          // default value
            EResourcePropertyChanged,
            ESessionPropertyChanged,
            EHandleError
            };


        TMusUiPropertyObserverFunction iCalledObserver;
        TInt iErrorCode;


    };

#endif