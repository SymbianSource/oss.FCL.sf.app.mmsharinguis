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


#ifndef CWPCHARACTERISTIC_H
#define CWPCHARACTERISTIC_H

// INCLUDES
#include <e32base.h>
#include "mwpelement.h"
#include "mwpvisitor.h"

// FORWARD DECLARATIONS
class CWPParameter;

// CLASS DECLARATION

/**
*  CWPCharacteristic contains one characteristic.
*
*  @lib ProvisioningEngine
*  @since 2.0
*/ 
class CWPCharacteristic : public CBase, public MWPElement
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CWPCharacteristic* NewL( TInt aType );

        /**
        * Two-phased constructor.
        */
        static CWPCharacteristic* NewLC( TInt aType );

        /**
        * Destructor.
        */
        ~CWPCharacteristic();

    public:// New functions

        /**
        * Inserts a parameter to the characteristic. Ownership is tranferred.
        * @param aParameter Parameter to insert.
        */
        IMPORT_C void InsertL(MWPElement* aElement);
        
        /**
        * Sets the characteristic's name.
        * @param aName Characteristic's new name
        */
        IMPORT_C void SetNameL( const TDesC& aValue );

        /**
        * Returns the characteristic's name.
        * @return Characteristic's name
        */
        IMPORT_C const TDesC& Name() const;

        /**
        * Accepts a visitor for the parameters in characteristic.
        * @param aVisitor The visitor to be called for each element.
        */
        IMPORT_C void AcceptL(MWPVisitor& aVisitor);

    public: // From MWPElement

        void CallVisitorL( MWPVisitor& aVisitor );

        TInt Type() const;
        
        void ExternalizeL(RWriteStream& aStream) const;
        
        void InternalizeL(RReadStream& aStream);
        

    private:
        /**
        * C++ default constructor.
        */
        CWPCharacteristic( TInt aType );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:
        /// The type of the CHARACTERISTIC
        TInt iType;

        /// The name of the CHARACTERISTIC
        HBufC* iName;
    
        RPointerArray<MWPElement> iElements;
    };

#endif /* CWPCHARACTERISTIC_H */
