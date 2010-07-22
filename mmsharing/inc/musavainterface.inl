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
* Description:  ECOM interface default implementation. 
*
*/


//USER
#include "musavaobserver.h"
#include "musuid.hrh"
#include "muscleanupresetanddestroy.h"

// ---------------------------------------------------------------------------------
// C++ Destructor.
//
// ---------------------------------------------------------------------------------
//
inline CMusAvaInterface::~CMusAvaInterface()
	{
	REComSession::DestroyedImplementation( iDtor_ID_Key );
	}


// ---------------------------------------------------------------------------------
// CMusAvaInterface::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------------
//
inline CMusAvaInterface* CMusAvaInterface::NewL()

	{
	// Find implementation for our interface.
    // - KClientInterfaceUid is the UID of our custom ECOM
    //   interface.
    // - This call will leave, if the plugin architecture cannot find
    //   implementation.
    // - The returned pointer points to one of our interface implementation
    //   instances.
    RImplInfoPtrArray pluginArray;
    MusCleanupResetAndDestroy<RImplInfoPtrArray>::PushL( pluginArray );
    
    TUid interfaceUid = { KMusAvaPluginInterfaceUid };

    REComSession::ListImplementationsL( interfaceUid, pluginArray );
    
    if( pluginArray.Count() > 0 )
        {
        CImplementationInformation* info = pluginArray[ 0 ];
	    TAny* interface = REComSession::CreateImplementationL(
		        info->ImplementationUid(), 
		        _FOFF( CMusAvaInterface , iDtor_ID_Key ) );
    
	    CleanupStack::PopAndDestroy();//pluginArray
        //pluginArray.ResetAndDestroy();
        return reinterpret_cast <CMusAvaInterface *> ( interface );    
        }
        
    CleanupStack::PopAndDestroy();//pluginArray
    User::Leave( KErrNotFound );
    // to avoid compiler warning    
    return NULL;	
	}

