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
#include "mussettings.h"

#include <mussettingskeys.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sdpdocument.h>
#include <sdpmediafield.h>
#include <sdpcodecstringpool.h>
#include <sdpcodecstringconstants.h>
#include <sdpattributefield.h>

_LIT8( KSendRecv, "sendrecv");                  // For attribute checks
_LIT8( KRecvOnly, "recvonly");                  // For attribute checks
_LIT8( KSendOnly, "sendonly" );                 // For attribute checks

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
    if ( iCloseStringPool )
        {
        SdpCodecStringPool::Close();
        }
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
    
    TRAPD( err, SdpCodecStringPool::OpenL() );
    
    switch ( err )
        {
        case KErrNone:
            {
            //close pool at destructor, not opened by others
            iCloseStringPool = ETrue;
            break;
            }
        
        case KErrAlreadyExists:
            {
            //already opened, do not try to close at destructor
            iCloseStringPool = EFalse;
            break;
            }
        
        default:
            {
            User::Leave( err );
            }
        }
    
    MUS_LOG( "mus: [MUSSCR]  <- CMusSipCrPlugin::ConstructL()" );
    }


// -------------------------------------------------------------------------
// CMusSipCrPlugin::ChannelL
// -------------------------------------------------------------------------
//
TUid CMusSipCrPlugin::ChannelL( RStringF aMethod,
    const TDesC8& /*aRequestUri*/,
    const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
    const TDesC8& aContent,
    const CSIPContentTypeHeader* /*aContentType*/ )
    {
    MUS_LOG( "mus: [MUSSCR]  -> CMusSipCrPlugin::ChannelL()" );
    
    if ( aMethod == SIPStrings::StringF( SipStrConsts::EOptions ) )
        {
        TUid uid;
        uid.iUid = ( TInt ) CMusManager::ESipOptions;
        MUS_LOG1( "mus: [MUSSCR]     <- CMusSipCrPlugin::ChannelL(): KSipOptions %d",
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
            MUS_LOG1( "mus: [MUSSCR]     <- CMusSipCrPlugin::ChannelL(): \
                      KNotAllowedSipInvite %d", uid.iUid );
            return uid;
            }
        else
            {
            TUid uid;
            uid.iUid = ( TInt ) CMusManager::ESipInviteDesired;
            TBool twoWaySupported = MultimediaSharingSettings::VideoDirectionL() == 
                                    MusSettingsKeys::ETwoWayVideo;
            
            if ( ( aContent.Length() > 0 ) && twoWaySupported )
                {
                MUS_LOG( "mus: [MUSSCR]  2 way supported, parsing SDP..." );
                CSdpDocument* sdpDocument = CSdpDocument::DecodeLC( aContent );
                TBool sendRecv = CheckForSendRecvAttributeL( sdpDocument->MediaFields() );
                if ( sendRecv )
                    {
                    uid.iUid = ( TInt ) CMusManager::ESipInviteDesired2WayVideo;
                    }
                    
                CleanupStack::PopAndDestroy( sdpDocument );
                }

            MUS_LOG1( "mus: [MUSSCR]     <- CMusSipCrPlugin::ChannelL(): \
                      KAllowedSipInvite %d", uid.iUid );
            return uid;
            }
        }
    }

// -------------------------------------------------------------------------
// CMusSipCrPlugin::CheckForSendRecvAttribute
// -------------------------------------------------------------------------
//
TBool CMusSipCrPlugin::CheckForSendRecvAttributeL(
    RPointerArray<CSdpMediaField>& aFields ) const
    {
    MUS_LOG( "mus: [MUSSCR]  -> CMusSipCrPlugin::CheckForSendRecvAttribute()" );
    
    TBool sendRecv = EFalse;
    TBool sendAttrFound = EFalse;
    TBool videoFound = EFalse;
    RStringF videoType = SdpCodecStringPool::StringPoolL().StringF(
                    SdpCodecStringConstants::EMediaVideo,
                    SdpCodecStringPool::StringTableL() );
    const TInt fieldcount = aFields.Count();
    
    for ( TInt i = 0; i < fieldcount && !videoFound; i++ )
        {
        CSdpMediaField* mField = aFields[i];
        
        //only check video fields
        videoFound = mField->Media() == videoType;
        
        if ( videoFound )
            {
            RPointerArray< CSdpAttributeField > attrList =
                mField->AttributeFields();
            
            TInt attrCount = attrList.Count();
            for (TInt j = 0; j < attrCount && !sendAttrFound; j++ )
                {
                CSdpAttributeField* attributeField = attrList[j];
                RStringF attribute = attributeField->Attribute();                
                
                if ( KErrNotFound != attribute.DesC().FindF( KSendRecv ) )
                    {
                    sendRecv = ETrue;
                    sendAttrFound = ETrue;
                    MUS_LOG( "mus: [MUSSCR]  <sendrecv> attribute found!" );
                    }
                else if ( ( KErrNotFound != attribute.DesC().FindF( KSendOnly ) ) ||
                          ( KErrNotFound != attribute.DesC().FindF( KRecvOnly ) ) )
                    {
                    MUS_LOG( "mus: [MUSSCR]  <sendonly>/<recvonly> attribute found!" );
                    sendAttrFound = ETrue;
                    }
                }
            
            if ( !sendAttrFound )
                {
                MUS_LOG( "mus: [MUSSCR]  no send/recv related attributes found!" );
                sendRecv = ETrue;
                }
            }
        }
    MUS_LOG1( "mus: [MUSSCR]  <- CMusSipCrPlugin::CheckForSendRecvAttribute(), \
            sendrecv: %d", sendRecv );
   
    return sendRecv;
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
         aUid.iUid == CMusManager::ESipInviteDesired2WayVideo ||
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




