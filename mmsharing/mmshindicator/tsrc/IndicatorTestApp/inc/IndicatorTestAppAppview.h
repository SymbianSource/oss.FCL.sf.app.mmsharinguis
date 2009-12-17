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
* Description:
*
*/

#ifndef __INDICATORTESTAPP_APPVIEW_H__
#define __INDICATORTESTAPP_APPVIEW_H__


#include <coecntrl.h>

/*! 
  @class CIndicatorTestAppAppView
  
  @discussion An instance of the Application View object for the IndicatorTestApp 
  example application
  */
class CIndicatorTestAppAppView : public CCoeControl
    {
public:

/*!
  @function NewL
   
  @discussion Create a CIndicatorTestAppAppView object, which will draw itself to aRect
  @param aRect the rectangle this view will be drawn to
  @result a pointer to the created instance of CIndicatorTestAppAppView
  */
    static CIndicatorTestAppAppView* NewL(const TRect& aRect);

/*!
  @function NewLC
   
  @discussion Create a CIndicatorTestAppAppView object, which will draw itself to aRect
  @param aRect the rectangle this view will be drawn to
  @result a pointer to the created instance of CIndicatorTestAppAppView
  */
    static CIndicatorTestAppAppView* NewLC(const TRect& aRect);


/*!
  @function ~CIndicatorTestAppAppView
  
  @discussion Destroy the object and release all memory objects
  */
     ~CIndicatorTestAppAppView();


public:  // from CCoeControl
/*!
  @function Draw
  
  @discussion Draw this CIndicatorTestAppAppView to the screen
  @param aRect the rectangle of this view that needs updating
  */
    void Draw(const TRect& aRect) const;
  

private:

/*!
  @function ConstructL
  
  @discussion  Perform the second phase construction of a CIndicatorTestAppAppView object
  @param aRect the rectangle this view will be drawn to
  */
    void ConstructL(const TRect& aRect);

/*!
  @function CIndicatorTestAppAppView
  
  @discussion Perform the first phase of two phase construction 
  */
    CIndicatorTestAppAppView();
    };


#endif // __INDICATORTESTAPP_APPVIEW_H__
