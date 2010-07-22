/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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


// USER
#include "musengmceoutsession.h"
#include "mussettings.h"
#include "musengmceutils.h"
#include "musenguriparser.h"
#include "musenglogger.h"
#include "mussipprofilehandler.h"
#include "mussessionproperties.h"
#include "musresourceproperties.h"

// SYSTEM
#include <lcsessionobserver.h>
#include <lcuiprovider.h>
#include <mcestreambundle.h>
#include <mcemediastream.h>
#include <mceoutsession.h>
#include <mcevideostream.h>
#include <mcemediasource.h>
#include <mcefilesource.h>
#include <mcemediasink.h>
#include <mcertpsink.h>
#include <mcespeakersink.h>
#include <mcedisplaysink.h>
#include <mcevideocodec.h>
#include <mceaudiocodec.h>
#include <e32property.h>

#include <sipprofile.h>
#include <sipextensionheader.h>
#include <sipaddress.h>
#include <uri8.h>

const TInt KMusEngSipReasonCodeRecipientNotFound  = 404;
const TInt KMusEngSipReasonCodeRequestTimeout = 408;
const TInt KMusEngSipReasonCodeUnsupportedMediaType = 415;
const TInt KMusEngSipReasonCodeUnsupportedUriScheme = 416;
const TInt KMusEngSipReasonCodeTemporarilyNotAvailable = 480;
const TInt KMusEngSipReasonCodeBusyHere = 486;
const TInt KMusEngSipReasonCodeRequestCancelled = 487;
const TInt KMusEngSipReasonCodeNotAcceptableHere = 488;
const TInt KMusEngSipReasonCodeDecline = 603;
const TInt KMusEngSipReasonCodeNotAcceptable = 606;
// The next code represents unofficial sip error code
// "479 Regretfuly, we were not able to process the URI (479/SL)
const TInt KMusEngSipReasonCode479NotAbleToProcessURI = 479;

const TUint8 KMusEngPayloadTypeVideoH263 = 96;
const TUint8 KMusEngPayloadTypeAudio = 97;
const TUint8 KMusEngPayloadTypeVideoAvc = 98;

