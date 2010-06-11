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
* Description:  
*
*/


// INCLUDE FILES
#include "cenrepeditormodel.h"
#include "musresourceproperties.h"
#include "mussettingskeys.h"
#include <cenrepeditor.rsg>
#include <e32property.h>
#include <aknnotewrappers.h>
#ifdef _KODIAK_PHONE_CALL
#include <CTSYDomainPSKeys.h>
#else
const TUid KPSUidCtsyCallInformation = {0x102029AC};
const TUint32 KCTsyKodiakPtt = 0x00000003;    
enum TPSCTsyKodiakPtt
    {
    EPSCTsyKodiakPttNotActive,
    EPSCTsyKodiakPttActive
    };
#endif


// ================= MEMBER FUNCTIONS =======================

CPhoneResources* CPhoneResources::NewLC()
    {
    CPhoneResources* self = new (ELeave) CPhoneResources();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPhoneResources* CPhoneResources::NewL()
    {
    CPhoneResources* self = CPhoneResources::NewLC();
    CleanupStack::Pop();
    return self;
    }

CPhoneResources::~CPhoneResources()
    {
    }

void CPhoneResources::ConstructL()
	{
    // If a P&S key is not found, set it to KErrNotFound.
	// This is why the RProperty::Get return values are not handled below.
	iCameraAvailability = KErrNotFound;
    RProperty::Get(NMusResourceApi::KCategoryUid, 
                   NMusResourceApi::KCameraAvailability, 
                   iCameraAvailability);

    iKeypadAvailability = KErrNotFound;
    RProperty::Get(NMusResourceApi::KCategoryUid, 
                   NMusResourceApi::KKeypadAvailability, 
                   iKeypadAvailability);

    iVideoPlayerAvailability = KErrNotFound;  
    RProperty::Get(NMusResourceApi::KCategoryUid, 
                   NMusResourceApi::KVideoPlayerAvailability, 
                   iVideoPlayerAvailability );

    iCameraInformation = KErrNotFound;    
    RProperty::Get(NMusResourceApi::KCategoryUid, 
                   NMusResourceApi::KCameraInformation, 
                   iCameraInformation );   
    
    iKodiakPhoneCall = KErrNotFound;
    TInt err = RProperty::Get(KPSUidCtsyCallInformation, 
                              KCTsyKodiakPtt, 
                              iKodiakPhoneCall );
    if (err != KErrNone)
        {  
        iKodiakPhoneCall = EPSCTsyKodiakPttNotActive;        
        RProperty::Define(KPSUidCtsyCallInformation,KCTsyKodiakPtt,EPSCTsyKodiakPttNotActive);
        }
	}

void CPhoneResources::SaveL()
    {
    SetValueL(NMusResourceApi::KCategoryUid, 
              NMusResourceApi::KCameraAvailability, 
              iCameraAvailability);

    SetValueL(NMusResourceApi::KCategoryUid, 
              NMusResourceApi::KKeypadAvailability, 
              iKeypadAvailability);

    SetValueL(NMusResourceApi::KCategoryUid, 
              NMusResourceApi::KVideoPlayerAvailability, 
              iVideoPlayerAvailability);

    SetValueL(NMusResourceApi::KCategoryUid, 
              NMusResourceApi::KCameraInformation, 
              iCameraInformation);

    SetValueL(KPSUidCtsyCallInformation, 
              KCTsyKodiakPtt, 
              iKodiakPhoneCall);                
    }

void CPhoneResources::SetValueL(TUid aCategory, TUint aKey, TInt aValue)
    {
    // Don't care if the key is not found initially
    if (aValue >= 0)
        {
        User::LeaveIfError(RProperty::Set(aCategory, aKey, aValue));
        }
    }

COtherResources* COtherResources::NewLC()
    {
    COtherResources* self = new (ELeave) COtherResources();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

COtherResources* COtherResources::NewL()
    {
    COtherResources* self = COtherResources::NewLC();
    CleanupStack::Pop();
    return self;
    }

void COtherResources::ConstructL()
    {
    iRepository = CRepository::NewL(MusSettingsKeys::KRepositoryUid);
    
    // If a CenRep key is not found, set it to KErrNotFound.
    // This is why the CRepository::Get return values are not handled below.
    iActivation = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KActivation, iActivation);
    
    iOperatorVariant = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KOperatorVariant,iOperatorVariant);
    
    iAuditoryNotification = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KAuditoryNotification, iAuditoryNotification);
    
    iPopupNotification = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KPopupNotification, iPopupNotification);
    
    iPopupNotificationType = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KPopupNotificationType, iPopupNotificationType);
    
    iEdgeDtmSupport = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KEdgeDtmSupport, iEdgeDtmSupport);
    
    iSessionSetupMethod = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KForceInternetSignaling, iSessionSetupMethod);
    
    iAutoRecord = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KAutoRecord, iAutoRecord);
    
    iVideoLocation = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KVideoLocation, iVideoLocation);
    
    iSipProfileId = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KSipProfileId, iSipProfileId);
    
    iUiOrientation = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KUiOrientation, iUiOrientation);
    
    iCapabilityQuery = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KCapabilityQuery, iCapabilityQuery);
    
    iProductModeVariation = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KProductModeVariation, iProductModeVariation);    
    
    iRepository->Get(MusSettingsKeys::KEncoderConfigurationInfo, iEncoderInfo );
    
    iEncodingDevice = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KEncodingDevice, iEncodingDevice );
    iEncodingDeviceStr.Zero();
    if ( iEncodingDevice >= 0 )
        {
        iEncodingDeviceStr.AppendNum(iEncodingDevice, EHex); 
        }
    
    iOnlyWithActiveCSCall = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KAllowOnlyWithActiveCSCall, iOnlyWithActiveCSCall);
    
    iOnlyIn3GNetwork = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KAllowOnlyIn3GNetwork, iOnlyIn3GNetwork); 
    
    iCameraUsage = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KCameraUsage, iCameraUsage); 
    
    iVideoDirection = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KVideoDirection, iVideoDirection);
    
    iVideoBandwidth = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KVideoBandwidth, iVideoBandwidth);
    
    iFastMode = KErrNotFound;
    iRepository->Get(MusSettingsKeys::KFastStartupMode, iFastMode);
    }

