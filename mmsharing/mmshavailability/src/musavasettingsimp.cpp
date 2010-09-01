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
* Description:   This is module interface class for MusAvailability Plug-in
*
*/



// INCLUDE FILES
#include "musavasettingsimp.h"
#include "musavasettingsobserver.h"
#include "muslogger.h"
#include "musavasip.h"

#include <e32base.h>
#include <e32def.h>
#include <e32std.h>
#include <badesca.h>

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusAvaSettingsImp* CMusAvaSettingsImp::NewL()
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::NewL()" )
    CMusAvaSettingsImp* self = new (ELeave) CMusAvaSettingsImp;
    CleanupStack::PushL (self);
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::NewL()" )
    return self;
    }

// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaSettingsImp::~CMusAvaSettingsImp()
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::~CMusAvaSettingsImp()" )
    
    delete iTelNumber;
    iTelNumber = NULL ;

    delete iContactName;
    iContactName = NULL;
    
    delete iOptionSentTelNumber;
    iOptionSentTelNumber = NULL ;

    delete iSipAddressProposal;
    iSipAddressProposal = NULL ;

    if( iSipAddresses )
        {
        iSipAddresses->Reset();
        delete iSipAddresses;
		iSipAddresses = NULL ;
        }
    
	if( iVideoCodecs )
        {
        iVideoCodecs->Reset();
        delete iVideoCodecs;
        iVideoCodecs = NULL ;
        }	
   
    if( iAudioCodecs )
        {
        iAudioCodecs->Reset();
        delete iAudioCodecs;
        iAudioCodecs = NULL ;
        }
    

    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::~CMusAvaSettingsImp()" )
    }

// -----------------------------------------------------------------------------
// C++ Constructor
// -----------------------------------------------------------------------------
//
CMusAvaSettingsImp::CMusAvaSettingsImp()
    {

    }

// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::ConstructL()" )
    iTelNumber = HBufC::NewL( 0 );
    
    iOptionSentTelNumber = HBufC::NewL( 0 );

    iContactName = HBufC::NewL( 0 );

    iSipAddressProposal = HBufC::NewL( 0 );

    iContactId = KErrNotFound;

    iSipProfileId = KErrNotFound;

    iSipAddresses = new( ELeave ) CDesCArrayFlat( 1 );

    iVideoCodecs = new( ELeave ) CDesCArrayFlat( 1 );

    iAudioCodecs = new( ELeave ) CDesCArrayFlat( 1 );

    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::ConstructL()" )
    }  

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
MMusAvaSettings::TManualActivation CMusAvaSettingsImp::ManualActivation()
	{
	return iManualActivation;	
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusAvaSettingsImp::TelNumber() const
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::TelNumber()" )
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::TelNumber()" )
    return *iTelNumber;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetOptionSentNumber( const TDesC& aTelNumber )
	{
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetOptionSentNumber() " )
    MUS_LOG_TDESC( "mus: [MUSAVA]	   aTelNumber = ", aTelNumber )
    delete iOptionSentTelNumber;
    iOptionSentTelNumber = NULL ;
    TRAPD ( error , iOptionSentTelNumber = aTelNumber.AllocL() );
    if ( error )
    	{
        MUS_LOG1("CMusAvaSettingsImp::SetOptionSentNumber() leave code: %d ", error);
    	}
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetOptionSentNumber() " )
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusAvaSettingsImp::OptionSentTelNumber() const
	{
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::OptionSentTelNumber() " )
	return *iOptionSentTelNumber;
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::ReleaseOptionSentNumber ()
	{
	MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::ReleaseOptionSentNumber() " )
	delete iOptionSentTelNumber;
	iOptionSentTelNumber = NULL;
	TRAPD ( err , iOptionSentTelNumber = HBufC::NewL( 0 ) );
    if ( err )
    	{
        MUS_LOG1("CMusAvaSettingsImp::ReleaseOptionSentNumber() leave code: %d ", err);
    	}
	MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::ReleaseOptionSentNumber() " )
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusAvaSettingsImp::ContactName() const
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::ContactName()" )
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::ContactName()" )
    return *iContactName;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const MDesCArray& CMusAvaSettingsImp::SipAddresses() const
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SipAddresses() " )
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SipAddresses() " )
    return *iSipAddresses;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusAvaSettingsImp::SipAddressProposal() const
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SipAddressProposal() " )
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SipAddressProposal() " )
    return *iSipAddressProposal;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusAvaSettingsImp::ContactId() const
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::ContactId() " )
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::ContactId() " )
    return iContactId;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const MDesCArray& CMusAvaSettingsImp::VideoCodecs() const
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::VideoCodecs() " )
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::VideoCodecs() " )
    return *iVideoCodecs;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const MDesCArray& CMusAvaSettingsImp::AudioCodecs() const
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::AudioCodecs() " )
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::AudioCodecs() " )
    return *iAudioCodecs;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusAvaSettingsImp::SipProfileId() const
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SipProfileId() " )
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SipProfileId() " )
    return iSipProfileId;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetTelNumberL( const TDesC& aTelNumber )
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetTelNumberL() " )
    MUS_LOG_TDESC( "mus: [MUSAVA]	   aTelNumber = ", aTelNumber )
    delete iTelNumber;
    iTelNumber = NULL ;
    iTelNumber = aTelNumber.AllocL();
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetTelNumberL() " )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetContactNameL( const TDesC& aContactName )
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetContactNameL() " )
    MUS_LOG_TDESC( "mus: [MUSAVA]	   aContactName = ", aContactName )
    delete iContactName;
    iContactName = NULL ;
    iContactName = aContactName.AllocL();
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetContactNameL() " )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetSipAddressProposalL( const TDesC& aTelNumber,
                                                 const TDesC& aDomain )
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::\
    		 SetSipAddressProposalL( const TDesC& aTelNumber, \
    		 const TDesC& aDomain ) " )
	_LIT( KAt, "@" );

    HBufC* buffer = HBufC::NewL( aTelNumber.Length() +
                                 KAt().Length() +
                                 aDomain.Length() );
    buffer->Des().Append( aTelNumber )                                 ;
    buffer->Des().Append( KAt() );
    buffer->Des().Append( aDomain );

    delete iSipAddressProposal;
    iSipAddressProposal = buffer;
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::\
    		 SetSipAddressProposalL( const TDesC& aTelNumber, \
    		 const TDesC& aDomain ) " )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::ReleseTelNumberL()
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::ReleseTelNumberL() " )
    delete iTelNumber;
    iTelNumber = NULL;
    iTelNumber = HBufC::NewL( 0 );
   
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::ReleseTelNumberL() " )
   	}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetSipAddressesL( const MDesCArray& aSipAddresses )
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetSipAddressesL( MDesCArray& aSipAddresses ) " )
    CopyDescArrayL( *iSipAddresses, aSipAddresses );
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetSipAddressesL() " )
    }

// -----------------------------------------------------------------------------
// CMusAvaSettingsImp::SetSipAddressesL(CDesCArrayFlat* aSipAddresses )
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetSipAddressesL( CDesCArray* aSipAddresses )
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetSipAddressesL(CDesCArrayFlat*) " )
    
    iSipAddresses->Reset();
    delete iSipAddresses;
    iSipAddresses = aSipAddresses;    
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetSipAddressesL(CDesCArrayFlat*) " )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetContactId( TInt aContactId )
    {
    MUS_LOG1( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetContactId( %d )", aContactId )
    iContactId = aContactId;
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetContactId()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetVideoCodecsL( const MDesCArray& aVideoCodecs)
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetVideoCodecsL( MDesCArray& aVideoCodecs) " )
    CopyDescArrayL( *iVideoCodecs, aVideoCodecs );
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetVideoCodecsL() " )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetAudioCodecsL( MDesCArray& aAudioCodecs )
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetAudioCodecsL( MDesCArray& aAudioCodecs )" )
    CopyDescArrayL( *iAudioCodecs, aAudioCodecs );
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetAudioCodecsL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetSipProfileId( TInt aSipProfileId )
    {
    MUS_LOG1( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetSipProfileId( %d )", aSipProfileId )
    iSipProfileId = aSipProfileId;
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetSipProfileId()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::CopyDescArrayL( CDesCArray& aTarget,
                                         const MDesCArray& aSource )
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::CopyDescArrayL(\
             CDesCArrayFlat& aTarget, MDesCArray& aSource )" )
    aTarget.Reset();

    for( TInt i(0); i<aSource.MdcaCount(); i++ )
        {
        aTarget.AppendL( aSource.MdcaPoint( i ) );
        }
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::CopyDescArrayL(\
             CDesCArrayFlat& aTarget, MDesCArray& aSource )" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetObserver( MMusAvaSettingsObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetObserver" )
    iObserver = &aObserver;
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetObserver" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetManualActivation( TManualActivation aManualActivation )
    {
    MUS_LOG( "mus: [MUSAVA]	-> CMusAvaSettingsImp::SetManualActivation" )
    iManualActivation = aManualActivation;
    MUS_LOG( "mus: [MUSAVA]	<- CMusAvaSettingsImp::SetManualActivation" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MMusAvaSettingsObserver::TApplicationState CMusAvaSettingsImp::ApplicationState()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaSettingsImp::ApplicationState()" )
    MMusAvaSettingsObserver::TApplicationState state(
                  MMusAvaSettingsObserver::EApplicationStateNotDefined );

    if( iObserver )
        {
        state = iObserver->ApplicationState();
        }

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSettingsImp::ApplicationState()" )
    return state;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusAvaSettingsImp::CallDirection()
	{
	return iCallDirection;	
	}
	
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp::SetCallDirection( TInt aDirection )
	{
	iCallDirection = aDirection;	
	}
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusAvaSettingsImp::CallPrivacy()
	{
	return iCallPrivacy;	
	}
	
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaSettingsImp:: SetCallPrivacy( TInt aPrivacy )
	{
	iCallPrivacy = aPrivacy;	
	}

// end of file
