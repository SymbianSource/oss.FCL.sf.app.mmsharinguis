/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares document for application.
*
*/

#ifndef CPHONERESOURCE_H
#define CPHONERESOURCE_H

// INCLUDES
#include <akndoc.h>
#include <e32cmn.h>
#include <BADESCA.H>
#include <centralrepository.h>
#include "musmanagercommon.h"
#include "musmanager.h"
   
// CONSTANTS
const TInt KMaxUidStringLength = 8;

// FORWARD DECLARATIONS
class CPhoneResources;
class COtherResources;
// CLASS DECLARATION

class COtherResources : public CBase
	{
	public:
		static COtherResources* NewL();
		static COtherResources* NewLC();
		~COtherResources();       
    public :
    
        void SaveL();
        void SaveCRKeyValuesL();
        void ReadL();
        void ReadCRKeyValuesL();
        void ReadKeyValueL(CRepository* repository, const TInt & aKey , TInt & aVal);
        void SetKeyValueL(CRepository* repository, const TInt & aKey , TInt & aVal);

        void ReadKeyValueL(CRepository* repository, TInt aKey, TDes& aVal);
        void SetKeyValueL(CRepository* repository, TInt aKey, const TDesC& aVal);

    private : 
        void HandleError(TBool aType ,TInt aError , TPtrC aKeyName );

    public:
        TInt iActivation;
        TInt iOperatorVariant;
        TInt iAuditoryNotification;
        TInt iPopupNotification;
        TInt iPopupNotificationType;
        TInt iEdgeDtmSupport;
        TInt iSessionSetupMethod;
        TInt iAutoRecord;
        TInt iVideoLocation;
        TInt iSipProfileId;
        TInt iUiOrientation; 
        TInt iCapabilityQuery;  
        TInt iProductModeVariation;
        TInt iEncodingDevice;  
        TInt iPrivacyVariation;
        TInt iDummy; 

        TBuf<NCentralRepositoryConstants::KMaxBinaryLength>  iEncoderInfo;
        TBuf<KMaxUidStringLength> iEncodingDeviceStr;
        
    private :
        void ConstructL();
	};

class CPhoneResources : public CBase
	{
	public:
		static CPhoneResources* NewL();
		static CPhoneResources* NewLC();
		~CPhoneResources();       
    public :
        void SaveL();
        void ReadL();        
    private :
        void HandleError(TBool aType ,TInt aError , TPtrC aKeyName );
    public:
        TInt iCameraAvailability;
        TInt iKeypadAvailability;
        TInt iVideoPlayerAvailability;
        TInt iCameraInformation;
        TInt iKodiakPhoneCall;
        TInt iDummy; 

    private :
        void ConstructL();
        TInt SetValueL(TUid aCategory, TUint aKey, TInt aValue);
	};

class CMusApplication : public CBase
	{
	public:
		static CMusApplication* NewL();
		static CMusApplication* NewLC();
		~CMusApplication();       
    public :
        
        TBool Start(TInt aUseCase);
        TBool Stop();
        TInt  MonitorAvailability();
        TInt  Availability();
        TBool AvailabilityInfo();
        void MusUseCaseInfo(TBool aStart);        
    private :
        void HandleError(TInt aError , TPtrC aVal );
    public:
        TInt iUseCase;           
        CMusManager *iMultimediasharing;   
    private :
        void ConstructL();
	};


 
#endif

// End of File

