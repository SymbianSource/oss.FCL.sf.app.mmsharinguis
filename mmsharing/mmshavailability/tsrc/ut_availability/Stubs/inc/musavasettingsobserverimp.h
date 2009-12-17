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


#ifndef __UT_MUSAVASETTINGSOBSERVERIMP_H__
#define __UT_MUSAVASETTINGSOBSERVERIMP_H__

//  INCLUDES
#include "musavasettingsobserver.h"
#include "musavaobserver.h"

#include <e32base.h>
#include <e32std.h>

//class MMusAvaObserver;

/**
 *  This class Implements mandatory setting baste behavior which is 
 *  need for availability report.
 *
 *  CMusAvaSettingsObserverImps is implementation class for madatory 
 *  setting which are needed for availability report. 
 *  class represent the interface of application.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class CMusAvaSettingsObserverImp : public CBase,
                                   public MMusAvaSettingsObserver
	{
public:
    
	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return Returns pointer to CMusAvaSettingsObserverImp object
     */
     
	static CMusAvaSettingsObserverImp* NewL();
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	~CMusAvaSettingsObserverImp();
	
protected:
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return 
     */

	CMusAvaSettingsObserverImp( );
	
    /**
     * Perform the second phase construction of a
     *             CMusAvaSettingsObserverImp object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
                           
// from base class MMusAvaSettingsObserver

public:	

    TApplicationState ApplicationState();

    TBool OptionAllowed();
    
public: //data

    TBool iOptionAllowed;
   
	};

#endif //__UT_MUSAVASETTINGSOBSERVERIMP_H__

