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
* Description:  Sent capability query (OPTIONS)
*
*/


#ifndef CMUSAVASIPHEADERUTIL_H
#define CMUSAVASIPHEADERUTIL_H


#include <e32base.h>    
#include <stringpool.h>

class CSIPHeaderBase;
class CSIPRequestElements;
class CSIPConnection;

/**
 * Utility class to add some of the sip headers.
 * @precondition SIPStrings::OpenL() and
 *               SdpCodecStringPool::OpenL() should be called.
 *               Else most of the function will leave.
 * @lib AlwaysOnlinePlugin.dll
 */
class CMusAvaSipheaderUtil : public CBase
    {

public: 

    /**
     * Utility class to add some of the sip headers.
     * @precondition SIPStrings::OpenL() and
     *               SdpCodecStringPool::OpenL() should be called.
     *               Else most of the function will leave.
     * @lib AlwaysOnlinePlugin.dll
     */

    static void AddAcceptContactHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders,
                            const RStringF& aFeatureTag );

    /**
     * Utility class to add some of the sip headers.
     * @precondition SIPStrings::OpenL() and
     *               SdpCodecStringPool::OpenL() should be called.
     *               Else most of the function will leave.
     * @lib AlwaysOnlinePlugin.dll
     */

    static void AddAcceptEncodingHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders );

    /**
     * Utility class to add some of the sip headers.
     * @precondition SIPStrings::OpenL() and
     *               SdpCodecStringPool::OpenL() should be called.
     *               Else most of the function will leave.
     * @lib AlwaysOnlinePlugin.dll
     */

    static void AddAcceptLanguageHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders );

    /**
     * Utility class to add some of the sip headers.
     * @precondition SIPStrings::OpenL() and
     *               SdpCodecStringPool::OpenL() should be called.
     *               Else most of the function will leave.
     * @lib AlwaysOnlinePlugin.dll
     */

    static void AddSdpL( CSIPRequestElements* aRequest,HBufC8* aContent );

    /**
     * Utility class to add some of the sip headers.
     * @precondition SIPStrings::OpenL() and
     *               SdpCodecStringPool::OpenL() should be called.
     *               Else most of the function will leave.
     * @lib AlwaysOnlinePlugin.dll
     */

    static void AddPreferredIdentityHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders,
                            const TDesC8& aVal );

    /**
     * Utility class to add some of the sip headers.
     * @precondition SIPStrings::OpenL() and
     *               SdpCodecStringPool::OpenL() should be called.
     *               Else most of the function will leave.
     * @lib AlwaysOnlinePlugin.dll
     */

    static void LocalHostL(const CSIPConnection& aConn, TPtrC8& aLocalHost);

    /**
     * Utility class to add some of the sip headers.
     * @precondition SIPStrings::OpenL() and
     *               SdpCodecStringPool::OpenL() should be called.
     *               Else most of the function will leave.
     * @lib AlwaysOnlinePlugin.dll
     */

    static void AddAcceptSdpHeaderL(RPointerArray<CSIPHeaderBase>& aRequestHeaders );
    
    };


#endif // CMUSAVASIPHEADERUTIL_H
