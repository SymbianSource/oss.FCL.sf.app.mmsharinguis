/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "LcConfiguration.h"
#include <stdio.h>

// File name
const char fileName[] = "c:\\data\\lcconfig.xml";

// XML configuration elements
const char setupElement[] = "setup";
const char timerElement[] = "timer";

// XML configuration attriputes
const char layoutAttr[] = "layout";
const char sessionNameAttr[] = "sessionname";
const char phoneNoAttr[] = "phoneno";
const char remoteAttr[] = "remote";
const char durationAttr[] = "duration";


// -----------------------------------------------------------------------------
// LcConfiguration::LcConfiguration
// -----------------------------------------------------------------------------
//
LcConfiguration::LcConfiguration()
    :mLayout(5),
    mSessionTitle(QString::fromAscii("Homer Simpson")),
    mRecipient(QString::fromAscii("sip:march@simpsons.com")),
    mPhoneNumber(QString::fromAscii("+35850898282")),
    mTimerDuration(4*1000)
{    
}

// -----------------------------------------------------------------------------
// LcConfiguration::LcConfiguration
// -----------------------------------------------------------------------------
//
LcConfiguration::~LcConfiguration()
{
}

// -----------------------------------------------------------------------------
// TestResultXmlParser::parse
// -----------------------------------------------------------------------------
//
int LcConfiguration::parse()
{
    QFile file(fileName);
    QXmlInputSource inputSource(&file);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    return reader.parse(inputSource);
}

// -----------------------------------------------------------------------------
// LcConfiguration::startElement
// -----------------------------------------------------------------------------
//
bool LcConfiguration::startElement(
    const QString& /*namespaceURI*/,
    const QString& /*localName*/,
    const QString& qName,
    const QXmlAttributes& atts)
{
    if( qName == setupElement) {
        mLayout = atts.value(layoutAttr).toInt();
        mSessionTitle = atts.value(sessionNameAttr);
        mRecipient = atts.value(phoneNoAttr);
        mPhoneNumber = atts.value(durationAttr);
    }
    else if (qName == timerElement) {
        mTimerDuration = atts.value(durationAttr).toInt()*1000;// ms to s
    }    
    return true;
}

// End of File.
