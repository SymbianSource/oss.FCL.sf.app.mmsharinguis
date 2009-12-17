/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Mus Applications Event Observer Interface
*
*/



#ifndef MUSUIPROPERTYOBSERVER_H
#define MUSUIPROPERTYOBSERVER_H

#include <e32base.h>

class MMusUiPropertyObserver
	{
	
public:

	//virtual void ResourcePropertyChanged( const TUint aKey, const TInt aValue ) = 0;
	
	//virtual void SessionPropertyChanged( const TUint aKey, const TInt aValue ) = 0;

    virtual void PropertyChanged( const TUint aKey, const TInt aValue ) = 0;	
	
	virtual void HandlePropertyError( const TInt aReason ) = 0;
	
	};

#endif // MUSUIPROPERTYOBSERVER_H
            
// end of file
