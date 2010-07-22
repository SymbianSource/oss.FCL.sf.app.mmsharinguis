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
* Description:  project specification
*
*/



// USER
#include "muswpadapter.h"
#include "muswpitem.h"
#include "mussettings.h"
#include "muslogger.h"

// SYSTEM
#include <muswpadapterresource.rsg>
#include <CWPCharacteristic.h>
#include <CWPParameter.h>
#include <utf.h>
#include <WPAdapterUtil.h>




// CONSTANTS

_LIT( KMusAppID, "w9017" );
_LIT8( KSipAppID8, "w9010" );

_LIT( KMusWpAdapterName, "muswpadapterresource" );

_LIT( KMUsWpCharacteristicNameCapability, "CAPABILITY" );
_LIT( KMUsWpCharacteristicNameActivation, "ACTIVATION" );

_LIT( KMusWpParamNameEdgeDtm, "EDGEDTM" );
_LIT( KMusWpParamNameInternetSignaling, "INTERNETSIGNALING" );
_LIT( KMusWpParamNameQuery, "QUERY" );
_LIT( KMusWpParamNameCondition, "CONDITION" );

const TUint32 KMusWpNotSet = 0;



// -----------------------------------------------------------------------------
// Symbian two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusWpAdapter* CMusWpAdapter::NewL()
    {
    MUS_LOG( "mus: [MUSWP]  ->  CMusWpAdapter::NewL" )
    CMusWpAdapter* self = new( ELeave ) CMusWpAdapter;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMusWpAdapter::~CMusWpAdapter()
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::~CMusWpAdapter" )
    delete iAppID;
    delete iWpItem;
    delete iNewWpItem;
    delete iSummaryTitle;
    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::~CMusWpAdapter" )
    }
    

// -----------------------------------------------------------------------------
// From base class CWAdapter. Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
TInt CMusWpAdapter::ItemCount() const
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::ItemCount()" )
    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::ItemCount()" )
    
    if ( iWpItem )
        {
        return 1;
        }

    return 0;
    }


// -----------------------------------------------------------------------------
// From base class CWAdapter. Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
const TDesC16& CMusWpAdapter::SummaryTitle( TInt /*aIndex*/ ) const
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::SummaryTitle()" )
    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::SummaryTitle()" )
    
    return *iSummaryTitle;
    }


// -----------------------------------------------------------------------------
// From base class CWAdapter. Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
const TDesC16& CMusWpAdapter::SummaryText( TInt /*aIndex*/ ) const
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::SummaryText( ... )" )
    if( iWpItem )
        {
        return iWpItem->Name();   
        }

    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::SummaryText( ... )" )
    return *iSummaryTitle;
    }


// -----------------------------------------------------------------------------
// From base class CWAdapter. Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
void CMusWpAdapter::SaveL( TInt /*aItem*/ )
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::SaveL" )
    
    if ( iWpItem )
        {
        iWpItem->SaveL();
        }
        
    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::SaveL" )
    }


// -----------------------------------------------------------------------------
// From base class CWAdapter. Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
TBool CMusWpAdapter::CanSetAsDefault( TInt /*aIndex*/ ) const
    {
    return EFalse;
    }


// -----------------------------------------------------------------------------
// From base class CWAdapter. Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
void CMusWpAdapter::SetAsDefaultL( TInt /*aIndex*/ )
    {
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// From base class CWAdapter. Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
TInt CMusWpAdapter::DetailsL( TInt /*aItem*/, MWPPairVisitor& /*aVisitor */ )
    {
    return KErrNotSupported;
    }


// -----------------------------------------------------------------------------
// From base class CWPAdapter. Overrides the empty implementation of that class.
// -----------------------------------------------------------------------------
//
void CMusWpAdapter::SettingsSavedL( const TDesC8& aAppIdOfSavingItem,
                                     const TDesC8& aAppRef,
                                     const TDesC8& aStorageIdValue )
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::SettingsSavedL" )    

    // If settings are saved by SIPAdapter...
    if ( aAppIdOfSavingItem == KSipAppID8 )
        {
        // ...and are ment for us...
        if ( iWpItem )
            {
            HBufC8* tmpValue = HBufC8::NewLC( 4*iWpItem->ToAppRef().Length() );
            TPtr8 ptrTmpValue( tmpValue->Des() );
            CnvUtfConverter::ConvertFromUnicodeToUtf8( ptrTmpValue, 
                                                       iWpItem->ToAppRef() );
            
            if ( aAppRef == *tmpValue )
                {
                MUS_LOG( "   CMusWpAdapter, Prepared to save setting" )
                // ...prepare to save profile Id
                TLex8 lex( aStorageIdValue );
                User::LeaveIfError( lex.Val( iProfileId, EDecimal ) );
                iProfileIdReceived = ETrue;
                }
            
            CleanupStack::PopAndDestroy( tmpValue );
            }
        }    
    
    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::SettingsSavedL" )
    };


