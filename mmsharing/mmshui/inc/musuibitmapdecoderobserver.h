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
* Description:  Mus Applications Event Observer Interface
*
*/



#ifndef MUSUIBITMAPDECODEROBSERVER_H
#define MUSUIBITMAPDECODEROBSERVER_H

class CFbsBitmap;

class MMusUiBitmapDecoderObserver
    {

public:

    virtual void UpdateBitmap( CFbsBitmap* aBitmap ) = 0;

    };

#endif // MUSUIBITMAPDECODEROBSERVER_H

// end of file
