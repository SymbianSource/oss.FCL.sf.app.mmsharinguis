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

#ifndef __INDICATORTESTAPP_DOCUMENT_H__
#define __INDICATORTESTAPP_DOCUMENT_H__


#include <akndoc.h>

// Forward references
class CIndicatorTestAppAppUi;
class CEikApplication;


/*! 
  @class CIndicatorTestAppDocument
  
  @discussion An instance of class CIndicatorTestAppDocument is the Document part of the AVKON
  application framework for the IndicatorTestApp example application
  */
class CIndicatorTestAppDocument : public CAknDocument
    {
public:

/*!
  @function NewL
  
  @discussion Construct a CIndicatorTestAppDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CIndicatorTestAppDocument
  */
    static CIndicatorTestAppDocument* NewL(CEikApplication& aApp);

/*!
  @function NewLC
  
  @discussion Construct a CIndicatorTestAppDocument for the AVKON application aApp 
  using two phase construction, and return a pointer to the created object
  @param aApp application creating this document
  @result a pointer to the created instance of CIndicatorTestAppDocument
  */
    static CIndicatorTestAppDocument* NewLC(CEikApplication& aApp);

/*!
  @function ~CIndicatorTestAppDocument
  
  @discussion Destroy the object and release all memory objects
  */
    ~CIndicatorTestAppDocument();

public: // from CAknDocument
/*!
  @function CreateAppUiL 
  
  @discussion Create a CIndicatorTestAppAppUi object and return a pointer to it
  @result a pointer to the created instance of the AppUi created
  */
    CEikAppUi* CreateAppUiL();

private:

/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CIndicatorTestAppDocument object
  */
    void ConstructL();

/*!
  @function CIndicatorTestAppDocument
  
  @discussion Perform the first phase of two phase construction 
  @param aApp application creating this document
  */
    CIndicatorTestAppDocument(CEikApplication& aApp);

    };


#endif // __INDICATORTESTAPP_DOCUMENT_H__
