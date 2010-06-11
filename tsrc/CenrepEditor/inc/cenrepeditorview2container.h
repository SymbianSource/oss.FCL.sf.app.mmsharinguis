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

#ifndef AVKONVIEWEXAMPLECONTAINER2_H
#define AVKONVIEWEXAMPLECONTAINER2_H

// INCLUDES
#include <coecntrl.h>
#include <AknLists.h>
   
#include "cenrepeditorListbox2.h"
#include "cenrepeditormodel.h"

// CLASS DECLARATION

/**
*  CCenrepEditorView2Container  container control class.
*  
*/
class CCenrepEditorView2Container : public CCoeControl
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect,TInt aResourceId,COtherResources* aOtherResources);

        /**
        * Store the Settings .        
        */
        void StoreSettingsL();

        /**
        * Destructor.
        */
        ~CCenrepEditorView2Container();

    public: // New functions

        

    public: // Functions from base classes

    private: // Functions from base classes

       /**
        * From CoeControl,SizeChanged.
        */
        void SizeChanged();

       /**
        * From CoeControl,CountComponentControls.
        */
        TInt CountComponentControls() const;

       /**
        * From CCoeControl,ComponentControl.
        */
        CCoeControl* ComponentControl(TInt aIndex) const;

       /**
        * From CCoeControl,Draw.
        */
        void Draw(const TRect& aRect) const;   
	

        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

        
    private: //data            
        COtherResourcesListbox * iOtherResourceListBox;

    };

#endif

// End of File
