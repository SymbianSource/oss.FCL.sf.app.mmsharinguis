/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Draws availability indication to screen using direct screen
*                access.
*
*/



#ifndef C_MUSINDICATORDSA_H
#define C_MUSINDICATORDSA_H


#include "musunittesting.h"
#include <e32base.h>
#include <w32std.h>
#include <coecntrl.h>

const TInt KMusIndicatorDelay = 400000;
const TInt KMusRedrawDelay = 25000;

/**
 *  CMusIndicatorDsa provides multimedia sharing ready indicator "box" 
 *  indcating videosharing availability to user.
 *
 *  @code
 *   // Activates indication
 *  CMusIndicatorDsa* dsa = CMusIndicatorDsa::NewL();
 *
 *  // Deactivates indication
 *  delete dsa;
 *
 *  @endcode
 *
 *  @lib musindicator.lib
 */
class CMusIndicatorDsa : public CCoeControl
    {
public:

    MUS_UNITTEST( UT_CMusIndicatorDsa )

    static CMusIndicatorDsa* NewL();

    /**
     * Destructor.
     */
    ~CMusIndicatorDsa();

    
public: //from CCoeControl
    
    
    /**
     * Draws video sharing ready box and text
     */
    void Draw( const TRect& aRect ) const;
    
    /**
     * Hanldes resource change i.e layout change
     * 
     */
    void HandleResourceChange( TInt aType);
    
private:

    /**
     * Initializes
     */
     void Initialize();

    /**
     * Loads image.
     */
    void LoadImageL();

    /**
     * Loads resource string.
     */
    void LoadResourceStringL();


private:

    CMusIndicatorDsa();

    void ConstructL();

    const CFont& Font() const;

    void TryActivateL();

    
    /**
    * Callback function to periodic timer.
    *
    * @param instanse of this.
    * @return TInt value indicating if function completed succesfully
    */
   static TInt TryActivate( TAny* aThis );

    /**
     * Callback function to periodic timer.
     *
     * @param instanse of this.
     * @return TInt value indicating if function completed succesfully
     */
    static TInt DoHandleResourceChange( TAny* aThis );
   
    void StartPeriodicTimer( const TCallBack& aCallBack) const;
    
    /**
     * Stops peridoic timer.
     */
    void StopPeriodicTimer();
    
    
private: // data


    /**
     * Periodic timer.
     * Own.
     */
    CPeriodic* iPeriodic;

    /**
     * Used screen rectangle.
     */
    TRect iScreenRect;

    /**
     * Used client area size
     */
    TSize iScreenSize;
    
    /**
     * Calculated position of left-top point of text.
     */
    TPoint iTextpos;

    /**
     * Color of DSA boxes border.
     */
    TRgb iBorderColor;

    /**
     * DSA boxes background color.
     */
    TRgb iBorderBgColor;

    /**
     * Color of used font.
     */
    TRgb iFontColor;

    /**
     * Displayed text.
     * Own.
     */
    HBufC* iText;
    
    CFbsBitmap* iIcon;
    CFbsBitmap* iIconMask;
    
    TSize iImageSize;
    TPoint iImagepos;
    
    TCallBack iActivateCallBack;
    TCallBack iRedrawCallBack;
    
    
    };

#endif // C_MUSINDICATORDSA_H
