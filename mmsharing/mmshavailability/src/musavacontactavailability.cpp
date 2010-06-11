/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Provide interface for the client requestin availability class.
*
*/


#include "musavacontactavailability.h"
#include "musavaavailabilityobserver.h"
#include "musavasettingsimp.h"
#include "musavaobserver.h"
#include "muslogger.h"
#include "mussettings.h"

#include <cntviewbase.h>
#ifdef UNIT_TESTING // if unit testing
#include <digia/eunit/eunitdecorators.h>
#endif // UNIT_TESTING

const TInt KContactsArraySize = 10;

_LIT( KMusAvaTel,               "tel:" );
_LIT( KMusAvaPlus,              "+"    );

CMusAvaContactAvailability* CMusAvaContactAvailability::NewL(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::NewL()" )

    CMusAvaContactAvailability* self =
        CMusAvaContactAvailability::NewLC(
            aObserver,
            aSettings );
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaContactAvailability::NewL()" )
    return self;
    }


CMusAvaContactAvailability* CMusAvaContactAvailability::NewLC(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::NewL()" )
    CMusAvaContactAvailability* self =
     new( ELeave ) CMusAvaContactAvailability(
        aObserver,
        aSettings );
    CleanupStack::PushL( self );
    self->ConstructL();

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaContactAvailability::NewL()" )
    return self;
    }


CMusAvaContactAvailability::~CMusAvaContactAvailability()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::\
             ~CMusAvaContactAvailability()" )
    delete iContactName;
    iContactName = NULL;
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaContactAvailability::\
             ~CMusAvaContactAvailability()" )
    }


CMusAvaContactAvailability::CMusAvaContactAvailability(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    :CMusAvaAvailability( aObserver ),
    iContactName( NULL ),
    iSettings( aSettings )
    {
    }


void CMusAvaContactAvailability::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::ConstructL()" )
    iOperatorVariant = MultimediaSharingSettings::OperatorVariantSettingL();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaContactAvailability::ConstructL()" )
    }


// -------------------------------------------------------------------------
//  Starts the loopy execution.
// -------------------------------------------------------------------------
//
void CMusAvaContactAvailability::DoExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::DoExecuteL()" )

    CDesCArrayFlat* contactsArray
        = new ( ELeave ) CDesCArrayFlat( KContactsArraySize );
    CleanupStack::PushL( contactsArray );

    ActiveCallL( contactsArray );

    iSettings.SetSipAddressesL( *contactsArray );

    contactsArray->Reset();

    CleanupStack::PopAndDestroy( contactsArray );

    SetState( MMusAvaObserver::EMusAvaStatusAvailable );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaContactAvailability::DoExecuteL()" )
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
TBool CMusAvaContactAvailability::ActiveCallL( CDesCArrayFlat*& aContactsArray )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::ActiveCallL()" )

    const TDesC& phoneNumber = iSettings.TelNumber();
    TInt retval = EFalse;
    const MDesCArray& addresses = iSettings.SipAddresses();
    TInt addressesCount = addresses.MdcaCount();
    
    if ( phoneNumber.Length() > 0 &&
         ( iOperatorVariant == MusSettingsKeys::EOperatorSpecific ||
           phoneNumber.Left(1).Compare( KMusAvaPlus() ) == 0 ) )
        {
        MUS_LOG( "mus: [MUSAVA]     Phone number found with + prefix \
                                     (or operator variant)" )
        // No, it doesn't, so add the prefix to the new address.
        MUS_LOG( "mus: [MUSAVA]     Prefix to the new address is added" )
        //Create contact array
        //Create tel bufer
        HBufC* tmpDescriptor = HBufC::NewLC( KMusAvaTel().Length() +
                                              phoneNumber.Length() );
        tmpDescriptor->Des().Append( KMusAvaTel() );
        tmpDescriptor->Des().Append( phoneNumber );
        //Add to array
        aContactsArray->AppendL( tmpDescriptor->Des() );
        CleanupStack::PopAndDestroy( tmpDescriptor );
        retval = ETrue;
        }
	else if ( addressesCount > 0 )
        {
        MUS_LOG1( "mus: [MUSAVA]    addresses.MdcaCount() %d", 
                        addressesCount )
        const TDesC& sipAddress = addresses.MdcaPoint( 0 );
        MUS_LOG_TDESC( "mus: [MUSAVA]       SIP Address: ", 
            sipAddress )                       
        aContactsArray->AppendL( sipAddress );
        retval = ETrue;
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA]     Phone number or SIP URL not found " )
        retval = EFalse;
        }
    MUS_LOG1( "mus: [MUSAVA]     <- CMusAvaContactAvailability::\
              ActiveCallL return %d", retval )

    return retval;
    }

MMusAvaObserver::TAvailabilityName CMusAvaContactAvailability::Name()
    {
    return MMusAvaObserver::EMusAvaNameContactStatus;
    }




// End of file
