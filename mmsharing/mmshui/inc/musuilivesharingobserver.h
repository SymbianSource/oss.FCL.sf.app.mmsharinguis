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



#ifndef MUSUILIVESHARINGOBSERVER_H
#define MUSUILIVESHARINGOBSERVER_H

#include "musuidefinitions.h"

#include <aknappui.h>
#include <badesca.h>

class MMusUiLiveSharingObserver
	{
	
public:
    
    virtual void SetZoomValues( TInt aMinZoom, TInt aMaxZoom ) = 0;

    virtual void SetBrightnessValues( TInt aMinBrightness, TInt aMaxBrightness ) = 0;

    virtual void SetZoomL( TInt aZoomLevel ) = 0;
    
    virtual void SetBrightnessL( TInt aBrightnessLevel ) = 0;
    
    virtual void SetZoomVisible( TBool aVisible ) = 0;
    
    virtual void SetBrightnessVisible( TBool aVisible ) = 0;

	
	};

#endif // MUSUILIVESHARINGOBSERVER_H
            
// end of file
