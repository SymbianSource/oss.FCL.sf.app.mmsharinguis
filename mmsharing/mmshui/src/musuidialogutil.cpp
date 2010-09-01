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


#include "musuidialogutil.h"
#include <musui.rsg>

#include <AknGlobalConfirmationQuery.h>
#include <aknnotewrappers.h>
#include <AknGlobalNote.h>
#include <aknlistquerydialog.h>
#include <StringLoader.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool MusUiDialogUtil::ShowGlobalConfirmationQueryDialogL( const TDesC& aPrompt )
    {
	CAknGlobalConfirmationQuery* dlg = CAknGlobalConfirmationQuery::NewLC();
	TRequestStatus status;
	dlg->ShowConfirmationQueryL( status, aPrompt, R_AVKON_SOFTKEYS_YES_NO );
	User::WaitForRequest( status );
	CleanupStack::PopAndDestroy( dlg );
    return ( status == EAknSoftkeyYes ) ? ETrue : EFalse;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiDialogUtil::ShowInformationDialogL( const TDesC& aPrompt )
    {
    CAknInformationNote* dlg = new ( ELeave ) CAknInformationNote( ETrue );
    dlg->ExecuteLD( aPrompt );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiDialogUtil::ShowInformationDialogL( TInt aResourceId )
    {
    HBufC* dlgPrompt = StringLoader::LoadLC( aResourceId );
    ShowInformationDialogL( *dlgPrompt );
    CleanupStack::PopAndDestroy( dlgPrompt );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiDialogUtil::ShowGlobalInformationDialogL( const TDesC& aMessage )
    {
    TRequestStatus status;
    CAknGlobalNote* dlg= CAknGlobalNote::NewLC();
    dlg->ShowNoteL( status, EAknGlobalInformationNote, aMessage );
    User::WaitForRequest( status );
    CleanupStack::PopAndDestroy( dlg );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiDialogUtil::ShowGlobalInformationDialogL( TInt aResourceId )
    {
    HBufC* dlgPrompt = StringLoader::LoadLC( aResourceId );
	ShowGlobalInformationDialogL( *dlgPrompt );
    CleanupStack::PopAndDestroy( dlgPrompt );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiDialogUtil::ShowGlobalErrorDialogL( const TDesC& aMessage )
    {
    CAknGlobalNote* dlg = CAknGlobalNote::NewLC();
    dlg->ShowNoteL( EAknGlobalErrorNote, aMessage );
    CleanupStack::PopAndDestroy( dlg );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiDialogUtil::ShowGlobalErrorDialogL( TInt aResourceId )
    {
    HBufC* dlgPrompt = StringLoader::LoadLC( aResourceId );
    ShowGlobalErrorDialogL( *dlgPrompt );
    CleanupStack::PopAndDestroy( dlgPrompt );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiDialogUtil::ShowGlobalErrorModalDialogL( const TDesC& aMessage )
    {
    CAknGlobalNote* dlg = CAknGlobalNote::NewLC();
    TRequestStatus status;
    dlg->ShowNoteL( status , EAknGlobalErrorNote, aMessage );
    User::WaitForRequest( status );
    CleanupStack::PopAndDestroy( dlg );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void MusUiDialogUtil::ShowGlobalErrorModalDialogL( TInt aResourceId )
    {
    HBufC* dlgPrompt = StringLoader::LoadLC( aResourceId );
    ShowGlobalErrorModalDialogL( *dlgPrompt );
    CleanupStack::PopAndDestroy( dlgPrompt );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool MusUiDialogUtil::SelectAddressDialogL( CDesCArray* aAddressArray,
                                             TDes& aSelectedAddress )
    {
    __ASSERT_ALWAYS( aAddressArray, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aAddressArray->Count() > 0, User::Leave( KErrArgument ) );
    
    TInt selectedIndex( 0 );
    CAknListQueryDialog* dlg = new ( ELeave ) CAknListQueryDialog( &selectedIndex );
    dlg->PrepareLC( R_MUS_LIVESHARINGVIEW_ADDRESS_QUERY );
    dlg->SetItemTextArray( aAddressArray );
    dlg->SetOwnershipType( ELbmDoesNotOwnItemArray );
    TInt dlgResponse( dlg->RunLD() );
    aSelectedAddress = (*aAddressArray)[ selectedIndex ];
    delete aAddressArray;
    aAddressArray = NULL;
    return dlgResponse;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool MusUiDialogUtil::ShowTextQueryDialogL( const TDesC& aPrompt, TDes& aValue )
    {
    CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL( aValue );
    CleanupStack::PushL( dlg );
    dlg->SetPromptL( aPrompt );
    CleanupStack::Pop( dlg );
    return dlg->ExecuteLD( R_MUS_TEXT_QUERY_DIALOG );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool MusUiDialogUtil::ShowTextQueryDialogL( TInt aPromptResourceId, TDes& aValue )
    {
    TBool retval( EFalse );
    HBufC* dlgPrompt = StringLoader::LoadLC( aPromptResourceId );
    retval = ShowTextQueryDialogL( *dlgPrompt, aValue );
    CleanupStack::PopAndDestroy( dlgPrompt );
    return retval;
    }


// end of file
