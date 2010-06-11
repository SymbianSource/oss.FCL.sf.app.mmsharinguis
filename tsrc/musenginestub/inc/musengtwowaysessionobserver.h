/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSHENGTWOWAYSESSIONOBSERVER_H
#define MUSHENGTWOWAYSESSIONOBSERVER_H



// CLASS DECLARATION

/**
*  An interface to be implemented by clients of Multimedia Sharing Engine if 
*  they wish to receive two-way sharing specific events.
*/
class MMusEngTwoWaySessionObserver
    {
    
    public:
    
      /**    
    	* Indicates that video receiving state has changed
    	* @param aReceivingVideo, ETrue if receiving, EFalse if not receiving
    	*/
    	virtual void ReceivingStateChanged( TBool aReceivingVideo ) = 0;
		
    };

#endif