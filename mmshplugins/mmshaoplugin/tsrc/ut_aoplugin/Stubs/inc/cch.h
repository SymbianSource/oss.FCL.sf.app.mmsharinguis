/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The new cch client api class
*
*/



#ifndef C_CCH_H
#define C_CCH_H

#include <e32base.h>

//class CCchImpl;
class CCchServiceStub;

const TInt KServiceId = 5;

/**
 *  Class for cch service management
 *
 *  CCchService is used for launching various kinds of service functions
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib ?library
 */
class CCchService : public CBase
    {

public:

 
protected:

    virtual ~CCchService() { };
    };
    
/**
 *  Class for basic cch functionality, getting services
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib cch
 */
class CCch : public CBase
    {

public:

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CCch* NewL();
    
    /**
     * Two-phased constructor.
     */
    IMPORT_C static CCch* NewLC();
    
    /**
     * Destructor.
     */
    ~CCch();

    /**
     * Returns one service
     *
     * @param aId Service id
     * @return Service or NULL if not found. Ownership is not transferred.
     */
    IMPORT_C CCchService* GetService( TInt aId );
    
    /**
     * Returns service ids of all configured services
     *
     * @param  aIds Array of TInt to be filled with ids
     * @return Symbian error code
     */
    IMPORT_C TInt GetServiceIds( RArray<TInt>& aIds ) const;
    
    /**
     * Returns ETrue if connectivity dialogs are allowed in service enabling.
     *
     * @return ETrue if connectivity dialogs are allowed in service enabling
     */
    IMPORT_C TBool ConnectivityDialogsAllowed() const;
       
    /**
     * For allowing/disallowing connectivity dialogs showing in 
     * service enabling.
     * 
     * @param aConnectivityDialogsAllowed Boolean to set connectivity dialogs
     *        allowed/disallowed
     */ 
    IMPORT_C void SetConnectivityDialogsAllowed( 
        TBool aConnectivityDialogsAllowed );

    
        
protected:

    /**
     * The constructor
     */
    CCch();
    
private:
    
    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private: // data

    CCchServiceStub* iServiceStub;
    };


class HelpData
    {
public:
    static HBufC* iUserName;
    static TInt iGetConnectionParamError;
    };

#endif // C_CCH_H
