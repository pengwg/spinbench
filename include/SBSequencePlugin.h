/***************************************************************************
 
 Copyright 2007-2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

#import <Cocoa/Cocoa.h>
#import "SBPropertyListPlugin.h"

@class SBWaveformParams;
@class SBWaveformSegment;
@class SBKSpaceData;
@class SBReadoutTag;
@class SBSegmentMetadata;

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBSequencePlugin 
 *   @brief       Abstract superclass for all Sequence Plugins
 *   @details     All sequence plugins must descend from SBSequencePlugin.  In particular, #calculateOutput, #waveformSegmentForTrNum:ofOutput:, and #metadataForTrNum:ofOutput: should be reimplemented.
 *   @note        Typically, this class is not instantiated directly.  Use subclasses (SBRepeatPlugin, SBDrawPulsesPlugin, ...) instead.
 *   @author      HeartVista, Inc.
 *   @see         SBPropertyListPlugin
 */

@interface SBSequencePlugin : SBPropertyListPlugin {

	NSMutableArray *outputParamsArray;
	int numInputs;

@private
	NSMutableArray *inputParamsArray;  //in subclasses, don't use this array directly; instead, call [self inputParamsAtIndex:#];
}

/**
 *    @brief     Number of connected inputs
 *    @result    The number of inputs connected to this plugin.
 */
- (int)numInputs;

/**
 *    @brief     Number of available outputs
 *    @result    The number of outputs available to other plugins
 */
- (int)numOutputs;

/**
 *    @brief     Array of output parameters
 *    @details   This is an array of #SBWaveformParams objects, of length numOutputs.  Subclasses should update outputParamsArray in #calculateOutput so that the outputParamsArray accurately reflects the parameters of the waveforms that are available on each output.
 *    @result    An immutable NSArray of #SBWaveformParams objects
 */
- (NSArray *)outputParamsArray;
/**
 *    @brief     Accesses a #SBWaveformParams object at the given input index. 
 *    @details   This returns a #SBWaveformParams object that specifies parameters of the waveform at the specified input index.
 *    @param     index The index of the desired input
 *    @result    The #SBWaveformParams object at the specified input index
 */
- (SBWaveformParams *)inputParamsAtIndex:(int)index;

/**
 *    @brief     Calculate updated outputs
 *    @details   This function gets called whenever one of this class's attributeKeys is changed.  Any Sequence Plugins that exist earlier in the sequencing chain than this plugin can be expected to be updated at the time calculateOutput is called.  Implement this in subclasses to calculate the output array given inputs.  Call super at the end of the function in order to propagate any changes to other plugins.
 *    @note      Subclasses should not need to call this function directly
 */
- (void)calculateOutput;

/**
 *    @brief     Set equal to another object of the same class
 *    @details   Performs a deep copy of all attributeKeys to make the callee equal to the passed object.
 *    @param     obj The object whose state is to be copied
 */
- (void)setStateEqualTo:(SBSequencePlugin *)obj;

/**
 *    @brief     Notify immediately upon attribute change
 *    @details   This function gets called immediately whenever one of this class's attributeKeys is changed.  This may be overridden in subclasses to perform fast computations that can't wait in the computation chain until the next call to #calculateOutput.  Plugins should only very rarely need to implement this function.
 *    @note      Subclasses should not need to call this function directly 
 */
- (void)attributeDidChange;

/**
 *    @brief     Returns the plugin's waveform output for a given trNum
 *    @details   This method returns a #SBWaveformSegment object containing the waveform output of this sequencing block for a given tr number and output number.  Reimplement this function to appropriately process any inputs and return the specified output.
 *    @note      Subclasses should not need to call this function directly 
 *    @param     trNum The tr number for the requested output object
 *    @param     outputNum The number of the requested output.  This number can be expected to be in the range of [0,numOutputs-1].
 *    @result    A #SBWaveformSegment object containing all the waveforms for the requested tr number and output number.
 */
- (SBWaveformSegment *)waveformSegmentForTrNum:(int)trNum ofOutput:(int)outputNum;

/**
 *    @brief     Returns the plugin's metadata output for a given trNum
 *    @details   This method returns a #SBSegmentMetadata object containing the metadata output of this sequencing block for a given tr number and output number.  Reimplement this function to appropriately process any inputs and return the specified output.
 *    @note      Subclasses should not need to call this function directly 
 *    @param     trNum The tr number for the requested output object
 *    @param     outputNum The number of the requested output.  This number can be expected to be in the range of [0,numOutputs-1].
 *    @result    A #SBSegmentMetadata object containing all the metadata and tags for the requested tr number and output number.
 */
- (SBSegmentMetadata *)metadataForTrNum:(int)trNum ofOutput:(int)outputNum;

/**
 *    @brief     Returns auto-generated k-space data for a given interval
 *    @details   This method returns a #SBKSpaceData object containing automatically generated k-space data with a specified moment for a given #SBReadoutTag object, tr number, and output number.  This function creates the k-space data numerically based on the gradient output of #waveformSegmentForTrNum:ofOutput:.  In many cases, readout-specific k-space data is more useful; this can be found in #SBReadoutTag.
 *    @param     moment a non-negative integer specifying the moment of the desired k-space data.  Set to zero for conventional k-space data.
 *    @param     readout A #SBReadoutTag object specifying the start and end timings for the desired readout.  If nil, then the resulting k-space data spans the entire TR interval
 *    @param     trNum The tr number for the requested output object
 *    @param     outputNum The number of the requested output.  This number can be expected to be in the range of [0,numOutputs-1].
 *    @result    A #SBKSpaceData object containing all the k-space data for the requested tr number and output number.
 *    @see       SBReadoutTag#kSpace, SBReadoutTag#kSpaceDensity
 */
- (SBKSpaceData *)kSpaceWithMoment:(int)moment tag:(SBReadoutTag *)readout trNum:(int)trNum output:(int)outputNum;

/**
 *    @brief     Specifies any additional view index keys supplied by this plugin
 *    @details   Override this method to return an NSArray of NSStrings defining the names of view-index keys added by this plugin in #metadataForTrNum:ofOutput:.  By convention, Sequence Plugins define at least one output view index key named "index" which increments sequentially with the tr number at the output.
 *    @result    An NSArray object containing all view index keys defined by this plugin.
 */
- (NSArray *)viewIndexKeys;

/**
 *    @brief     Private; do not use.
 *    @note      This is a private core function and should not be used or overridden in subclasses. 
 */
- (NSArray *)setInputsFromAvailableInputs:(NSArray *)availableInputs; // private; do not reimplement

@end
