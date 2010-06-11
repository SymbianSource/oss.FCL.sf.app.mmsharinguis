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

#ifndef __E32PROPERTY_H__
#define __E32PROPERTY_H__


#include <e32base.h>

class RProperty
    {
    
public:

    inline RProperty() { iRequestStatus = NULL; }

    /**
    The largest supported property value, in bytes, for byte-array (binary)
    types and text types.
    */
    enum { KMaxPropertySize = 512 };
    /**
    The largest supported property value, in bytes, for large byte-array (binary)
    types and large text types.
    */
    enum { KMaxLargePropertySize = 65535 };


    /**
    Property type attribute.
    */
    enum TType
        {
        /**
        Integral property type.
        */
        EInt,


        /**
        Byte-array (binary data) property type.
        This type provides real-time guarantees but is limited to a maximum size
        of 512 bytes.

        @see KMaxPropertySize
        */
        EByteArray,


        /**
        Text property type.
        This is just a programmer friendly view of a byte-array property, and
        is implemented in the same way as EByteArray.
        */
        EText = EByteArray,


        /**
        Large byte-array (binary data) property type.
        This type provides no real-time guarantees but supports properties
        of up to 65536 bytes.

        @see KMaxLargePropertySize
        */
        ELargeByteArray,


        /**
        Large text property type.
        This is just a programmer friendly view of a byte-array property, and
        is implemented in the same way as EByteArray.
        */
        ELargeText = ELargeByteArray,


        /**
        Upper limit for TType values.
        It is the maximal legal TType value plus 1.
        */
        ETypeLimit,


        /**
        Bitmask for TType values coded within TInt attributes.
        */
        ETypeMask = 0xff
        };


public:    // Maintain the same order as in original header!

    static TInt Define(TUid aCategory, TUint aKey, TInt aAttr ,TInt aPreallocate=0 );
    //IMPORT_C static TInt Define(TUid aCategory, TUint aKey, TInt aAttr, const TSecurityPolicy& aReadPolicy, const TSecurityPolicy& aWritePolicy, TInt aPreallocated=0);
    //IMPORT_C static TInt Define(TUint aKey, TInt aAttr, const TSecurityPolicy& aReadPolicy, const TSecurityPolicy& aWritePolicy, TInt aPreallocated=0);
    static TInt Delete(TUid aCategory, TUint aKey);
    //IMPORT_C static TInt Delete(TUint aKey);
    static TInt Get(TUid aCategory, TUint aKey, TInt& aValue);
    static TInt Get(TUid aCategory, TUint aKey, TDes8& aValue);
    static TInt Get(TUid aCategory, TUint aKey, TDes16& aValue);
    static TInt Set(TUid aCategory, TUint aKey, TInt aValue);
    static TInt Set(TUid aCategory, TUint aKey, const TDesC8& aValue);
    static TInt Set(TUid aCategory, TUint aKey, const TDesC& aValue);
    TInt Get(TInt& aValue);

    TInt Attach( TUid aCategory, TUint aKey );//, TOwnerType aType = EOwnerProcess)
    void Subscribe(TRequestStatus& aRequest);
    void Cancel();
    
    //TInt Get(TInt& aValue);
	//TInt Get(TDes8& aValue);

	//TInt Get(TDes16& aValue);
    
    
	TInt Set(TInt aValue);
	//TInt Set(const TDesC8& aValue);

	//TInt Set(const TDesC16& aValue);


public: // Stub functions which is not in original e32property	
    void Close(); // in original header this is inherited from RHandleBase  
    static TInt Get(TUint& aKey,TInt& aVal);

public: // Stub data's   
    enum TPropertyCalledFunctions
        {
        ENone,
        ECancel,
        EClose,
        EDefine
        };
    
    TRequestStatus* iRequestStatus;
    TInt iPValue;
    
    };

class PropertyHelper
    {
    public :
    static void SetErrorCode(TInt aVal); 
    static RProperty::TPropertyCalledFunctions GetCalledFunction();   
    static void Close(); 
    
    };


#endif
