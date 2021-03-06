/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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


#include "SipMessageElements.h"
#include "SipHeaderBase.h"
#include "SipFromHeader.h"
#include "SipContactHeader.h"
#include "SipToHeader.h"
#include "SipCSeqHeader.h"
#include "SipContentTypeHeader.h"
#include "SipExtensionHeader.h"
#include "sipstrings.h"
#include "SipStrConsts.h"



// -----------------------------------------------------------------------------
// CSIPMessageElements::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPMessageElements* CSIPMessageElements::NewL()
	{
    CSIPMessageElements* self = CSIPMessageElements::NewLC();
    CleanupStack::Pop (self);
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPMessageElements* CSIPMessageElements::NewLC ()
	{
	CSIPMessageElements* self = new(ELeave)CSIPMessageElements;
    CleanupStack::PushL (self);
    self->ConstructL ();
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::CSIPMessageElements
// -----------------------------------------------------------------------------
//
CSIPMessageElements::CSIPMessageElements ()
 : iHeaderLookupOpen(EFalse) 
	{
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::ConstructL
// -----------------------------------------------------------------------------
//
void CSIPMessageElements::ConstructL()
	{
//	SIPHeaderLookup::OpenL();
    iHeaderLookupOpen = ETrue;
    iCSeqHeader = CSIPCSeqHeader::DecodeL( _L8("1 INVITE") );
    
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::~CSIPMessageElements
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPMessageElements::~CSIPMessageElements () 
	{
	/*if (iHeaderLookupOpen)
        {
	    SIPHeaderLookup::Close();
        }*/
	delete iContent;
	iUserHeaders.ResetAndDestroy();
    delete iFromHeader;
    delete iToHeader;
    delete iCSeqHeader;
    delete iContentTypeHeader;
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::SetUserHeadersL
// -----------------------------------------------------------------------------
//
EXPORT_C void 
CSIPMessageElements::SetUserHeadersL (RPointerArray<CSIPHeaderBase>& aHeaders)
	{
    RPointerArray<CSIPHeaderBase> tmpHeaders;
    CleanupClosePushL(tmpHeaders);
	for (TInt i=0; i<aHeaders.Count(); i++)
        {
        CSIPHeaderBase* header = aHeaders[i];
        CheckUserHeaderL (header);
        User::LeaveIfError(tmpHeaders.Append(header));
        }
    CleanupStack::Pop(1); // tmpHeaders
    iUserHeaders.ResetAndDestroy();
    iUserHeaders = tmpHeaders;
    aHeaders.Reset();
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::UserHeaders
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CSIPHeaderBase>& 
CSIPMessageElements::UserHeaders() const
	{
	return iUserHeaders;
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::SetContentL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPMessageElements::SetContentL (HBufC8* aContent,
                                                CSIPContentTypeHeader* aType)
	{
	__ASSERT_ALWAYS (aContent != 0, User::Leave(KErrArgument));
    __ASSERT_ALWAYS (aContent->Length() > 0, User::Leave(KErrArgument));
	__ASSERT_ALWAYS (aType != 0, User::Leave(KErrArgument));

    delete iContentTypeHeader;
    iContentTypeHeader = aType;
	delete iContent;
	iContent = aContent;
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::Content
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CSIPMessageElements::Content () const
    {
    if (iContent)
        {
        return *iContent;
        }
    return KNullDesC8;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::Content
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPContentTypeHeader* CSIPMessageElements::ContentType() const
    {
    return iContentTypeHeader;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::ExtractContent
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* CSIPMessageElements::ExtractContent ()
    {
    HBufC8* tmp = iContent;
    iContent = 0;
    delete iContentTypeHeader;
    iContentTypeHeader = 0;
    return tmp;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::InternalizeL
// -----------------------------------------------------------------------------
//
CSIPMessageElements* 
CSIPMessageElements::InternalizeL (RReadStream& aReadStream)
	{
	CSIPMessageElements* self = CSIPMessageElements::NewLC();
    self->DoInternalizeL(aReadStream);
	CleanupStack::Pop(); // self
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::DoInternalizeL
// -----------------------------------------------------------------------------
//
void CSIPMessageElements::DoInternalizeL (RReadStream& /*aReadStream*/)
	{

	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::ExternalizeL
// -----------------------------------------------------------------------------
//
void 
CSIPMessageElements::ExternalizeL (RWriteStream& /*aWriteStream*/) const
	{

	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::UserHeaderCount
// -----------------------------------------------------------------------------
//
TInt CSIPMessageElements::UserHeaderCount (RStringF aName) const
    {
    TInt headerCount = 0;
	for (TInt i=0; i < iUserHeaders.Count(); i++)
		{
        if (iUserHeaders[i]->Name() == aName)
            {
            headerCount++;
            }
		}
    return headerCount;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::UserHeadersL
// -----------------------------------------------------------------------------
//
const RPointerArray<CSIPHeaderBase> 
CSIPMessageElements::UserHeadersL (RStringF aName) const
    {
    RPointerArray<CSIPHeaderBase> headers;
    CleanupClosePushL(headers);
	for (TInt i=0; i < iUserHeaders.Count(); i++)
		{
        if (iUserHeaders[i]->Name() == aName)
            {
            User::LeaveIfError(headers.Append(iUserHeaders[i]));
            }
		}
    CleanupStack::Pop(1); // headers
    return headers;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::RemoveHeaders
// -----------------------------------------------------------------------------
//
TInt CSIPMessageElements::RemoveHeaders (RStringF aName)
    {
    TInt err = KErrNotFound;
	for (TInt i=iUserHeaders.Count()-1; i>=0; i--)
		{
        if (iUserHeaders[i]->Name() == aName)
            {
            CSIPHeaderBase* header = iUserHeaders[i];
            iUserHeaders.Remove(i);
            delete header;
            err = KErrNone;
            }
		}
    return err;
    }   

// -----------------------------------------------------------------------------
// CSIPMessageElements::DetachUserHeader
// -----------------------------------------------------------------------------
//
void CSIPMessageElements::DetachUserHeader (CSIPHeaderBase* aHeader)
    {
	for (TInt i=0; i < iUserHeaders.Count(); i++)
		{
        if (iUserHeaders[i] == aHeader)
            {
            iUserHeaders.Remove(i);
            }
		}
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::AddHeaderL 
// -----------------------------------------------------------------------------
//
void CSIPMessageElements::AddHeaderL (CSIPHeaderBase* aHeader)
	{
	__ASSERT_ALWAYS (aHeader != 0, User::Leave(KErrArgument));
	
    if (aHeader->Name() == SIPStrings::StringF(SipStrConsts::EFromHeader))
        {
        delete iFromHeader;
        iFromHeader = static_cast<CSIPFromHeader*>(aHeader);
        }
    else if (aHeader->Name() == SIPStrings::StringF(SipStrConsts::EToHeader))
        {
        delete iToHeader;
        iToHeader = static_cast<CSIPToHeader*>(aHeader);
        }
    else if (aHeader->Name() == SIPStrings::StringF(SipStrConsts::ECSeqHeader))
        {
        delete iCSeqHeader;
        iCSeqHeader = static_cast<CSIPCSeqHeader*>(aHeader);
        }
    else if (aHeader->Name() == 
             SIPStrings::StringF(SipStrConsts::EContentTypeHeader))
        {
        delete iContentTypeHeader;
        iContentTypeHeader = static_cast<CSIPContentTypeHeader*>(aHeader);
        }
    else
        {
        User::LeaveIfError(iUserHeaders.Append(aHeader));
        }
	}

// -----------------------------------------------------------------------------
// CSIPMessageElements::SetToL
// -----------------------------------------------------------------------------
//
void CSIPMessageElements::SetToL (CSIPToHeader* aTo)
    {
    __ASSERT_ALWAYS (aTo != 0, User::Leave(KErrArgument));
    __ASSERT_ALWAYS (!aTo->HasParam(SIPStrings::StringF(SipStrConsts::ETag)),
                     User::Leave(KErrArgument));

    delete iToHeader;
    iToHeader = aTo;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::To
// -----------------------------------------------------------------------------
//
const CSIPToHeader* CSIPMessageElements::To() const
    {
    return iToHeader;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::SetFromL
// -----------------------------------------------------------------------------
//
void CSIPMessageElements::SetFromL (CSIPFromHeader* aFrom)
    {
    __ASSERT_ALWAYS (aFrom != 0, User::Leave(KErrArgument));
    __ASSERT_ALWAYS (!aFrom->HasParam(SIPStrings::StringF(SipStrConsts::ETag)),
                     User::Leave(KErrArgument));

    delete iFromHeader;
    iFromHeader = aFrom;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::From
// -----------------------------------------------------------------------------
//
const CSIPFromHeader* CSIPMessageElements::From() const
    {
    return iFromHeader;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::CSeq
// -----------------------------------------------------------------------------
//
const CSIPCSeqHeader* CSIPMessageElements::CSeq() const
    {
    return iCSeqHeader;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::SetContent
// -----------------------------------------------------------------------------
//
void CSIPMessageElements::SetContent (HBufC8* aContent)
    {
    delete iContent;
    iContent = aContent;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::DetachContent
// -----------------------------------------------------------------------------
//
void CSIPMessageElements::DetachContent ()
    {
    iContent = 0;
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::CheckUserHeaderL
// -----------------------------------------------------------------------------
//
void CSIPMessageElements::CheckUserHeaderL (const CSIPHeaderBase* /*aHeader*/) const
    {
 
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::ExternalizeUserHeadersL
// -----------------------------------------------------------------------------
//   
void 
CSIPMessageElements::ExternalizeUserHeadersL (RWriteStream& /*aWriteStream*/) const
    {
  
    }

// -----------------------------------------------------------------------------
// CSIPMessageElements::ExternalizeL
// -----------------------------------------------------------------------------
//  
void CSIPMessageElements::ExternalizeL (const CSIPExtensionHeader* /*aHeader*/,
                                        RWriteStream& /*aWriteStream*/) const
    {
 
    }

