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


#ifndef MUSUISTARTCONTROLLER_H
#define MUSUISTARTCONTROLLER_H

#include "musmanagercommon.h"
#include "musunittesting.h"


/**
 * Class to read startup parameter from publish & subscribe
 */
class MusUiStartController
    {

private: // second phase constructors

    /**
     * Disable instantiation by declaring C++ constructor private.
     */
	MusUiStartController();


public: // static new functions
	
    /**
     * Reads a P&S integer value from the category KCategoryUid
     * @pre P&S property KUseCase has to be defined.
     * @leave One of the system wide error codes, if RProperty::Get()
     *        returns an error value.
     * @return Defined use case (TMusUseCase) for launching MUS.
     */
	static MultimediaSharing::TMusUseCase ReadUseCaseL();


    MUS_UNITTEST( UT_CMusUiStartController; )
    
    };

#endif // MUSUISTARTCONTROLLER_H

// end of file
