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
* Description:  This is implementation class of availability setting class. 
*
*/


#ifndef __MUSAVAINVITERESPONDEROBSERVERIMP_H__
#define __MUSAVAINVITERESPONDEROBSERVERIMP_H__

//  INCLUDES
#include "musavainviteresponderobserver.h"
#include "musavaobserver.h"

#include <e32base.h>
#include <e32std.h>

//class MMusAvaObserver;

/**
 *  This class Implements mandatory setting baste behavior which is 
 *  need for availability report.
 *
 *  CMusAvaAvailabilityObserverImps is implementation class for madatory 
 *  setting which are needed for availability report. 
 *  class represent the interface of application.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class CMusAvaInviteResponderObserverImp : public CBase,
                                          public MMusAvaInviteResponderObserver
	{
public:
    
	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return Returns pointer to CMusAvaInviteResponderObserverImp object
     */
     
	static CMusAvaInviteResponderObserverImp* NewL();
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	~CMusAvaInviteResponderObserverImp();
	
protected:
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return 
     */

	CMusAvaInviteResponderObserverImp( );
	
    /**
     * Perform the second phase construction of a
     *             CMusAvaInviteResponderObserverImp object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
                           
// from base class MMusAvaInviteResponderObserver

public:	

    MMusAvaSettingsObserver::TApplicationState ApplicationState();
    
    void InvitationAnsweredLD();
    
public:

    MMusAvaSettingsObserver::TApplicationState iApplicationState;
	};

#endif

