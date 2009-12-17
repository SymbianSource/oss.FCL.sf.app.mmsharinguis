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
* Description:  This observer is used to get function call 
*                from the ConnectionMonitor
*
*/


#ifndef __MMUSAVACONNECTIONMONITOROBSERVER_H__
#define __MMUSAVACONNECTIONMONITOROBSERVER_H__

#include "musavaobserver.h"

class CConnMonEventBase;
/**
 *  ConnectionMonitor Observer base class 
 *
 *  Observer interface for ConnectionMonitor.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class MMusAvaConnectionMonitorObserver
    {

public:
                        
   	/**
	 * Event from connection monitor server
	 *
	 */
    virtual void EventL(const CConnMonEventBase& aConnMonEvent) = 0; 
 
    };


#endif //__MMUSAVACONNECTIONMONITOROBSERVER_H__
