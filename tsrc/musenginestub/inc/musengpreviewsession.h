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


#ifndef MUSHENGPREVIEWSESSION_H
#define MUSHENGPREVIEWSESSION_H

class MMusEngPreviewSessionObserver;

class CMusEngPreviewSession : public CMusEngSession
    {

    private:
       // MMusEngPreviewSessionObserver* iSessionObserver;

    public:
        /**
         * Implements virtual from CMusEngSession
         */
        IMPORT_C void Play();


        /**
         * Implements virtual from CMusEngSession
         */
        IMPORT_C void Pause();

        /**
         * Implements virtual from CMusEngSession
         */
        IMPORT_C void Close();

        /**
         * Implements virtual from CMusEngSession
         */
        IMPORT_C void Mute();

        /**
         * Implements virtual from CMusEngSession
         */
        IMPORT_C void Unmute();

    public:
        /**
         * Implements 2nd phase constructor from CMusEngSession
         */
        void ConstructL( /*const TDesC& aFileName*/ );

    public:
        /**
        * Sets callback interface pointer
        *
        * @param aSessionObserver Pointer to class which implements interface
        */
        void SetSessionObserver( MMusEngPreviewSessionObserver* aSessionObserver );


        /**
        * Creates new MultimediaSharing Preview session.
        *
        * @param aFileName Media filename
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aEngObserver Engine specific callbacks
        * @param aSessionObserver Session specific callbacks
        * @return CMusEngPreviewSession* New instanse of specified class
        */
        IMPORT_C CMusEngPreviewSession* NewL( const TDesC& aFileName,
                                               const TRect& aRect,
                                               MMusEngPreviewSessionObserver* aSessionObserver);
        ~CMusEngPreviewSession();

    public:
        //CMusEngPreviewSession();
        CMusEngPreviewSession( MMusEngPreviewSessionObserver* aSessionObserver, const TRect& aRect);



        // TODO: FixMe
        void SipProfileChanged()
            {
            };




    };

#endif //
