/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "stringloader.h"

HBufC* StringLoader::iReturnValue = NULL;

HBufC* StringLoader::LoadL( TInt aResourceId, const TDesC& aString )
    {
        //_LIT( KRes , "res" );
        //HBufC* iReturnValue = KRes().AllocL();
    HBufC* res = iReturnValue->AllocL();
    return res;
    }

HBufC* StringLoader::LoadLC( TInt aResourceId, const TDesC& aString )
    {   
        //_LIT( KRes , "res" );
        //HBufC* res = KRes().AllocLC();
    HBufC* res = iReturnValue->AllocLC();
    return res;
    }


HBufC* StringLoader::LoadL( TInt aResourceId )
    {
        //_LIT( KRes , "res" );
        //HBufC* res = KRes().AllocL();
    HBufC* res = iReturnValue->AllocL();
    return res;
    }


HBufC* StringLoader::LoadLC( TInt aResourceId )
    {
    HBufC* res = iReturnValue->AllocLC();
    return res;
    }

HBufC* StringLoader::LoadL( TInt aResourceId, const MDesCArray& aStrings )
    {
    //StringLoader::LoadL( R_MUS_VIEW_NOTE_VIDEO_SAVED,
    //                                         *iSavingTextsArray );    
    HBufC* res = iReturnValue->AllocL();
    return res;
    }

