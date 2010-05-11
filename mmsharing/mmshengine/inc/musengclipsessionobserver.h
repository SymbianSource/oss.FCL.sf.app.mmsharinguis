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


#ifndef MUSENGCLIPSESSIONOBSERVER_H
#define MUSENGCLIPSESSIONOBSERVER_H

// INCLUDES
#include <e32def.h>

// CLASS DECLARATION

/**
*  An interface to be implemented by users of Multimedia Sharing Engine 
*  if they wish to receive clip sharing specific events.
*
*  This observer is set using CMusEngClipSession::SetSessionObserver function.
*
*/
class MMusEngClipSessionObserver
    {
    public: 
    
        /**
	    * Indicates that shared clip has ended.
	    */
        virtual void EndOfClip() = 0;        
        
        /**
	    * Indicates that chosen clip must be transcoded before sharing.
	    * @param aDueUnknownRemoteCapabilities, ETrue if transcoding is needed because
	    *  it is unknown whether remote end can support current format of clip.
	    */
        virtual void TranscodingNeeded(TBool aDueUnknownRemoteCapabilities) = 0;
        
        /**
	    * Indicates that transcoding has progressed.
	    * @param aPercentage Percentage of completion
	    */
        virtual void TranscodingProgressed( TInt aPercentage ) = 0;
        
        /**
	    * Indicates that transcoding has successfully complited
	    * but session establishment is not yet proceeding.
	    */
        virtual void TranscodingCompletedInit() = 0;
        
        /**
	    * Indicates that transcoding has successfully complited
	    * and session establishment continues.
	    */
        virtual void TranscodingCompletedFinalize() = 0;
        
        /**
	    * Indicates that transcoding has failed.
	    */
        virtual void TranscodingFailed() = 0;
        
    };

#endif
