/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The Mus ECOM interface implementation of the SIP
*                stack client.
*
*/



#include "mussipcrplugin.h"
#include "muscommon.h"
#include "muslogger.h"
#include "musmanager.h"
#include "musuid.hrh"
#include "musunittesting.h"

#include <sipstrings.h>
#include <sipstrconsts.h>
#include <featmgr.h>

_LIT8(KCapabilities,
"<SIP_CLIENT ALLOW_STARTING=\"YES\">\
<SIP_HEADERS>\
<ACCEPT value=\"application/sdp\"/>\
<ACCEPT_CONTACT value=\"*;+g.3gpp.cs-voice\"/>\
</SIP_HEADERS>\
<SDP_LINES>\
<LINE name=\"m\" value=\"video 0 RTP/AVP 96\">\
<MEDIA_ATTRIBUTE value=\"application:com.nokia.rtvs\"/>\
<MEDIA_ATTRIBUTE value=\"X-application:com.nokia.rtvs\"/>\
</LINE>\
<LINE name=\"m\" value=\"audio 0 RTP/AVP 97\">\
</LINE>\
</SDP_LINES>\
</SIP_CLIENT>");

// -------------------------------------------------------------------------
// Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusSipCrPlugin* CMusSipCrPlugin::NewL()
    {
    MUS_LOG( "mus: [MUSSCR]  -> CMusSipCrPlugin::NewL()" );
    CMusSipCrPlugin* self = new(ELeave) CMusSipCrPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSSCR]  <- CMusSipCrPlugin::NewL()" );
    return self;
    }


// -------------------------------------------------------------------------
// C++ destructor.
// -------------------------------------------------------------------------
//
CMusSipCrPlugin::~CMusSipCrPlugin()
    {
    MUS_LOG( "mus: [MUSSCR]  -> CMusSipCrPlugin::~CMusSipCrPlugin()" );
    delete iManager;
    MUS_LOG( "mus: [MUSSCR]  <- CMusSipCrPlugin::~CMusSipCrPlugin()" );
    }


// -------------------------------------------------------------------------
// C++ constructor.
// -------------------------------------------------------------------------
//
CMusSipCrPlugin::CMusSipCrPlugin()
    {
    }


// -------------------------------------------------------------------------
// Symbian second-phase constructor.
// -------------------------------------------------------------------------
//
void CMusSipCrPlugin::ConstructL()
    {
    MUS_LOG( "mus: [MUSSCR]  -> CMusSipCrPlugin::ConstructL()" );
    iManager = CMusManager::NewL();
    MUS_LOG( "mus: [MUSSCR]  <- CMusSipCrPlugin::ConstructL()" );
    }


// -------------------------------------------------------------------------
// CMusSipCrPlugin::ChannelL
// -------------------------------------------------------------------------
//
TUid CMusSipCrPlugin::ChannelL( RStringF aMethod,
    const TDesC8& /*aRequestUri*/,
    const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
    const TDesC8& /*aContent*/,
    const CSIPContentTypeHeader* /*aContentType*/)
    {
	FeatureManager::InitializeLibL();
    TBool support = FeatureManager::FeatureSupported( KFeatureIdMultimediaSharing );
	FeatureManager::UnInitializeLib();
	TUid uid;
	uid.iUid = ( TInt ) CMusManager::ESipInviteNotDesired;
	if ( support )
		{
	    uid = DoChannelL( aMethod );  
		}
    return uid;	
    }
	
	
// -------------------------------------------------------------------------
// CMusSipCrPlugin::DoChannelL
// -------------------------------------------------------------------------
TUid CMusSipCrPlugin::DoChannelL( RStringF aMethod )
    {
    MUS_LOG( "mus: [MUSSCR]  -> CMusSipCrPlugin::DoChannelL()" );

    if ( aMethod == SIPStrings::StringF( SipStrConsts::EOptions ) )
        {
        TUid uid;
        uid.iUid = ( TInt ) CMusManager::ESipOptions;
        MUS_LOG1( "mus: [MUSSCR]     <- CMusSipCrPlugin::DoChannelL(): KSipOptions %d",
                  uid.iUid );
        return uid;
        }
    else
        {
        MultimediaSharing::TMusAvailabilityStatus capability
            = iManager->AvailabilityL();

        if( capability < KErrNone || 
            capability >= MultimediaSharing::EErrServerShutDown )
            {
            TUid uid;
            uid.iUid = ( TInt ) CMusManager::ESipInviteNotDesired;
            MUS_LOG1( "mus: [MUSSCR]     <- CMusSipCrPlugin::DoChannelL(): \
                      KNotAllowedSipInvite %d", uid.iUid );
            return uid;
            }
        else
            {
            TUid uid;
            uid.iUid = ( TInt ) CMusManager::ESipInviteDesired;
            MUS_LOG1( "mus: [MUSSCR]     <- CMusSipCrPlugin::DoChannelL(): \
                      KAllowedSipInvite %d", uid.iUid );
            return uid;
            }
        }
    }



// -------------------------------------------------------------------------
// CMusSipCrPlugin::ConnectL
// -------------------------------------------------------------------------
//
void CMusSipCrPlugin::ConnectL( TUid aUid )
    {
    MUS_LOG1( "mus: [MUSSCR]     -> CMusSipCrPlugin::ConnectL( %d )", aUid.iUid );
    if ( aUid.iUid == CMusManager::ESipOptions ||
         aUid.iUid == CMusManager::ESipInviteDesired ||
         aUid.iUid == CMusManager::ESipInviteNotDesired )
        {
        iManager->HandleSipRequestL(( CMusManager::TRequestType ) aUid.iUid );
        }
    else
        {
        MUS_LOG( "mus: [MUSSCR]  <- CMusSipCrPlugin::ChannelL()\
                       ( User::Leave KErrNotSupported )" );
        User::Leave( KErrNotSupported );
        }        
    MUS_LOG( "mus: [MUSSCR]  <- CMusSipCrPlugin::ConnectL()" );
    }


// -------------------------------------------------------------------------
// CMusSipCrPlugin::Capabilities()
// -------------------------------------------------------------------------
//
const TDesC8& CMusSipCrPlugin::Capabilities()
    {
    MUS_LOG( "mus: [MUSSCR]  -> CMusSipCrPlugin::Capabilities()" );
    MUS_LOG( "mus: [MUSSCR]  <- CMusSipCrPlugin::Capabilities()" );
    return KCapabilities;
    }




