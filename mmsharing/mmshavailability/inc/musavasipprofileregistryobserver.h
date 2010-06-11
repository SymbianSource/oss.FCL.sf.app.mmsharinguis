/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The observer class for monitoring SIP profiles.
*
*/


#ifndef MSIPPROFILEREGISTRYOBSERVER_H
#define MSIPPROFILEREGISTRYOBSERVER_H

//  INCLUDES
#include "musavasipprofileregistryobserver.h"
#include "musunittesting.h"

#include <e32base.h>
#include <e32std.h>
#include <sipprofileregistryobserver.h>

// FORWARD DECLARATIONS

class MMusAvaSipProfileRegistryAdapter;

// CLASS DECLARATION
/**
*  @publishedAll
*  @released
*
*  The observer class for monitoring SIP profiles.
*  Class must be implemented by the user in order to 
*  observe changes in the SIP profiles.
*/
class CMusAvaSipProfileRegistryObserver : public CBase, 
                                          public MSIPProfileRegistryObserver
																				
    {
public:
    
    /** 
    * 
    **/
    static CMusAvaSipProfileRegistryObserver* NewL();
    
    /** 
    * 
    **/
    ~CMusAvaSipProfileRegistryObserver(); 
    
public:

    /** 
    * 
    **/
    void AddAdapterL( MMusAvaSipProfileRegistryAdapter& aAdapter );
    
    /** 
    * 
    **/
    void RemoveAdapter( MMusAvaSipProfileRegistryAdapter& aAdapter );    
    
public: // from MSIPProfileRegistryObserver

    /** 
    * An event related to SIP Profile has accorred
    * @param aProfileId a profile Id
    * @param aEvent an occurred event
    **/
    void ProfileRegistryEventOccurred(TUint32 aProfileId,
                                              TEvent aEvent);

    /**
    * An asynchronous error has occurred related to SIP profile
    * Event is send to those observers, who have the
    * corresponding profile instantiated.
    * @param aProfileId the id of failed profile 
    * @param aError an occurred error
    */
    void ProfileRegistryErrorOccurred(TUint32 aProfileId,
                                              TInt aError);

private:

    /** 
    * 
    **/
    CMusAvaSipProfileRegistryObserver(); 

private:

    MMusAvaSipProfileRegistryAdapter* iAdapter;

    MUS_UNITTEST( UT_CMusAvaSipProfileRegistryObserver )
    };

#endif // MSIPPROFILEREGISTRYOBSERVER_H
