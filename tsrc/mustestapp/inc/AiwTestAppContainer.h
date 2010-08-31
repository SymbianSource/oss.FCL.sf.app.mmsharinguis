/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares container control for application.
*
*/

#ifndef AIWTESTCONTAINER_H
#define AIWTESTCONTAINER_H

// INCLUDES
#include <coecntrl.h>
   
// FORWARD DECLARATIONS
class CEikLabel; // for example labels

// CLASS DECLARATION

/**
*  CAiwTestContainer  container control class.
*/
class CAiwTestContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        /**
        * EPOC default constructor.
        * @param aRect Frame rectangle for container.
        */
        void ConstructL(const TRect& aRect);

        /**
        * Destructor.
        */
        virtual ~CAiwTestContainer();

        void SetLabelL(TDesC& aText);
        
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
        
        /**
        * From MCoeControlObserver
        * Acts upon changes in the hosted control's state. 
        *
        * @param    aControl    The control changing its state
        * @param    aEventType  The type of control event 
        */
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
                
    private: //data
        
        CEikLabel* iLabel;          // example label
        CEikLabel* iToDoLabel;      // example label        
    };

#endif

// End of File
