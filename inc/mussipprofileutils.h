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
* Description:  This interface is used by
*                clients to get and availability setting for Video Charging
*
*/


#ifndef __MUSSIPPROFILEUTILS_H__
#define __MUSSIPPROFILEUTILS_H__

//  INCLUDES
#include "mussipprofileregistryobserver.h"
#include "mussipobserver.h"

#include <e32base.h>

class CSIPManagedProfile;
class MDesC8Array;
class CDesC8ArrayFlat;

/**
 *  Defines setting interface for the MusAvailability Plug-in
 *
 *  ECOM clients set and get setting are used by MusAvailability Plug-in.
 *
 *  @lib musavailabilityplugin.lib
 */

class MusSipProfileUtils : public MMusSipProfileRegistryObserver,
                           public MMusSipObserver
	{
public:
    	
    inline static void InsertContactHeaderL( TInt aSipProfileId );
    
    inline static void RemoveContactHeaderL( TInt aSipProfileId );

private:
    
    inline static void AddFeatureTagL( CSIPManagedProfile& aProfile );
    
    inline static void RemoveFeatureTagL( CSIPManagedProfile& aProfile );

    inline static CDesC8ArrayFlat* AddVsTagLC( const MDesC8Array& aArray );

    inline static CDesC8ArrayFlat* RemoveVsTagLC( const MDesC8Array& aArray, TInt aIndex );
    
    inline static TInt VsTagIndex( const MDesC8Array& aArray );
    
    inline MusSipProfileUtils();

    inline ~MusSipProfileUtils();

	};

#include "mussipprofileutils.inl"

#endif // __MUSSIPPROFILEUTILS_H__

