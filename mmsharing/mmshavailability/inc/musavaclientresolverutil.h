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
* Description:  Utility class to handle SIP client resolver cenrep table.
*
*/




#ifndef MUSAVACLIENTRESOLVERUTIL_H
#define MUSAVACLIENTRESOLVERUTIL_H

#include <e32base.h>

class CRepository;
class CSIPProfile;

/**
 *  Utility class to handle SIP client resolver cenrep table.
 *  This class must be used in __VOIP enabled builds in order to use common
 *  SIP client resolver plugin with VoIP.
 *
 *  @code
 *      clientResolver = CMusAvaClientResolverUtil::NewLC();
 *      clientResolver->RegisterClientWithUserL( 
 *              KImplementation, iContactHeaderUser, KResolver );
 *      CleanupStack::PopAndDestroy( clientResolver );
 *  @endcode
 *
 */
NONSHARABLE_CLASS( CMusAvaClientResolverUtil ) : public CBase
    {
public:

    /**
     * Two-phased constructor.
     */
    static CMusAvaClientResolverUtil* NewL();
    /**
     * Two-phased constructor.
     */
    static CMusAvaClientResolverUtil* NewLC();

    /**
    * Destructor.
    */
    virtual ~CMusAvaClientResolverUtil();

    /**
     * Register to SIP client resolver to be able to receive incoming sessions.
     *
     * @param aProfile
     */
    void RegisterClientWithUserL( CSIPProfile& aProfile ) const;
                
    /**
     * Register to SIP client resolver to be able to receive incoming sessions.
     *
     * @param aImplementation UID of client who handles incoming session
     * @param aProfileContactHeaderUser Globally unique identifier of SIP profile
     * @param aResolver UID of client resolver plug-in
     */
    void RegisterClientWithUserL(
                const TUid& aImplementation,
                const TDesC8& aProfileContactHeaderUser, 
                const TUid& aResolver ) const;

    /**
     * Un-register from SIP client resolver to deny accepting incoming sessions.
     *
     * @param aProfile 
     */
    void UnRegisterClientWithUserL( CSIPProfile& aProfile ) const;
                
    /**
     * Un-register from SIP client resolver to deny accepting incoming sessions.
     *
     * @param aImplementation UID of client who handles incoming session
     * @param aProfileContactHeaderUser Globally unique identifier of SIP profile
     */
    void UnRegisterClientWithUserL(
                const TUid& aImplementation,
                const TDesC8& aProfileContactHeaderUser ) const;

	 /**
     * Resolve implementation Uid for incoming session.
     *
     * @param aImplementation UID of client who handles incoming session
     * @param aProfileContactHeaderUser Globally unique identifier of SIP profile
     */
    void GetImplementationUidWithUserL( 
        const TDesC8& aProfileContactHeaderUser,
        TUid& aImplementation ) const;


private:

    /**
     * C++ default constructor.
     */
    CMusAvaClientResolverUtil();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Finds all rows that match given search criterias.
     *
     * @param aImplementation UID of client who handles incoming session
     * @param aProfileContactHeaderUser Globally unique identifier of SIP profile
     * @param aFoundKeys On return contains keys that matched requested rows
     * @return 
     */
    void GetClientWithUserL(
                const TUid& aImplementation, 
                const TDesC8& aProfileContactHeaderUser, 
                RArray<TUint32>& aFoundKeys ) const;
                
    /**
     * Creates unique key
     *
     * @param aNewKey New unique key to be used when inserting row to table
     * @return 
     */
    void CreateNewKeyL( TUint32& aNewKey ) const;

    /**
     * Gets Protocol Profile's User name from Contact header
     * @param aProfile
     * @param aContactHeaderUser
     * @return KErrNone if succeed.
     */                                     
    TInt GetContactHeaderUser( CSIPProfile& aProfile,
                               RBuf8& aContactHeaderUser ) const;


private: // data

    /**
     * Instance of central repository
     * Own.
     */
    CRepository* iRepository;

    };

#endif // C_SIPCLIENTRESOLVERUTILS_H
