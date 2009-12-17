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


#ifndef MUSHENGSESSION_H
#define MUSHENGSESSION_H

// USER INCLUDES

//SYSTEM INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class MMusEngSessionObserver;

// CLASS DECLARATION

/**
* Base class for Multimedia Sharing Engine sessions, inherited by
* CMusEngMceSession.
*
* CMusEngSession encapsulates all common activities example using
* resources.
*
* @lib musengine.lib
* @since S60 v3.2
*/
class CMusEngSession : public CBase
    {

    public:

        /**
        * Desctructor, finalize session
        *
        * @since S60 v3.2
        */
        ~CMusEngSession();


    public:  // API FUNCTIONS

        /**
        * Returns currently assigned drawing area
        *
        * @since S60 v3.2
        * @return TRect This session drawing area recttangle
        */
        IMPORT_C TRect Rect() const;

        /**
        * Sets new drawing area
        *
        * @since S60 v3.2
        * @param TRect This session new drawing area rectangle
        */
        IMPORT_C void SetRectL( const TRect& aRect );

        /**
        * Activates/deactivates loadspeaker
        *
        * @since S60 v3.2
        * @param TBool Enable or disable loudspeaker
        * @param TBool Show activation dialog
        */
        IMPORT_C void EnableLoudspeakerL( TBool aEnable, TBool aShowDialog );

        /**
        * Checks is loudspeaker enabled or disabled.
        *
        * @since S60 v3.2
        * @return TBool true if enabled
        */
        IMPORT_C TBool IsLoudSpeakerEnabled() const;

        /**
        * Returns current volume level
        *
        * @since S60 v3.2
        */
        IMPORT_C TInt VolumeL() const;


    public:  // VIRTUAL API FUNCTIONS

        /**
        * Increases volume level by one.
        *
        * @since S60 v3.2
        */
        IMPORT_C virtual void VolumeUpL();

        /**
        * Decreases volume level by one
        *
        * @since S60 v3.2
        */
        IMPORT_C virtual void VolumeDownL();

        /**
        * Enables or disables display. Call to this function is considered
        * as a permission or denial to draw to the display.
        *
        * @since S60 v3.2
        */
        IMPORT_C virtual void EnableDisplayL( TBool aEnable ) = 0;

        /**
        * Mutes current audio stream meaning that any audio is not played or
        * sent to network.
        *
        * @since S60 v3.2
        */
        IMPORT_C virtual void MuteL() = 0;

        /**
        * Unmute current audio stream meaning that all the audio is played
        * and if needed also sent to network.
        *
        * @since S60 v3.2
        */
        IMPORT_C virtual void UnmuteL() = 0;


    protected: // INTERNAL

        virtual void RectChangedL() = 0;


    protected:

        /**
        * Constructor, private cannot be instantiate
        *
        *
        * @since S60 v3.2
        * @param TRect reserved drawing area for this session
        */
        CMusEngSession( const TRect& aRect );


    protected:

        /**
        * second-phase constructor, called by those session specific
        * constructors
        *
        * @since S60 v3.2
        */
        void ConstructL();


    protected: // MEMBERS

        /**
        * Callback pointer to interface, not own
        *
        * @since S60 v3.2
        */
        MMusEngSessionObserver* iSessionObserver;


    public: // MEMBERS

        /**
        * Drawing area rect
        *
        * @since S60 v3.2
        */
        TRect iRect;

        /**
        * Telephone utilities
        *
        * @since S60 v3.2
        */
        //CMusEngTelephoneUtils* iTelephoneUtils;

        TInt iErrorCode;


    };

#endif //MUSHENGSESSION_H