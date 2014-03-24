/***************************************************************************
 
 Copyright 2006-2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
#import <Cocoa/Cocoa.h>
#ifdef __cplusplus
}
#endif
#import "SBPulsePlugin.h"

@class SBPulsePlugin;
@class SBSequencePlugin;
@class SBWaveformParams;
@class SBNewTRPlugin;
@class SBParamsObject;

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBWaveform
 *   @brief       Stores waveform-related model parameters
 *   @details     Information stored here includes the pulse and sequence plugin arrays, number of gradient axes, and the #SBWaveformParams object at the output of the sequencing pipeline.  Evaluation of the sequencing pipeline occurs in this class.  Each #SBParamsObject should be associated with exactly one #SBWaveform.  Serializations of #SBWaveform contain the following keys:
 *   - "PulsePlugins"    : An ordered array of SBPulsePlugins
 *   - "SequencePlugins" : An ordered array of SBSequencePlugins
 *   - "numGradAxes"     : The number of gradient axes available to the system
 *   - "numOtherAxes"    : The number of other (non-gradient, non-RF) axes available to the system
 *
 *   Note that there is a 1:1 relationship between #SBParamsObject and #SBWaveform.  Thus, #initWithParams: should only be called by #SBParamsObject.  To create a new #SBWaveform, create a new #SBParamsObject.  To update the state of an existing #SBWaveform, consider #setStateEqualTo: or SBParamsObject#setFromPropertyList:.
 *   @author      HeartVista, Inc.
 *   @see         SBParamsObject
 */
@interface SBWaveform : NSObject {
@private
	SBParamsObject *params;
	int numGradAxes;
	int numOtherAxes;
	int totalSamples;
	BOOL forceUpdate;
	int numTr;
	NSMutableArray *pulsePlugins;
	NSMutableArray *sequencePlugins;
	SBWaveformParams *outputParamsObject;
	SBNewTRPlugin *globalTrObject;
}

/**
 *    @brief     Initializes a new instance associated with a #SBParamsObject
 *    @details   Because there is a 1:1 relationship between #SBParamsObject and #SBWaveform, this method should only be called by #SBParamsObject and subclasses.
 *    @param     _params An #SBParamsObject that is to be the parent of this #SBWaveform.
 *    @result    An initialized #SBWaveform object
 *    @note      Designated initializer.
 */
- (id)initWithParams:(SBParamsObject *)_params;

/**
 *    @brief     Resets internal state to default values
 *    @details   Removes all pulse and sequence plugins, resets attributeKeys to defaults.
 */
- (void)reset;

/**
 *    @brief     Update internal state from a property list serialization
 *    @details   This method is typically used during file reading as part of instantiating a file from a SPV file.  All state is replaced with this new waveform.  The property list must include all required keys (see class description for a list).
 */
- (void)setFromPropertyList:(NSDictionary *)plist;

/**
 *    @brief     Serializes the waveform attributes into a dictionary
 *    @details   See the #SBWaveform class information for more details on expected keys.  The returned dictionary is autoreleased.
 */
- (NSMutableDictionary *)propertyListSerialization;

/**
 *   @brief      Copy internal state from the supplied waveform object
 *   @details    This call may be used to perform a deep copy of one waveform object into another.  This does only the minimum required to ensure that the state variables are set.  Follow-up with #forceSequenceUpdate to finish the copy operation
 */
- (void)setStateEqualTo:(SBWaveform *)obj;

/**
 *   @brief     Print state variables to the console
 *   @details   Useful for debugging, prints the current values of attributeKeys and other pertinent information.
 */
- (void)printState;

/**
 *   @brief     Set flag indicating all sequence plugins need updating at the next sequence update
 *   @details   Defers computation until later in case other sequencing changes will be made in the interim.
 */
- (void)setNeedsUpdate;

/**
 *   @brief     Updates sequence if needed
 *   @details   Deferred computation is performed here.  When finished, the needsUpdate flag is reset.
 */
- (void)updateIfNeeded;

/**
 *   @brief     Updates sequence immediately regardless of needsUpdate status
 *   @details   When finished, the needsUpdate flag is reset.
 */
- (void)forceSequenceUpdate;

/**
 *   @brief     Updates only the pulse waveforms for all pulses regardless of need
 *   @details   Sequencing must still be updated separately.
 */
- (void)forcePulseUpdate;

- (int)numRfTxChannels; /**< @brief The number of (possibly complex-valued) RF transmitters available to the system */
- (int)numGradAxes; /**< @brief The number of gradient axes available to the system */
- (int)numOtherAxes; /**< @brief The number of non-gradient, non-RF waveforms associated with the system */
- (int)numAxes; /**< @brief The total number of gradient, RF and other waveforms associated with the system */

- (int)numTr; /**< @brief     Total number of TR intervals */
- (int)totalSamples; /**< @brief     Total number of samples for the waveform */
- (int)samplesForTrNum:(int)trNum; /**< @brief     Number of samples for a given TR interval */
- (int)startOfTrNum:(int)trNum; /**< @brief     Starting sample number for a given TR interval */

// wro: these should not be returned mutable; additional methods should be included for array manipulation
- (NSMutableArray *)pulsePluginArray; /**< @brief Returns an ordered array of all instantiated pulse plugins */
- (NSMutableArray *)sequencePluginArray; /**< @brief Returns an ordered array of all instantiated sequence plugins */

/**
 *   @brief     Constructs the sequencing pipeline based upon array rules
 *   @details   When called, the #sequencePluginArray object is processed and connections between inputs and outputs are formed based upon their internal settings.
 *   @note      It should not be necessary to explicitly call this method in plugin code.
 */
- (void)constructSequencePluginPipeline;

/**
 *   @brief     Provides access to the output of the sequencing pipeline
 *   @details   An #SBWaveformParams object that represents the current sequence waveforms.  Through this object, direct access is provided to the waveform data, metadata, and tags.
 */
- (SBWaveformParams *)outputParamsObject;

/**
 *   @brief     Provides access to the empty global TR object
 *   @details   Direct access to the #SBSequencePlugin object at the beginning of most sequencing pipelines.  In any case where an explicit input connection is not present in the sequencing pipeline, the output of this #SBNewTRPlugin object is implicitly connected.
 *   @note      Changing parameters of this object (including duration) may have an unexpected effect.  Use SBParamsObject#setTr: (for example) instead.
 */
- (SBNewTRPlugin *)globalTrObject;

/**
 *   @brief      Provides an array of all pulse and sequence plugins
 *   @details    This array includes the globalTrObject.
 *   @deprecated This is an internal function and may not be available in future versions.  Please use other means of accessing this information.
 */
- (NSArray *)pulseAndSequencePlugins;

@end
