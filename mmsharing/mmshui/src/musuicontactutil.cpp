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
* Description: 
*
*/


#include "musuicontactutil.h"
#include "musuidefinitions.h"
#include "muslogger.h"
#include "musenguriparser.h"
#include "musuid.hrh"

#include <RPbkViewResourceFile.h>         // Phonebook view resource
#include <CPbkContactEditorDlg.h>
#include <CPbkFieldsInfo.h>
#include <TPbkContactItemField.h>

#include <cntdb.h>
#include <cntitem.h>
#include <cntfield.h>
#include <cntdef.h>
#include <cntfldst.h>
#include <CPbkContactEngine.h>
#include <apgtask.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool MusUiContactUtil::ShareViewFieldExistsL( CCoeEnv& aEnv, TInt aContactId )
    {
    MUS_LOG( "mus: [MUSUI ]  -> MusUiContactUtil::ShareViewFieldExistsL" );

    TBool ret( EFalse );

    RPbkViewResourceFile pbkResourceFile( aEnv );
    CleanupClosePushL( pbkResourceFile );
    pbkResourceFile.OpenL();
    CPbkContactEngine* pbkContactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( pbkContactEngine );
    CPbkContactItem* contactItem = NULL;
    CleanupStack::PushL( contactItem );
    if ( aContactId > 0 )
        {
        contactItem = pbkContactEngine->OpenContactL( aContactId );
        if ( contactItem->FindField( EPbkFieldIdShareView ) )
            {
            MUS_LOG( "mus: [MUSUI ]  EPbkFieldIdShareView found!!" );
            ret = ETrue;
            }
        else if ( contactItem->FindField( EPbkFieldIdSIPID ) )
            {
            MUS_LOG( "mus: [MUSUI ]  EPbkFieldIdSIPID found, NOT EPbkFieldIdShareView defined!!!" );
            ret = ETrue;
            }
        else
            {
            ret = EFalse;
            }
        }
    else
        {
        ret = EFalse;
        }
    CleanupStack::PopAndDestroy( 3 );   // contactItem, pbkContactEngine, &pbkResourceFile    
    MUS_LOG( "mus: [MUSUI ]  <- MusUiContactUtil::ShareViewFieldExistsL" );
    return ret;

    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool MusUiContactUtil::CheckPbContactForVSAddressesL(	CCoeEnv& aEnv,
														const TDesC& aOriginator,
														TInt aContactId)
{
	MUS_LOG( "mus: [MUSUI ]  -> MusUiContactUtil::CheckPbContactForVSAddressesL" );
	
	TBool sipAddrMatches(EFalse);
	TBool shareViewAddrMatches(EFalse);
	TBool ret(EFalse);
	
	if(MatchTextWithPbFieldsL(aEnv, EPbkFieldIdSIPID, aOriginator, aContactId))
    	{
    	sipAddrMatches=ETrue;	
    	}
	if(MatchTextWithPbFieldsL(aEnv, EPbkFieldIdShareView, aOriginator, aContactId))
	    {
	    shareViewAddrMatches=ETrue;	
    	}
	
	if(aOriginator.Find(KMusPrefixSipUri) != KErrNotFound)
	    {
    	//pb field without sip:-prefix are checked.
    	HBufC * withoutPrefix;
    	withoutPrefix = HBufC::NewLC(aOriginator.Length());
    	*withoutPrefix = aOriginator;
    	TPtr ptr = withoutPrefix->Des();
    	ptr.Delete(0,KMusPrefixMaxLength);
	
    	if(MatchTextWithPbFieldsL(aEnv, EPbkFieldIdSIPID, *withoutPrefix, aContactId))
        	{
        	sipAddrMatches=ETrue;	
        	}
    	if(MatchTextWithPbFieldsL(aEnv, EPbkFieldIdShareView, *withoutPrefix, aContactId))
        	{
        	shareViewAddrMatches=ETrue;	
        	}
    	
    	CleanupStack::PopAndDestroy(withoutPrefix);
	    }
        
    if(sipAddrMatches||shareViewAddrMatches)
        {
        ret=ETrue;	
        }
    else
        {
        ret=EFalse;	
        }
    
	MUS_LOG( "mus: [MUSUI ]  -> MusUiContactUtil::CheckPbContactForVSAddressesL" );
	return ret;
}
// -----------------------------------------------------------------------------
// @function MatchTextWithPbFieldsL 
// searches all contactId´s fieldIds of same type for a text
// returns ETrue if text is found from at least one of them.   
// -----------------------------------------------------------------------------
//
TBool MusUiContactUtil::MatchTextWithPbFieldsL( CCoeEnv& aEnv,
											   TPbkFieldId aFieldId,
											   const TDesC& aText,
											   TInt aContactId )
{
	MUS_LOG( "mus: [MUSUI ]  -> MusUiContactUtil::MatchTextWithPbFieldsL" );
	
	TBool textMatch(EFalse);
	
	RPbkViewResourceFile pbkResourceFile(aEnv);
	CleanupClosePushL( pbkResourceFile );
    pbkResourceFile.OpenL();
	CPbkContactEngine* pbkContactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( pbkContactEngine );
	CPbkContactItem* contactItem = NULL;
    CleanupStack::PushL( contactItem );

	if ( aContactId > 0 )
        {
		contactItem = pbkContactEngine->OpenContactL( aContactId );	
     	
     	TInt index(0);
  
       		while(index!=KErrNotFound)
       			{
       			TPbkContactItemField* contactItemField = 
       								contactItem->FindField( aFieldId, index );
			        
			    if( contactItemField )
			    	{
				    if( contactItemField->Text() == aText && !textMatch )
				    	{
				    	textMatch=ETrue;	
				    	}
				   	}

				if( index != KErrNotFound ) 
					{
					index++;	       	
					}
	       		}
	       		
		}

	CleanupStack::PopAndDestroy( 3 );   // contactItem, pbkContactEngine, &pbkResourceFile    

	MUS_LOG( "mus: [MUSUI ] <- MusUiContactUtil::MatchTextWithPbFieldsL" );
	return textMatch;
		
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiContactUtil::ActivateContactViewL( CCoeEnv& aEnv, 
                                             TContactItemId aId, 
                                             const TDesC& aTypedAddress, 
                                             const TDesC& aMusTelNumberValue )
    {
    MUS_LOG( "mus: [MUSUI ]  -> MusUiContactUtil::ActivateContactViewL" );
    RPbkViewResourceFile pbkResourceFile( aEnv );
    CleanupClosePushL( pbkResourceFile );
    pbkResourceFile.OpenL();
    CPbkContactEngine* pbkContactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( pbkContactEngine );
    
    CPbkContactItem* contactItem = NULL;
    CleanupStack::PushL( contactItem );
    
    if ( aId > 0 )
        {
        MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::ActivateContactViewL\
                  TContactItemId > 0 => Opening Existing Contact..." );
        contactItem = pbkContactEngine->OpenContactL( aId );
        EditContactL( *contactItem, 
                      EFalse, 
                      pbkContactEngine, 
                      aTypedAddress, 
                      aMusTelNumberValue, 
                      aId );
        }
    else
        {
        MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::ActivateContactViewL\
                  TContactItemId < 1 => Creating new Contact..." );
        contactItem = pbkContactEngine->CreateEmptyContactL();
        EditContactL( *contactItem, 
                      ETrue, 
                      pbkContactEngine, 
                      aTypedAddress, 
                      aMusTelNumberValue, 
                      aId );
        }

    CleanupStack::PopAndDestroy( 3 );   // contactItem, pbkContactEngine, &pbkResourceFile
    MUS_LOG( "mus: [MUSUI ]  <- MusUiContactUtil::ActivateContactViewL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiContactUtil::EditContactL( CPbkContactItem& aContact,
                                     TBool aNewOne,
                                     CPbkContactEngine* aPbkContactEngine,
                                     const TDesC& aTypedAddress,
                                     const TDesC& aMusTelNumberValue,
                                     TContactItemId aContactId )
    {
    MUS_LOG( "mus: [MUSUI ]  -> MusUiContactUtil::EditContactL" );

    TMusEngUriParser parser( aTypedAddress );
    parser.ParseUriL();
    HBufC* uri = NULL;
    CleanupStack::PushL( uri );

    CContactTextField* addressField = NULL;
    CleanupStack::PushL( addressField );

    if ( parser.UriType() == TMusEngUriParser::ESip )
        {
        MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditContactL\
                                parser.UriType() == TMusEngUriParser::ESip" );
        addressField = EditableContactFieldL(
                            aContact,
                            EPbkFieldIdShareView,
                            aPbkContactEngine );
        uri = parser.GetUri16L( ETrue );
        }
    else if ( parser.UriType() == TMusEngUriParser::ETel )
        {
        MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditContactL\
                                parser.UriType() == TMusEngUriParser::ETel" );
        addressField = EditableContactFieldL(
                            aContact,
                            EPbkFieldIdPhoneNumberMobile,
                            aPbkContactEngine );
        uri = parser.GetUri16L( EFalse );
        }

    if ( addressField )
        {
        MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditContactL\
                                addressField OK" );
        addressField->SetTextL( *uri );
        }

    if ( ( aMusTelNumberValue.Length() > 0 ) && aContactId == KErrNotFound )
        {
        MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditContactL\
                                !iPhoneNumberFieldFound" );
        CContactTextField* phoneNumberField = EditableContactFieldL(
                                aContact,
                                EPbkFieldIdPhoneNumberMobile,
                                aPbkContactEngine );
        if ( phoneNumberField )
            {
            MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditContactL\
                                    phoneNumberField OK" );
            phoneNumberField->SetTextL( aMusTelNumberValue );
            }
        }

    MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditContactL: Creating dialog..." );

    // Bring MuSh UI foreground
    RWsSession wsSession;
    User::LeaveIfError( wsSession.Connect() );
    CleanupClosePushL( wsSession );
    TApaTaskList taskList( wsSession );
    TUid appUid;
    appUid.iUid = KMusUiUid;
    TApaTask task = taskList.FindApp( appUid );
    task.BringToForeground();
    CleanupStack::PopAndDestroy( &wsSession );
    
    // Launch pbk new entry editor dlg
    CPbkContactEditorDlg* editorDlg =  CPbkContactEditorDlg::NewL(
                            *aPbkContactEngine,
                            aContact,
                            aNewOne,
                            -1, // Move focus to the topmost field
                            ETrue );
    editorDlg->ResetWhenDestroyed( &editorDlg );
    editorDlg->HideExitCommand();
    editorDlg->ExecuteLD();
    
    CleanupStack::Pop( 2 );   // uri, addressField

    MUS_LOG( "mus: [MUSUI ]  <- MusUiContactUtil::EditContactL" );
    }


// -----------------------------------------------------------------------------
// Get editable contact field.
// -----------------------------------------------------------------------------
//
CContactTextField* MusUiContactUtil::EditableContactFieldL(
                        CPbkContactItem& aContact,
                        TInt aFieldId,
                        CPbkContactEngine* aPbkContactEngine )
    {
    MUS_LOG( "mus: [MUSUI ]  -> MusUiContactUtil::EditableContactFieldL" );

    // Get fields info from pbk engine
    const CPbkFieldsInfo& fieldsInfo = aPbkContactEngine->FieldsInfo();

    // Try to get the SIP URI storage field for this contact
    TPbkContactItemField* itemField = aContact.FindField( aFieldId );

    if ( !itemField )
        {
        MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditableContactFieldL: !itemField" );
        CPbkFieldInfo* fieldInfo = fieldsInfo.Find( aFieldId );
        // Add field to this contact
        if ( fieldInfo )
            {
            MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditableContactFieldL\
                            fieldInfo OK" );
            // Contact takes ownership of the field.
            aContact.AddFieldL( *fieldInfo );
            itemField = aContact.FindField( aFieldId );
            return ( itemField ) ? itemField->TextStorage() : NULL;
            }
        else
            {
            MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditableContactFieldL\
                            fieldInfo == NULL" );
            return NULL;
            }
        }
    else
        {
        // Does not pass ownership.
        MUS_LOG( "mus: [MUSUI ]     MusUiContactUtil::EditableContactFieldL\
                        itemField OK" );
        return itemField->TextStorage();
        }
    }


// end of file
