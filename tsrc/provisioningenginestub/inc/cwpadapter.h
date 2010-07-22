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


#ifndef CWPADAPTER_H
#define CWPADAPTER_H

// INCLUDES
#include <E32Base.h>
#include "MWPVisitor.h"

// FORWARD DECLARATIONS
class MWPPairVisitor;
class MWPContextExtension;

// DATA TYPES
enum TWPSaveLeaveCode
    {
    EWPAccessPointsProtected = -49152,
    EWPCommsDBLocked = -49153,
    EWPBookmarksLocked = -49154
    };

// CLASS DECLARATION

/**
 * CWPAdapter has to be implemented by each adapter.
 * CWPAdapter contains only pure virtual methods, which must be 
 * implemented by adapters, and inline methods, which can be
 * used by adapters or engine. Inlining prevents circular binary
 * dependency.
*
*  @lib ProvisioningEngine
*  @since 2.0
 */ 
class CWPAdapter : public CBase, public MWPVisitor
    {
    public:

        /**
        * Destructor.
        */
        IMPORT_C ~CWPAdapter();

    public:

        /**
        * Check the number of settings items.
        * @return Number of items
        */
        virtual TInt ItemCount() const = 0;

        /**
        * Returns the title of the nth summary line. 
        * @param aIndex Number of summary line
        * @return Summary line title
        */
        virtual const TDesC16& SummaryTitle(TInt aIndex) const = 0;
        
        /**
        * Returns the text of the nth summary line. 
        * @param aIndex Number of summary line
        * @return Summary line text
        */
        virtual const TDesC16& SummaryText(TInt aIndex) const = 0;
        
        /**
        * Saves one setting handled by the adapter.
        * @param Setting number
        */
        virtual void SaveL( TInt aItem ) = 0;

        /**
        * Returns ETrue if the adapter can set the settings as default.
        * @param Setting number
        * @return ETrue if the setting can be made default
        */
        virtual TBool CanSetAsDefault( TInt aItem ) const = 0;

        /**
        * Adapter sets the settings as default.
        * @param Setting number
        */
        virtual void SetAsDefaultL( TInt aItem ) = 0;

        /**
        * Calls the pair visitor for each detail line of an item.
        * @param aItem Item whose details are needed
        * @param aVisitor Pair visitor
        * @return KErrNotSupported if not supported
        */
        virtual TInt DetailsL( TInt aItem, MWPPairVisitor& aVisitor ) = 0;

    public: // From MWPVisitor

        void VisitL(CWPCharacteristic& aCharacteristic) = 0;
        void VisitL(CWPParameter& aParameter) = 0;
        void VisitLinkL(CWPCharacteristic& aCharacteristic ) = 0;

    };

#endif /* CWPADAPTER_H */
