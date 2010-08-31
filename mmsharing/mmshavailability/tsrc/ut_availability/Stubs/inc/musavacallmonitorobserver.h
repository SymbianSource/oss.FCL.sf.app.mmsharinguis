/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit test stub of MMusCallMonitorObserver
*
*/


#ifndef __MUSAVACALLMONITOROBSERVER_H__
#define __MUSAVACALLMONITOROBSERVER_H__

#include "musavaavailabilityobserver.h"
#include "muscallmonitorobserver.h"
#include <e32base.h>

/**
 *  Test stub of MMusCallMonitorObserver
 */
class CMusAvaCallMonitorObserverStub :
	public CBase,
	public MMusCallMonitorObserver                            
	{
public:

	/**
     * Two-phased constructor
 	 *
	 * @return New CMusAvaCallMonitorObserverStub instance
	 */
	static CMusAvaCallMonitorObserverStub* NewL();
	
	/**
     * Destructor
     */
	~CMusAvaCallMonitorObserverStub();

public: // From MMusCallMonitorObserver

	void CallConnectedL( const TDesC& aTelNumber, TBool aIsSipUri );

	void CallHoldL( const TDesC& aTelNumber, TBool aIsSipUri );

	void ConferenceCallL();

	void NoActiveCallL();
	
private:
	/**
     * Constructor
     */
	CMusAvaCallMonitorObserverStub();

    /**
     * Perform the second phase construction
     */
	void ConstructL();
	
	void CopyDescriptorL( const TDesC& aSource );

public:

	// Most recently received tel number. Owned.
	HBufC* iTelNumber;

	// Contains the most recent value a of aIsSipUri parameter, from
	// CallConnectedL or CallHoldL.
	TBool iIsSipUri;
    };


#endif // __MUSAVACALLMONITOROBSERVER_H__