COtherResources::~COtherResources()
    {
    delete iRepository;
    }

void COtherResources::SaveL()
    {
    User::LeaveIfError(
        iRepository->StartTransaction(CRepository::EConcurrentReadWriteTransaction));    
    iRepository->CleanupCancelTransactionPushL();
    
    SetKeyValueL(MusSettingsKeys::KActivation, iActivation);
 	SetKeyValueL(MusSettingsKeys::KOperatorVariant, iOperatorVariant);
 	SetKeyValueL(MusSettingsKeys::KAuditoryNotification, iAuditoryNotification);
	SetKeyValueL(MusSettingsKeys::KPopupNotification, iPopupNotification);
	SetKeyValueL(MusSettingsKeys::KPopupNotificationType, iPopupNotificationType);
	SetKeyValueL(MusSettingsKeys::KEdgeDtmSupport, iEdgeDtmSupport);
	SetKeyValueL(MusSettingsKeys::KForceInternetSignaling, iSessionSetupMethod);
	SetKeyValueL(MusSettingsKeys::KAutoRecord, iAutoRecord);
	SetKeyValueL(MusSettingsKeys::KVideoLocation, iVideoLocation);
	SetKeyValueL(MusSettingsKeys::KSipProfileId, iSipProfileId);
	SetKeyValueL(MusSettingsKeys::KUiOrientation, iUiOrientation);    
	SetKeyValueL(MusSettingsKeys::KCapabilityQuery, iCapabilityQuery);    
    SetKeyValueL(MusSettingsKeys::KProductModeVariation, iProductModeVariation);    
    User::LeaveIfError( 
        iRepository->Set(MusSettingsKeys::KEncoderConfigurationInfo, iEncoderInfo));
    TLex lex( iEncodingDeviceStr );
    TUint tmpUid;
    lex.SkipSpace();
    if ( lex.Val(tmpUid, EHex) == KErrNone )
        {
        iEncodingDevice = tmpUid;     
        }
    else
        {
        iEncodingDevice = KErrNotFound;
        }
    SetKeyValueL(MusSettingsKeys::KEncodingDevice, iEncodingDevice);     
    SetKeyValueL(MusSettingsKeys::KAllowOnlyWithActiveCSCall, iOnlyWithActiveCSCall);
    SetKeyValueL(MusSettingsKeys::KAllowOnlyIn3GNetwork, iOnlyIn3GNetwork);
    SetKeyValueL(MusSettingsKeys::KCameraUsage, iCameraUsage);
    SetKeyValueL(MusSettingsKeys::KVideoDirection, iVideoDirection);
    SetKeyValueL(MusSettingsKeys::KVideoBandwidth, iVideoBandwidth);
    SetKeyValueL(MusSettingsKeys::KFastStartupMode, iFastMode);
    
    TUint32 modifiedKeyCount(0);
    User::LeaveIfError(iRepository->CommitTransaction(modifiedKeyCount));
    CleanupStack::Pop(); // transaction
    }

void COtherResources::SetKeyValueL(
    const TUint32& aKey, 
    TInt& aVal)
    {     
    TBool keyExistsInRepository(EFalse);
    TInt tmpVal(0);
    keyExistsInRepository = (iRepository->Get(aKey,tmpVal) == KErrNone);
    if (aVal < 0)
        {
        if (keyExistsInRepository)
            {
            User::LeaveIfError(iRepository->Delete(aKey));
            }
        }
    else
        {
        if (keyExistsInRepository)
            {
            User::LeaveIfError(iRepository->Set(aKey,aVal));
            }
        else
            {
            User::LeaveIfError(iRepository->Create(aKey,aVal));
            }
        }    
    }

