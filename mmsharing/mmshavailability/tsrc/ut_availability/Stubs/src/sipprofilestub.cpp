/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  implementation
*
*/


//  INCLUDE FILES

#include "sipprofile.h"
//#include "sipmanagedprofile.h"
#include "sipprofileregistry.h"
#include "CSipSseTestTls.h"
//#include "sipconcreteprofile.h"
//#include "SIPProfileTypeInfo.h"
//_LIT8(KSIPProfileExtenstionParameter, "%u");
//const TUint KProfileBufferSize = 25;

_LIT8(KSIPProfileDefAOR, "user@aa");

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CSIPProfile::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSIPProfile* CSIPProfile::NewL(CSIPProfileRegistry* aSIPRegistry)
	{
    CSIPProfile* self = CSIPProfile::NewLC (aSIPRegistry);
    CleanupStack::Pop();
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSIPProfile* CSIPProfile::NewLC(CSIPProfileRegistry* aSIPRegistry)
	{
	CSIPProfile* self = new(ELeave)CSIPProfile(aSIPRegistry);
    CleanupStack::PushL (self);
    self->ConstructL ();
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::~CSIPProfile
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPProfile::~CSIPProfile()
	{
	delete iRegisteredContact;
	if ( iArray )
	    {
	    if ( iArray->Count() > 0 )
	        {
	        iArray->Reset();    
	        }
	    delete iArray;
	    }
	}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C const TSIPProfileTypeInfo& CSIPProfile::Type() const
    {
    return iTypeInfo;
    }

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter( TUint32 /*aParam*/, 
                                         TDesC8 const *& aVal ) const
	{
	if ( iRegisteredContact )
	    {
	    aVal = iRegisteredContact;
	    return KErrNone;
	    }
	else{
	    return KErrNotFound;
	    }
	}	

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 aParam, TUint32& aVal) const
	{
	CSipSseTestTls* tls = CSipSseTestTls::Storage();
	if ( tls )
	    {
	    if ( tls->iProfileBehavior == KSIPProfileIdFetchFails && 
	         aParam == KSIPProfileId )
	        {
	        return KErrNotFound;
	        }
	    else if ( tls->iProfileBehavior == KSIPIapIdFetchFails && 
	              aParam == KSIPAccessPointId )
	        {
	        return KErrNotFound;
	        }
	    }
	    
    if ( iTUint32ValueError == KErrNone )
	    {
	    aVal = iTUint32Value;
	    }
	return iTUint32ValueError;
	}	

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 aParam, TBool& aVal) const
	{
	if ( iTBoolValueError == KErrNone )
	    {
	    if ( aParam == KSIPDefaultProfile )
	        {
	        aVal = iIsDefaultProfile;
	        }
	    else
	        {
	        aVal = iTBoolValue;
	        }
	    }
	    
	return iTBoolValueError;
	}


// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter( TUint32 /*aParam*/, 
                                         MDesC8Array const *& aVal) const
    {
    if ( iArray )
        {
        aVal = iArray;
        }
    else
        {
        aVal = NULL;
        }
    return KErrNone;
    }

	
// -----------------------------------------------------------------------------
// CSIPProfile::CSIPProfile
// -----------------------------------------------------------------------------
//	
CSIPProfile::CSIPProfile(CSIPProfileRegistryBase* aRegistry):
	iSIPProfileRegistry(aRegistry), 
	iEnabled(EFalse),
	iTUint32Value( 1 ),
    iTUint32ValueError( KErrNone ),
    iTBoolValue( ETrue ),
    iTBoolValueError( KErrNone ),
    iIsDefaultProfile( EFalse )
	{
	iTypeInfo.iSIPProfileClass = TSIPProfileTypeInfo::EInternet;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::ConstructL
// -----------------------------------------------------------------------------
//	
void CSIPProfile::ConstructL()
	{
	}

// -----------------------------------------------------------------------------
// CSIPProfile::IsContextActive
// -----------------------------------------------------------------------------
//	
TBool CSIPProfile::IsContextActive() const
	{
	return ETrue;
	}
	
// -----------------------------------------------------------------------------
// CSIPProfile::ContextId
// -----------------------------------------------------------------------------
//	
TUint32 CSIPProfile::ContextId() const
	{
	return 1;
	}

// end of file
