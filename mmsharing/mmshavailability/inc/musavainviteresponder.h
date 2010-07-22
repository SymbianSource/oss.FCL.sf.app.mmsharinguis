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
* Description:  Provide interface for the client requestin network availability.
*
*/


#ifndef __MUSAVAINVITERESPONDER_H__
#define __MUSAVAINVITERESPONDER_H__

#include "musunittesting.h"
#include <e32base.h>

class CSIPServerTransaction;
class MMusAvaInviteResponderObserver;
class CSIPTransactionBase;
class CSIPResponseElements;

/**
 *  Responds to a non-desired invitation.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaInviteResponder : public CBase
	{
public:

    /**
     * 
     *
     * @since S60 v3.2
     */   		
    static CMusAvaInviteResponder* NewL( CSIPServerTransaction* aTransaction,
                                  MMusAvaInviteResponderObserver& aObserver );
                                  
    /**
     * 
     *
     * @since S60 v3.2
     */   		
    static CMusAvaInviteResponder* NewLC( 
                                   CSIPServerTransaction* aTransAction,
                                   MMusAvaInviteResponderObserver& aObserver );
    /**
     * 
     *
     * @since S60 v3.2
     */   		
    ~CMusAvaInviteResponder();
    

public:

    /**
     * 
     *
     * @since S60 v3.2
     */   		
    void AnswerL();
    
    /**
     * Send response to request.
     *
     * @since S60 v3.2
     */ 

    void SendResponseL( TUint aStatusCode,
                        const TDesC8& aReasonPhrase );
    
    /**
     * Returns response status.
     *
     * @since S60 v3.2
     */ 
    TBool ResponseComplete();

     /**
     * Send Response to reques.
     *
     * @since S60 v3.2
     */ 
    void ResponseL( CSIPResponseElements* aResponseElements );


                                   
private:

    /**
     * 
     *
     * @since S60 v3.2
     */   		
    CMusAvaInviteResponder( CSIPServerTransaction* aTransAction,
                            MMusAvaInviteResponderObserver& aObserver );
                            
	
private:

    /**
     * Reference to observer.
     */
    MMusAvaInviteResponderObserver& iObserver;
    
    /**
     * SIP transaction. Owns.
     */
    CSIPServerTransaction* iTransaction;
    MUS_UNITTEST( UT_CMusAvaInviteResponder )
    };


#endif // __MUSAVAINVITERESPONDER_H__