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


#ifndef MCCTRANSCODERLOGS_H
#define MCCTRANSCODERLOGS_H

#ifdef _DEBUG

#include <utf.h>
#include <e32debug.h>

class TMccTranscoderLog;

_LIT( KAnyFormat, "Mcc/Transcoder:" );
_LIT( KMccTranscoderLogWarning, "Mcc: Text contains not allowed characters, log ignored" );

#define __TRANSCODER( a ) \
	{  _LIT( KStr, a ); TMccTranscoderLog::Print( KAnyFormat, KStr ); }
#define __TRANSCODER_INT1( a, b ) \
	{  _LIT( KStr, a ); TMccTranscoderLog::Print( KAnyFormat, KStr, b ); }

class TMccTranscoderLog
	{
	public:

    inline static void Print( const TDesC16& aName, const TDesC16& aStr ) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			TBuf<256> str;
			_LIT( KMcc, "%S %S" );
			str.Format( KMcc(), &aName, &aStr );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMccTranscoderLogWarning );							
			}	
		}	

	inline static void Print( 
	    const TDesC16& aName, 
	    const TDesC16& aStr, 
	    TInt           aValue ) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			TBuf<256> str;
			_LIT( KMcc, "%S %S %d" );
			str.Format( KMcc(), &aName, &aStr, aValue );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMccTranscoderLogWarning );							
			}	
		}

    }; 
    
#else

#define __TRANSCODER( a )
#define __TRANSCODER_INT1( a, b )

#endif

#endif // End of define MCCTRANSCODERLOGS_H

// end of file 