/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Used for unit testing other classes.
*
*/


#ifndef MMUSAVASETTINGSSTUB_H
#define MMUSAVASETTINGSSTUB_H

#include "musavasettingsimp.h"

/**
 *  Stub implementation of MMusAvaSettings class.
 *  Used in unit tests of other classes.
 *
 *  @since S60 v3.2
 */
class CMusAvaSettings2 : public CMusAvaSettingsImp
    {
public:

    void ConstructL();

    ~CMusAvaSettings2();


// from baseclass MMusAvaSettings.

    /**
     * From MMusAvaSettings.
     * Returns telephone number.
     *
     * @since S60 v3.2
     * @return TDesC reference containing telephone number.
     */
    virtual const TDesC& TelNumber() const;

    /**
     * Returns the contact name of remote host of active CS call.
     *
     * @return The contact name.
     */
    virtual const TDesC& ContactName() const;

    /**
     * From MMusAvaSettings.
     * Returns proposal for SIP address.
     *
     * @since S60 v3.2
     * @return TDesC reference containing proposal for SIP address.
     */
    virtual const TDesC& SipAddressProposal() const;

    /**
     * From MMusAvaSettings.
     * Returns SIP addresses.
     *
     * @since S60 v3.2
     * @return reference to MDesCArray containing SIP addresses.
     */
    const MDesCArray& SipAddresses() const;

    /**
     * From MMusAvaSettings.
     * Returns contact Id.
     *
     * @since S60 v3.2
     * @return Contact Id.
     */
    virtual TInt ContactId() const;

    /**
     * From MMusAvaSettings.
     * Returns video codecs.
     *
     * @since S60 v3.2
     * @return reference to MDesCArray containing video codecs.
     */
    const MDesCArray& VideoCodecs() const;

    /**
     * From MMusAvaSettings.
     * Returns audio codecs.
     *
     * @since S60 v3.2
     * @return reference to MDesCArray containing audio codecs.
     */
    const MDesCArray& AudioCodecs() const;

    /**
     * From MMusAvaSettings.
     * Returns Id of used SIP profile.
     *
     * @since S60 v3.2
     * @return SIP profile Id.
     */
    virtual TInt SipProfileId() const;

private: // data

    /**
     * Buffer containing telephone number.
     * Own.
     */
    HBufC* iTelNumber;

    /**
     * Buffer containing contact name.
     * Own.
     */
    HBufC* iContactName;

    /**
     * Proposal for SIP address.
     * Own.
     */
    HBufC* iSipAddressProposal;

    /**
     * Id of contact.
     */
    TInt iContactId;

    /**
     * Id of used SIP profile.
     */
    TInt iSipProfileId;

    /**
     * Array of SIP addresses.
     * Own.
     */
    CDesCArrayFlat* iSipAddresses;

    /**
     * Array of video codecs.
     * Own.
     */
    CDesCArrayFlat* iVideoCodecs;

    /**
     * Array of audio codecs.
     * Own.
     */
    CDesCArrayFlat* iAudioCodecs;

    MUS_UNITTEST( UT_CMusAvailabilityPluginManager )

    };

#endif // MMUSAVASETTINGSSTUB_H
