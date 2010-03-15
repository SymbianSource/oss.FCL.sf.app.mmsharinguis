/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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



#ifndef CENREPNOTIFYHANDLER_H
#define CENREPNOTIFYHANDLER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MCenRepNotifyHandlerCallback;
class CRepository;



class CCenRepNotifyHandler : public CBase//CActive
    {
    public:
        /**
        * Defines different key types. Enumeration is used to indicate the 
        * key type that is listened to. 
        */
        enum TCenRepKeyType
        {
            EIntKey,    ///< Key holds a TInt value.
            ERealKey,   ///< Key holds a TReal value.
            EStringKey, ///< Key holds a TDesC16 value.
            EBinaryKey  ///< Key holds a TDesC8 value.
        };

        IMPORT_C static CCenRepNotifyHandler* NewL( MCenRepNotifyHandlerCallback& aCallback, 
                                                    CRepository& aSession );

        IMPORT_C static CCenRepNotifyHandler* NewLC( MCenRepNotifyHandlerCallback& aCallback, 
                                                     CRepository& aSession );
        IMPORT_C void StartListeningL();

        IMPORT_C void StopListening();


        /**
        * Destructor.
        */
        IMPORT_C virtual ~CCenRepNotifyHandler();
/*
    protected:

        void RunL();
    
        TInt RunError( TInt aError );

        void DoCancel();
*/
    private:
                                                   
        CCenRepNotifyHandler( MCenRepNotifyHandlerCallback& aCallback, 
                              CRepository& aSession );
       
    private:
        
        CRepository& iSession;  // not owned by this class
        MCenRepNotifyHandlerCallback& iCallback; // not owned by this class
//        TCenRepKeyType iKeyType;
//        TUint32 iId;        
//        TBool iWholeRepository;  // The flag to indicate if listening is for whole repository
    };


/**
* Class provides a callback interface for handling the notifification
* events from the Central Repository. The Client derives a class 
* from this interface and implements the HandleNotify-methods that 
* interest it.
* An empty default implementation is provided for all of the methods.
* In debug build the default implementations print out a debug trace.
*
* @publishedPartner
* @released
*/
class MCenRepNotifyHandlerCallback
    {

    public:

    /** 
    * This callback method is used to notify the client about
    * changes in keys when the whole repository is listened for.
    *
    * Note: It is not guaranteed that a notification will be received
    *       for all keys, if multiple keys are changed in rapid succession
    *       by multiple threads or when the whole repository is reset,
    *       therefore only listen for whole repository if this is not an issue.
    *
    * @param aId Id of the key that has changed. If multiple keys were changed by
    *            whole repository reset, value will be KInvalidNotificationId.
    * @capability Dependent Capability required depends on implementation of override.
    */
    IMPORT_C virtual void HandleNotifyGeneric( TUint32 aId );

    };

#endif      // CENREPNOTIFYHANDLER_H

// End of File
