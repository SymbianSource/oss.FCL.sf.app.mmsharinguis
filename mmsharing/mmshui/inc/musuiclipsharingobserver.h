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
* Description:  Mus Applications Event Observer Interface
*
*/



#ifndef MUSUICLIPSHARINGOBSERVER_H
#define MUSUICLIPSHARINGOBSERVER_H

#include "musuidefinitions.h"

#include <aknappui.h>
#include <badesca.h>

class MMusUiClipSharingObserver
    {

public:

    virtual void RunFetchDialogL() = 0;

    virtual void ShowTranscodingProgressDialogL() = 0;

    virtual void UpdateClipTranscodingPercentageL( TInt aPercetage ) = 0;
    
    virtual void CancelTranscodingDialogL() = 0;

	virtual void UpdateClipPositionL( TInt aPositionInSeconds,
	                                  const TDesC& aFormattedPosition ) = 0;
    
    virtual void SetDurationValue( TInt aDurationValue ) = 0;

    virtual void SetPositionValueL( TInt aPositionValue ) = 0;

    virtual void SetDurationVisible() = 0;

    };

#endif // MUSUICLIPSHARINGOBSERVER_H

// end of file
