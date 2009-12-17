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



#ifndef MUSUIEVENTOBSERVER_H
#define MUSUIEVENTOBSERVER_H

#include "musuidefinitions.h"

#include <e32base.h>
#include <eikenv.h>
#include <aknappui.h>
#include <cntdef.h>

class MMusUiEventObserver
	{
	
public:

	virtual void ActivateLocalMusViewL( TUid aViewId ) = 0;
	
	virtual void ActivateLocalMusViewL( TUid aViewId, 
	                                    TUid aCustomMessageId,
	                              const TDesC8& aCustomMessage ) = 0;

    virtual void ShowAsyncConfirmationQueryDialogL( const TDesC& aPrompt ) = 0;
    
    virtual void SetToolbarVisibility( TBool aVisible ) = 0;
    
    /**
    * 
    */
    virtual void ShowNaviPaneIconL( TMusUiNaviMediaDecorator aIcon ) = 0;

    virtual CEikonEnv* EikonEnv() const = 0;
    
    virtual void TerminateCall() = 0;

    /**
     * Return current app orientation.
     */
    virtual CAknAppUiBase::TAppUiOrientation AppOrientation() const = 0;
    
    virtual void SwitchOrientationL( 
                        CAknAppUiBase::TAppUiOrientation aOrientation ) = 0;
        
    virtual void HandleError( const TInt aReason ) = 0;
    
    virtual void HandleExit() = 0;
    
    virtual TBool OrientationCanBeChanged() const = 0;
    
    virtual TInt GetUiVolumeValue() = 0;
	};

#endif // MUSUIEVENTOBSERVER_H
            
// end of file
