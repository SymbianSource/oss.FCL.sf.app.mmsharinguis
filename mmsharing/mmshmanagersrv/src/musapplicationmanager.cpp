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
#include "mussettings.h"
/* PS keys related to NMusSessionInformationApi defined here */
#include "mussesseioninformationapi.h"
#include "muscleanupresetanddestroy.h"
#include "lcengine.h"

#include <apacmdln.h>
#include <apgtask.h>
#include <e32cmn.h>
#include <e32property.h>
#include <utf.h>
#include <apadef.h>


// CONSTANTS
_LIT8( KDefaultEngineName, "MultimediaSharing" );
_LIT( KMusUiProcessName, "musui.exe" );

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

    SetPropertyL( NMusSessionApi::KFastMode, MusSettingsKeys::EFastModeOff );

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
    
    TFindProcess findProcess( KMusUiProcessName );
    TFullName name;
    return ( findProcess.Next( name ) == KErrNone );
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
        MUS_LOG( "mus: [MUSSRV]     Starting app" );
        
        RProcess process;
        CleanupClosePushL( process );
        HBufC* args = CreateCommandLineArgsLC();
        User::LeaveIfError( process.Create( KMusUiProcessName, *args ) );
        process.Resume();
        CleanupStack::PopAndDestroy( args );
        CleanupStack::PopAndDestroy( &process );
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

    if( ApplicationRunning() )
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
    
    // TODO: How to bring a QT application to foreground?
    
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
	TRAPD( error,  		   
    if ( aUseCase == MultimediaSharing::EMusLiveVideo &&
            MultimediaSharingSettings::VideoDirectionL() == 
                                            MusSettingsKeys::ETwoWayVideo )
        {
		MUS_LOG( "mus: [MUSSRV]  UseCase MultimediaSharing::EMusTwoWayVideo" );
        aUseCase = MultimediaSharing::EMusTwoWayVideo;
        }
		)
	if ( error == KErrNoMemory )
		{
		User::Leave( error );
		}
    // set use case property
    SetPropertyL( NMusSessionApi::KUseCase, ( TInt ) aUseCase );

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

    // fast mode  
    TInt fastMode;
    User::LeaveIfError( RProperty::Get( NMusSessionApi::KCategoryUid, 
                                        NMusSessionApi::KFastMode, 
                                        fastMode ) );  
    if ( fastMode != MusSettingsKeys::EFastModeDisabled )
        {
        lex.Assign( aSessionParameters.MdcaPoint( KFastMode ) );
        lex.Val( val );
        SetPropertyL( NMusSessionApi::KFastMode, val );
        }
    else
        {        
        MUS_LOG( "mus: [MUSSRV]  fast mode disabled, do not update it" );
        }
    
   // set status property
    SetStatusL( aStatus );

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
// Reads the callprovider description from the PS key
// return aProvider will have the proper callprovider name.
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::CallProviderL(TDes8& aProvider)
    {
    MUS_LOG( "mus: [MUSSRV] -> CMusApplicationManager::CallProviderL");
    const TInt KMusMgrMaxPluginNameLen = 256; 
    TBuf<KMusMgrMaxPluginNameLen> pluginName;
    
    User::LeaveIfError(RProperty::Get( 
                                NMusSessionInformationApi::KCategoryUid,
                                NMusSessionInformationApi::KMUSCallProvider,
                                pluginName ));
    User::LeaveIfError( 
            CnvUtfConverter::ConvertFromUnicodeToUtf8( aProvider, pluginName ));
    
    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::CallProviderL");
    }

// -----------------------------------------------------------------------------
// Resolves name of the ecom plugin which implements the livecomms engine api
// by using the callprovider
// -----------------------------------------------------------------------------
//
void CMusApplicationManager::ResolvePluginNameL(TDes8& aPluginName)
    {
    MUS_LOG( "mus: [MUSSRV] -> CMusApplicationManager::ResolvePluginNameL");
    
    TBuf8<RProperty::KMaxPropertySize> providerName ;

    // Read the call provider name 
    CallProviderL( providerName );

    RImplInfoPtrArray pluginArray;
    MusCleanupResetAndDestroy<RImplInfoPtrArray>::PushL( pluginArray );

    TEComResolverParams resolverParams;
    
    resolverParams.SetDataType ( providerName );
    resolverParams.SetWildcardMatch (ETrue);
    
    // Use default plugin even if listing fails
    TRAPD( err, 
       REComSession::ListImplementationsL(KLcEngineInterfaceUid, resolverParams, pluginArray) );
    MUS_LOG1( "mus: [MUSSRV]    List implementations compl:%d", err );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    
    if( pluginArray.Count() > 0 )
        {
        CImplementationInformation* info = pluginArray[ 0 ];
        __ASSERT_ALWAYS( aPluginName.MaxLength() >= info->DataType().Length(), 
                         User::Leave( KErrArgument ) );
        aPluginName.Copy( info->DataType() );
        }
    else
        {
        __ASSERT_ALWAYS( aPluginName.MaxLength() >= KDefaultEngineName().Length(), 
                                 User::Leave( KErrArgument ) );
        aPluginName.Copy( KDefaultEngineName() );
        }
    CleanupStack::PopAndDestroy();//pluginArray
    MUS_LOG( "mus: [MUSSRV] <- CMusApplicationManager::ResolvePluginNameL");
    }

// -----------------------------------------------------------------------------
// Creates command line used when starting application.
// -----------------------------------------------------------------------------
//
HBufC* CMusApplicationManager::CreateCommandLineArgsLC()
    {
    MUS_LOG( "mus: [MUSSRV] -> CMusApplicationManager::CreateCommandLineLC");
    
    TBuf8<KMusMgrMaxPluginNameLen> pluginNameUtf8;
    ResolvePluginNameL( pluginNameUtf8 );
    
    MUS_LOG_TDESC8( "mus: [MUSSRV]     Plugin name:", pluginNameUtf8 );
    
    HBufC* commandLineArgs = HBufC::NewLC( KMusMgrMaxPluginNameLen );
    TPtr commandLineArgsPtr( commandLineArgs->Des() );
    User::LeaveIfError( 
        CnvUtfConverter::ConvertToUnicodeFromUtf8( 
            commandLineArgsPtr, pluginNameUtf8 ) );
    
    MUS_LOG_TDESC( "mus: [MUSSRV] <- command line args:", commandLineArgsPtr );
    
    return commandLineArgs;
    }

// End of File
