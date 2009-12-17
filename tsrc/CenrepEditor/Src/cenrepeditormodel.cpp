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
    // create a new expense object
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

void CPhoneResources::ConstructL()
	{
    iCameraAvailability = -1;
    iKeypadAvailability = -1 ;
    iVideoPlayerAvailability = -1;
    iCameraInformation = -1;
    iKodiakPhoneCall = -1;
    iDummy = 1; // just a postive value , dirty idea but just works at this time 
	}

void CPhoneResources::ReadL()
	{
    TInt err = KErrNone;
            // 1. camera property
    err = RProperty::Get( NMusResourceApi::KCategoryUid, 
                             NMusResourceApi::KCameraAvailability, 
                              iCameraAvailability);
    if(err)HandleError(ETrue,err,_L("CameraAvailability"));
            // 2. keypad property
    err = RProperty::Get( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KKeypadAvailability, 
                               iKeypadAvailability );
    if(err)HandleError(ETrue , err,_L("KeypadAvailability"));
            // 3. video player property
    err = RProperty::Get( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KVideoPlayerAvailability, 
                               iVideoPlayerAvailability );
    if(err)HandleError( ETrue, err ,_L("VideoPlayerAvailability"));
            // 3. video player property
    err = RProperty::Get( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KCameraInformation, 
                               iCameraInformation );
    if(err)HandleError( ETrue, err ,_L("CameraInformation"));    
    err = RProperty::Get( KPSUidCtsyCallInformation, 
                               KCTsyKodiakPtt, 
                               iKodiakPhoneCall );
    if(err)
        {  
        iKodiakPhoneCall = EPSCTsyKodiakPttNotActive;        
        RProperty::Define(KPSUidCtsyCallInformation,KCTsyKodiakPtt,EPSCTsyKodiakPttNotActive);
        }   
	}


CPhoneResources::~CPhoneResources()
	{
	}

void CPhoneResources::SaveL()
    {
    //TInt err = KErrNone;

    User::LeaveIfError(SetValueL( NMusResourceApi::KCategoryUid, 
                             NMusResourceApi::KCameraAvailability, 
                              iCameraAvailability));
    //if(err)HandleError( EFalse, err ,_L("CameraAvailability"));
            // 2. keypad property
    User::LeaveIfError(SetValueL( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KKeypadAvailability, 
                               iKeypadAvailability ));
    //if(err)HandleError(EFalse, err,_L("KeypadAvailability"));
          // 3. video player property
    User::LeaveIfError(SetValueL( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KVideoPlayerAvailability, 
                               iVideoPlayerAvailability ));
    //if(err)HandleError(EFalse, err,_L("VideoPlayerAvailability"));
          // 3. video player property
    User::LeaveIfError(SetValueL( NMusResourceApi::KCategoryUid, 
                               NMusResourceApi::KCameraInformation, 
                                iCameraInformation ));
    //if(err)HandleError(EFalse, err,_L("CameraInformation"));
             // 4. Kodiak PS Key    
    User::LeaveIfError(SetValueL( KPSUidCtsyCallInformation, 
                               KCTsyKodiakPtt, 
                                iKodiakPhoneCall ));
    //if(err)HandleError( EFalse, err ,_L("KodiakPSKeyInformation"));                 
    }

TInt CPhoneResources::SetValueL(TUid aCategory, TUint aKey, TInt aValue)
    {
    if(aValue<0)return KErrNone; // dont care if the key is not read intially
    return RProperty::Set( aCategory, aKey, aValue );
    }


void CPhoneResources::HandleError(TBool aType ,TInt aError , TPtrC aKeyName )
    {
    CAknErrorNote *dlg = new CAknErrorNote(ETrue);
    TBuf<100> buf;
    if(aType)
        {
        buf.Append(_L("Unable to Read Key "));
        }
    else
        {
        buf.Append(_L("Unable to Save "));
        }
    buf.Append(aKeyName);
    buf.Append(_L(" Error "));
    buf.AppendNum(aError);
    dlg->ExecuteLD(buf); 
    }

COtherResources* COtherResources::NewLC()
    {
    // create a new expense object
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
    iActivation = -1 ;
    iOperatorVariant = -1 ;
    iAuditoryNotification = -1;
    iPopupNotification = -1 ;
    iPopupNotificationType = -1 ;
    iEdgeDtmSupport = -1 ;
    iSessionSetupMethod = -1 ;
    iAutoRecord = -1 ;
    iVideoLocation = -1 ;
    iSipProfileId = -1 ;
    iUiOrientation = -1 ;
    iCapabilityQuery = -1;  
    iProductModeVariation=-1;
    iEncodingDevice = -1;
    iPrivacyVariation = -1;
    iDummy = 1; // just a postive value      
    }

