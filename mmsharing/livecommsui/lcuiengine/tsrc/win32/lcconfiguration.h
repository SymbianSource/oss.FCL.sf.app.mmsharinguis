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

#ifndef LCCONFIGURATION_H
#define LCCONFIGURATION_H

#include <QXmlDefaultHandler>


class LcConfiguration : public QXmlDefaultHandler
{
public: // Constructors and destructor
    LcConfiguration();
    ~LcConfiguration();

public:
    int parse();

protected: // From QXmlContentHandler
    bool startElement(
        const QString& namespaceURI,
        const QString& localName,
        const QString& qName,
        const QXmlAttributes& atts);

public: // Data
    int mLayout;
    QString mSessionTitle;
    QString mRecipient;
    QString mPhoneNumber;
    int mTimerDuration;
};


#endif // LCCONFIGURATION_H





