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
#include "CSipSseTestTls.h"
#include "musunittesting.h"
//#include "sipmanagedprofile.h"
#include "sipprofileregistry.h"
//#include "sipconcreteprofile.h"
#include "SIPProfileTypeInfo.h"

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
	delete iArray;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 /*aParam*/, TDesC8 const *& aVal ) const
	{
	const TDesC8& val= KSIPProfileDefAOR;
    aVal = &val;
	return iError;
	}	

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 /*aParam*/, TUint32& aVal) const
	{
	aVal = 1;
	   
    CSipSseTestTls* tls = CSipSseTestTls::Storage();
    if ( tls )
        {
        return tls->Error();
        }
    else
        {
       	return iError; 
        }
	}	

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 /*aParam*/, TBool& aVal) const
	{
	aVal = iRegistered;
	return iError;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPProfile::GetParameter(TUint32 /*aParam*/, 
	MDesC8Array const *& aVal) const
	{
	if ( !iError )
	    {
	    aVal = iArray;
	    }
	return iError;
	}

// -----------------------------------------------------------------------------
// CSIPProfile::Type
// -----------------------------------------------------------------------------
//	
EXPORT_C const TSIPProfileTypeInfo& CSIPProfile::Type() const
	{
	return iSIPProfileType;
	}	
	
// -----------------------------------------------------------------------------
// CSIPProfile::CSIPProfile
// -----------------------------------------------------------------------------
//	
CSIPProfile::CSIPProfile(CSIPProfileRegistryBase* aRegistry):
	iSIPProfileRegistry(aRegistry), iEnabled(EFalse), iRegistered( ETrue ), iError(0)
	{
	}

// -----------------------------------------------------------------------------
// CSIPProfile::ConstructL
// -----------------------------------------------------------------------------
//	
void CSIPProfile::ConstructL()
	{
    CDesC8Array* array = new( ELeave ) CDesC8ArrayFlat( 5 );
    CleanupStack::PushL( array );
	array->AppendL( _L8( "sip:user@domain.com" ) );
	array->AppendL( _L8( "sip:user2@domain.com" ) ); 
	CleanupStack::Pop( array );
	iArray = array;    
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

