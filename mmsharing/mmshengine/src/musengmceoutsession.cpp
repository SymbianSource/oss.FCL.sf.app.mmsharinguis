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
#include "musengoutsessionobserver.h"
#include "musenglivesessionobserver.h"
#include "mussettings.h"
#include "musengmceutils.h"
#include "musenguriparser.h"
#include "musenglogger.h"
#include "mussesseioninformationapi.h"
#include "mussipprofilehandler.h"

// SYSTEM
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

#include <sipprofile.h>
#include <sipextensionheader.h>
#include <sipaddress.h>
#include <uri8.h>
#include <e32property.h>



const TInt KMusEngSipReasonCodeBadRequest = 400;
const TInt KMusEngSipReasonCodeUnauthorized = 401;
const TInt KMusEngSipReasonCodePaymentRequired = 402;
const TInt KMusEngSipReasonCodeRecipientNotFound  = 404;
const TInt KMusEngSipReasonCodeProxyAuthenticationRequired = 407;
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

using namespace NMusSessionInformationApi;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceOutSession::~CMusEngMceOutSession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::~CMusEngMceOutSession()" )

    delete iRecipient;
    delete iVideoCodecList;
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::~CMusEngMceOutSession()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceOutSession::InviteL( const TDesC& aRecipient )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::InviteL()")

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

    MUS_LOG_TDESC( "mus: [ENGINE]      CMusEngMceOutSession::InviteL() recipient=",
                   aRecipient )

    // delete possibly existing recipient
    delete iRecipient;
    iRecipient = NULL;  
    
    TMusEngUriParser parser( aRecipient ); 
    parser.ParseUriL();    
    iRecipient = parser.GetUri8L();

    CreateMceSessionStructureL();

    EstablishSessionL();

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::InviteL()")
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceOutSession::CancelInviteL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::CancelInviteL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    MUS_ENG_LOG_SESSION_STATE( *iSession ) 

    if( iSession->State() == CMceSession::EOffering )
        {
        MUS_LOG( "mus: [ENGINE]     CMceOutSession->CancelL()" )
        static_cast<CMceOutSession*>( iSession )->CancelL();
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::CancelInviteL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceOutSession::SetSupportedVideoCodecListL( 
                                                const TDesC& aVideoCodecs )
    {
    MUS_LOG_TDESC( "mus: [ENGINE]  -> CMusEngMceOutSession::SetSupportedVideoCodecListL: ",
                   aVideoCodecs )

    HBufC8* newVideoCodecList = 
                    CnvUtfConverter::ConvertFromUnicodeToUtf8L( aVideoCodecs );
    delete iVideoCodecList;
    iVideoCodecList = newVideoCodecList;

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::SetSupportedVideoCodecListL" )
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
      
     /* Add the privacy header if cs call privacy setting is switched on */
    if ( iPrivate && iPrivateNumber )
        {
        AddPrivacyHeaderL( *headers );
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

        case KMusEngSipReasonCodeBadRequest :
            {
            iOutSessionObserver.SessionBadRequest();
            break;
            }
        case KMusEngSipReasonCodeUnauthorized :
            {
            iOutSessionObserver.SessionUnauthorized();
            break;
            }
        case KMusEngSipReasonCodePaymentRequired :
            {
            iOutSessionObserver.SessionPaymentRequired();
            break;
            }
        case KMusEngSipReasonCodeRecipientNotFound : 
            //lint -fallthrough
        case KMusEngSipReasonCode479NotAbleToProcessURI:
            //lint -fallthrough
        case KMusEngSipReasonCodeUnsupportedUriScheme : 
            {
            iOutSessionObserver.SessionRecipientNotFound();
            break;
            }
        case KMusEngSipReasonCodeProxyAuthenticationRequired :
            {
            iOutSessionObserver.SessionProxyAuthenticationRequired();
            break;
            }
        case KMusEngSipReasonCodeRequestTimeout :
            {
            iOutSessionObserver.SessionRequestTimeOut();
            break;
            }
        case KMusEngSipReasonCodeUnsupportedMediaType :
            //lint -fallthrough
        case KMusEngSipReasonCodeNotAcceptableHere:
            //lint -fallthrough
        case KMusEngSipReasonCodeNotAcceptable:
            {
            iOutSessionObserver.SessionUnsupportedMediaType();
            break;
            }
        case KMusEngSipReasonCodeBusyHere :
            {
            // Operator variant uses 486 to rejection instead of 603
            if ( iOperatorVariant )
                {
                iOutSessionObserver.SessionRejected();
                }
            else
                {
                iOutSessionObserver.SessionBusyHere();
                }
            break;
            }
        case KMusEngSipReasonCodeRequestCancelled :  
            {
            iOutSessionObserver.SessionRequestCancelled();
            break;
            }
        case KMusEngSipReasonCodeDecline :
            {
            iOutSessionObserver.SessionRejected();
            break;
            }
        case KMusEngSipReasonCodeTemporarilyNotAvailable :
            {
            iOutSessionObserver.SessionTemporarilyNotAvailable();
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
void CMusEngMceOutSession::AdjustVideoCodecL( CMceVideoCodec& aVideoCodec )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::AdjustVideoCodecL()" )
    
    CMusEngMceSession::AdjustVideoCodecL( aVideoCodec );
    
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
CMusEngMceOutSession::CMusEngMceOutSession( 
                                const TRect& aRect,
                                MMusEngSessionObserver& aSessionObserver,
                                MMusEngOutSessionObserver& aOutSessionObserver )
    : CMusEngMceSession( aRect, aSessionObserver ),      
      iOutSessionObserver( aOutSessionObserver )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::ConstructL( TUint aSipProfileId )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::ConstructL()" )

    CMusEngMceSession::ConstructL();
    iSipProfileHandler->CreateProfileL( aSipProfileId );
    
    // Check if feature specific behavior is expected
    iPrivate = ( MultimediaSharingSettings::PrivacySetting());
        
    NMusSessionInformationApi::TMusClirSetting clir;
    // Ignore RProperty::Get return value.Incase of error it should behave default.
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMusClirSetting,
                    reinterpret_cast<TInt&>( clir ) );
    iPrivateNumber = ( clir == NMusSessionInformationApi::ESendOwnNumber )? EFalse: ETrue;
      
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceOutSession::ConstructL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::CreateMceSessionStructureL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceOutSession::CreateMceSessionStructureL()" )

 	CSIPProfile* profile = iSipProfileHandler->Profile();
    __ASSERT_ALWAYS( profile != NULL, User::Leave( KErrNotReady ) );

    // Create session
    if ( iPrivate && iPrivateNumber )
        {
        HBufC8* originator = KMusAnonymousHeader().AllocLC();
        iSession = CMceOutSession::NewL( *iManager, *profile, *iRecipient, originator );
        CleanupStack::Pop();
        }
    else
        {
        iSession = CMceOutSession::NewL( *iManager, *profile, *iRecipient );
        }


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
        
    SetSessionSdpLinesL( *iSession );

    // Create bundle for local streams
    
    CMceStreamBundle* localBundle = 
                        CMceStreamBundle::NewLC( CMceStreamBundle::ELS );

    // To complete session structure, let the sibling classes add video out
    // and possibly needed audio streams
    CompleteSessionStructureL( *localBundle );

    // Create stream to display and share source with video outstream
    CMceVideoStream* streamForDisplay = CMceVideoStream::NewLC();

    CMceVideoStream* videoOut = MusEngMceUtils::GetVideoOutStreamL( *iSession );
    
    SetMediaSdpLinesL( *videoOut );
		
    streamForDisplay->SetSourceL( videoOut->Source() );

    MusEngMceUtils::AddDisplayL( *streamForDisplay, *iManager, Rect() );

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
// Add Privacy header if own phone number/id should not be sent to remote party
// -----------------------------------------------------------------------------
//
void CMusEngMceOutSession::AddPrivacyHeaderL( CDesC8Array& aHeaders )
    {
    MUS_LOG( "mus: [ENGINE]  -> AddPrivacyHeaderL()" )
    _LIT8( KMusPrivacyHeaderValue, "id" );
    CSIPExtensionHeader* header = CSIPExtensionHeader::NewLC( 
                                  KMusPrivacyHeader, KMusPrivacyHeaderValue );
    HBufC8* headInText = header->ToTextL();
    MUS_LOG_TDESC8( " mus: [ENGINE] Privacy header : ", headInText->Des() );
    CleanupStack::PopAndDestroy( header );
    CleanupStack::PushL( headInText );
    aHeaders.AppendL( *headInText );
    CleanupStack::PopAndDestroy( headInText );
    MUS_LOG( "mus: [ENGINE]  <- AddPrivacyHeaderL()" )
    }
