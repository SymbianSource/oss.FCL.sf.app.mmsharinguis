/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Observer for listening status of current cs call
*
*/



#ifndef MUSCALLSTATUSMONITOROBSERVER_H
#define MUSCALLSTATUSMONITOROBSERVER_H


/**
 * Observer for the monitor any call status changes
 *
 * @lib musaoplugin.dll
 */
class MMusCallStatusMonitorObserver
    {

public: // type definitions

    virtual void StatusChanged() = 0;

    };

#endif // MUSCALLSTATUSMONITOROBSERVER_H
