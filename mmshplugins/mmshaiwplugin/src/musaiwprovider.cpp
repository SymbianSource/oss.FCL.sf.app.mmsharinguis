/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The ECOM interface implementation.
*
*/




#include "musaiwprovider.h"
#include "musaiwprovider.hrh"
#include "muslogger.h"
#include "musmanagercommon.h"
#include "musmanager.h"
#include "musuid.hrh"
#include "musresourcefinderutil.h"
#include "mussettings.h"
#include "mussesseioninformationapi.h"

#include <musresourceproperties.h>

#include <featmgr.h>
#include <e32property.h>
#include <AiwCommon.h>               // AIW classes and definitions
#include <AiwCommon.hrh>             // AIW constants
#include <AiwMenu.h>                 // AIW Menu pane
#include <aknnotewrappers.h>         // CAknInformationNote
#include <avkon.rsg>                 // Resources
#include <apacmdln.h>
#include <apgtask.h>
#include <bautils.h>
#include <charconv.h>
#include <eikenv.h>
#include <musaiwproviderui.rsg>
#include <StringLoader.h>
#include <AknGlobalNote.h>
#include <utf.h>
#include <features.hrh>
#include <aknenv.h>

////////  public -- constructor and destructor  ////////

_LIT( KMusAiwProviderResource, "musaiwproviderui.rsc" );

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusAiwProvider* CMusAiwProvider::NewL()
    {
    MUS_LOG( "mus: [AIWPRO]  -> CMusAiwProvider::NewL" )
    CMusAiwProvider* self = new( ELeave ) CMusAiwProvider;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAiwProvider::~CMusAiwProvider()
    {
    MUS_LOG( "mus: [AIWPRO]  -> CMusAiwProvider::~CMusAiwProvider" )
    delete iResourceFileName ;
    iResourceFileName = NULL ;  

    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::~CMusAiwProvider" )
    }


////////  private -- constructors  ////////

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusAiwProvider::CMusAiwProvider()
:   iIsWarning( ETrue )
    {
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::ConstructL()
    {
    MUS_LOG( "mus: [AIWPRO]  -> CMusAiwProvider::ConstructL" )
    iResourceFileName = MusResourceFinderUtil::ResourcePathL(
                                                    KMusAiwProviderResource );
    
    DoInitialiseL();
    
    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::ConstructL" )
    }



////////  public -- from CAiwServiceIfBase  ////////

// -----------------------------------------------------------------------------
// Called by the AIW framework to initialize the provider with necessary
// information.
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::InitialiseL(
    MAiwNotifyCallback&   /* aFrameworkCallback*/,
    const RCriteriaArray& /* aInterest */ )
    {
    MUS_LOG( "mus: [AIWPRO]  -> CMusAiwProvider::InitialiseL")
    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::InitialiseL")
    }





////////  public -- from CAiwServiceIfMenu  ////////

// -----------------------------------------------------------------------------
// Initializes menu pane by adding provider-specific menu items.
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::InitializeMenuPaneL(
        CAiwMenuPane&               aMenuPane,
        TInt                        aIndex,
        TInt                        /* aCascadeId */,
        const CAiwGenericParamList& /* aInParamList */ )
    {
	FeatureManager::InitializeLibL();
    TBool support = FeatureManager::FeatureSupported( KFeatureIdMultimediaSharing );
	FeatureManager::UnInitializeLib();
	if( support )
		{
	    DoInitializeMenuPaneL( aMenuPane, aIndex );
		}
	}
	
	
// -----------------------------------------------------------------------------
// Initializes menu pane by adding provider-specific menu items.
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::DoInitializeMenuPaneL(
        CAiwMenuPane&               aMenuPane,
        TInt                        aIndex)
    {
    MUS_LOG1( "mus: [AIWPRO]  -> CMusAiwProvider::DoInitializeMenuPaneL( %d )",
              aIndex )
    
    if( iResourceFileName == NULL )
        {
        iResourceFileName = MusResourceFinderUtil::ResourcePathL(
                                                    KMusAiwProviderResource );
        }   
    TFileName filename ( *iResourceFileName );
    MUS_LOG_TDESC( "mus: [AIWPRO]     Resource filename: ", filename ) 
    
    if( ApplicationRunningL() )
    	{
        MUS_LOG( "mus: [AIWPRO]     Application already running:\
                 giving _continue sharing_ menu command" )
        // Application is already started.  Add resources to menu items.
        aMenuPane.AddMenuItemsL( filename,
                                 R_MUSAIWPROVIDER_RUNNING_CASCADED_SUBMENU,
                                 KMusAiwProviderUid,
                                 aIndex );
        
        }
	else
		{
		// check for variant
	    if ( MultimediaSharingSettings::OperatorVariantSettingL()
	            == MusSettingsKeys::EOperatorSpecific )
	        {
	       	OperatorVariantStartUpL( filename, aMenuPane, aIndex );
	        }
		else
			{
	        NoVariationStartUpL( filename, aMenuPane, aIndex );
	        }	
		}   

    
    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::DoInitializeMenuPaneL" )
    }

// -----------------------------------------------------------------------------
// 
// 
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::DoInitialiseL()
    {
    MUS_LOG( "mus: [AIWPRO]  -> CMusAiwProvider::DoInitialiseL")

    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::DoInitialiseL")
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CMusAiwProvider::ApplicationRunningL()
    {
    MUS_LOG( "mus: [AIWPRO]  -> CMusAiwProvider::ApplicationRunningL" )    
    RWsSession wsSession;
    User::LeaveIfError( wsSession.Connect() );
    CleanupClosePushL( wsSession );
    TApaTaskList taskList( wsSession );
    TUid appUid;
    appUid.iUid = KMusUiUid;
    TApaTask task = taskList.FindApp( appUid );
    TBool running = task.Exists();
    CleanupStack::PopAndDestroy( &wsSession );
    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::ApplicationRunningL" )
    return running;
    }
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::NoVariationStartUpL( TFileName&     aFileName,
										  CAiwMenuPane& aMenuPane,
									      TInt          aIndex )
	{
    MUS_LOG( "mus: [AIWPRO]  -> CMusAiwProvider::NoVariationStartUpL" )    
	CMusManager* manager =  CMusManager::NewLC();
	iAvailability = manager->AvailabilityL();
    
    /* Multimediasharing Menu items should be shown in call menu when 
       1.SIP Registration is still ongoing or it has failed for some reason.
       2.Regardless of Options execution ( All options are just informative.
         Cant be considered as Error situation.)
       3.When iAvailability plugin says OK ( Based on call,network,settingui item)
       4.Also in confrence and hold cases
    */        
    if ( iAvailability == MultimediaSharing::ESipRegistrationPending || 
         iAvailability == MultimediaSharing::ESipOptionsSent ||
         iAvailability == MultimediaSharing::EMultimediaSharingAvailable ||
         iAvailability == MultimediaSharing::EErrCallOnHold ||
         iAvailability == MultimediaSharing::EErrConferenceCall ||             
         iAvailability == MultimediaSharing::ESipOptionsNotCapable ||
         iAvailability == MultimediaSharing::ESipOptionsNotSent ||
         iAvailability == MultimediaSharing::EErrNetwork ||
         iAvailability == MultimediaSharing::EManualActivation ||
         iAvailability == MultimediaSharing::EErrConnection ||
         iAvailability == MultimediaSharing::EErrSipRegistration )           
        {
        MUS_LOG1( "mus: [AIWPRO]  Showing AIW Menu -> iAvailability = %d ",iAvailability )
        TInt resourceID =  R_MUSAIWPROVIDER_BASIC_CASCADED_SUBMENU ;           
        if ( !FeatureManager::FeatureSupported( KFeatureIdCamera ) )
            {
             MUS_LOG( "mus: [AIWPRO]  Camera Not Available" )
             resourceID =  R_MUSAIWPROVIDER_NOCAMERA_CASCADED_SUBMENU ;                 
            }            
            aMenuPane.AddMenuItemsL( aFileName,
                                 resourceID,
                                 KMusAiwProviderUid,
                                 aIndex );
        }
    else
        {
        MUS_LOG1( "mus: [AIWPRO] Not Showing AIW Menu -> iAvailability = %d ",iAvailability )
        }
    CleanupStack::PopAndDestroy( manager );	
    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::NoVariationStartUpL" )   
	}
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::OperatorVariantStartUpL( TFileName&     aFileName,
										      CAiwMenuPane& aMenuPane,
									          TInt          aIndex )
	{
    MUS_LOG( "mus: [AIWPRO]  -> CMusAiwProvider::OperatorVariantStartUpL" )   

    CMusManager* manager = CMusManager::NewLC();
	iAvailability = manager->AvailabilityL();

    // In operator specific mode Multimediasharing Menu items 
    // should be shown in call menu only if iAvailability plugin returns OK 
    if ( iAvailability == MultimediaSharing::EMultimediaSharingAvailable )           
        {
        MUS_LOG1( "mus: [AIWPRO]  Showing AIW Menu -> iAvailability = %d ", iAvailability )
        TInt resourceID =  R_MUSAIWPROVIDER_BASIC_CASCADED_SUBMENU ;           
        if ( !FeatureManager::FeatureSupported( KFeatureIdCamera ) )
            {
            MUS_LOG( "mus: [AIWPRO]  Camera Not Available" )
            resourceID =  R_MUSAIWPROVIDER_NOCAMERA_CASCADED_SUBMENU ;                 
            }            
            aMenuPane.AddMenuItemsL( aFileName,
                                     resourceID,
                                     KMusAiwProviderUid,
                                     aIndex );
        }
    else
        {
        MUS_LOG1( "mus: [AIWPRO] Not Showing AIW Menu -> iAvailability = %d ",iAvailability )
        }
        
    CleanupStack::PopAndDestroy( manager );	

    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::OperatorVariantStartUpL" )  
	}
	
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
HBufC* CMusAiwProvider::ErrorNoteTextLC( TInt aError )
    {
    MUS_LOG1( "mus: [AIWPRO]  -> CMusAiwProvider::ErrorNoteTextLC( %d )",
              aError )   
    
    TFileName filename( *iResourceFileName );    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    RResourceFile resourceFile;
    resourceFile.OpenL( fs, filename );
    CleanupClosePushL( resourceFile );

    // Fetch error text
    HBufC8* dlgPrompt8 = ErrorNoteText8L( aError, resourceFile );
    CleanupStack::PushL( dlgPrompt8 );
    const TUint8* ptr8 = dlgPrompt8->Des().Ptr();
    TPtrC ptr16( ( TUint16* ) ptr8, dlgPrompt8->Length()/2 );
    HBufC* dlgPrompt = ptr16.AllocL();

    // CnvUtfConverter::ConvertToUnicodeFromUtf8L( *dlgPrompt8 );            
    CleanupStack::PopAndDestroy( dlgPrompt8 );

    CleanupStack::PopAndDestroy( &resourceFile );
    CleanupStack::PopAndDestroy( &fs );        
    CleanupStack::PushL( dlgPrompt );
    
    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::ErrorNoteTextLC" )
    return dlgPrompt;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
HBufC8* CMusAiwProvider::ErrorNoteText8L( TInt aError,
                                          RResourceFile& aResourceFile )
    {
    MUS_LOG1( "mus: [AIWPRO]  -> CMusAiwProvider::ErrorNoteTextL( %d )",
              aError )
              
    HBufC8* errorText = NULL;
    
    switch( aError )
        {
        case MultimediaSharing::EErrCallOnHold:
            {
            errorText = aResourceFile.AllocReadL( R_MUSAIWPROVIDER_ON_HOLD );
            iIsWarning = ETrue ;
            break;
            }        
        case MultimediaSharing::EErrNetwork:
            {
            errorText = aResourceFile.AllocReadL( 
                                    R_MUSAIWPROVIDER_NETWORK_INCOMPATIBLE );
            iIsWarning = ETrue ;
            break;
            }
        case MultimediaSharing::EErrConferenceCall:
            {
            errorText = aResourceFile.AllocReadL( 
                                    R_MUSAIWPROVIDER_CONFERENCE_CALL );
            iIsWarning = ETrue ;
            break;
            }
        case MultimediaSharing::ESipOptionsNotCapable:
            {
            errorText = aResourceFile.AllocReadL( 
                                    R_MUSAIWPROVIDER_RECIPIENT_INCAPABLE );
            iIsWarning = ETrue ;
            break;
            }
        case MultimediaSharing::EErrSipRegistration:
            {
            errorText = aResourceFile.AllocReadL( R_MUSAIWPROVIDER_NO_REG );
            iIsWarning = ETrue ;
            break;
            }
        case MultimediaSharing::EErrConnection:
            {
            errorText = aResourceFile.AllocReadL( R_MUSAIWPROVIDER_NO_REG );
            iIsWarning = ETrue ;
            break;
            }
        default:
            {
            errorText = aResourceFile.AllocReadL( R_MUSAIWPROVIDER_APP_GENERAL );  
            iIsWarning = EFalse ; 
            break;
            }
        }
    
    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::ErrorNoteTextL" )
    return errorText;
    }


// -----------------------------------------------------------------------------
// Handles a service command invoked by the Handler.
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::HandleServiceCmdL(
    const TInt&                 aCmdId,
    const CAiwGenericParamList& aInParamList,
    CAiwGenericParamList&       aOutParamList,
    TUint                       aCmdOptions,
    const MAiwNotifyCallback*   aCallback )
    { 
    HandleMenuCmdL( aCmdId,
                    aInParamList,
                    aOutParamList,
                    aCmdOptions,
                    aCallback );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::ShowErrorNoteL( TInt aError )
    {
    MUS_LOG1( "mus: [AIWPRO]  -> CMusAiwProvider::ShowErrorNote( %d )",
              aError )        
    HBufC* dlgPrompt = ErrorNoteTextLC( aError );    
    MUS_LOG_TDESC( "mus: [AIWPRO]  Error : ", ( *dlgPrompt  ) )
    CAknGlobalNote* note = CAknGlobalNote::NewLC();
    TRequestStatus status;
    if( iIsWarning )
        {
        note->ShowNoteL( status,EAknGlobalInformationNote,*dlgPrompt ); 
        }
    else
        {
        note->ShowNoteL( status,EAknGlobalErrorNote,*dlgPrompt );   
        }    
    User::WaitForRequest( status );        
    CleanupStack::PopAndDestroy( note );
    CleanupStack::PopAndDestroy( dlgPrompt );    
    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::ShowErrorNote" )
    }


// -----------------------------------------------------------------------------
// Handles a menu command invoked by the Handler.
// -----------------------------------------------------------------------------
//
void CMusAiwProvider::HandleMenuCmdL(
                                TInt                        aMenuCmdId,
                                const CAiwGenericParamList& /*aInParamList*/,
                                CAiwGenericParamList&       /*aOutParamList*/,
                                TUint                       /*aCmdOptions*/,
                                const MAiwNotifyCallback*   /*aCallback*/ )
    {
    MUS_LOG( "mus: [AIWPRO]  -> CMusAiwProvider::HandleMenuCmdL" )

    TInt error( KErrNone );

    CMusManager* manager = CMusManager::NewLC();
    
    if ( iAvailability == MultimediaSharing::EManualActivation )
    	{
    	MUS_LOG( "mus: [AIWPRO]  Manual activation" )
    	TRAP( error, manager->HandleCommandL( MultimediaSharing::ECommandManualActivation ));	
    	iAvailability = manager->AvailabilityL(); 
    	}

    switch ( aMenuCmdId )
        {
        case EMusCommandLiveShare:
            {
            MUS_LOG( "mus: [AIWPRO]     \
                    EMusCommandLiveShare: Will now attempt to start Mus." )
            TRAP( error, manager->StartApplicationL( 
                                        MultimediaSharing::EMusLiveVideo ) );
            break;
            }
        case EMusCommandClipShare:
            {
            MUS_LOG( "mus: [AIWPRO]     \
                    EMusCommandClipShare: Will now attempt to start Mus." )
            TRAP( error, manager->StartApplicationL(
                                        MultimediaSharing::EMusClipVideo ) );
            break;
            }
        case EMusCommandImageShare:
            {
            MUS_LOG( "mus: [AIWPRO]     \
                    EMusCommandImageShare: Will now attempt to start Mus." )
            TRAP( error, manager->StartApplicationL(
                                        MultimediaSharing::EMusStillImage ) );
            break;
            }
        case EMusCommandContinue:
            {
            MUS_LOG( "mus: [AIWPRO]     \
                    EMusCommandContinue: Will now attempt to start/continue Mus." )
            TRAP( error, manager->StartApplicationL( 
                                        MultimediaSharing::EMusContinue ) ); 
            break;
            }
        default:
            {
            MUS_LOG( "mus: [AIWPRO]     \
                    Unknown command. Leave with KErrArgument" )
            User::Leave( KErrArgument );
            break;
            }
        }

    CleanupStack::PopAndDestroy( manager );

    if( error != KErrNone )
        {
        ShowErrorNoteL( error );
        }

    MUS_LOG( "mus: [AIWPRO]  <- CMusAiwProvider::HandleMenuCmdL" )
    }

// end of file
