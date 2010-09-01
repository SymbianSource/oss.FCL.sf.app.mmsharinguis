/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Mus Application's Send Observer Interface
*
*/



#ifndef MUSUISENDOBSERVER_H
#define MUSUISENDOBSERVER_H

class MMusUiSendObserver
    {
	
    public:

        virtual void ManualAddressEntryL( const TDesC& aAddress ) = 0;	
        

    };

#endif // MUSUISENDOBSERVER_H
            
// end of file
