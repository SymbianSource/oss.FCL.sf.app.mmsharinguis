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
* Description:  Mus level indicator Event Observer Interface
*
*/



#ifndef MUSUILEVELINDICATOROBSERVER_H
#define MUSUILEVELINDICATOROBSERVER_H


class MMusUiLevelIndicatorObserver 
	{
	public:

    	virtual void SetLevelIndicatorVisibility(TBool aVisible) = 0;
    	
    	virtual void IndicatorLevelChanged( TInt aNewLevel ) = 0;
    	
	};

#endif // MUSUILEVELINDICATOROBSERVER_H
            
// end of file
