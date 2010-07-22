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

#ifndef __INDICATORTESTAPP_APPUI_H__
#define __INDICATORTESTAPP_APPUI_H__

#include <aknappui.h>
#include "musindicatorobserver.h"

// Forward reference
class CIndicatorTestAppAppView;
class CMusIndicatorApi;

/*! 
  @class CIndicatorTestAppAppUi
  
  @discussion An instance of class CIndicatorTestAppAppUi is the UserInterface part of the AVKON
  application framework for the IndicatorTestApp example application
  */
class CIndicatorTestAppAppUi : public CAknAppUi, public MMusIndicatorObserver
    {
public:
/*!
  @function ConstructL
  
  @discussion Perform the second phase construction of a CIndicatorTestAppAppUi object
  this needs to be public due to the way the framework constructs the AppUi 
  */
    void ConstructL();

/*!
  @function CIndicatorTestAppAppUi
  
  @discussion Perform the first phase of two phase construction.
  This needs to be public due to the way the framework constructs the AppUi 
  */
    CIndicatorTestAppAppUi();


/*!
  @function ~CIndicatorTestAppAppUi
  
  @discussion Destroy the object and release all memory objects
  */
    ~CIndicatorTestAppAppUi();


public: // from CAknAppUi
/*!
  @function HandleCommandL
  
  @discussion Handle user menu selections
  @param aCommand the enumerated code for the option selected
  */
    void HandleCommandL(TInt aCommand);

private: // from MMusIndicatorObserver

    void StartLiveSharingL();

	void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);


private:
/*! @var iAppView The application view */
    CIndicatorTestAppAppView* iAppView;
    
    
    CMusIndicatorApi* iIndicator; 
    };


#endif // __INDICATORTESTAPP_APPUI_H__