void COtherResources::ReadL()
	{     
    
    TRAPD(err,ReadCRKeyValuesL());
    if(err) HandleError (ETrue,err,_L("Read Error"));
    
	}
	
void COtherResources::ReadCRKeyValuesL()
{

	TInt err=KErrNone;
    CRepository* cr = NULL ;
    TRAP(err,cr = CRepository::NewL( MusSettingsKeys::KRepositoryUid ));
    cr->CleanupCancelTransactionPushL();
    if(err)
        {
        CAknErrorNote *dlg = new CAknErrorNote(ETrue);
        dlg->ExecuteLD(_L(\
                   "Unable to Construct CRepository for UID 0x1028238B"));        
        return ; // dont leave 
        }    
   
    ReadKeyValueL( cr, MusSettingsKeys::KActivation,iActivation ); 
	ReadKeyValueL( cr, MusSettingsKeys::KOperatorVariant,iOperatorVariant);
  	ReadKeyValueL( cr, MusSettingsKeys::KAuditoryNotification, iAuditoryNotification);
   	ReadKeyValueL( cr, MusSettingsKeys::KPopupNotification, iPopupNotification);
    ReadKeyValueL( cr, MusSettingsKeys::KPopupNotificationType, iPopupNotificationType);
    ReadKeyValueL( cr, MusSettingsKeys::KEdgeDtmSupport, iEdgeDtmSupport);
    ReadKeyValueL( cr, MusSettingsKeys::KForceInternetSignaling, iSessionSetupMethod);
   	ReadKeyValueL( cr, MusSettingsKeys::KAutoRecord, iAutoRecord);
	ReadKeyValueL( cr, MusSettingsKeys::KVideoLocation, iVideoLocation);
 	ReadKeyValueL( cr, MusSettingsKeys::KSipProfileId, iSipProfileId);
  	ReadKeyValueL( cr, MusSettingsKeys::KUiOrientation, iUiOrientation);    
 	ReadKeyValueL( cr, MusSettingsKeys::KCapabilityQuery, iCapabilityQuery);     	
 	ReadKeyValueL( cr, MusSettingsKeys::KProductModeVariation, iProductModeVariation);    
 	ReadKeyValueL( cr, MusSettingsKeys::KEncoderConfigurationInfo, iEncoderInfo );
 	ReadKeyValueL( cr, MusSettingsKeys::KEncodingDevice, iEncodingDevice );
 	ReadKeyValueL( cr, MusSettingsKeys::KPrivacyExchange, iPrivacyVariation ); 

 	iEncodingDeviceStr.Zero();
 	iEncodingDeviceStr.AppendNum(iEncodingDevice, EHex); 	    
 	
 	CleanupStack::PopAndDestroy(cr);
   	delete cr;

}


COtherResources::~COtherResources()
	{
	}

void COtherResources::SaveL()
    {
   
   	TRAPD(err,SaveCRKeyValuesL());
   	if(err) HandleError (ETrue,err,_L("Save Error"));
    
    }

