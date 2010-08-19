/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This is module interface class for MusAvailability Plug-in 
*
*/


#ifndef __MUSAVASETTINGSIMP_H__
#define __MUSAVASETTINGSIMP_H__

//  INCLUDES
#include "musavasettings.h"
#include "musavasettingsobserver.h"
#include "musunittesting.h"

#include <e32base.h>
#include <e32std.h>
#include <badesca.h>

class CMusAvaSip;

/**
 *  CMusAvaSettingsImp class represent the interface for module fetcher.
 *
 *  This interface class encapsulates module selection for availability Plug-in.
 *
 *  @lib musavailabilityplugin.lib
 */

class CMusAvaSettingsImp : public CBase,
                           public MMusAvaSettings
	{
public:

    /**
     * Two-phased constructor
     *
     * @return Returns pointer to CMusAvaSettingsImp object
     */
     
	static CMusAvaSettingsImp* NewL();
	
	/**
     * Destructor
     *
     */

	~CMusAvaSettingsImp();
	
	
public: // from MMusAvaSettings

	/**
     * Returns the manual activation
     *
     */   		
    virtual TManualActivation ManualActivation();
	
    /**
     * Returns the telephone number of remote host of active CS call.
     *
     * @return The telephone number.
     */   		
    virtual const TDesC& TelNumber() const;    	
	
    /**
     * Returns the contact name of remote host of active CS call.
     *
     * @return The telephone number.
     */   		
    virtual const TDesC& ContactName() const;    	
    
    /**
     * Returns a guess for the SIP address of the remote host
     *
     * @return The proposal for SIP address
     */   		
    virtual const TDesC& SipAddressProposal() const;    	

    /**
     * Returns the possible SIP addresses of the remote host.
     *
     * @return The SIP addresses in an array.
     */   		
    virtual const MDesCArray& SipAddresses() const;
    
    /**
     * Returns the contact id of the remote host.
     *
     * @return The contact id; KErrNotFound, if not found any.
     */   		
    virtual TInt ContactId() const;

    /**
     * Returns the supported video codecs.
     *
     * @return The video codecs
     */   		
    virtual const MDesCArray& VideoCodecs() const;
    
    /**
     * Returns the supported audio codecs.
     *
     * @return The supported audio codecs.
     */   		
    virtual const MDesCArray& AudioCodecs() const;

    /**
     * Returns the id of the preferable SIP profile.
     *
     * @return The SIP profile id.
     */   		
    virtual TInt SipProfileId() const;
    
     /**
     * Sets the settings observer pointer
     *
     * @param aObserver Pointer to observer intercase 
     */   		
    virtual void SetObserver( MMusAvaSettingsObserver& aObserver );

     /**
     * Sets the for the manual activation
     *
     */   		
    virtual void SetManualActivation( TManualActivation aManualActivation );
    
    
    /**
    * Returns the direction of the mobile call.
    *
    * @return 0, if the direction of the call is unknown.
    *         1, call is from public number.
    *         2, call is from private number   
    */         
    virtual TInt CallPrivacy();

     
public:

    /**
     * Returns the current application state.
     */
    MMusAvaSettingsObserver::TApplicationState ApplicationState();
    
     /**
     * Returns the direction of the mobile call.
     *
     * @return 0, if the direction of the call is unknown.
     *		   1, if the call was originated by this phone.
     *		   2, if the call was terminated by this phone	
     */   	
    TInt CallDirection(); 
    
    /**
     * Sets the call direction of mobile call.
     * @param aDirection direction of mobile call 
     *         0, the direction of the call is unknown.
     *		   1, the call was originated by this phone.
     *		   2, the call was terminated by this phone	
     */   		
    void SetCallDirection( TInt aDirection );    
    
     /**
     * Sets the telephone number of remote host of active CS call.
     * @param aTelNumber telephone number of remote host 
     */   		
    void SetTelNumberL( const TDesC& aTelNumber );   

    /**
     * Sets the contact name of remote host of active CS call.
     * @param aContactName contact name of remote host 
     */   		
    void SetContactNameL( const TDesC& aContactName );    

    
    /**
     * Sets the telephone number of remote host of active CS call.
     * @param aTelNumber username part of remote host 
     * @param aDomain host part of remote host 
     */   		
    void SetSipAddressProposalL( const TDesC& aTelNumber, 
                                 const TDesC& aDomain );    
        
     /**
     * Releases current telphone number 
     *
     */
     void ReleseTelNumberL();	
    
    /**
     * Sets the possible SIP addresses of the remote host.
     *
     * @param aSipAddresses the SIP addresses in an array.
     */   		
    void SetSipAddressesL( const MDesCArray& aSipAddresses );

    /**
     * Overaloaded function.
     * Sets the possible SIP addresses of the remote host.
     *
     * @param aSipAddresses the SIP addresses in an array.
     *        Ownership is transferred.
     */   
    void SetSipAddressesL(CDesCArray* aSipAddresses );
    
    /**
     * Sets the contact id of the remote host.
     *
     * @param aContactId The contact id;
     */   		
    void SetContactId( TInt aContactId );

    /**
     * Sets the supported video codecs.
     *
     * @param aVideoCodecs the video codecs in an array
     */   		
    void SetVideoCodecsL( const MDesCArray& aVideoCodecs);
    
    /**
     * Sets the supported audio codecs.
     *
     * @param aAudioCodecs the supported audio codecs in an array
     */   		
    void SetAudioCodecsL( MDesCArray& aAudioCodecs );

    /**
     * Sets the id of the preferable SIP profile.
     *
     * @param aProfileId the SIP profile id.
     */   		
    void SetSipProfileId( TInt aProfileId );
    
    
    /**
     * Sets the call privacy e.g. current call number is from private number, 
     * should be shown on MT.
     *
     * @param aPrivacy the SIP profile id.
     */  
    void SetCallPrivacy( TInt aPrivacy );
     
    /**
     * Returns the id of the preferable SIP profile.
     *
     * @return The SIP profile id.
     */   		
    CMusAvaSip& Sip();
    
    void SetOptionSentNumber( const TDesC& aTelNumber );
    
    const TDesC& OptionSentTelNumber() const;
    
    void ReleaseOptionSentNumber();
    

     
     
private: 

    void CopyDescArrayL( CDesCArray& aTarget,
                         const MDesCArray& aSource );
     
protected:
	/**
     * Constructor
     *
     */

	CMusAvaSettingsImp();
	
    /**
     * Perform the second phase construction of a 
     * CMusAvaSettingsImp object
     *
     */

	void ConstructL();
	
     
private: // data

    /**
     * Current telphone number
     * Owns.
     */   
    HBufC* iTelNumber;
    
    HBufC* iOptionSentTelNumber;


	/**
     * Current Call Direction
     */   
    TInt iCallDirection;
    
    /**
     * Current contact name
     * Owns.
     */   
    HBufC* iContactName;
     
    /**
     * Current SIP address guess for the remote host
     * Owns.
     */   
    HBufC* iSipAddressProposal;

    /**
     * Contact id of the remote host
     */   
    TInt iContactId;
     
    /**
     * Preferable SIP profile ID
     */   
    TInt iSipProfileId;
    
    /**
     * Current contact list
     * Owns.
     */
	CDesCArray* iSipAddresses;
	
    /**
     * Supported video codecs
     * Owns.
     */
	CDesCArray* iVideoCodecs;
	
    /**
     * Supported audio codecs
     * Owns.
     */
	CDesCArray* iAudioCodecs;
	
    /**
     * Setting observer.
     * Not owns.
     */
	MMusAvaSettingsObserver* iObserver;
	
	/**
     * Setting for the Manual activation.
     */
	TManualActivation iManualActivation;
	
	/**
  * Call privacy indicator
  * 
  */
  
	TInt iCallPrivacy;
	
    MUS_UNITTEST( UT_CMusAvaSettingsImp )	
    MUS_UNITTEST( UT_CMusAvaOptionHandler )   
    MUS_UNITTEST( UT_CMusAvaTerminal )  
	};

#endif __MUSAVASETTINGSIMP_H__

