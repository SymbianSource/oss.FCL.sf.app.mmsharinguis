/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSUICONTACTUTIL_H
#define MUSUICONTACTUTIL_H

#include <e32base.h>
#include <coemain.h>
#include <CPbkContactEngine.h>
#include <CPbkContactItem.h>

class MusUiContactUtil
    {

    public:

        /**
         */
        static TBool ShareViewFieldExistsL( CCoeEnv& aEnv, TInt aContactId );
    
    	/**
         */
   		static TBool CheckPbContactForVSAddressesL(	CCoeEnv& aEnv,
													const TDesC& aOriginator,
													TInt aContactId);
        /**
         */
        static TBool MatchTextWithPbFieldsL(	CCoeEnv& aEnv,
        									TPbkFieldId aFieldId,
											const TDesC& aText,
											TInt aContactId);
        /**
         */
        static void ActivateContactViewL( CCoeEnv& aEnv, 
                                          TContactItemId aId, 
                                          const TDesC& aTypedAddress, 
                                          const TDesC& aMusTelNumberValue );
        
    private:
    
        /**
         */
        static void EditContactL( CPbkContactItem& aContact,
                                  TBool aNewOne,
                                  CPbkContactEngine* aPbkContactEngine, 
                                  const TDesC& aTypedAddress,
                                  const TDesC& aMusTelNumberValue,
                                  TContactItemId aId );

        /**
         */
        static CContactTextField* EditableContactFieldL(
                                        CPbkContactItem& aContact,
                                        TInt aFieldId,
                                        CPbkContactEngine* aPbkContactEngine );
    };



#endif // MUSUICONTACTUTIL_H