const TInt KMusEngSipAddressMaxLength = 256;
const TInt KMusEngSipAddressesMaxAmount  =   8;
_LIT( KMusEngCommaSymbol, "," );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceOutSession::CMusEngMceOutSession()
    : CMusEngMceSession(),
      iTriedInvitations( 0 ),
      iAsyncBrakeCallBack( AsyncBrakeCompleted, this ),
      iRegistrationCallBack( RegistrationTimerExpired, this ),
      iInvitationResponseCallBack( InvitationResponseTimerExpired, this )
    {
    iAsyncBrakeEntry.Set( iAsyncBrakeCallBack );
    iRegistrationEntry.Set( iRegistrationCallBack );
    iInvitationResponseEntry.Set( iInvitationResponseCallBack );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::ConstructL()" )
    
    CMusEngMceSession::ConstructL();
    TInt sipProfileId;
    TInt error = KErrNone;
    error = ( RProperty::Get( NMusSessionApi::KCategoryUid, 
                             NMusSessionApi::KSipProfileId, sipProfileId ) );
    if ( error != KErrNone )
       {
       sipProfileId = KErrNone;
       error = KErrNone;
       }
    iSipProfileHandler->CreateProfileL( sipProfileId );
    iVideoCodecList = NULL;
    TBuf<RProperty::KMaxPropertySize> buffer;
    error = ( RProperty::Get( NMusSessionApi::KCategoryUid, 
                                 NMusSessionApi::KVideoCodecs, buffer ) );
    if ( error == KErrNone && buffer.Length() )
        {
        iVideoCodecList = 
                    CnvUtfConverter::ConvertFromUnicodeToUtf8L( buffer );
        }
        
    iDeltaTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
    
    /* Read the contact name set by availability plugin */    
    error = RProperty::Get( NMusSessionApi::KCategoryUid,
                                 NMusSessionApi::KContactName,
                                 buffer );
    iRemoteDisplayName = ( error == KErrNone && buffer.Length() ) 
                         ? buffer.AllocL() : KNullDesC().AllocL();
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceOutSession::~CMusEngMceOutSession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::~CMusEngMceOutSession()" )
 
    if ( iAddressQueried && iRecipient )
        {
        TRAP_IGNORE( SaveContactL( *iRecipient ) )
        }
    
    delete iDeltaTimer;
    delete iRecipient;
    delete iVideoCodecList;
    delete iRemoteSipAddressProposal;
    delete iRemoteDisplayName;
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::~CMusEngMceOutSession()" )
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::EstablishLcSessionL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::EstablishLcSessionL()")

    iAddressQueried = EFalse;
    if ( LcUiProvider() && !IsBackgroundStartup() )
        {
        LcUiProvider()->HandleForegroundStatus( ETrue );
        }
    TRAPD( err, DoInviteL() );
        
    // If address is in wrong format, Manual Address Entry Dialog is displayed
    if ( ( err == KErrArgument ) && DoSyncRetryL() )
        {  
        err = KErrNone; // Doing retry
        }
    
    User::LeaveIfError( err );
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::EstablishLcSessionL")
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::TerminateLcSessionL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::TerminateLcSessionL" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    if( iSession->State() == CMceSession::EOffering )
        {
        static_cast< CMceOutSession* >( iSession )->CancelL();
        }
    else
        {
        iSession->TerminateL();
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::TerminateLcSessionL" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::AddDisplayL( CMceMediaStream& aStream )
    {
    MusEngMceUtils::AddDisplayL( aStream, *iManager, Rect() );
    }

// -----------------------------------------------------------------------------
// Calls MCE function EstablishL with feature tag in Accept-Contact header.
// Adjusts stream and codec values.
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::EstablishSessionL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::EstablishSessionL()" )
    
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    AdjustStreamsAndCodecsL();
    
    CDesC8Array* headers = new ( ELeave ) CDesC8ArrayFlat( 1 );
    CleanupStack::PushL( headers );
    headers->AppendL( KMusEngAcceptContactHeader() );
    headers->AppendL( KMusAcceptHeader() ); 
    if ( iOperatorVariant )
        {
        CUri8* originator = iSipProfileHandler->UserFromProfileLC();
        
        CSIPExtensionHeader* header = CSIPExtensionHeader::NewLC( 
                    KMusPPreferredIdentity, originator->Uri().UriDes() );
        HBufC8* headInText = header->ToTextL();
        MUS_LOG_TDESC8( " mus: [ENGINE] P-Preferred-Identity header : ", headInText->Des() );
        CleanupStack::PopAndDestroy( header );
        CleanupStack::PopAndDestroy( originator );
        CleanupStack::PushL( headInText );
        headers->AppendL( *headInText );
        CleanupStack::PopAndDestroy( headInText );
        }
      
      
    static_cast<CMceOutSession*>( iSession )->EstablishL( 0, headers );
    CleanupStack::Pop( headers );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::EstablishSessionL()" )
    }

// -----------------------------------------------------------------------------
// Handle MCE session termination. Called by MCE observer function of the
// base class.
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::HandleTermination( TInt aStatusCode,
                                              const TDesC8& aReasonPhrase )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::HandleTermination()" )

    switch ( aStatusCode )
        {
        case KMusEngSipReasonCodeRecipientNotFound : 
            //lint -fallthrough
        case KMusEngSipReasonCode479NotAbleToProcessURI:
            //lint -fallthrough
        case KMusEngSipReasonCodeUnsupportedUriScheme : 
            {
            InitRecipientNotFoundHandling();
            break;
            }
        case KMusEngSipReasonCodeRequestTimeout :
            {
            InformObserverAboutSessionFailure( MLcSession::ENoAnswer );
            break;
            }
        case KMusEngSipReasonCodeUnsupportedMediaType :
            //lint -fallthrough
        case KMusEngSipReasonCodeNotAcceptableHere:
            //lint -fallthrough
        case KMusEngSipReasonCodeNotAcceptable:
            {
            InformObserverAboutSessionFailure( MLcSession::ESessionRejected );
            break;
            }
        case KMusEngSipReasonCodeBusyHere :
            {
            // Operator variant uses 486 to rejection instead of 603
            if ( iOperatorVariant )
                {
                InformObserverAboutSessionFailure( 
                    MLcSession::ESessionRejected );
                }
            else
                {
                InformObserverAboutSessionFailure( 
                    MLcSession::ERecipientBusy );
                }
            break;
            }
        case KMusEngSipReasonCodeRequestCancelled :  
            {
            InformObserverAboutSessionFailure( MLcSession::ESessionCancelled );
            break;
            }
        case KMusEngSipReasonCodeDecline :
            {
            InformObserverAboutSessionFailure( MLcSession::ESessionRejected );
            break;
            }
        case KMusEngSipReasonCodeTemporarilyNotAvailable :
            {
            InformObserverAboutSessionFailure( 
                MLcSession::ERecipientTemporarilyNotAvailable );
            break;
            }
        default:
            {
            // Termination reason is not outsession specific.
            // Let the base class to handle the termination.
            CMusEngMceSession::HandleTermination( aStatusCode, aReasonPhrase );
            }
        }


    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::HandleTermination()" )
    }

