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
#include "musavacalleventmonitor.h"

#include <CPbkContactItem.h>
#include <CPbkContactEngine.h>
#include <CPbkFieldInfo.h>
#include <TPbkContactItemField.h>
#include <cntviewbase.h>
#ifdef UNIT_TESTING // if unit testing
#include <digia/eunit/eunitdecorators.h>
#endif // UNIT_TESTING

const TInt KNumberMatchingDigitsRequired = 7;
const TInt KContactsArraySize = 10;


_LIT( KMusAvaSip,               "sip:" );
_LIT( KMusAvaTel,               "tel:" );
_LIT( KMusAvaPlus,              "+"    );
_LIT( KMusAvaAtSign ,           "@"    );
_LIT( KMusEmptyStringSymbol,    ""     );
_LIT( KMusSpaceSymbol,          " "    );
_LIT( KMusAvaTestString,        "*#"   );


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
    if ( iPhoneStatus )
        {
        iPhoneStatus->Cancel();
        }
    delete iPhoneStatus;
    iPhoneStatus = NULL; 
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
    iPhoneStatus = CMusAvaCallEventMonitor::NewL( *this );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaContactAvailability::ConstructL()" )
    }


// -------------------------------------------------------------------------
//  Starts the loopy execution.
// -------------------------------------------------------------------------
//
void CMusAvaContactAvailability::DoExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::DoExecuteL()" )
    iPhoneStatus->StartMonitoringL();

    CContactIdArray* idArray = NULL;

    #ifdef UNIT_TESTING // if unit testing
    EUNIT_DISABLE_ALLOC_DECORATOR;
    #endif // UNIT_TESTING
    CPbkContactEngine* pbkContactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( pbkContactEngine );

    // resolve the name order     
    CContactViewBase& contactView = pbkContactEngine->AllContactsView();
    const RContactViewSortOrder& sortOrder = contactView.SortOrderL();
    #ifdef UNIT_TESTING // if unit testing
    EUNIT_ENABLE_ALLOC_DECORATOR;
    #endif // UNIT_TESTING
    
    iBeginWithFirstName = ETrue;
    TBool foundOrder = EFalse;
	for( TInt i = 0; i < sortOrder.Count() && !foundOrder; ++i )
	    {
	    if ( sortOrder[i] == KUidContactFieldFamilyName )
		    {
            iBeginWithFirstName = EFalse;
            foundOrder = ETrue;
		    }
	    else if ( sortOrder[i] == KUidContactFieldGivenName )
		    {
		    iBeginWithFirstName = ETrue;
		    foundOrder = ETrue;
		    }
	    }

    CDesCArrayFlat* contactsArray
        = new ( ELeave ) CDesCArrayFlat( KContactsArraySize );
    CleanupStack::PushL( contactsArray );

    if ( ContactL( idArray, *pbkContactEngine ) )
        {
        CleanupStack::PushL( idArray );

        TInt idArrayCount = idArray->Count();
        MUS_LOG1( "mus: [MUSAVA]     idArray->Count %d", idArrayCount )
        CPbkContactItem* pbkItem = NULL;
        TPbkFieldId fieldId = ContactIdSearch( *idArray, *pbkContactEngine );

        for ( TInt i = 0; i < idArrayCount; i++ )
            {
            MUS_LOG1( "mus: [MUSAVA]     i %d", i )
            TContactItemId itemId( KErrNotFound );
            itemId = ( *idArray )[i];

            TRAPD( error, pbkItem = pbkContactEngine->ReadContactL( itemId ) );

            if ( !error )
                {
                TBool found = EFalse;
                if ( fieldId == EPbkFieldIdShareView )
                    {
                    found = ResolveAddressesL( 
                                *pbkItem, contactsArray, EPbkFieldIdShareView );
                    }
                else if( fieldId == EPbkFieldIdSIPID)
                    {
                    found = ResolveAddressesL( 
                                *pbkItem, contactsArray, EPbkFieldIdSIPID );
                    }

                if ( !found )
                    {
                    PhoneResolutionL( *pbkItem, contactsArray );
                    }
                    
                    // tbd: causes test case to fail
                    // break;
                }

            delete pbkItem; 
            pbkItem = NULL;
            }

        idArray->Reset();
        CleanupStack::PopAndDestroy( idArray );
        }
    else
        {
        ActiveCallL( contactsArray );
        }

    iSettings.SetSipAddressesL( *contactsArray );

    contactsArray->Reset();

    CleanupStack::PopAndDestroy( contactsArray );

    CleanupStack::PopAndDestroy( pbkContactEngine );

    SetState( MMusAvaObserver::EMusAvaStatusAvailable );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaContactAvailability::DoExecuteL()" )
    }

