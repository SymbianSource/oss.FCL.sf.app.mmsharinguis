/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/



#ifndef MUSUNITTESTING_H
#define MUSUNITTESTING_H

#ifdef UNIT_TESTING // if unit testing

    #define MUS_UNITTEST( aClassName ) friend class aClassName;

#else

    #define MUS_UNITTEST( aClassName ) 

#endif // UNIT_TESTING


#endif // MUSUNITTESTING_H

// End of File
