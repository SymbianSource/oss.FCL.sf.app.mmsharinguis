/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  project specification
*
*/



#ifndef MUSWPADAPTER_H
#define MUSWPADAPTER_H


#include "musunittesting.h"

#include <CWPAdapter.h>


class CWPCharacteristic;
class CMusWpItem;


/**
 * CMusWpAdapter handles application-specific OTA (Over-The-Air) settings
 * of Mus.
 */
class CMusWpAdapter : public CWPAdapter
    {
    MUS_UNITTEST( UT_CMusWpAdapter )
    
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CMusWpAdapter* NewL();

        /**
         * Destructor.
         */
        virtual ~CMusWpAdapter();


    public: // from CWPAdapter, pure virtual in base class

        /**
         * Check the number of settings items.
         *
         * @return Number of items
         */
        TInt ItemCount() const;

        /**
         * Returns the title of the nth summary line.
         *
         * @param aIndex Number of summary line
         * @return Summary line title
         */
        const TDesC16& SummaryTitle(TInt aIndex) const;

        /**
         * Returns the text of the nth summary line.
         *
         * @param aIndex Number of summary line
         * @return Summary line text
         */
        const TDesC16& SummaryText(TInt aIndex) const;

        /**
         * Saves one setting handled by the adapter.
         *
         * @param Setting number
         */
        void SaveL( TInt aItem );

        /**
         * Returns ETrue if the adapter can set the settings as default.
         *
         * @param Setting number
         * @return ETrue if the setting can be made default
         */
        TBool CanSetAsDefault( TInt aItem ) const;

        /**
         * Adapter sets the settings as default.
         *
         * @param Setting number
         */
        void SetAsDefaultL( TInt aItem );

        /**
         * Calls the pair visitor for each detail line of an item.
         *
         * @param aItem Item whose details are needed
         * @param aVisitor Pair visitor
         * @return KErrNotSupported if not supported
         */
        TInt DetailsL( TInt aItem, MWPPairVisitor& aVisitor );


    public: // from CWPAdapter, empty implementations in base class

        /**
         * Informs the adapters of the saved settings.
         *
         * @param TDesC8& aAppIdOfSavingItem. UID of the adapter component.
         * @param HBufC8& aAppRef. APPREF of the saved settings.
         * @param HBufC8& aStorageIdValue. Value that identifies the settings in
         *        its storage.
         * @param TBool& aIsLastOfItsType. ETrue is gotten if the saved set
         *        was last of its type.
         */
        void SettingsSavedL ( const TDesC8& aAppIdOfSavingItem,
                              const TDesC8& aAppRef,
                              const TDesC8& aStorageIdValue );

        /**
         * Informs the adapters of the finalization of saving.
         */
        void SavingFinalizedL();


    public: // from CWPAdapter, which derives them from MWPVisitor

        /**
         * Called for each characteristic found.
         *
         * @param aCharacteristic The characteristic found
         */
        void VisitL(CWPCharacteristic& aElement);

        /**
         * Called for each parameter found.
         *
         * @param aParameter The parameter found
         */
        void VisitL(CWPParameter& aElement);

        /**
         * Called for each link to a logical proxy or access point.
         * No implementation needed.
         *
         * @param aCharacteristic The characteristic found by following the 
         *        link.
         */
        void VisitLinkL(CWPCharacteristic& aLink );


    private:  // Constructors

        /**
         * C++ default constructor.
         */
        CMusWpAdapter();

        /**
         * 2nd phase constructor.
         */
        void ConstructL();


    private:  // data

        // Title to be shown in provisioning message
        HBufC* iSummaryTitle;

        //  The application id of the current characteristic.
        HBufC* iAppID;
        
        // Settings
        CMusWpItem* iWpItem;
        
        // Parsed settings
        CMusWpItem* iNewWpItem;
        
        // ETrue if we have profile id to save
        TBool iProfileIdReceived;
        
        
    private: // saved settings
        
        /**
         * Profile Id of SIP profile saved by SIPAdapter meant
         * to be used with Mus.
         */
        TUint32 iProfileId;


    };


#endif // MUSWPADAPTER_H
