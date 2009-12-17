/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The observer class for observer monitor call states change
*
*/


#ifndef MMUSTSYPROPERTYOBSERVER_H_
#define MMUSTSYPROPERTYOBSERVER_H_

#include <e32base.h>

/**
 * The observer class for monitor RMobileCall state change 
 */
class MMusTsyPropertyObserver
    {
public:
    /**
     * Notify call state changed
     * @param aVal The call status
     * @param aCallName The name of the call
     */
    virtual void NotifyCallStateChanged( NMusSessionInformationApi::TMusCallEvent aVal, TName& aCallName ) = 0;
    };

#endif /*MMUSPROPERTYOBSERVER_H_*/

