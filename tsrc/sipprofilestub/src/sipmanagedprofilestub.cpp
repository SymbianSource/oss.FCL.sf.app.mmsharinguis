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
#include "sipmanagedprofile.h"
#include "sipprofileregistry.h"
#include "sipmanagedprofileregistry.h"

/*

//#include "sipconcreteprofile.h"
//#include "SIPProfileTypeInfo.h"
//_LIT8(KSIPProfileExtenstionParameter, "%u");
//const TUint KProfileBufferSize = 25;

_LIT8(KSIPProfileDefAOR, "user@aa");
        /** 
        * Sets profile parameter
        * @param aParam a parameter to set
        * @param aVal values to set; an empty array resets the value
        * @return KErrNotFound if parameter was not found, 
        *         KErrNoMemory if out of memory          
        *          KErrNone otherwise        
        */
EXPORT_C TInt CSIPManagedProfile::SetParameter( TUint32 /*aParam*/, 
                                         const MDesC8Array& /*aVal*/ )
    {
    return KErrNone;
    }

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CSIPManagedProfile::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSIPManagedProfile* CSIPManagedProfile::NewL(CSIPManagedProfileRegistry* aSIPRegistry)
	{
    CSIPManagedProfile* self = CSIPManagedProfile::NewLC (aSIPRegistry);
    CleanupStack::Pop();
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPManagedProfile::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSIPManagedProfile* CSIPManagedProfile::NewLC(CSIPManagedProfileRegistry* aSIPRegistry)
	{
	CSIPManagedProfile* self = new(ELeave)CSIPManagedProfile(aSIPRegistry);
    CleanupStack::PushL (self);
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPManagedProfile::~CSIPManagedProfile
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPManagedProfile::~CSIPManagedProfile()
	{
	}
/*
// -----------------------------------------------------------------------------
// CSIPManagedProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPManagedProfile::GetParameter(TUint32 aParam, TDesC8 const *& aVal ) const
	{
	const TDesC8& val= KSIPProfileDefAOR;
    aVal = &val;
	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CSIPManagedProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPManagedProfile::GetParameter(TUint32 aParam, TUint32& aVal) const
	{
	aVal = 1;
	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CSIPManagedProfile::GetParameter
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSIPManagedProfile::GetParameter(TUint32 aParam, TBool& aVal) const
	{
	aVal = ETrue;
	return KErrNone;
	}
*/	
// -----------------------------------------------------------------------------
// CSIPManagedProfile::CSIPManagedProfile
// -----------------------------------------------------------------------------
//	
CSIPManagedProfile::CSIPManagedProfile(CSIPManagedProfileRegistry* aRegistry) :
    CSIPProfile( aRegistry )
	{
	}

/*
// -----------------------------------------------------------------------------
// CSIPManagedProfile::IsContextActive
// -----------------------------------------------------------------------------
//	
TBool CSIPManagedProfile::IsContextActive() const
	{
	return ETrue;
	}
	
// -----------------------------------------------------------------------------
// CSIPManagedProfile::ContextId
// -----------------------------------------------------------------------------
//	
TUint32 CSIPManagedProfile::ContextId() const
	{
	return 1;
	}
*/