// -----------------------------------------------------------------------------
// From base class CWPAdapter. Overrides the empty implementation of that class.
// -----------------------------------------------------------------------------
//
void CMusWpAdapter::SavingFinalizedL()
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::SavingFinalizedL" )

    if ( iProfileIdReceived )
        {
        MultimediaSharingSettings::SetSipProfileSettingL( 
                                        static_cast<TInt>(iProfileId) );
        }
        
    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::SavingFinalizedL" )
    }


// -----------------------------------------------------------------------------
// From base class CWPAdapter, which derives this from MWPVisitor.
// Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
void CMusWpAdapter::VisitL( CWPCharacteristic& aCharacteristic )
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::VisitL (Characteristic)" )
    switch ( aCharacteristic.Type() )
        {
        case KWPApplication:
            {
            iNewWpItem = CMusWpItem::NewL();
           
            aCharacteristic.AcceptL( *this );
            
            // If Mus settings, store information
            if( iAppID && *iAppID == KMusAppID )
                {
                MUS_LOG( "   CMusWpAdapter, Mus Settings found" )
                delete iWpItem;
                iWpItem = iNewWpItem;
                }
            else
                {
                delete iNewWpItem;
                }
                
            iNewWpItem = NULL;
            break;
            }
        case KWPNamedCharacteristic:
            {
            if ( iNewWpItem )
                {
                // We are inside APPLICATION-characteristic
                if ( aCharacteristic.Name() == 
                        KMUsWpCharacteristicNameCapability() ||
                     aCharacteristic.Name() == 
                        KMUsWpCharacteristicNameActivation() )
                    {
                    aCharacteristic.AcceptL( *this );
                    }
                }
            break;
            }
        default:
            {
            break;
            }
        }
    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::VisitL (Characteristic)" )
    }


// -----------------------------------------------------------------------------
// From base class CWPAdapter, which derives this from MWPVisitor.
// Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
void CMusWpAdapter::VisitL( CWPParameter& aParameter )
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::VisitL (Parameter)" )
    
    switch ( aParameter.ID() )
        {
        case EWPNamedParameter: // Name is used instead of ID
            {
            if ( aParameter.Name() == KMusWpParamNameEdgeDtm() )
                {
                iNewWpItem->SetEdgeDtmSupport( ETrue );
                }
            else if ( aParameter.Name() == KMusWpParamNameInternetSignaling() )
                {
                iNewWpItem->SetForceInternetSignaling( ETrue );
                }
            else if ( aParameter.Name() == KMusWpParamNameQuery() )
                {
                iNewWpItem->SetCapabilityQuery( aParameter.Value() );
                }
            else if ( aParameter.Name() == KMusWpParamNameCondition() )
                {
                iNewWpItem->SetActivationCondition( aParameter.Value() );
                }
            else
                {
                // To keep PC-Lint happy
                }
            break;
            }
        case EWPParameterAppID: // APPLICATION/APPID
            {
	        delete iAppID; 
	        iAppID = NULL;
	        iAppID = aParameter.Value().AllocL();
            break;
            }
        case EWPParameterProviderID: // APPLICATION/PROVIDER-ID
            {
            iNewWpItem->SetProviderIdL( aParameter.Value() );
            break;
            }
        case EWPParameterName: // APPLICATION/NAME
            {
            iNewWpItem->SetNameL( aParameter.Value() );
            break;
            }
        case EWPParameterToAppRef: // APPLICATION/TO-APPREF
            {
            iNewWpItem->SetToAppRefL( aParameter.Value() );
            break;
            }
		default:
            {
			break;
            }
        }
    
    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::VisitL (Parameter)" )
    }


// -----------------------------------------------------------------------------
// From base class CWPAdapter, which derives this from MWPVisitor.
// Definition of the pure virtual function.
// -----------------------------------------------------------------------------
//
void CMusWpAdapter::VisitLinkL(CWPCharacteristic& /*aLink*/ )
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// C++ default constructor
// -----------------------------------------------------------------------------
//
CMusWpAdapter::CMusWpAdapter() 
    :CWPAdapter(),
    iProfileIdReceived( EFalse ),
    iProfileId( KMusWpNotSet )
    {
    }


// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CMusWpAdapter::ConstructL()
    {
    MUS_LOG( "mus: [MUSWP]  -> CMusWpAdapter::ConstructL" )
    
    TFileName fileName;
    Dll::FileName(fileName);
    
    iSummaryTitle = WPAdapterUtil::ReadHBufCL( fileName, 
                                               KMusWpAdapterName, 
                                               R_MUS_WP_SUMMARYTITLE );

    MUS_LOG( "mus: [MUSWP]  <- CMusWpAdapter::ConstructL" )
    }






