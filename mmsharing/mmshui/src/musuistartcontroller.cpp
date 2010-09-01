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
* Description:  Application's UI class.
*
*/



#include "musuistartcontroller.h"
#include "musmanagercommon.h"
#include "mussessionproperties.h"
#include "muslogger.h" // debug logging

#include <e32property.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MusUiStartController::MusUiStartController()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MultimediaSharing::TMusUseCase MusUiStartController::ReadUseCaseL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStartController::UseCasePropertyL()" );
    TInt val;
    User::LeaveIfError( RProperty::Get( NMusSessionApi::KCategoryUid, 
                                        NMusSessionApi::KUseCase, 
                                        val ) );
                                        
    MUS_LOG1( "mus: [MUSUI ]     <- CMusUiStartController::UseCasePropertyL():\
                            [%d]", val );    
                            
    return static_cast< MultimediaSharing::TMusUseCase >( val );
    }