void COtherResources::SaveCRKeyValuesL()
    {
	CRepository* cr = NULL ;
    TRAPD(err,cr = CRepository::NewL( MusSettingsKeys::KRepositoryUid ));        
    cr->CleanupCancelTransactionPushL();
    if(err)
        {
        CAknErrorNote *dlg = new CAknErrorNote(ETrue);
        dlg->ExecuteLD(_L(\
                   "Unable to Construct CRepository for UID 0x1028238B"));  
        User::Leave(err);        
        }      
    SetKeyValueL( cr,MusSettingsKeys::KActivation, iActivation);
 	SetKeyValueL( cr,MusSettingsKeys::KOperatorVariant, iOperatorVariant);
 	SetKeyValueL( cr,MusSettingsKeys::KAuditoryNotification,  iAuditoryNotification);
	SetKeyValueL( cr,MusSettingsKeys::KPopupNotification, iPopupNotification);
	SetKeyValueL( cr,MusSettingsKeys::KPopupNotificationType, iPopupNotificationType);
	SetKeyValueL( cr,MusSettingsKeys::KEdgeDtmSupport, iEdgeDtmSupport);
	SetKeyValueL( cr,MusSettingsKeys::KForceInternetSignaling, iSessionSetupMethod);
	SetKeyValueL( cr,MusSettingsKeys::KAutoRecord, iAutoRecord);
	SetKeyValueL( cr,MusSettingsKeys::KVideoLocation, iVideoLocation);
	SetKeyValueL( cr,MusSettingsKeys::KSipProfileId,  iSipProfileId);
	SetKeyValueL( cr,MusSettingsKeys::KUiOrientation, iUiOrientation);    
	SetKeyValueL( cr,MusSettingsKeys::KCapabilityQuery, iCapabilityQuery);    
    SetKeyValueL( cr,MusSettingsKeys::KProductModeVariation, iProductModeVariation);    
    SetKeyValueL( cr,MusSettingsKeys::KEncoderConfigurationInfo, iEncoderInfo );
    SetKeyValueL( cr,MusSettingsKeys::KPrivacyExchange, iPrivacyVariation );
    
    TLex lex( iEncodingDeviceStr );
    TUint uid;
    err = lex.Val(uid, EHex);    
    if ( (err == KErrNone) && lex.Eos() )
        {
        iEncodingDevice = uid;
        SetKeyValueL( cr, MusSettingsKeys::KEncodingDevice, iEncodingDevice );        
        }
           
    CleanupStack::PopAndDestroy(cr);
	delete cr;
    }

void COtherResources::SetKeyValueL(CRepository* repository, const TInt & aKey , TInt & aVal)
    {  
    if(aVal<0)  return; // dont care if key is not intially read
    TUint32 key(aKey);
    User::LeaveIfError(repository->StartTransaction(
                        CRepository::EConcurrentReadWriteTransaction));    
    User::LeaveIfError(repository->Set(key,aVal));
    User::LeaveIfError(repository->CommitTransaction(key));    
    }

void COtherResources::ReadKeyValueL(CRepository* repository, const TInt & aKey , TInt & aVal)
    {
    TUint32 key(aKey);
    User::LeaveIfError(repository->StartTransaction(
                        CRepository::EConcurrentReadWriteTransaction));
    User::LeaveIfError(repository->Get(key,aVal));
    User::LeaveIfError(repository->CommitTransaction(key));
    }

void COtherResources::SetKeyValueL(CRepository* repository, TInt aKey, const TDesC& aVal)
    {
    TUint32 key(aKey);
    User::LeaveIfError(repository->StartTransaction(
                        CRepository::EConcurrentReadWriteTransaction));    
    User::LeaveIfError(repository->Set(key,aVal));
    User::LeaveIfError(repository->CommitTransaction(key));
    }

void COtherResources::ReadKeyValueL(CRepository* repository, TInt aKey, TDes& aVal)
    {
    TUint32 key(aKey);
    User::LeaveIfError(repository->StartTransaction(
                        CRepository::EConcurrentReadWriteTransaction));
    User::LeaveIfError(repository->Get(key,aVal));
    User::LeaveIfError(repository->CommitTransaction(key));
    }

void COtherResources::HandleError(TBool aType ,
                                    TInt aError , TPtrC aKeyName )
    {
    CAknErrorNote *dlg = new CAknErrorNote(ETrue);
    TBuf<100> buf;
    if(aType)
        {
        buf.Append(_L("Unable to Read Key "));
        }
    else
        {
        buf.Append(_L("Unable to Save Key "));
        }
    buf.Append(aKeyName);
    buf.Append(_L(" Error "));
    buf.AppendNum(aError);
    dlg->ExecuteLD(buf); 
    }

CMusApplication* CMusApplication::NewLC()
    {
    // create a new expense object
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
    //iMultimediasharing=CMusManager::NewL();    
    //iMultimediasharing->ExamineAvailabilityL();
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
    /*TRAPD(err,iMultimediasharing->StartApplicationL(
                            MultimediaSharing::TMusUseCase(aUseCase)));
    if(err)HandleError(err,_L("MusStart "));    
    return (err)?EFalse :ETrue; */       
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
    TInt status = KErrGeneral;
    /*TRequestStatus requestStatus;
    TRAPD(err,iMultimediasharing->
            MonitorAvailabilityL(
                requestStatus,
                MultimediaSharing::TMusAvailabilityStatus(status))
                                );
    if(err)HandleError(err,_L("MusAvailability "));
    User::WaitForRequest(requestStatus);    
    */
    return status;        
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

