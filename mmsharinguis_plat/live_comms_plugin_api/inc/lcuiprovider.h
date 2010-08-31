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

#ifndef MLCUIPROVIDER_H
#define MLCUIPROVIDER_H

// INCLUDES
#include <e32std.h>
#include <bamdesca.h> 

// CLASS DECLARATION
/**
* A callback for fetching session related information from the end user.
*/
class MLcUiProvider
    {
    public:

        /**
        * Asks to select the recipient for the session from 
        * a predefined list of recipients.
        * @param aRecipientAddresses the list of possible recipients
        * @param aSelectedRecipientAddress on return contains 
        * the selected recipient
        * @return ETrue if the recipient was selected, otherwise EFalse
        */
        virtual TBool SelectRecipient( 
            MDesCArray& aRecipientAddresses, 
            TDes& aSelectedRecipientAddress ) = 0;        
        
        /**
        * Asks to input the recipient for the session.
        * @param aRecipientAddress on return contains the recipient address
        * @return ETrue if the recipient was inserted, otherwise EFalse
        */
        virtual TBool InputRecipient( TDes& aRecipientAddress ) = 0;
        
        /**
        * The application foreground status has to be changed. 
        * @param aForeground ETrue if application should be brought to 
        * foreground.
        */
        virtual void HandleForegroundStatus( TBool aForeground ) = 0;
        
        /**
        * UI needs to be blocked/unblocked. If engine is processing
        * asynchronous operation and cannot handle other actions meanwhile,
        * it can ask UI to block user controls until asynchronous operation
        * completes. NOTE: session termination is allowed even if blocking 
        * is active.
        * @param aBlocked, ETrue is UI should be blocked, EFalse
        *   if UI can be unblocked.
        */
        virtual void BlockUi( TBool aBlocked ) = 0;
    };
   
    
#endif // MLCUIPROVIDER_H

// end of file
