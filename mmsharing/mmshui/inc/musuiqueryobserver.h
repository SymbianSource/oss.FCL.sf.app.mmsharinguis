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
* Description:  The MUS application's UI class.
*
*/



#ifndef MUSUIQUERYOBSERVER_H
#define MUSUIQUERYOBSERVER_H


class MMusUiQueryObserver
    {
public:    
    
    /**
     *
     */
    virtual void QueryAcceptedL( TBool aAccepted ) = 0;
    
    /**
     * Error handler for QueryAcceptedL.
     *
     * @param aError is error to be handled.
     * @return KErrNone means handled value. All other values will lead to
     *         a panic.
     */
    virtual TInt HandleQueryError( TInt aError ) = 0;
    
    };


#endif // MUSUIQUERYOBSERVER_H

// end of file
