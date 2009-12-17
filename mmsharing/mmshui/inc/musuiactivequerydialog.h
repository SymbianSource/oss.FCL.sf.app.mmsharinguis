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



#ifndef MUSUIACTIVEQUERYDIALOG_H
#define MUSUIACTIVEQUERYDIALOG_H

//#include "musuiactivetimerobserver.h"

#include <e32base.h>

class CAknGlobalConfirmationQuery;
class MMusUiQueryObserver;

/**
 *
 *
 *
 * @lib musui.exe
 */
class CMusUiActiveQueryDialog : public CActive
    {
    
public: // constructor

    static CMusUiActiveQueryDialog* NewL( MMusUiQueryObserver& aObserver );
    
    ~CMusUiActiveQueryDialog();

private:

    CMusUiActiveQueryDialog( MMusUiQueryObserver& aObserver );

    void ConstructL();


public: // new functions

    void ShowL( const TDesC& aPrompt );


protected: // from CActive

    void RunL();
    
    void DoCancel();
    
    TInt RunError( TInt aError );


private: // data

    /** A global confirmation query. Own. */
    CAknGlobalConfirmationQuery* iGlobalQuery;
    
    /** Observer. */
    MMusUiQueryObserver& iObserver;
    };


#endif // MUSUIACTIVEQUERYDIALOG_H

// end of file
