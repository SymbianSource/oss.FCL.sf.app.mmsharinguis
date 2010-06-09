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


#include "musuimmcmonitor.h"
#include "muslogger.h" // debug logging

#include <pathinfo.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiMmcMonitor* CMusUiMmcMonitor::NewL( MMusUiMmcObserver& aObserver )
    {
    CMusUiMmcMonitor* self = new ( ELeave ) CMusUiMmcMonitor( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiMmcMonitor::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiMmcMonitor::ConstructL" );
    User::LeaveIfError( iFs.Connect() );
    const TDesC& memoryCardPath = PathInfo::MemoryCardRootPath();
    TInt driveNumber = 0;
    if ( memoryCardPath.Length() > 0 )
        {
        MUS_LOG( "mus: [MUSUI ]     CMusUiMmcMonitor::ConstructL: \
                                ( memoryCardPath.Length() > 0 )" );
        TChar driveChar( memoryCardPath[0] );
        iFs.CharToDrive( driveChar, driveNumber );
        iDriveNumber = static_cast<TDriveNumber>(driveNumber);
        }
    else
        {
        MUS_LOG( "mus: [MUSUI ]     CMusUiMmcMonitor::ConstructL: else" );
        TParsePtrC parser( RProcess().FileName() );
        TChar driveChar( parser.Drive()[0] );
        iFs.CharToDrive( driveChar, driveNumber );
        iDriveNumber = static_cast<TDriveNumber>(driveNumber);
        }
        
    iRemovableDrive = IsRemovableDrive( iDriveNumber );
    iDriveRemoved = DriveRemoved();
    MonitorMmc();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiMmcMonitor::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiMmcMonitor::CMusUiMmcMonitor( MMusUiMmcObserver& aObserver )  
    : CActive( CActive::EPriorityStandard ),
      iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiMmcMonitor::~CMusUiMmcMonitor()
    {
    Cancel();
    iFs.Close();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiMmcMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiMmcMonitor::RunL" );
    if( DriveRemoved() )
        {
        iObserver.MmcRemoved();
        }
    else // continue monitoring
        {
        MonitorMmc();
        }    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiMmcMonitor::RunL" );
    }

// -------------------------------------------------------------------------
//  If RunL() leaves,It should be handled here.
// -------------------------------------------------------------------------
//
TInt CMusUiMmcMonitor::RunError( TInt aError )
    {
	MUS_LOG( "mus: [MUSUI ]  -> CMusUiMmcMonitor::RunError" );
    // Nothing can be done here.
    aError = KErrNone;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiMmcMonitor::RunError" );
    return aError;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiMmcMonitor::DoCancel()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiMmcMonitor::DoCancel" );
    iFs.NotifyChangeCancel();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiMmcMonitor::DoCancel" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiMmcMonitor::MonitorMmc()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiMmcMonitor::MonitorMmc" );
    if( !IsActive() && iRemovableDrive && !iDriveRemoved )
        {
        MUS_LOG( "mus: [MUSUI ]     CMusUiMmcMonitor::MonitorMmc: calling iFs.NotifyChange" );
        iFs.NotifyChange( ENotifyDisk, iStatus );
        SetActive();
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiMmcMonitor::MonitorMmc" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiMmcMonitor::DriveRemoved()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiMmcMonitor::DriveRemoved" );
    if ( iDriveRemoved )
        {
        return iDriveRemoved;
        }

    TDriveInfo driveInfo;
    iFs.Drive( driveInfo, iDriveNumber ); 

    if ( driveInfo.iType == EMediaNotPresent )
        {
        iDriveRemoved = ETrue;
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiMmcMonitor::DriveRemoved" );
    return iDriveRemoved;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiMmcMonitor::IsRemovableDrive( TDriveNumber aDriveNumber )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiMmcMonitor::IsRemovableDrive" );
    TDriveInfo driveInfo;
    iFs.Drive( driveInfo, aDriveNumber );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiMmcMonitor::IsRemovableDrive" );
    return ( driveInfo.iDriveAtt & KDriveAttRemovable );
    }

//  End of File
