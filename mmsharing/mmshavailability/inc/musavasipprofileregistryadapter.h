/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The observer class for monitoring SIP profiles.
*
*/


#ifndef MMUSAVASIPPROFILEREGISTRYADAPTER_H
#define MMUSAVASIPPROFILEREGISTRYADAPTER_H

#include <e32std.h>
#include <sipprofileregistryobserver.h>

/**
*  @publishedAll
*  @released
*
*  The observer class for monitoring SIP profiles.
*  Class must be implemented by the user in order to
*  observe changes in the SIP profiles.
*/
class MMusAvaSipProfileRegistryAdapter
    {
public: // from MSIPProfileRegistryObserver

    /**
    * An event related to SIP Profile has accorred
    * @param aProfileId a profile Id
    * @param aEvent an occurred event
    **/
    virtual void ProfileRegistryEventOccurred(
                        TUint32 /*aProfileId*/,
                        MSIPProfileRegistryObserver::TEvent /*aEvent*/ ) { }

    /**
    * An asynchronous error has occurred related to SIP profile
    * Event is send to those observers, who have the
    * corresponding profile instantiated.
    * @param aProfileId the id of failed profile
    * @param aError an occurred error
    */
    virtual void ProfileRegistryErrorOccurred( TUint32 /*aProfileId*/,
                                               TInt /*aError*/ ) { }
    };

#endif // MMUSAVASIPPROFILEREGISTRYADAPTER_H