CMusApplication* CMusApplication::NewLC()
    {
    CMusApplication* self = new (ELeave) CMusApplication();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CMusApplication* CMusApplication::NewL()
    {
    CMusApplication* self = CMusApplication::NewLC();
    CleanupStack::Pop();
    return self;
    }

void CMusApplication::ConstructL()
    {
    iUseCase = MultimediaSharing::EMusLiveVideo;     
    iMultimediasharing=CMusManager::NewL();    
    iMultimediasharing->ExamineAvailabilityL();
    }

CMusApplication::~CMusApplication()
	{
    delete iMultimediasharing;
    iMultimediasharing=NULL;
	}

void CMusApplication::HandleError(TInt aError , TPtrC aVal )
    {
    CAknErrorNote *dlg = new CAknErrorNote(ETrue);
    TBuf<100> buf;        
    buf.Append(aVal);
    buf.Append(_L(" Error "));
    buf.AppendNum(aError);
    dlg->ExecuteLD(buf); 
    }

TBool CMusApplication::Start(TInt aUseCase)
    {
    TRAPD(err,iMultimediasharing->StartApplicationL(
                            MultimediaSharing::TMusUseCase(aUseCase)));
    if(err)HandleError(err,_L("MusStart "));    
    return (err)?EFalse :ETrue;        
    }
     
TBool CMusApplication::Stop()
    {
    TRAPD(err,iMultimediasharing->StopApplicationL());
    if(err)HandleError(err,_L("MusStop "));
    return (err)?EFalse :ETrue;    
    }

TInt CMusApplication::Availability()
    {
    TInt status=-1;
    TRAPD(err,status=iMultimediasharing->AvailabilityL());
    if(err)HandleError(err,_L("MusAvailability "));
    return status;    
    }
TInt CMusApplication::MonitorAvailability()
    {
    return KErrGeneral;        
    }

TBool CMusApplication::AvailabilityInfo()
    {                                   
    TBuf<100> buf(_L("Availability Info: ")); 
    TBool available = EFalse ;
    switch(Availability())
        {
            case MultimediaSharing::ESipRegistrationPending:
                 buf.Append(_L("SipRegistrationPending "));
                 break;

            case MultimediaSharing::ESipOptionsSent:
                 buf.Append(_L("SipOptionsSent "));
                 available = ETrue ;
                 break;

            case MultimediaSharing::ESipOptionsNotCapable:
                 buf.Append(_L("SipOptionsNotCapable "));
                 available = ETrue ;
                 break;

            case MultimediaSharing::ESipOptionsNotSent:
                 buf.Append(_L("SipOptionsNotSent "));
                 available = ETrue ;
                 break;

            case MultimediaSharing::EErrSipRegistration:
                 buf.Append(_L("ErrSipRegistration "));
                 break;

            case MultimediaSharing::EErrNoActiveCall:
                 buf.Append(_L("ErrNoActiveCall "));
                 break;

            case MultimediaSharing::EErrCallOnHold:
                 buf.Append(_L("ErrCallOnHold "));
                 break;

            case MultimediaSharing::EErrConferenceCall:
                 buf.Append(_L("ErrConferenceCall "));
                 break;

            case MultimediaSharing::EErrRoaming:
                 buf.Append(_L("ErrRoaming "));
                 break;

            case MultimediaSharing::EErrNetwork:
                 buf.Append(_L("ErrNetwork "));
                 break;

            default:
                 buf.Append(_L("Error Occured "));
                 break;

        }     
    CAknErrorNote *dlg = new CAknErrorNote(ETrue);    
    dlg->ExecuteLD(buf);     
    return available;
    }

void CMusApplication::MusUseCaseInfo(TBool aStart)
    {
    CAknErrorNote *dlg = new CAknErrorNote(ETrue);                                   
    TBuf<50> buf;  
    switch (iUseCase)
        {
            case MultimediaSharing::EMusLiveVideo:                    
            buf.Append(_L("MusLiveVideo "));
            break;
            case MultimediaSharing::EMusClipVideo:
            buf.Append(_L("MusClipVideo "));
            break;                     
            case MultimediaSharing::EMusStillImage:
            buf.Append(_L("MusStillImage "));
            break;
            case MultimediaSharing::EMusReceive:
            buf.Append(_L("MusReceive "));
            break;
            case MultimediaSharing::EMusContinue:
            buf.Append(_L("MusContinue "));
            break;
            default :
            buf.Append(_L("Unknown Usecase "));
            break;
        }          
    (aStart) ? buf.Append(_L("Started")) : buf.Append(_L("Stoped"));
    dlg->ExecuteLD(buf);      
    }

// End of File  

