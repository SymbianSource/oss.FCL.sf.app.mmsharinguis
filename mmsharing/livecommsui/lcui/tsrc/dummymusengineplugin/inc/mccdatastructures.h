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
* Description:  
*
*/




#ifndef MCCDATASTRUCTURES_H
#define MCCDATASTRUCTURES_H

#include <E32Base.h>

enum TDataId 
{
  EUninitialized,
  EInitComplete,
  EEstablish,
  EProceeding,
  EEstablished,
  ETerminate,
  ETerminated
};

class TDataMessage
{
public:
    TDataId iDataId;
    TInt iDataIntValue;
    TBuf<50> iDataBufValue;

};



#endif