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


#ifndef MUSENGTELEPHONEUTILS_H
#define MUSENGTELEPHONEUTILS_H


//#include "musunittesting.h"

// SYSTEM
#include <e32base.h>
//#include <mtelephonyaudioroutingobserver.h>
//#include <telephonyaudiorouting.h>

// FORWARD DECLARATIONS
//class CRepository;
//class CTelephonyAudioRouting;

/**
*
* @lib musengine.lib
* @since S60 v3.2
*/
class CMusEngTelephoneUtils : public CBase
                              //public MTelephonyAudioRoutingObserver
    {
    public:

        /**
        *
        * @since S60 v3.2
        */
        static CMusEngTelephoneUtils* NewL();

        /**
        *
        * @since S60 v3.2
        */
        ~CMusEngTelephoneUtils();


    public: // API

        /**
        *
        * @since S60 v3.2
        */
        void LoudspeakerL( TBool aEnable, TBool aShowDialog );

        /**
        *
        * @since S60 v3.2
        */
        TBool IsLoudSpeakerEnabled() const;

        /**
        *
        * @since S60 v3.2
        */
        TInt GetVolumeL() const;

        /**
        *
        * @since S60 v3.2
        */
        void SetVolumeL( TInt aVolume );



    private:

        /**
        *
        * @since S60 v3.2
        */
        CMusEngTelephoneUtils();

        /**
        *
        * @since S60 v3.2
        */
        void ConstructL();


    private: // HELPER

        /**
        *
        * @since S60 v3.2
        */
        TInt ValidateVolume( const TInt aVolume ) const;


    public: // DATA

        /**
         *  Stores the audio routing state at startup.
         */
        //CTelephonyAudioRouting::TAudioOutput iAudioOutputAtStartup;

        /**
        * Telephony audio routing instance. Owned.
        */
        //CTelephonyAudioRouting* iTelephonyAudioRouting;

        /**
        * Central repository. Owned.
        *
        * @since S60 v3.2
        */
        //CRepository* iRepository;

    };


#endif // MUSENGTELEPHONEUTILS_H