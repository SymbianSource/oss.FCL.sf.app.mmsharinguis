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


#include "musunittesting.h"
#include "muslogger.h"
#include "musapplicationmanager.h"
#include "musmanagercommon.h"
#include "musmanagerservercommon.h"
#include "mussessionproperties.h"
#include "musuid.hrh"

#include <apacmdln.h>
#include <apgtask.h>
//#include <badesca.h>
#include <e32cmn.h>
#include <e32property.h>

// CONSTANTS


// -----------------------------------------------------------------------------
// CMusApplicationManager::NewL
// -----------------------------------------------------------------------------
//
CMusApplicationManager* CMusApplicationManager::NewL()
    {
    MUS_LOG( "mus: [MUSSRV] -> CMusApplicationManager* \
    				 CMusApplicationManager::NewL()" );

    CMusApplicationManager* self = CMusApplicationManager::NewLC();
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager* \
    				 CMusApplicationManager::NewL()" );
    return self;
    }


// -----------------------------------------------------------------------------
// CMusApplicationManager::NewLC
// -----------------------------------------------------------------------------
//
CMusApplicationManager* CMusApplicationManager::NewLC()
    {
    MUS_LOG( "mus: [MUSSRV] -> CMusApplicationManager* \
    				 CMusApplicationManager::NewLC()" );

    CMusApplicationManager* self = new( ELeave ) CMusApplicationManager();
    CleanupStack::PushL( self );
    self->ConstructL();

    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager* \
    				 CMusApplicationManager::NewLC()" );
    return self;
    }


// -----------------------------------------------------------------------------
// CMusApplicationManager::~CMusApplicationManager
// -----------------------------------------------------------------------------
//
CMusApplicationManager::~CMusApplicationManager()
    {
    MUS_LOG( "mus: [MUSSRV] -> \
    					CMusApplicationManager::~CMusApplicationManager()" );

    iApaSession.Close();
    iWsSession.Close();

    MUS_LOG( "mus: [MUSSRV] <- \
    					CMusApplicationManager::~CMusApplicationManager()" );
    }


// -----------------------------------------------------------------------------
// CMusApplicationManager::ConstructL
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::ConstructL()
    {
    MUS_LOG( "mus: [MUSSRV] -> CMusApplicationManager::ConstructL()" );

    User::LeaveIfError( iApaSession.Connect() );
    User::LeaveIfError( iWsSession.Connect() );

    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::ConstructL()" );
    }


// -----------------------------------------------------------------------------
// CMusApplicationManager::CMusApplicationManager
// -----------------------------------------------------------------------------
//
CMusApplicationManager::CMusApplicationManager()
    {
    }


// -----------------------------------------------------------------------------
// CMusApplicationManager::ApplicationRunning
// -----------------------------------------------------------------------------
//
TBool CMusApplicationManager::ApplicationRunning()
    {
    MUS_LOG( "mus: [MUSSRV] <> CMusApplicationManager::ApplicationRunning()" );
    return GetApaTask().Exists();
    }


// -----------------------------------------------------------------------------
// CMusApplicationManager::ApplicationRunning
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::StartApplicationL()
    {
    MUS_LOG( "mus: [MUSSRV] -> CMusApplicationManager::StartApplicationL()" );

    if( ApplicationRunning() )
        {
        ShowApplicationL();
        }
    else
        {
        TUid appUid;
        appUid.iUid = KMusUiUid;
        
        TThreadId aThreadId;
        
        User::LeaveIfError( 
                    iApaSession.StartDocument( KNullDesC, appUid, aThreadId) );
        }

    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::StartApplicationL()" );
    }


// -----------------------------------------------------------------------------
// CMusApplicationManager::ApplicationRunning
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::StopApplicationL()
    {
    MUS_LOG( "mus: [MUSSRV] -> CMusApplicationManager::StopApplicationL()" );

    TApaTask task = GetApaTask();
    if( task.Exists() )
        {
        SetPropertyL( NMusSessionApi::KStatus, 
                      MultimediaSharing::EMultimediaSharingNotAvailable );
        }

    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::StopApplicationL()" );
    }