// -------------------------------------------------------------------------
//  This function is called when a cs call is connected.
// -------------------------------------------------------------------------
//
void CMusAvaContactAvailability::CallConnectedL( const TDesC& aTelNumber )
    {
    MUS_LOG( "mus: [MUSAVA]  \
                -> CMusAvaContactAvailability::CallConnectedL()" )
                
    iSettings.SetTelNumberL(aTelNumber);              
    DoExecuteL();

    MUS_LOG( "mus: [MUSAVA]  \
                <- CMusAvaContactAvailability::CallConnectedL()" )
    }
void CMusAvaContactAvailability::ConferenceCallL()
    {   
    }


// -------------------------------------------------------------------------
//  This function is called when a cs call goes hold
// -------------------------------------------------------------------------
//
void CMusAvaContactAvailability::CallHoldL( const TDesC& /*aTelNumber*/ )
    {    
    }


// -------------------------------------------------------------------------
//  This function is called when a cs call goes into a state
//  that is not connected.
// -------------------------------------------------------------------------
//
void CMusAvaContactAvailability::NoActiveCallL()
    {
    }


void CMusAvaContactAvailability::ConferenceCallLTerminated ()
	{	
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
    else
        {
        MUS_LOG( "mus: [MUSAVA]     Phone number not found " )
        retval = EFalse;
        }
    MUS_LOG1( "mus: [MUSAVA]     <- CMusAvaContactAvailability::\
              ActiveCallL return %d", retval )

    return retval;
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
TBool CMusAvaContactAvailability::ContactL(
                                    CContactIdArray*& aContactIdArray,
                                    CPbkContactEngine& aPbkContactEngine )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::ContactL()" )
    CPbkContactItem* pbkContactItem = NULL;

    TInt conRes = EFalse;

    TInt err = ContactArrayL( aContactIdArray, aPbkContactEngine );
    if ( !err )
        {
        err = ContactItem( *aContactIdArray, pbkContactItem, aPbkContactEngine );
        // NOTE: KErrNotSupported means there is more than one contact id.
        if ( err == KErrNotSupported )
            {
            iSettings.SetContactId( KErrNotSupported );
            conRes = ETrue;
            }
        else if( err != KErrNone )
            {
            MUS_LOG( "mus: [MUSAVA]     No contact item found; contact id = \
                KErrNotFound" )
            conRes = EFalse;
            }
        else
            {
            TContactItemId itemId = pbkContactItem->Id();
            MUS_LOG1( "mus: [MUSAVA]        Setting contact id to be %d",
                      itemId )
            CleanupStack::PushL( pbkContactItem );
            iSettings.SetContactId( itemId );
            UpdateContactNameL( pbkContactItem->ContactItem() );
            CleanupStack::Pop( pbkContactItem );
            conRes = ETrue;
            }
        }
    MUS_LOG1( "mus: [MUSAVA]     <- CMusAvaContactAvailability::\
              ContactL return %d", conRes )

    delete pbkContactItem;
    pbkContactItem = NULL;

    return conRes;
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
void CMusAvaContactAvailability::UpdateContactNameL( 
                                                CContactItem& aContactItem )
    {
    MUS_LOG( "mus: [MUSAVA ]  -> CMusAvaContactAvailability::\
        UpdateContactName" )
    //TBuf<KMusNameFieldMaxLength> givenName;
    //TBuf<KMusNameFieldMaxLength> familyName;

    HBufC* givenName = NULL;
    HBufC* familyName = NULL;

    delete iContactName;
    iContactName = NULL;
    iContactName = HBufC::NewL( 0 );

    CContactItemFieldSet& fieldSet = aContactItem.CardFields();

    TInt pos = fieldSet.Find( KUidContactFieldGivenName );
    if ( pos != KErrNotFound )
        {
        CContactItemField& itemField = fieldSet[pos];
        //givenName = HBufC::NewL( itemField.TextStorage()->Text().Length() );

        //givenName.Copy( itemField.TextStorage()->Text().Alloc() );
        givenName = itemField.TextStorage()->Text().AllocLC();

        }
    else
        {
        //givenName = KMusEmptyStringSymbol;
        givenName = KMusEmptyStringSymbol().AllocLC();
        }

    pos = fieldSet.Find( KUidContactFieldFamilyName );
    if ( pos != KErrNotFound )
        {
        CContactItemField& itemField = fieldSet[pos];
        //familyName = HBufC::NewL( itemField.TextStorage()->Text().Length() );
        //familyName.Copy( field.TextStorage()->Text() );
        familyName = itemField.TextStorage()->Text().AllocLC();
        }
    else
        {
        familyName = KMusEmptyStringSymbol().AllocLC();
        }

    TInt strLength = NULL;
    TBool bothNames = givenName->Length() > 0 && familyName->Length() > 0;
    
    if ( bothNames )
        {
        strLength = givenName->Length() +
                     KMusSpaceSymbol().Length() +
                     familyName->Length();
        }
    else
        {
        strLength = givenName->Length() + familyName->Length();
        }
    
    if ( strLength > iContactName->Length() )
        {
        iContactName = iContactName->ReAllocL( strLength );
        }

    if ( strLength > 0 )
        {
        TPtr ptr = iContactName->Des();
        if ( iBeginWithFirstName )
            {
            if ( bothNames )
                {
                ptr.Append( givenName->Des() );
                ptr.Append( KMusSpaceSymbol );
                ptr.Append( familyName->Des() );
                }
            else
                {
                ptr.Append( givenName->Des() );
                ptr.Append( familyName->Des() );
                }
            }
        else            
            {
            if ( bothNames )
                {
                ptr.Append( familyName->Des() );
                ptr.Append( KMusSpaceSymbol );
                ptr.Append( givenName->Des() );
                }
            else
                {
                ptr.Append( familyName->Des() );
                ptr.Append( givenName->Des() );
                }
            } 
        }

    CleanupStack::PopAndDestroy( familyName );
    CleanupStack::PopAndDestroy( givenName );

    TRAP_IGNORE( iSettings.SetContactNameL( iContactName->Des() ) )

    MUS_LOG( "mus: [MUSAVA ]  <- CMusAvaContactAvailability::\
        UpdateContactName" )
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
TInt CMusAvaContactAvailability::ContactArrayL(
                                    CContactIdArray*& aContactIdArray,
                                    CPbkContactEngine& aPbkContactEngine )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::ContactArrayL()" )

    const TDesC& phoneNumber = iSettings.TelNumber();
    TInt retval = KErrNotFound;

    if ( phoneNumber.Length() > 0 )
        {
        MUS_LOG( "mus: [MUSAVA]     Phone number found " )
        // Instantiate a PhonebookContactEngine object, which represents a
        // connection to the Phonebook contact database:
        #ifdef UNIT_TESTING // if unit testing
        EUNIT_DISABLE_ALLOC_DECORATOR;
        #endif // UNIT_TESTING
        CContactIdArray* tmpContactIdArray =
                        aPbkContactEngine.MatchPhoneNumberL(
                                             phoneNumber,
                                             KNumberMatchingDigitsRequired );
        #ifdef UNIT_TESTING // if unit testing
        EUNIT_ENABLE_ALLOC_DECORATOR;
        #endif // UNIT_TESTING
        if ( tmpContactIdArray->Count() > 0 )
            {
            aContactIdArray = tmpContactIdArray;    
            retval = KErrNone;
            }
        else
            {
            tmpContactIdArray->Reset();
            delete tmpContactIdArray;
            MUS_LOG( 
            "mus: [MUSAVA]     Phone number not found in CPbkContactEngine" )
            }
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA]     Phone number not found " )
        }
    
    MUS_LOG1( "mus: [MUSAVA]     <- CMusAvaContactAvailability::\
          ContactArrayL return %d", retval )

    return retval;
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
TInt CMusAvaContactAvailability::ContactItem( 
                                        CContactIdArray& aContactIdArray,
                                        CPbkContactItem*& aPbkContactItem,
                                        CPbkContactEngine& aPbkContactEngine )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::ContactItem()" )
    CPbkContactItem* pbkItem = NULL;
    TInt retval = KErrNotFound;
    TInt contactIdsCount = aContactIdArray.Count();
    
    if( contactIdsCount > 1 )
      {
      // NOTE: KErrNotSupported means there is more than one contact id.
      MUS_LOG( "mus: [MUSAVA]    More than one Contact ID found." )
      retval = KErrNotSupported;
      }
    else if ( contactIdsCount == 1 )
        {
        // Contact card whose phone number matches that of the other party
        // was found. The ID(s) of the contacts are recorded in iIdArray.
        MUS_LOG( "mus: [MUSAVA]     Contact ID found." )

        TContactItemId itemId( KErrNotFound );

        MUS_LOG1( "mus: [MUSAVA]    ID array has <%d> elements",
                  contactIdsCount )
        // iterate through the contact cards we consider as belonging to
        // the other party, that is, those saved in iIdArray:
        for ( TInt i = 0; i < contactIdsCount; i++ )
            {
            itemId = ( aContactIdArray )[i];
            MUS_LOG2( "mus: [MUSAVA]        Current array index == <%d>, \
                    contact ID == <%d>", i, itemId )
            TRAPD( pbkFailure, pbkItem =
                   aPbkContactEngine.ReadContactL( itemId ) );

            if ( pbkFailure )
                {
                // Contact lookup failed.
                MUS_LOG1( "mus: [MUSAVA]    Leave code <%d> occurred at call \
                        of ReadContactL.", pbkFailure )
                }
            else
                {
                aPbkContactItem = pbkItem;
                retval = KErrNone;
                }
            }
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA]     No contact card whose phone number\
             matches that of the other party" )
        }
    
    MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaContactAvailability::\
                 ContactItem() return %d", retval )
        
    return retval;
    }

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
TBool CMusAvaContactAvailability::PhoneResolutionL( CPbkContactItem& aPbkItem,
                                           CDesCArrayFlat*& aContactsArray )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::PhoneResolutionL()" )
    TInt retval = ETrue;
    if ( ResolvePhoneContactL( aPbkItem, aContactsArray ) )
        {
        MUS_LOG( "mus: [MUSAVA]     Stop Execution" )
        }
    else
        {
        if ( ActiveCallL( aContactsArray ) )
            {
            MUS_LOG( "mus: [MUSAVA]     Stop Execution" )
            }
        else
            {
            MUS_LOG( "mus: [MUSAVA]     Stop Execution" )
            retval = EFalse;
            }
        }
        
    MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaContactAvailability::\
                 PhoneResolutionL() return %d", retval )
    return retval;
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
TBool CMusAvaContactAvailability::ContactId( CPbkContactItem& aPbkItem,
                                                  TPbkFieldId aTPbkFieldId  )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::ContactId()" )

    CPbkFieldArray& fieldArray = aPbkItem.CardFields();
    TPbkContactItemField itemField;
    TBool returnValue = EFalse;

    // Get the number of detail fields of the phonebook contact item.
    const TInt fieldCount( fieldArray.Count() );

    for ( TInt index = 0; index < fieldCount && returnValue == EFalse; 
        index++ )
        {
        itemField = fieldArray.At( index );

        if ( itemField.FieldInfo().Match( aTPbkFieldId ) &&
              itemField.Text().Find( KMusAvaAtSign() ) != KErrNotFound )
            {
            returnValue = ETrue;
            MUS_LOG( "mus: [MUSAVA]     Contact id found" )
            }
        }
    MUS_LOG1( "mus: [MUSAVA]     <- CMusAvaContactAvailability::\
              ContactId() return %d", returnValue )
    return returnValue;
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
TPbkFieldId CMusAvaContactAvailability::ContactIdSearch(
                                        CContactIdArray& aContactIdArray,
                                        CPbkContactEngine& aPbkContactEngine )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::\
    ContactIdSearch()" )
    
    TInt contactIdsCount = aContactIdArray.Count();
    MUS_LOG1( "mus: [MUSAVA]     aContactIdArray.Count %d", \
    contactIdsCount )

    CPbkContactItem* pbkItem = NULL;
    TPbkFieldId retFieldId = EPbkFieldIdNone;

    for ( TInt i = 0; 
        i < contactIdsCount && retFieldId == EPbkFieldIdNone; 
        i++ )
        {
        MUS_LOG1( "mus: [MUSAVA]     i %d", i )
        TContactItemId itemId( KErrNotFound );
        itemId = ( aContactIdArray )[i];

        #ifdef UNIT_TESTING // if unit testing
        EUNIT_DISABLE_ALLOC_DECORATOR;
        #endif // UNIT_TESTING
        TRAPD( error, pbkItem = aPbkContactEngine.ReadContactL( itemId ) )
        #ifdef UNIT_TESTING // if unit testing
        EUNIT_ENABLE_ALLOC_DECORATOR;
        #endif // UNIT_TESTING
        if ( error != KErrNone )
            {
            return retFieldId;
            }
        if ( ContactId( *pbkItem, EPbkFieldIdShareView ) )
            {
            MUS_LOG( "mus: [MUSAVA]     Contact have a prefix = \
            EPbkFieldIdShareView" )
            delete pbkItem;
            retFieldId = EPbkFieldIdShareView;
            }
        else if ( ContactId( *pbkItem, EPbkFieldIdSIPID ) )
            {
            MUS_LOG( "mus: [MUSAVA]     Contact have a prefix = \
            EPbkFieldIdSIPID" )
            delete pbkItem;
            retFieldId = EPbkFieldIdSIPID;
            }
        else if ( ContactId( *pbkItem, EPbkFieldIdPhoneNumberGeneral ) )
            {
            // this part never run!!!!
            MUS_LOG( "mus: [MUSAVA]     Contact have a prefix = \
            EPbkFieldIdPhoneNumberGeneral" )
            delete pbkItem;
            retFieldId = EPbkFieldIdPhoneNumberGeneral;
            }
        else
            {
            MUS_LOG( "mus: [MUSAVA]     Contact have no prefix " )
            delete pbkItem;
            }
        }
    return retFieldId;
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
TBool CMusAvaContactAvailability::ResolveAddressesL( CPbkContactItem& aPbkItem,
                                        CDesCArrayFlat*& aContactsArray,
                                        TPbkFieldId aTPbkFieldId  )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::\
        ResolveAddressesL()" )

    CPbkFieldArray& fieldArray = aPbkItem.CardFields();
    TPbkContactItemField itemField;
    HBufC* tmpDescriptor( NULL );
    TBool returnValue( EFalse );

    // Get the number of detail fields of the phonebook contact item.
    const TInt fieldCount( fieldArray.Count() );

    for ( TInt index = 0; index < fieldCount; index++ )
        {
        itemField = fieldArray.At( index );

        if ( itemField.FieldInfo().Match( aTPbkFieldId ) &&
             itemField.Text().Find( KMusAvaAtSign() ) != KErrNotFound && 
             ( iOperatorVariant != MusSettingsKeys::EOperatorSpecific || 
               itemField.Text().Find( KMusAvaTestString() ) != KErrNotFound ) )
            {
            returnValue = ETrue;
            MUS_LOG( "mus: [MUSAVA]     Contact id found" )            
            
            HBufC* sipAddress;
            if ( iOperatorVariant == MusSettingsKeys::EOperatorSpecific )
                {
                sipAddress = HBufC::NewLC( itemField.Text().Length() - 
                                           KMusAvaTestString().Length() );
                TInt pos = itemField.Text().Find( KMusAvaTestString() );
                sipAddress->Des().Append( itemField.Text().Left( pos ) );
                sipAddress->Des().Append( itemField.Text().Right( 
                                    itemField.Text().Length() - 
                                    pos - KMusAvaTestString().Length() ) );
                }
            else                
                {
                sipAddress = itemField.Text().AllocLC();
                }

            if ( sipAddress->Find( KMusAvaSip() ) == KErrNotFound )
                {
                // No, it doesn't, so add the prefix to the new address.
                MUS_LOG( "mus: [MUSAVA]     Prefix to the new address is added" )
                tmpDescriptor = HBufC::NewLC( KMusAvaSip().Length() +
                                              sipAddress->Length() );
                tmpDescriptor->Des().Append( KMusAvaSip() );
                tmpDescriptor->Des().Append( *sipAddress );
                // if multiple contact have same phone number then 
                // sip address can be repeated . To avoid this check
                // whether sip address exist already in array
                // it not exist then add it to array.
                if(! IsExistAlready(*aContactsArray,*tmpDescriptor))
                    {
                    aContactsArray->AppendL( *tmpDescriptor );    
                    }                
                CleanupStack::PopAndDestroy( tmpDescriptor );
                }
            else
                {
                MUS_LOG( "mus: [MUSAVA]     Contact have a prefix" )
                 // if multiple contact have same phone number then 
                // sip address can be repeated . To avoid this check
                // whether sip address exist already in array
                // it not exist then add it to array.
                if(! IsExistAlready(*aContactsArray,*sipAddress))
                    {
                    aContactsArray->AppendL( *sipAddress );
                    }
                }
                
            CleanupStack::PopAndDestroy( sipAddress );                
            }
        }
    MUS_LOG1( "mus: [MUSAVA]     <- CMusAvaContactAvailability::\
              ResolveAddressesL() return %d", returnValue )
    return returnValue;
    }


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
TBool CMusAvaContactAvailability::ResolvePhoneContactL(
                                                CPbkContactItem& aPbkItem,
                                              CDesCArrayFlat*& aContactsArray )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaContactAvailability::\
        ResolvePhoneContactL()" )
    CPbkFieldArray& fieldArray = aPbkItem.CardFields();
    TPbkContactItemField itemField;
    HBufC* tmpDescriptor( NULL );
    TBool returnValue = EFalse;

    // Get the number of detail fields of the phonebook contact item.
    const TInt fieldCount( fieldArray.Count() );

    for ( TInt index = 0; index < fieldCount; index++ )
        {
        itemField = fieldArray.At( index );

        if ( itemField.FieldInfo().IsPhoneNumberField() && 
             ( iOperatorVariant == MusSettingsKeys::EOperatorSpecific ||
               itemField.Text().Left(1) == KMusAvaPlus() ) )
            {
            MUS_LOG( "mus: [MUSAVA]     Contact id found" )
            // No, it doesn't, so add the prefix to the new address.
            MUS_LOG( "mus: [MUSAVA]     Prefix to the new address is added" )
            tmpDescriptor = HBufC::NewLC( KMusAvaTel().Length() +
                                          itemField.Text().Length() );
            tmpDescriptor->Des().Append( KMusAvaTel() );
            tmpDescriptor->Des().Append( itemField.Text() );
            aContactsArray->AppendL( *tmpDescriptor );
            CleanupStack::PopAndDestroy( tmpDescriptor );
            returnValue = ETrue;
            }
        }

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaContactAvailability::\
        ResolvePhoneContactL()" )
    return returnValue;
    }


MMusAvaObserver::TAvailabilityName CMusAvaContactAvailability::Name()
    {
    return MMusAvaObserver::EMusAvaNameContactStatus;
    }


// -------------------------------------------------------------------------
// Checks that whether aDesItem present in aDesArray.
// Utility function used to avoid dublication.
// -------------------------------------------------------------------------
//
TBool CMusAvaContactAvailability::IsExistAlready(const MDesCArray& aDesArray,
                                          const TDesC& aDesItem)                                              
    {    
    for ( TInt i=0;i<aDesArray.MdcaCount();i++)
        {
        if (! aDesArray.MdcaPoint(i).Compare( aDesItem ) ) 
            {
            return ETrue;            
            }
        }
    return EFalse;
    }
