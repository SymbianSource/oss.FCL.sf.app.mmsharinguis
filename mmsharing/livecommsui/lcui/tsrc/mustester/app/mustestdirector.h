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
* Description:  
*
*/


#ifndef CMUSTESTDIRECTOR_H
#define CMUSTESTDIRECTOR_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>
#include <musmanagercommon.h>


class CMusTestDirector :  public CBase
	{
    public:
        CMusTestDirector();
        static CMusTestDirector* NewL();
        ~CMusTestDirector();
    public:
        void SetUseCase( MultimediaSharing::TMusUseCase aUseCase );
        void ConfigureLiveSharing() ;
        void ConfigureReceiveSharing();
    private:
        void DeleteProperty(TInt aKey);
        void DeleteProperties();
        void DefinePropertyL( TInt aKey,
                RProperty::TType aType,
                const TDesC& aVal );
        void DefinePropertyL(TInt aKey,RProperty::TType aType,TInt aVal);
        void DefinePropertiesL();
        void DeleteSessionProperties();
        void DefineSessionPropertyL(TInt aKey,RProperty::TType aType,TInt aVal);
        void DefineSessionPropertyL( TInt aKey,RProperty::TType aType,const TDesC& aVal );
        void ConstructL();
    };

#endif // CMUSTESTDIRECTOR_H

