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



#ifndef MUSUIMMCOBSERVER_H
#define MUSUIMMCOBSERVER_H


/**
*  Interface for listening MMC removal indication. 
*/
class MMusUiMmcObserver
    {
    public:

        /**
        * Called when MMC was removed.
        */
        virtual void MmcRemoved() = 0;
    };

#endif // MUSUIMMCOBSERVER_H

// End of File
