/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


#ifndef MUSMANAGERIMPL_H
#define MUSMANAGERIMPL_H

#include "musunittesting.h"
#include "musmanagerclientsession.h"

#include <e32base.h>


enum TServerStart 
    {
    EServerAlreadyRunning,
    EServer
    
    };

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib musmanagerclient.dll
 *  @since S60 v3.2
 */
 
class MMusAvailabilityObserver; 
 
class CMusManagerImpl : public CBase
    {
  	
  	MUS_UNITTEST( UT_CMusManagerImpl )
  	
public: // first phase constructors

    static CMusManagerImpl* NewL();

    static CMusManagerImpl* NewLC();
    
    static CMusManagerImpl* NewL( MMusAvailabilityObserver* aObserver );

    static CMusManagerImpl* NewLC( MMusAvailabilityObserver* aObserver );    
    
    virtual ~CMusManagerImpl();

public: // new functions

    void ExamineAvailabilityL();
    
    MultimediaSharing::TMusAvailabilityStatus AvailabilityL();
    
	/**
	 * This function allows clients to request coomand's to be executed by 
	 * the MUS manager server.
	 *
	 * In erroneous cases, this function will leave
	 * with an error code that describes the error. Positive leave codes
	 * are defined in MultimediaSharing::TMusAvailabilityStatus.
	 *
	 * @param aRequest Defines the coomand type. 
	 *                 
	 */
	void HandleCommandL( MultimediaSharing::TCommandType aCommandType );
    
    void HandleSipRequestL( TInt aChannelId );
    
    void StartApplicationL( MultimediaSharing::TMusUseCase aUseCase );
    
    void StopApplicationL();

private: // second phase constructors

    CMusManagerImpl();
    
    CMusManagerImpl( MMusAvailabilityObserver* aObserver );

    void ConstructL();

private: // private helper functions

    void StartServer();
    
    TBool ServerStarted();
    
    void ConnectL();
    
    void InitSessionL();

private: // private new functions

    MultimediaSharing::TMusAvailabilityStatus QueryAvailabilityL();
    
    void MonitorAvailabilityL( TRequestStatus& aRequestStatus );

    void CancelMonitoring();    

private: // data

    RMusManagerClientSession iSession;

    TBool iConnected;   
    
	TIpcArgs iPcArgs;
	
	TPckgBuf<MultimediaSharing::TMusAvailabilityStatus> iPckg;
	
	MMusAvailabilityObserver* iAvailabilityObserver;
	
private: // class

    /**
     * Active object class listening for 
     * availability status changes
     */
    class CMusManagerImplListener : public CActive
        {
        
    public:
    
        static CMusManagerImplListener* NewL( CMusManagerImpl* aMusManagerImpl );

        virtual ~CMusManagerImplListener();

        void ListenL();

    private: // from CActive

        void RunL();
            
        void DoCancel();
        
        TInt RunError( TInt aError );
        
    private:
    
        CMusManagerImplListener( CMusManagerImpl* aMusManagerImpl );        
        
        void ConstructL();

    private:
     
        CMusManagerImpl* iMusManagerImpl;

        };  	
        
private: // data

	CMusManagerImplListener* iListener; 	
    };




#endif // MUSMANAGERIMPL_H
