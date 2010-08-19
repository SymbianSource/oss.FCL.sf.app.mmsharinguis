/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Observer for listening status of current cs call
*
*/



#ifndef MUSCALLMONITOROBSERVER_H
#define MUSCALLMONITOROBSERVER_H

#include <e32cmn.h>
#include <CPbkContactItem.h>

/**
 * Observer for the monitor
 *
 * @lib musaoplugin.dll
 */
class MMusCallMonitorObserver
    {

public: // type definitions

    virtual void CallConnectedL( const TDesC& aTelNumber ) = 0;
    
    virtual void CallHoldL( const TDesC& aTelNumber ) = 0;
    
    virtual void ConferenceCallL() = 0;
    
    virtual void NoActiveCallL() = 0;
    
    virtual void ConferenceCallLTerminated() = 0;
   
    /**
    *
    */
    virtual TBool ResolveAddressesL( CPbkContactItem& /*aPbkItem*/,
                            CDesCArrayFlat*& /*aContactsArray*/,
                            TPbkFieldId /*aTPbkFieldId*/  )
        {
        return EFalse;
        }
    };

#endif // MUSCALLMONITOROBSERVER_H
