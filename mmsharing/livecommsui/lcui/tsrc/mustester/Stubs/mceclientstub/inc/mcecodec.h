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


#ifndef MCECODEC_H
#define MCECODEC_H

// INCLUDES
#include <e32base.h>
#include "mcedefs.h"

// FORWARD DECLARATIONS
class RReadStream;
class RWriteStream;
class CMceComCodec;

class CMceMediaStream;
class TMceEvent;
class TMceMediaId;
class TMceFactory;

// DATA TYPES
typedef TUint TMceCodecType;


// CLASS DECLARATION

/**
* Base class for codecs.
*
* It defines setters and getters for generic codec attributes.
*
* In some systems certain device resources cannot be shared. For example
* speaker of the device might not be able to play simultaneously several
* differently encoded audio streams. Or only one client at the time
* could use the camera of the device. Codec states can be used to overcome
* situations where single codec would occupy some resource for excessive
* period of time or permanently. 
*
* Codec can be in one of the three states:
*
* When codec is in state EEnabled, resources, associated with the codec,
* are reserved for the codec and in some cases cannot be used by other
* software components. Precondition for reserving resources for the codec
* is that associated device resource (e.g. speaker sink of downlink or
* camera source of uplink) is also enabled.
*
* When codec is in state EStandby, resources are not reserved for the codec
* and therefore can be used by other software components. If the stream 
* (uplink or downlink), into which this codec is attached, encounters data
* encoded with the codec, codec in state EStandby is tried to be enabled.
* If enabling succeeds, codec state changes to EEnabled and associated
* resources are reserved.
*
* When codec is in state EDisabled, resources are not reserved for the codec
* and are not even tried to be reserved.    
*
* @lib mceclient.lib
*/
class CMceCodec : public CBase
    {
    
    public:  // Codec states
    
        enum TState
            {
            EDisabled, 
            EStandby,
            EEnabled
            };
    
	public:  // Constructors and destructor
	        
	    /**
	    * Destructor.
	    */
	    virtual ~CMceCodec();

	public: // New functions
	         
	    /**
	    * Sets bitrate used with codec for encoding.
	    * @param aBitrate bitrate value for encoding
	    * @return KErrNotSupported if codec doesn't support bitrate
	    *         value issued
	    */
	    virtual TInt SetBitrate( TUint aBitrate ) = 0;
	    
	    /**
	    * Sets bitrates allowed with codec.
	    * @param aBitrates allowed bitrate values
	    * @return KErrNotSupported if codec doesn't support bitrate
	    *         values issued
	    */
	    virtual TInt SetAllowedBitrates( TUint aBitrates ) = 0;
	    
	    /**
	    * Sets the codec specific mode.
	    * @param aCodecMode mode of the codec 
	    * @return KErrNotSupported if codec doesnt' support codec mode
	    *         value issued
	    */
	    virtual TInt SetCodecMode( TUint aCodecMode ) = 0;
	    
	    /**
	    * Sets the payload type.
	    * @param aPayloadType type identifier of the payload
	    * @return KErrNotSupported if codec doesn't support payload type issued
	    */
	    virtual TInt SetPayloadType( TUint8 aPayloadType ) = 0;
	    
	
	public: // New functions          

        /**
        * Sets the state of the codec.
        * @param aState desired codec state
        */
        IMPORT_C void SetStateL( CMceCodec::TState aState );

        /**
        * Sets the timer period in seconds from starting of inactivity
        * to automatically changing to state CMceCodec::EStandby .
        * @param aTimerValue timer period in seconds
        */
        IMPORT_C void SetStandByTimerL( TUint32 aTimerValue );

        /**
	    * Sets MMF priority value for the codec.
	    * @param aPriority priority of the codec
	    */
        IMPORT_C void SetMMFPriorityL( TInt aPriority );
        
        /**
	    * Sets MMF priority preference value for the codec.
	    * @param aPriorityPreference priority preference of the codec
	    */
        IMPORT_C void SetMMFPriorityPreferenceL( TInt aPriorityPreference );
        
        /**
        * Set keep alive packets send timer value.
        * @param aTimerValue timer value in ms for keep alive packets
        * @return One of the standard system-wide error codes.
        */
        IMPORT_C void SetKeepAliveTimerL( TUint8 aTimerValue );

        /**
        * Set keep alive packets payload type.
        * @param aKeepAlivePT payload type for keep alive packets
        * @return One of the standard system-wide error codes.
        */
        IMPORT_C void SetKeepAlivePayloadTypeL( TUint8 aKeepAlivePT );

        /**
        * Set keep alive packets payload data.
        * @param aData payload data for keep alive packets
        * @return One of the standard system-wide error codes.
        */
        IMPORT_C void SetKeepAliveDataL( const TDesC8& aData );        
        

    public: // Getters

        /**
	    * Gets the state of the codec
	    * @return state of the codec
	    */	
	    IMPORT_C CMceCodec::TState State() const;
	     
        /**
	    * Media id of the codec.
	    * @return media id
	    */
		//IMPORT_C TMceMediaId Id() const;
		
		/**
	    * Type of the codec
	    * @return codec type
	    */
		IMPORT_C TMceCodecType Type() const;
    	    
	    /**
	    * Gets codec fourCC.
	    * @return fourCC value
	    */        
	    IMPORT_C TUint32 FourCC() const;

	    /**
	    * Gets current bitrate.
	    * @return bitrate value
	    */
	    IMPORT_C TUint Bitrate() const;

	    /**
	    * Gets allowed bitrate values.
	    * @return bitrate values
	    */
	    IMPORT_C TUint AllowedBitrates() const;
	    
	    /**
	    * Gets current frame size of the codec.
	    * @return size of single frame used by the codec
	    */        
	    IMPORT_C TUint FrameSize() const;

	    /**
	    * Gets the codec mode.
	    * @return codec mode
	    */
	    IMPORT_C TUint CodecMode() const;
	    	    
	    /**
	    * Gets the payload type identifier.
	    * @return payload type used
	    */
	    IMPORT_C TUint8 PayloadType() const;
	    
	    /**
	    * Gets the sdp name.
	    * @return sdp name of the codec
	    */
	    IMPORT_C const TDesC8& SdpName() const;

		/**
	    * Gets MMF priority
	    * @return MMF priority
	    */
		IMPORT_C TInt MMFPriority() const;
		
		/**
	    * Gets MMF priority preference
	    * @return current MMFPriorityPreference
	    */
		IMPORT_C TInt MMFPriorityPreference() const;
		
		/**
        * Gets keep alive packets send timer value.
        * @return keep alive packets send timer value (ms).
        */
        IMPORT_C TUint8 KeepAliveTimer() const;

        /**
        * Gets keep alive packets payload type.
        * @return keep alive packets payload type.
        */
        IMPORT_C TUint8 KeepAlivePayloadType() const;

        /**
        * Gets keep alive packets payload data.
        * @return keep alive packets payload data.
        */
        IMPORT_C const TDesC8& KeepAliveData() const;     
        

	public: // Preference manipulation    
	    
	    /**
	    * Gets the preference value.
	    * @return preference value of the codec
	    */
	    IMPORT_C TInt Preference() const; 
	    
	    /**
	    * Sets the preference value used in media negotiation.
	    * Codec with preference value 0 is considered as the most preferenced
	    * codec. Preference of codecs with the same preference value is
	    * determined by order they were added to the stream. 
	    * @param aPreference preference value of the codec
	    */
	    IMPORT_C void SetPreferenceL( TInt aPeference );
	    	    
	
	public: // Internal
	 
	    /**
	    * Initializes the codec.
	    * @param aParent the parent
	    */
	    virtual void InitializeL( CMceMediaStream& aParent );           
	    
	  
	                
	protected: // New functions
	    
	    /**
	    * Sets the sdp name.
	    * @param aSdpName sdp name for the codec
	    */
	    virtual void SetSdpNameL( const TDesC8& aSdpName ) = 0;

	    /**
	    * C++ default constructor.
	    */
	    CMceCodec();
		
	protected: // Data
		
		/**
		* Codec type.
		*/
		TMceCodecType iType;

	protected: // NOT owned data
			
	    /**
	    * Parent stream, not owned.
	    */
		CMceMediaStream* iStream;

    private: // Reserved for future use
    
        TAny* iReserved;       

    public: // stub data
    
        /**
         * type of session
         */
        //TMceMediaId iID;
    	
    	/**
         * Fmtp attribute
         */
        HBufC8* iFmtpAttr;

    	/**
        * Sdp name used in SDP messages
        */
    	TBuf8<KMceMaxSdpNameLength> iSdpName;		

    	/**
        * Payload type
        */
    	TUint8 iPayloadType;

        /**
         * Is enabled
         */
        TBool iIsEnabled;
        
       	/**
        * Stream's bit rate
        */
    	TUint iBitrate;

    	/**
        * Stream's allowed bit rates
        */
    	TUint32 iAllowedBitrates;

    	/**
        * Codec mode ( e.g. PCMU, PCMA, Bandwidth efficient, octet aligned )
        */
    	TUint32 iCodecMode;		
    	
    	/**
    	* Codec FourCC
    	*/
    	TUint32 iFourCC;

    	/**
        * Codecs frame size
        */
    	TUint iFrameSize;
    	
    	/**
        * 
        */
        TInt iMMFPriority;
        
        /**
        * 
        */
        TInt iMMFPriorityPreference;
        
    	/**
        * 
        */
        TUint8 iKeepAliveTimer;
        
    	/**
        * Codecs frame size
        */
    	TUint8 iKeepAlivePayloadType;

        /**
        * 
        */
        TBuf8<10> iKeepAliveData;
        
        TInt iPreference;
	
    };


#endif

// End of File
