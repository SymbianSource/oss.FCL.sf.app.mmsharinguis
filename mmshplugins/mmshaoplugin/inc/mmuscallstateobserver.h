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
* Description:  The observer class to monitor MuSh call state change
*
*/


#ifndef MMUSCALLSTATEOBSERVER_H_
#define MMUSCALLSTATEOBSERVER_H_


/**
 * The observer class for call state monitor 
 */

class MMusCallStateObserver
    {
public:
    /**
     * Notify Mush call state changed
     */
    virtual void MusCallStateChanged() = 0;
    };

#endif /*MMUSCALLSTATEOBSERVER_H_*/


// End of File.
