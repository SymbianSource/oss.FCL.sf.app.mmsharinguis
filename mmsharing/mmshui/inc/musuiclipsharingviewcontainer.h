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



#ifndef MUSUICLIPSHARINGVIEWCONTAINER_H
#define MUSUICLIPSHARINGVIEWCONTAINER_H

#include "musuisendviewcontainer.h"

#include <MMGFetchVerifier.h>        // Media Gallery fetcher verifier 
#include <AknProgressDialog.h>

class CAknProgressDialog;
class CEikProgressInfo;

/**
 *
 *
 *
 */
class CMusUiClipSharingViewContainer :	public CMusUiSendViewContainer,
										public MMGFetchVerifier,
										public MProgressDialogCallback

    {

public:  // constructors and destructor

    /**
     * Symbian second-phase constructor.
     *
     * @param aView         Parent view of container.
     * @param aRect         Frame rectangle for container.
     * @param aInputParams  Packaged data.
     */
    void ConstructL( CMusUiGeneralView* aView, const TRect& aRect );

    /**
     * Destructor.
     */
    ~CMusUiClipSharingViewContainer();


public:	// new functions
	
	void ShowTranscodingProgressDialogL();
	
    void UpdateClipTranscodingPercentageL( TInt aPercentage );
    
    void CancelTranscodingL();

    void RunFetchDialogL();
    
    const TDesC& FetchedFileName();
    
    TBool FileSelected();
	
    void SetDurationValue( TInt aDurationValue );

    void SetPositionValueL( TInt aPositionValue );

    void SetDurationIndicatorVisible( TBool aVisible );
   
public: // from base class MMGFetchVerifier

    /**
     * Verifies the current user selection.
     *
     * @param aSelectedFiles Descriptor array of selected files.
     * @return EFalse if selection can't be used. If ok, returns ETrue.
     */
    TBool VerifySelectionL( const MDesCArray* aSelectedFiles );

public: // from MProgressDialogCallback

    /**
    * Callback method
    *   Get's called when a dialog is dismissed.
    */
    void DialogDismissedL( TInt aButtonId );
    

private: // from base class CCoeControl    
    
    /**
     * Returns the number of control contained by this class.
     *
     * @return           Number of controls contained
     */
    TInt CountComponentControls() const;

    /**
     * Returns handle to control pointed by aIndex.
     *
     * @param aIndex     Wanted control's index [0..n]
     * @return           Handle to wanted control
     */
    CCoeControl* ComponentControl( TInt aIndex ) const;
    
    /**
    * Gets called by the application framework when drawing is
    * needed.
    *
    * @param aRect The rectangle that needs to be drawn.
    */
    void Draw( const TRect& aRect ) const;

    /**
     * Handles key events.
     *
     * @param aEvent  The event that occured.
     * @param aType   Type of key event: EEventKey, EEventKeyUp, or
     *                EEventKeyDown.
     * @return  The container's response to event: either
     *          EKeyWasNotConsumed or EKeyWasConsumed.
     */
    TKeyResponse OfferKeyEventL( const TKeyEvent& aEvent, TEventCode aType );

protected: // data

    /**
    * ETrue if operator specific functionality is needed 
    */
    TBool iOperatorSpecificFunctionality;
private: // data
    
    /**
     * A progress dialog to show the transcoding progress.  Own.
     */
    CAknProgressDialog* iTranscodingDialog;

    /**
     * A progress dialog to show the transcoding progress.  Own.
     */
    CEikProgressInfo* iTranscodingProgress;
    
    HBufC* iFileName;
	
    TBool iFileSelected;
    
    TBool iTranscodingOngoing;
    
    };


#endif // MUSUICLIPSHARINGVIEWCONTAINER_H

// end of file
