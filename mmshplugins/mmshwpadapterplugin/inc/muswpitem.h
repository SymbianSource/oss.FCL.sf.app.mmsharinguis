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



#ifndef MUSWPITEM_H
#define MUSWPITEM_H


#include "mussettingskeys.h"
#include "musunittesting.h"
#include <e32base.h>
 
/**
 * 
 */
class CMusWpItem : public CBase
    {
    MUS_UNITTEST( UT_CMusWpItem )
    MUS_UNITTEST( UT_CMusWpAdapter )
    
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CMusWpItem* NewL();

        /**
         * Destructor.
         */
        virtual ~CMusWpItem();


    public: // API
        
        /**
         * Gets PROVIDER-ID param
         */
        const TDesC& ProviderId() const;
        
        /**
         * Sets PROVIDER-ID param
         */
        void SetProviderIdL( const TDesC& aProviderId );

        /**
         * Gets name
         */
        const TDesC& Name() const;
        
        /**
         * Sets name
         */
        void SetNameL( const TDesC& aName);
        
        /**
         * Gets TO-APPREF param
         */
        const TDesC& ToAppRef() const;
        
        /**
         * Sets TO-APPREF param 
         */
        void SetToAppRefL( const TDesC& aToAppRef );

        /**
         * Sets EDGEDTM param
         */
        void SetEdgeDtmSupport( TBool aEdgeDtm );

        /**
         * Sets INTERNETSIGNALING param
         */
        void SetForceInternetSignaling( TBool aInternetSignaling );
        
        /**
         * Sets QUERY param
         */ 
        void SetCapabilityQuery( const TDesC& aValue );
        
        /**
         * Sets CONDITION param
         */ 
        void SetActivationCondition( const TDesC& aValue );
        
        /**
         * Saves this set of settings.
         */
        void SaveL();


    private:  // Constructors

        /**
         * C++ default constructor.
         */
        CMusWpItem();

        /**
         * 2nd phase constructor.
         */
        void ConstructL();


    private:  // data

        //  Identity of video sharing settings provider. Own.
        HBufC* iProviderId;  

        /**
         * User displayable name for multimedia sharing, e.g.
         * "Multimedia Sharing settings". Own.
         */
        HBufC* iName;

        /**
         * The APPREF of sip profile that is referenced from
         * multimeida sharing's TO-APPREF parameter. Own.
         */
        HBufC* iToAppRef;

        
    private: // saved settings
    
        MusSettingsKeys::TEdgeDtmSupport iEdgeDtmSupport;
        MusSettingsKeys::TForceInternetSignaling iForceInternetSignaling;
        MusSettingsKeys::TCapabilityQuery iCapabilityQuery;
        MusSettingsKeys::TActivation iActivationCondition;

    };


#endif // MUSWPITEM_H
