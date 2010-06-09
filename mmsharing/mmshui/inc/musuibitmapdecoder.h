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



#ifndef MUSUIBITMAPDECODER_H
#define MUSUIBITMAPDECODER_H

#include <e32base.h>
#include <f32file.h>

class MMusUiBitmapDecoderObserver;
class CImageDecoder;
class CFbsBitmap;

class CMusUiBitmapDecoder : public CActive
    {

public: // constructors and destructor

    /**
     * Destructor.
     */
    ~CMusUiBitmapDecoder();

    /**
     * Two-phased constructor. Leaves on failure.
     * @return The constructed CMusUiBitmapDecoder instance.
     */
    static CMusUiBitmapDecoder* NewL( MMusUiBitmapDecoderObserver& aObserver );

private:

    /**
     * C++ constructor.
     */
    CMusUiBitmapDecoder( MMusUiBitmapDecoderObserver& aObserver );

    /**
     * Symbian 2nd-phase constructor.
     */
    void ConstructL();


public: // new funtions:

    void ConvertL( HBufC8* aData, CFbsBitmap* aBitmap );
    

private: // functions from base class CActive

    void DoCancel();

    void RunL();
    
    TInt RunError( TInt aError );

private:    // Data

    MMusUiBitmapDecoderObserver& iObserver;
    
    CImageDecoder* iDecoder;
    
    CFbsBitmap* iBitmap;
    
    RFs iFs;
    
    HBufC8* iData;
    };

#endif // MUSUIBITMAPDECODER_H

// end of file
