/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The ECOM interface implementation.
*
*/


#ifndef MUSAIWPROVIDER_H
#define MUSAIWPROVIDER_H


#include "musunittesting.h"
#include "musmanagercommon.h"
#include <AiwServiceIfMenu.h>
#include <ecom/implementationproxy.h>

// forward declarations

class CEikonEnv;
class CMusManager;
class MAiwNotifyCallback;
class RResourceFile;
class CMusIndicatorApi;



// function declarations

IMPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount );

/**
 * CMusAiwProvider
 */
class CMusAiwProvider : public CAiwServiceIfMenu
    {
    MUS_UNITTEST( UT_CMusAiwProvider )
public: // constructors and destructor

    static CMusAiwProvider* NewL();

    virtual ~CMusAiwProvider();

private: // constructors

    CMusAiwProvider();

    void ConstructL();

public: // from CAiwServiceIfBase

    /**
     * Called by the AIW framework to initialise the provider with
     * necessary information.
     *
     * @param aFrameworkCallback Framework-provided callback for the
     *                           provider to send events to the
     *                           framework.
     * @param aInterest          List of criteria items which invoked
     *                           the provider.
     * @exception Symbian OS error code
     */
    void InitialiseL( MAiwNotifyCallback&   aFrameworkCallback,
                      const RCriteriaArray& aInterest );

    /**
     * Executes generic service commands included in criteria.
     *
     * @param aCmdId        The command to be executed.
     * @param aInParamList  Input parameters.  Can be an empty list.
     * @param aOutParamList Output parameters.  Can be an empty list.
     * @param aCmdOptions   Options for the command.  See
     *                      KAiwCmdOpt* constants.
     * @param aCallback     Callback for asynchronous command
     *                      handling, parameter checking, etc.
     * @see enum TServiceCmdOptions in GENERICPARAM.HRH.
     * @exception KErrArgument if callback is missing when required.
     * @exception KErrNotSupported if no provider support service.
     */
    void HandleServiceCmdL( const TInt&                 aCmdId,
                            const CAiwGenericParamList& aInParamList,
                            CAiwGenericParamList&       aOutParamList,
                            TUint                       aCmdOptions = 0,
                            const MAiwNotifyCallback*   aCallback = NULL );

public: // from CAiwServiceIfMenu

    /**
     * Initializes menu pane by adding provider-specific menu
     * items.  The AIW Framework gives the parameters to be used
     * in addition.
     *
     * @param aMenuPane    Menu pane handle.
     * @param aIndex       Position of item where to add menu items.
     * @param aCascadeId   ID of cascade menu item.
     * @param aInParamList The input parameter list for provider's
     *                     parameters checking.
     */
    void InitializeMenuPaneL( CAiwMenuPane&               aMenuPane,
                              TInt                        aIndex,
                              TInt                        aCascadeId,
                              const CAiwGenericParamList& aInParamList );

    /**
     * Handles a menu command invoked by the Handler.
     *
     * @param aMenuCmdId    Command ID for the menu command,
     *                      defined by the provider when adding
     *                      the menu commands.
     * @param aInParamList  Input parameters.  Could be empty list.
     * @param aOutParamList Output parameters.  Could be empty list.
     * @param aCmdOptions   Options for the command.  See
     *                      KAiwCmdOpt* constants.
     * @param aCallback     Callback if asynchronous command
     *                      handling is wanted by consumer.  The
     *                      provider might not support this.  If not,
     *                      the function leaves with KErrNotSupported.
     */
    void HandleMenuCmdL( TInt                        aMenuCmdId,
                         const CAiwGenericParamList& aInParamList,
                         CAiwGenericParamList&       aOutParamList,
                         TUint                       aCmdOptions = 0,
                         const MAiwNotifyCallback*   aCallback = NULL );


private: // helper functions

    TBool ApplicationRunningL();
    
    void NoVariationStartUpL( TFileName&    aFileName,
    						 CAiwMenuPane& aMenuPane,
                             TInt          aIndex );
    
    void OperatorVariantStartUpL( TFileName&    aFileName,
    							 CAiwMenuPane& aMenuPane,
                                 TInt          aIndex );

    void ShowErrorNoteL( TInt aError );

    HBufC* ErrorNoteTextLC( TInt aError );
    
    HBufC8* ErrorNoteText8L( TInt aError, RResourceFile& aResourceFile );

    void DoInitialiseL();
	
    void DoInitializeMenuPaneL( CAiwMenuPane& aMenuPane, TInt aIndex );

    
private: // data

    /**  The AIW resource file name. */
    HBufC* iResourceFileName;
    
     /**  Current availability status. */
    MultimediaSharing::TMusAvailabilityStatus iAvailability;
    
    /** To Differentiate Error and Warning Notes */
	TBool iIsWarning ;
	
	CMusIndicatorApi* iIndicator;
    };


#endif // MUSAIWPROVIDER_H
