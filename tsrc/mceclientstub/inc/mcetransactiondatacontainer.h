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
* Description:  
*
*/


#ifndef TMceTRANSACTIONDATACONTAINER_H
#define TMceTRANSACTIONDATACONTAINER_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
//#include <mcedefs.h>

// CLASS DECLARATION

/**
* Container class to hold detailed data of a transaction.
*
* It is assumed, that user has only one instance of this class.
* The same instance is passed between MCE and user, but the data
* inside of this container is just updated for each event.
*
* The data inside the container is valid only during the execution
* of the callback function. After this the remaining data in container
* is deleted.
*
* @lib mceclient.lib
*/
class TMceTransactionDataContainer
    {
    public:
     
		/**
		* Default constructor
		*/
     	IMPORT_C TMceTransactionDataContainer();
     	
		/**
		* Returns status code
		* @returns status code of transaction, if any.
		*/
     	IMPORT_C TInt GetStatusCode();
     	
		/**
		* Returns reason phrase, ownership is transferred.
		* @returns reason phrase of transaction, if any.
		*/
     	IMPORT_C HBufC8* GetReasonPhrase();
     	
		/**
		* Returns headers, ownership is transferred.
		* @returns headers of transaction, if any.
		*/
     	IMPORT_C CDesC8Array* GetHeaders();
     	
		/**
		* Returns content type, ownership is transferred.
		* @returns content type of transaction, if any.
		*/
     	IMPORT_C HBufC8* GetContentType();
     	
		/**
		* Returns content headers, ownership is transferred.
		* @returns headers of content, if any.
		*/
     	IMPORT_C CDesC8Array* GetContentHeaders();
     	
		/**
		* Returns content, ownership is transferred.
		* @returns content of transaction, if any.
		*/
     	IMPORT_C HBufC8* GetContent();
     	
    public: // IMPORTED FOR STUBBING PURPOSES, NORMALLY NOT IMPORTED
     
     	IMPORT_C void SetStatusCode( TInt aStatusCode );
     	
     	IMPORT_C void SetReasonPhrase( HBufC8* aReasonPhrase );
     	
     	IMPORT_C void SetHeaders( CDesC8Array* aHeaders );
     	
     	IMPORT_C void SetContentType( HBufC8* aContentType );
     	
        IMPORT_C void SetContent( HBufC8* aContent );
     	
     	IMPORT_C void SetContentHeaders( CDesC8Array* aHeaders );
     	
     	IMPORT_C void Clear();
     	
    private:
    
		TUint32 iStatusCode;
		
		HBufC8* iReasonPhrase;
		
		CDesC8Array* iHeaders;
		
        HBufC8* iContentType;
        
		CDesC8Array* iContentHeaders;
		
		HBufC8* iContent;
		
	
    };

#endif
