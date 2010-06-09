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



#ifndef MUSUIMMCMONITOR_H
#define MUSUIMMCMONITOR_H

// INCLUDES
#include "musuimmcobserver.h"

#include <e32base.h>
#include <f32file.h>


// CLASS DECLARATION

/**
* Listen changes on the filesystem and notifies observers if 
* MMC card has been removed.
* Notifies clients only once about MMC removal and only 
* in that case that user wanted to listen MMC drive
*/
class CMusUiMmcMonitor : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aObserver MMC observer pointer.
        */
        static CMusUiMmcMonitor* NewL( MMusUiMmcObserver& aObserver );

        /**
        * Destructor.
        */
        ~CMusUiMmcMonitor();
       
    private: // from CActive

        void RunL();
        TInt RunError( TInt aError );
        void DoCancel();
    
    private:    // New functions.
    
        void ConstructL();
        CMusUiMmcMonitor( MMusUiMmcObserver& aObserver );

        void MonitorMmc();
        TBool DriveRemoved();
        TBool IsRemovableDrive( TDriveNumber aDriveNumber );

    private: // Data

        RFs iFs;
        TDriveNumber iDriveNumber;
        TBool iRemovableDrive;
        TBool iDriveRemoved;
        MMusUiMmcObserver& iObserver;
    };

#endif  // MUSUIMMCMONITOR_H

// End of File

