/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef LCEXPORT_H
#define LCEXPORT_H

#include <qglobal.h>

#if !defined(UNIT_TESTING)
#if !defined(LIVECOMMS_EXPORT)
#if defined(LIVECOMMSUI_LIB)
#define LIVECOMMS_EXPORT Q_DECL_EXPORT
#else
#define LIVECOMMS_EXPORT Q_DECL_IMPORT
#endif
#endif
#else
#define LIVECOMMS_EXPORT
#endif
#endif // LCEXPORT_H
