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



#ifndef MUSUIINVITATIONQUERY_H
#define MUSUIINVITATIONQUERY_H

#include "musuiactivetimerobserver.h"

#include <e32base.h>

class CAknGlobalConfirmationQuery;
class CMusUiActiveTimer;
class MMusUiInvitationQueryObserver;

/**
 *
 *
 *
 * @lib musui.exe
 */
class CMusUiInvitationQuery : public CActive, 
                              public MMusUiActiveTimerObserver
    {
    
public: // constructor

    static CMusUiInvitationQuery* NewL( MMusUiInvitationQueryObserver& aView );
    
    ~CMusUiInvitationQuery();

private:

    CMusUiInvitationQuery( MMusUiInvitationQueryObserver& aView );

    void ConstructL();


public: // new functions

    void ShowL( const TDesC& aContact );


public: // from: MMusUiActiveTimerObserver

	void TimerComplete( CMusUiActiveTimer* aTimer );


protected: // from CActive

    void RunL();
    
    void DoCancel();
    
    TInt RunError( TInt aError );


private: // data

    /** A global confirmation query. Own. */
    CAknGlobalConfirmationQuery* iGlobalQuery;

    /** A descriptor buffer for prompting. Own. */
    HBufC* iPrompt;
    
    CMusUiActiveTimer* iActiveTimer;

    /** Observer. */
    MMusUiInvitationQueryObserver& iObserver;
    };


#endif // MUSUIINVITATIONQUERY_H

// end of file
