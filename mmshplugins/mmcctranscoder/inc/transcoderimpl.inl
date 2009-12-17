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


#ifndef MCCTRANSCODER_INL
#define MCCTRANSCODER_INL

// INCLUDE FILES
#include <e32std.h>
#include <vedmovie.h>


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipIndicesChanged
// -----------------------------------------------------------------------------
//   
inline void CMccTranscoderImpl::NotifyVideoClipIndicesChanged(
	  CVedMovie& /*aMovie*/, 
    TInt /*aOldIndex*/, 
    TInt /*aNewIndex*/ ) 
    {
    }
        
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipTimingsChanged
// -----------------------------------------------------------------------------
//                                           
inline void CMccTranscoderImpl::NotifyVideoClipTimingsChanged(
    CVedMovie& /*aMovie*/,
    TInt       /*aIndex*/ )
    {
        
    }
 
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipColorEffectChanged
// -----------------------------------------------------------------------------
//                                      
inline void CMccTranscoderImpl::NotifyVideoClipColorEffectChanged(
    CVedMovie& /*aMovie*/,
    TInt       /*aIndex*/ )
    {
        
    }
                                              
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipAudioSettingsChanged
// -----------------------------------------------------------------------------
// 
inline void CMccTranscoderImpl::NotifyVideoClipAudioSettingsChanged(
    CVedMovie& /*aMovie*/,
    TInt       /*aIndex*/)
    {
        
    }
                                                
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipGeneratorSettingsChanged
// -----------------------------------------------------------------------------
//                                        
inline void CMccTranscoderImpl::NotifyVideoClipGeneratorSettingsChanged(
    CVedMovie& /*aMovie*/,
    TInt       /*aIndex*/)
    {
        
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipDescriptiveNameChanged
// -----------------------------------------------------------------------------
//                                             
inline void CMccTranscoderImpl::NotifyVideoClipDescriptiveNameChanged(
    CVedMovie& /*aMovie*/,
    TInt       /*aIndex*/)
    {
        
    }
  
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyStartTransitionEffectChanged
// -----------------------------------------------------------------------------
// 
inline void CMccTranscoderImpl::NotifyStartTransitionEffectChanged(
    CVedMovie& /*aMovie*/ )
    {
        
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyMiddleTransitionEffectChanged
// -----------------------------------------------------------------------------
// 
inline void CMccTranscoderImpl::NotifyMiddleTransitionEffectChanged(
    CVedMovie& /*aMovie*/, 
    TInt       /*aIndex*/ )
    {
        
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyEndTransitionEffectChanged
// -----------------------------------------------------------------------------
//                                          
inline void CMccTranscoderImpl::NotifyEndTransitionEffectChanged(
    CVedMovie& /*aMovie*/ )
    {
        
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyAudioClipAdded
// -----------------------------------------------------------------------------
// 
inline void CMccTranscoderImpl::NotifyAudioClipAdded( 
    CVedMovie& /*aMovie*/, TInt /*aIndex*/ )
    {
        
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyAudioClipAddingFailed
// -----------------------------------------------------------------------------
// 
inline void CMccTranscoderImpl::NotifyAudioClipAddingFailed(
    CVedMovie& /*aMovie*/, TInt /*aError*/ )
    {
        
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyAudioClipRemoved
// -----------------------------------------------------------------------------
//
inline void CMccTranscoderImpl::NotifyAudioClipRemoved(
    CVedMovie& /*aMovie*/, TInt /*aIndex*/ )
    {
        
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyAudioClipIndicesChanged
// -----------------------------------------------------------------------------
//
inline void CMccTranscoderImpl::NotifyAudioClipIndicesChanged(
    CVedMovie& /*aMovie*/, 
    TInt       /*aOldIndex*/, 
    TInt       /*aNewIndex*/)
    {
        
    }
  
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyAudioClipTimingsChanged
// -----------------------------------------------------------------------------
//                                                        
inline void CMccTranscoderImpl::NotifyAudioClipTimingsChanged(
    CVedMovie& /*aMovie*/, TInt /*aIndex*/)
    {
        
    }
     
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyMovieReseted
// -----------------------------------------------------------------------------
//  
inline void CMccTranscoderImpl::NotifyMovieReseted( CVedMovie& /*aMovie*/ )
    {
        
    }          

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyAudioClipDynamicLevelMarkInserted
// -----------------------------------------------------------------------------
//         
inline void CMccTranscoderImpl::NotifyAudioClipDynamicLevelMarkInserted(CVedMovie& /*aMovie*/, 
                                                    TInt /*aClipIndex*/, 
                                                    TInt /*aMarkIndex*/)
    {
        
    }
 
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyAudioClipDynamicLevelMarkRemoved
// -----------------------------------------------------------------------------
//   
inline void CMccTranscoderImpl::NotifyAudioClipDynamicLevelMarkRemoved(CVedMovie& /*aMovie*/, 
                                                   TInt /*aClipIndex*/, 
                                                   TInt /*aMarkIndex*/)
    {
        
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipDynamicLevelMarkInserted
// -----------------------------------------------------------------------------
// 
inline void CMccTranscoderImpl::NotifyVideoClipDynamicLevelMarkInserted(CVedMovie& /*aMovie*/, 
                                                    TInt /*aClipIndex*/, 
                                                    TInt /*aMarkIndex*/)
    {
        
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipDynamicLevelMarkRemoved
// -----------------------------------------------------------------------------
//   
inline void CMccTranscoderImpl::NotifyVideoClipDynamicLevelMarkRemoved(CVedMovie& /*aMovie*/, 
                                                   TInt /*aClipIndex*/, 
                                                   TInt /*aMarkIndex*/)
    {
        
    }
    
#endif      // MCCTRANSCODER_INL   
            
// End of File
