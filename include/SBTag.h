/***************************************************************************
 
 Copyright 2008-2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

#import <Cocoa/Cocoa.h>

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBTag 
 *   @brief       Abstract class used for adding meta-information to waveform segments.
 *   @details     Meta-information can include the start and end times of specific pulses (RF, Gradient) or time intervals (RF Receiver windows, etc).
 *   @author      HeartVista, Inc.
 *   
 *   
 *   @note        Typically, this class is not instantiated directly.  Use subclasses (SBRFTag, SBReadoutTag, SBPulseTag, ...) instead.
 *                Tag creation and modification should be done during calls to SBPulsePlugin#tagsForTrNum:of: or SBSequencePlugin#metadataForTrNum:ofOutput:.  Tags may also be cached (for example, during calculateOutput) for use during subsequent calls to SBPulsePlugin#tagsForTrNum:of: or SBSequencePlugin#metadataForTrNum:ofOutput:.
 *   @see         SBPulsePlugin#tagsForTrNum:of:
 *   @see         SBSequencePlugin#metadataForTrNum:ofOutput:
 *   @see         SBPropertyListPlugin#calculateOutput
 */
@interface SBTag : NSObject {
@private
  NSMutableDictionary *userInfo;
}

/**
 *    @brief     Creates and returns an autoreleased instance of the class.
 *    @details   The returned instance is initialized to default values.
 *    @result    An autoreleased instance of the receiving class.
 */
+ (id)tag;

/**
 *    @brief     Accessor function for a dictionary of user-defined data.
 *    @details   User-defined data should be stored using a unique key.  In most cases, this should include the name of the class that added the key.  Modification of this dictionary is only appropriate during SBPulsePlugin#tagsForTrNum:of: or SBSequencePlugin#metadataForTrNum:ofOutput: or earlier in the sequencing chain.
 *    @result    A modifiable userInfo dictionary for this tag.
 *    @see       SBPulsePlugin#tagsForTrNum:of:
 *    @see       SBSequencePlugin#metadataForTrNum:ofOutput:
 */
- (NSMutableDictionary *)userInfo;
@end
