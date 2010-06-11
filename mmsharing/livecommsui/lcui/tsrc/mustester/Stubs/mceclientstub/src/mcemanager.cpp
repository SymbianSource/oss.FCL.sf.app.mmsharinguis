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


#include "mcemanager.h"
#include "mceinsessionobserver.h"
#include "mcesessionobserver.h"
#include "mcesession.h"
#include "mcetransactiondatacontainer.h"
#include "mceamrcodec.h"
#include "mceh263codec.h"
#include "mceavccodec.h"
// Test simulator
#include "mussessionsimulator.h"

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceManager::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceManager* CMceManager::NewL(
                        const TUid& aAppUid, 
						TMceTransactionDataContainer* aContainer )
    {
    
    CMceManager* self = 
        new (ELeave) CMceManager( aAppUid, aContainer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    
    }


// -----------------------------------------------------------------------------
// CMceManager::~CMceManager
// -----------------------------------------------------------------------------
//
EXPORT_C CMceManager::~CMceManager()
    {
    if (iContainer)
    	{
    	iContainer->Clear();
    	iContainer = 0;
    	}			
    
    iSessions.Reset();
    iSessions.Close();
    
    iSupportedAudioCodecs.ResetAndDestroy();
    iSupportedVideoCodecs.ResetAndDestroy();
    
    delete iSessionSimulator;
    
    }

// -----------------------------------------------------------------------------
// CMceManager::SetInSessionObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetInSessionObserver( 
                        MMceInSessionObserver* aInSessionObserver )
    {
    iInSessionObserver = aInSessionObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::SetInEventObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetInEventObserver( 
                        MMceInEventObserver* /*aInEventObserver*/ )
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// CMceManager::SetInReferObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetInReferObserver( 
                        MMceInReferObserver* /*aInReferObserver*/ )
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// CMceManager::SetSessionObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetSessionObserver( 
                        MMceSessionObserver* aSessionObserver )
    {
    iSessionObserver = aSessionObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::SetEventObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetEventObserver( 
                        MMceEventObserver* /*aEventObserver*/ )
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// CMceManager::SetReferObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetReferObserver( 
                        MMceReferObserver* /*aReferObserver*/ )
    {
    // NOP
    }
    
// -----------------------------------------------------------------------------
// CMceManager::SetMediaObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetMediaObserver( 
                        MMceStreamObserver* aMediaObserver )
    {
    iMediaObserver = aMediaObserver;
    }


// -----------------------------------------------------------------------------
// CMceManager::SetRtpObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetRtpObserver( 
                        MMceRtpObserver* aRtpObserver )
    {
    iRtpObserver = aRtpObserver;
    }


// -----------------------------------------------------------------------------
// CMceManager::SupportedAudioCodecs
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<const CMceAudioCodec>&
                                    CMceManager::SupportedAudioCodecs() const
    {
    return iSupportedAudioCodecs;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<const CMceVideoCodec>& 
                                    CMceManager::SupportedVideoCodecs() const
    {
    return iSupportedVideoCodecs;
    }


// -----------------------------------------------------------------------------
// CMceManager::Sessions
// -----------------------------------------------------------------------------
//
EXPORT_C const RPointerArray<CMceSession>& CMceManager::Sessions() const
    {
    return iSessions;
    }
    

// -----------------------------------------------------------------------------
// CMceManager::SetDefaultHeadersL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceManager::SetDefaultHeadersL( CDesC8Array* aMethods, 
                                               CDesC8Array* aHeaders )
    {
    delete aMethods;
    delete aHeaders;
    }


// -----------------------------------------------------------------------------
// CMceManager::DefaultHeadersL
// -----------------------------------------------------------------------------
//
EXPORT_C CDesC8Array* CMceManager::DefaultHeadersL( const TDesC8& /*aMethod*/ ) const
    {
    return NULL;
    }


// -----------------------------------------------------------------------------
// CMceManager::InSessionObserver
// -----------------------------------------------------------------------------
//
MMceInSessionObserver* CMceManager::InSessionObserver() const
    {
    return iInSessionObserver;
    }
	   

// -----------------------------------------------------------------------------
// CMceManager::SessionObserver
// -----------------------------------------------------------------------------
//
MMceSessionObserver* CMceManager::SessionObserver() const
    {
    return iSessionObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::MediaObserver
// -----------------------------------------------------------------------------
//
MMceStreamObserver* CMceManager::MediaObserver() const
    {
    return iMediaObserver;
    }

// -----------------------------------------------------------------------------
// CMceManager::RtpObserver
// -----------------------------------------------------------------------------
//
MMceRtpObserver* CMceManager::RtpObserver() const
    {
    return iRtpObserver;
    }
    

// -----------------------------------------------------------------------------
// CMceManager::TransactionContainer
// -----------------------------------------------------------------------------
//
TMceTransactionDataContainer* CMceManager::TransactionContainer() const
	{
	return iContainer;
	}


// -----------------------------------------------------------------------------
// CMceManager::CMceManager
// -----------------------------------------------------------------------------
//
CMceManager::CMceManager( const TUid& /*aAppUid*/, 
                        TMceTransactionDataContainer* aContainer )
    :iContainer( aContainer )
    {
    
    }


// -----------------------------------------------------------------------------
// CMceManager::ConstructL
// -----------------------------------------------------------------------------
//
void CMceManager::ConstructL()	
    {
    // Fake having PCMU codec
    iSupportedAudioCodecs.AppendL( CMceAmrCodec::NewLC( KMceSDPNamePCMU() ) );
    CleanupStack::Pop();
    
    iSupportedAudioCodecs.AppendL( CMceAmrCodec::NewLC( KMceSDPNameAMR() ) );
    CleanupStack::Pop();
    
    // Avc codec, level default 1
    iSupportedVideoCodecs.AppendL( CMceAvcCodec::NewLC( KMceSDPNameH264() ) );
    CleanupStack::Pop();

    // Avc codec, level default 1, no profile id or constraint set defined
    CMceVideoCodec* avc_2nd_Level_1 = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    avc_2nd_Level_1->SetAllowedBitrates( KMceAvcBitrateLevel1 );
    iSupportedVideoCodecs.AppendL( avc_2nd_Level_1 );
    CleanupStack::Pop( avc_2nd_Level_1 );    

    // Avc codec, level 1_3
    CMceVideoCodec* avcLevel_1_3 = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    avcLevel_1_3->SetAllowedBitrates( KMceAvcCodecProfileIdBaseline | 
	                                  KMceAvcCodecProfileIopConstraintSet | 
	                                  KMceAvcBitrateLevel1_3 );
    iSupportedVideoCodecs.AppendL( avcLevel_1_3 );
    CleanupStack::Pop( avcLevel_1_3 );

    // H263 codec
    iSupportedVideoCodecs.AppendL( CMceH263Codec::NewLC( KMceSDPNameH2632000() ) );
    CleanupStack::Pop();
    
    // Test simulator
    iSessionSimulator = CMusTestSessionSimulator::NewL( *this );
    }
    
    