// -----------------------------------------------------------------------------
// CMusApplicationManager::ShowApplication
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::ShowApplicationL()
    {
    MUS_LOG( "mus: [MUSSRV] -> CMusApplicationManager::ShowApplicationL()" );
    
    // does a task for multimediasharing exist?
    TApaTask task = GetApaTask();
    if( task.Exists() )
        {
        task.BringToForeground();
        }
    
    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::ShowApplicationL()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::WriteSessionPropertiesL(
                        MultimediaSharing::TMusUseCase aUseCase,
                        MultimediaSharing::TMusAvailabilityStatus aStatus,
                        MDesCArray& aSessionParameters )
    {
     MUS_LOG2( "mus: [MUSSRV] -> \
               CMusApplicationManager::WriteSessionPropertiesL( %d, %d )",
               ( TInt ) aUseCase, ( TInt ) aStatus );

    // set use case property
    SetPropertyL( NMusSessionApi::KUseCase,
                  ( TInt ) aUseCase );

    WriteSessionPropertiesL( aStatus, aSessionParameters );


    MUS_LOG( "mus: [MUSSRV] <- \
             CMusApplicationManager::WriteSessionPropertiesL()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::WriteSessionPropertiesL(
                        MultimediaSharing::TMusAvailabilityStatus aStatus,
                        MDesCArray& aSessionParameters )
    {
     MUS_LOG1( "mus: [MUSSRV] -> \
               CMusApplicationManager::WriteSessionPropertiesL( %d )",
               ( TInt ) aStatus );

    // set tel number property
    SetPropertyL( NMusSessionApi::KTelNumber,
                  aSessionParameters.MdcaPoint( KTelNumber ) );

    // set sip address property
    SetPropertyL( NMusSessionApi::KRemoteSipAddress,
                  aSessionParameters.MdcaPoint( KSipAddress ) );

    // set sip address proposal property
    SetPropertyL( NMusSessionApi::KRemoteSipAddressProposal,
                  aSessionParameters.MdcaPoint( KSipAddressProposal ) );

    // set video codec property
    SetPropertyL( NMusSessionApi::KVideoCodecs,
                  aSessionParameters.MdcaPoint( KVideoCodec ) );

    // set contact id property
    TLex lex( aSessionParameters.MdcaPoint( KContactId ) );
    TInt val;
    lex.Val( val );

    SetPropertyL( NMusSessionApi::KContactId, val );

    // set contact id property
    lex.Assign( aSessionParameters.MdcaPoint( KSipProfileId ) );
    lex.Val( val );

    SetPropertyL( NMusSessionApi::KSipProfileId, val );

    // contact name
    SetPropertyL( NMusSessionApi::KContactName,
                  aSessionParameters.MdcaPoint( KContactName ) );

    // set status property
    SetStatusL( aStatus );
    
    // set privacy property
    lex.Assign( aSessionParameters.MdcaPoint( KPrivacyStatus ) );
    lex.Val( val );

    SetPropertyL( NMusSessionApi::KPrivacyStatus, val );

    MUS_LOG( "mus: [MUSSRV] <- \
             CMusApplicationManager::WriteSessionPropertiesL()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::SetStatusL( 
                        MultimediaSharing::TMusAvailabilityStatus aStatus )
    {
    MUS_LOG1( "mus: [MUSSRV] -> CMusApplicationManager::SetStatusL( %d )",
              ( TInt ) aStatus );
    SetPropertyL( NMusSessionApi::KStatus, ( TInt ) aStatus );
    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::SetStatusL()" );
    }

	    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::SetIndicatorStatusL( TBool aLaunch )
    {
    TInt launch = aLaunch;
    MUS_LOG1( "mus: [MUSSRV] -> CMusApplicationManager::SetStatusL( %d )",
              launch );
              
    TInt indicatorStatus;
    TInt ret = RProperty::Get( NMusSessionApi::KCategoryUid, 
                               NMusSessionApi::KMusIndicatorStatus,
                               indicatorStatus );

    if ( ret != KErrNone && ret != KErrNotFound ) 
        {
        User::Leave( ret );
        }

    // set property only if not defined yet or if value changed
    if ( ret == KErrNotFound || indicatorStatus != launch )
        {
        SetPropertyL( NMusSessionApi::KMusIndicatorStatus, launch );    
        }
    
    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::SetStatusL()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::SetPropertyL( TUint aProperty, 
                                           const TDesC& aValue )
    {
    MUS_LOG1( "mus: [MUSSRV] -> CMusApplicationManager::SetPropertyL( %d )",
               aProperty );
    MUS_LOG_TDESC( "mus: [MUSSRV]    New property value: ", aValue );

    // set tel number property
    TInt retval = RProperty::Define( NMusSessionApi::KCategoryUid,
                                     aProperty,
                                     RProperty::EText );

    if( retval != KErrAlreadyExists && retval != KErrNone )
        {
        User::Leave( retval );
        }
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        aProperty,
                                        aValue ) );

    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::SetPropertyL()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::SetPropertyL( TUint aProperty, TInt aValue )
    {
    MUS_LOG2( "mus: [MUSSRV] -> CMusApplicationManager::SetPropertyL( %d, %d )",
             ( TInt ) aProperty, aValue );

    // set tel number property
    TInt retval = RProperty::Define( NMusSessionApi::KCategoryUid,
                                     aProperty,
                                     RProperty::EInt );
    
    if( retval != KErrAlreadyExists && retval != KErrNone )
        {
        User::Leave( retval );
        }
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        aProperty,
                                        aValue ) );

    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::SetPropertyL()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TApaTask CMusApplicationManager::GetApaTask() 
    {
    MUS_LOG( "mus: [MUSSRV] <> CMusApplicationManager::GetApaTask()" );
    TUid appUid;
    appUid.iUid = KMusUiUid;
    return TApaTaskList( iWsSession ).FindApp( appUid );
    }
