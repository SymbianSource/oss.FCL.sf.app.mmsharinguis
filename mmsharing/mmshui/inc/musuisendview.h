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
* Description:  The MUS application's UI class.
*
*/



#ifndef MUSUISENDVIEW_H
#define MUSUISENDVIEW_H


#include "musuigeneralview.h"     // Base view class
#include "musuisendobserver.h"
#include <aknappui.h>

class CMusUiSendViewContainer;
class CMusUiSendController;

/**
 *  The MUS application's share view.
 *
 *  @lib musui.exe
 */
class CMusUiSendView : public CMusUiGeneralView,
                       public MMusUiSendObserver
    {
    
public: // constructors and destructor

    /**
    * EPOC default constructor.
    */
	void ConstructL();

    /**
     * Destructor.
     */
    virtual ~CMusUiSendView();
    

public: // from MMusUiSendObserver

    virtual void ManualAddressEntryL( const TDesC& aAddress );

public: // from CMusUiGeneralView

    virtual void DoHandleForegroundEventL( TBool aForeground );     

protected: // New functions

    virtual CMusUiSendController* SendController() const = 0;

    virtual CMusUiSendViewContainer* SendContainer() const = 0;


protected: // from CMusUiGeneralView

    virtual CMusUiEventController* EventController() const;

    virtual CMusUiViewContainer& SharingContainer() const;
    
    
protected: // from base class CAknView

    /**
     * Gets called by the framework when it's activating this view.
     *
     * @param aPrevViewId Previous view id
     * @param aCustomMessageId Custom message's id
     * @param aCustomMessage Custom message
     */
    void DoActivateL( const TVwsViewId& aPrevViewId,
                      TUid aCustomMessageId,
                      const TDesC8& aCustomMessage );

    /**
     * Gets called by the framework when it's deactivating this view.
     */
    void DoDeactivate();

    
    

protected: // data
    
    CAknAppUiBase::TAppUiOrientation iSavedOrientation;
    TBool iPauseSelected;
    
    };

#endif // MUSUISENDVIEW_H

// end of file