// -----------------------------------------------------------------------------
// Sets payload type and calls overridden base class version
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::AdjustVideoCodecL( CMceVideoCodec& aVideoCodec,
                                              TMceSourceType aSourceType )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::AdjustVideoCodecL()" )
    
    CMusEngMceSession::AdjustVideoCodecL( aVideoCodec, aSourceType );
    
    if ( aVideoCodec.SdpName() == KMceSDPNameH263() ||
         aVideoCodec.SdpName() == KMceSDPNameH2632000() )
        {
        User::LeaveIfError( 
                aVideoCodec.SetPayloadType( KMusEngPayloadTypeVideoH263 ) );
        }
    else if ( aVideoCodec.SdpName() == KMceSDPNameH264() )
        {
        User::LeaveIfError(
                aVideoCodec.SetPayloadType( KMusEngPayloadTypeVideoAvc ) );
        }
    else
        {
        // NOP
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::AdjustVideoCodecL()" )
    }

// -----------------------------------------------------------------------------
// Sets payload type and calls overridden base class version
// -----------------------------------------------------------------------------
//        
void CMusEngMceOutSession::AdjustAudioCodecL( CMceAudioCodec& aAudioCodec )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::AdjustAudioCodecL()" )
    
    CMusEngMceSession::AdjustAudioCodecL( aAudioCodec );
    
    User::LeaveIfError( aAudioCodec.SetPayloadType( KMusEngPayloadTypeAudio ) );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::AdjustAudioCodecL()" )
    }  

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::ProfileRegistered()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::ProfileRegistered()" )
    
    if ( iRegistrationPending )
        {
        iDeltaTimer->Remove( iRegistrationEntry );
        iRegistrationPending = EFalse;
        
        
        HBufC* resolvedRecipient = NULL;
        TRAPD( error, resolvedRecipient = RemoteAddressL() )
        if ( error != KErrNone )
            {
            InformObserverAboutSessionFailure( error );
            }
        else
            {
            TRAP( error, DoInviteL( *resolvedRecipient ) )
            delete resolvedRecipient; 
            if ( error != KErrNone )
                {
                InformObserverAboutSessionFailure( error );
                }
            }
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::ProfileRegistered()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngMceOutSession::IsH264Supported() const
    {
    return ( iVideoCodecList && iVideoCodecList->FindF( KMceSDPNameH264() ) >= 0 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::CreateMceSessionStructureL( TBool aForceSdpBandwidth )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::CreateMceSessionStructureL()" )

    CSIPProfile* profile = iSipProfileHandler->Profile();
    __ASSERT_ALWAYS( profile != NULL, User::Leave( KErrNotReady ) );

    // Create session
    iSession = CMceOutSession::NewL( *iManager, *profile, *iRecipient );

    // Remove QoS-lines if needed
    if ( profile->Type().iSIPProfileClass == 
         TSIPProfileTypeInfo::EInternet ||
         MultimediaSharingSettings::ForceInternetSignalingSettingL() ==
         MusSettingsKeys::EForceInternetSignaling ||
         iOperatorVariant)
        {
        iSession->SetModifierL( KMcePreconditions, KMcePreconditionsNotUsed );
        MUS_LOG( "mus: [ENGINE]     Usage of preconditions denied" )
        }
        
    SetSessionSdpLinesL( *iSession, aForceSdpBandwidth );

    // Create bundle for local streams
    
    CMceStreamBundle* localBundle = 
                        CMceStreamBundle::NewLC( CMceStreamBundle::ELS );

    // To complete session structure, let the sibling classes add video out
    // and possibly needed audio streams
    CompleteSessionStructureL( *localBundle );

    // Create stream to display and share source with video outstream
    CMceVideoStream* streamForDisplay = CMceVideoStream::NewLC();

    CMceVideoStream* videoOut = MusEngMceUtils::GetVideoOutStreamL( *iSession );
    
    SetMediaSdpLinesL( *videoOut, aForceSdpBandwidth );
		
    streamForDisplay->SetSourceL( videoOut->Source() );

    AddDisplayL( *streamForDisplay );

    iSession->AddStreamL( streamForDisplay );
    CleanupStack::Pop( streamForDisplay );    

    // Check if there are audio streams to bundle with
    if ( localBundle->Streams().Count() > 0 ) 
        {
        localBundle->AddStreamL( *streamForDisplay );
        iSession->AddBundleL( localBundle );
        CleanupStack::Pop( localBundle );
        }
    else
        {
        CleanupStack::PopAndDestroy( localBundle );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::CreateMceSessionStructureL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::DoInviteL( const TDesC& aRecipient )
    {
    MUS_LOG_TDESC( "mus: [ENGINE]  -> CMusEngMceOutSession::DoInviteL(): ",
                   aRecipient ) 

    HBufC* resolvedRecipient = NULL;
    if ( aRecipient.Length() > 0 )
        {
        resolvedRecipient = aRecipient.AllocLC();
        }
    else
        {
        resolvedRecipient = ResolveRecipientLC();
        }
    
    // delete possibly existing recipient
    delete iRecipient;
    iRecipient = NULL;  
    
    TMusEngUriParser parser( *resolvedRecipient ); 
    parser.ParseUriL();    
    iRecipient = parser.GetUri8L();    
    
    /* Set the display name to recipient address if displayname is empty */
    if ( !iRemoteDisplayName || iRemoteDisplayName->Length() == 0 )
        {
        HBufC* tmp = parser.GetUri16L( ETrue );
        delete iRemoteDisplayName;
        iRemoteDisplayName = tmp;
        }
    
    CleanupStack::PopAndDestroy( resolvedRecipient );
    
    if ( iSession )
        {
        MUS_ENG_LOG_SESSION_STATE( *iSession ) 
        
        if ( iSession->State() == CMceSession::EIdle ||
             iSession->State() == CMceSession::ETerminated )
            {
            // This is the case when last invite has ended up to an error,
            // last sharing has ended normally, or construction of the session
            // stucture has not been completed. Delete old session and try to
            // continue normally.
            delete iSession;
            iSession = NULL;
            MUS_LOG( "mus: [ENGINE]     Existing session deleted")
            }
        else
            {
            // Session is already ongoing. Leave.
            User::Leave( KErrAlreadyExists );
            }
      }
    
    const TUint KMusEngOneMinuteTimeout = 60000000;
    if ( iSipProfileHandler->IsRegistered() )
        {
        CreateMceSessionStructureL();   
        EstablishSessionL();
	// Start one minute expiration timer
        TTimeIntervalMicroSeconds32 interval( KMusEngOneMinuteTimeout );
        iDeltaTimer->Remove( iInvitationResponseEntry );
        iDeltaTimer->Queue( interval, iInvitationResponseEntry );
        }
    else
        {
        iRegistrationPending = ETrue;
        // Start one minute expiration timer
        TTimeIntervalMicroSeconds32 interval( KMusEngOneMinuteTimeout );
        iDeltaTimer->Remove( iRegistrationEntry );
        iDeltaTimer->Queue( interval, iRegistrationEntry );
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::DoInviteL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC* CMusEngMceOutSession::ResolveRecipientLC()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::ResolveRecipientLC()" )
    
    HBufC* resolvedRecipient = NULL;
    TRAPD( err, 
        resolvedRecipient = 
            ReadDescPropertyL( NMusSessionApi::KRemoteSipAddress ) );
    if ( err != KErrNone )
        {
        __ASSERT_ALWAYS( err != KErrNoMemory, User::Leave( KErrNoMemory ) );
        resolvedRecipient = KNullDesC().AllocL();
        }
    CleanupStack::PushL( resolvedRecipient );
    
    delete iRemoteSipAddressProposal;
    iRemoteSipAddressProposal = NULL;
    TRAP( err, 
        iRemoteSipAddressProposal = 
            ReadDescPropertyL( NMusSessionApi::KRemoteSipAddressProposal ) );
    if ( err != KErrNone )
        {
        __ASSERT_ALWAYS( err != KErrNoMemory, User::Leave( KErrNoMemory ) );
        iRemoteSipAddressProposal = KNullDesC().AllocL();
        }
    
    if ( resolvedRecipient->Length() > 0 )
        {
        if ( resolvedRecipient->Find( KMusEngCommaSymbol ) != KErrNotFound )
            {
            // Split the addresses using KMusCommaSymbol as a separator
            CDesCArray* addresses =
                new( ELeave )CDesCArrayFlat( KMusEngSipAddressesMaxAmount );
            CleanupStack::PushL( addresses );
            SplitL( *resolvedRecipient, KMusEngCommaSymbol, addresses );          
            
            // Show List Query Dialog
            CleanupStack::Pop( addresses );
            CleanupStack::PopAndDestroy( resolvedRecipient );
            CleanupStack::PushL( addresses );
            resolvedRecipient = HBufC::NewLC( KMusEngSipAddressMaxLength );
            TPtr ptrRetryAddr( resolvedRecipient->Des() ); 
            if ( !LcUiProviderL().SelectRecipient( *addresses, ptrRetryAddr ) )
                {
                // Address not selected from provided list
                User::Leave( KErrNotFound );
                }
            CleanupStack::Pop( resolvedRecipient );
            CleanupStack::PopAndDestroy( addresses );
            CleanupStack::PushL( resolvedRecipient );
            }
        }
    else
        {
        __ASSERT_ALWAYS( ++iTriedInvitations < 2, User::Leave( KErrNotFound ) );
        
        CleanupStack::PopAndDestroy( resolvedRecipient );
        resolvedRecipient = NULL;
        resolvedRecipient = HBufC::NewLC( KMusEngSipAddressMaxLength );
        __ASSERT_ALWAYS( 
            iRemoteSipAddressProposal->Length() < KMusEngSipAddressMaxLength,
            User::Leave( KErrOverflow ) );
        resolvedRecipient->Des().Copy( *iRemoteSipAddressProposal );
        TPtr ptrRetryAddr( resolvedRecipient->Des() );
        InputRecipientL( ptrRetryAddr );
        iAddressQueried = ETrue;
        }
    
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::ResolveRecipientLC()" )
    
    return resolvedRecipient;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusEngMceOutSession::AsyncBrakeCompleted( TAny* aPtr )
    {
    if ( aPtr )
        {
        reinterpret_cast<CMusEngMceOutSession*>( aPtr )->HandleRecipientNotFound();
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusEngMceOutSession::RegistrationTimerExpired( TAny* aPtr )
    {
    if ( aPtr )
        {
        CMusEngMceOutSession* session = 
            reinterpret_cast< CMusEngMceOutSession* >( aPtr );
        session->InformObserverAboutSessionFailure( KErrGeneral );
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusEngMceOutSession::InvitationResponseTimerExpired( TAny* aPtr )
    {
    if ( aPtr )
        {
        CMusEngMceOutSession* session = 
            reinterpret_cast< CMusEngMceOutSession* >( aPtr );
        if ( !session->IgnoreErrorNote() )
            {
            session->InformObserverAboutSessionFailure( MLcSession::ENoAnswer );
            }
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngMceOutSession::IgnoreErrorNote()
    {
    TBool ignore = ETrue;
    if ( iSession && 
          ( iSession->State() == CMceSession::EOffering || 
         iSession->State() == CMceSession::EProceeding ) )
        {
        ignore = EFalse;
        }
     return ignore;   
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::InitRecipientNotFoundHandling()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::InitRecipientNotFoundHandling()" )
    
    iDeltaTimer->Remove( iAsyncBrakeEntry );
    const TUint KMusEngAsyncBrakeTimeout = 1;
    TTimeIntervalMicroSeconds32 interval( KMusEngAsyncBrakeTimeout );
    iDeltaTimer->Queue( interval, iAsyncBrakeEntry );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::InitRecipientNotFoundHandling()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::HandleRecipientNotFound()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::HandleRecipientNotFound()" )
    
    TInt err( KErrNotFound );
    
    TBool retry( EFalse );
    TRAP( err, retry = DoSyncRetryL() );
    
    if ( err != KErrNone || !retry )
        {
        InformObserverAboutSessionFailure( MLcSession::ERecipientNotFound );
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::HandleRecipientNotFound()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngMceOutSession::DoSyncRetryL()
    {
    __ASSERT_ALWAYS( iRemoteSipAddressProposal, User::Leave( KErrNotFound ) );
    __ASSERT_ALWAYS( 
        iRemoteSipAddressProposal->Length() < KMusEngSipAddressMaxLength,
        User::Leave( KErrOverflow ) );    
    
    TBool retry( EFalse );
    
    if ( ++iTriedInvitations < 2 )
        {        
        TBuf<KMusEngSipAddressMaxLength> retryAddress;
        retryAddress.Copy( *iRemoteSipAddressProposal );
        InputRecipientL( retryAddress );
        DoInviteL( retryAddress );

        retry = ETrue;
        iAddressQueried = ETrue;
        }
    return retry;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::SplitL( const TDesC& aDes,
                                   const TDesC& aChar,
                                   CDesCArray* aArray )
    {
    TInt charPos( 0 );
    TPtrC ptr( aDes );
    TBool found = EFalse;
    while( ( charPos = ptr.Find( aChar ) ) > 0 )
        {
        found = ETrue;
        TPtrC str = ptr.Left( charPos );
        aArray->AppendL( str );
        ptr.Set( ptr.Right( ptr.Length() - str.Length() - 1 ) );
        }
    if ( found && ptr.Length() > 0 )
        {
        aArray->AppendL( ptr );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC* CMusEngMceOutSession::ReadDescPropertyL( TUint aKey )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngMceOutSession::ReadDescPropertyL: aKey: [%d]",
                            aKey );

    TBuf<RProperty::KMaxPropertySize> buffer;

    User::LeaveIfError( RProperty::Get( NMusSessionApi::KCategoryUid,
                                        aKey,
                                        buffer ) );
    HBufC* desc = buffer.AllocL();
    MUS_LOG_TDESC( "mus: [ENGINE]    <- CMusEngMceOutSession::ReadDescPropertyL: val: ",
                            (*desc) );
    return desc;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC* CMusEngMceOutSession::RemoteAddressL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::RemoteAddressL()" )
    
    __ASSERT_ALWAYS( iRecipient, User::Leave( KErrNotReady ) );
    
    HBufC* remoteAddr = HBufC::NewLC( iRecipient->Length() );
    TPtr ptrRemoteAddr( remoteAddr->Des() );
    User::LeaveIfError( 
        CnvUtfConverter::ConvertToUnicodeFromUtf8( 
            ptrRemoteAddr, *iRecipient ) );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::RemoteAddressL()" )
    
    CleanupStack::Pop( remoteAddr );
    return remoteAddr;
    }

// -----------------------------------------------------------------------------
// From MLcSession, Should return the displayname of the remote terminal
// If found in phone book than contactname else user entered sip address
// -----------------------------------------------------------------------------
//
const TDesC& CMusEngMceOutSession::RemoteDisplayName()
    {      
    if ( !iRemoteDisplayName )
        {
        return KNullDesC;
        }
    return *iRemoteDisplayName;
    }

// -----------------------------------------------------------------------------
// Query to the user for sip address of the remote party.
// If the user entered a new sip address then reset the displayname 
// to user entered sip address.
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::InputRecipientL( TDes& aRecipientAddress )
    {   
    if ( !LcUiProviderL().InputRecipient( aRecipientAddress ) )
       {
       User::Leave( KErrCancel );
       }    
    /* displayname is no longer valid since user entered a new sip address
     * and displayname will be set to sip address when invitation sent.
     */
    delete iRemoteDisplayName;
    iRemoteDisplayName = NULL;
    }

// End of file
