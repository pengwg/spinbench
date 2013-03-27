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

// this is needed to ensure that docs are generated for the enums
/** @file */

#import <Cocoa/Cocoa.h>
#import "SBPropertyListPlugin.h"
@class SBAnchorController;
@class SBPulseData;
@class SBSegmentMetadata;

/**
 *    @enum  SBPulseTypeEnum
 *    @brief Identifies which types of waveforms are provided by a given pulse
 *    @note  Typedef'd to SBPulseType
 */
typedef enum SBPulseTypeEnum {
  SBNone=0x00,      /**<  None of the waveform types are included */
  SBRF=0x01,        /**<  Only RF waveforms are included          */
  SBGrad=0x02,      /**<  Only Gradient waveforms are included    */
  SBGradAndRF=0x03, /**<  Gradient and RF waveforms are included  */
  SBOther=0x04,     /**<  Only 'Other' waveforms are included     */
  SBAll=0x7f        /**<  All of the waveform types are included  */
} SBPulseType;

/**
 *   @class       SBPulsePlugin 
 *   @brief       Abstract superclass for all Pulse Plugins
 *   @details     All pulse plugins must descend from SBPulsePlugin.  In particular, #calculateOutput should be reimplemented.  For RF pulses, consider also conforming to the #SBRFPulseSharedParameters protocol.  For pulses with a readout component, consider also conforming to the #SBReadoutSharedParameters protocol.  If the pulse data differs from TR to TR, then also conform to #SBTrDependentPulseDataProtocol.
 *   @note        Typically, this class is not instantiated directly.  Use subclasses (#SBSincRF, #SBCartesianReadout, ...) instead.
 *   @author      HeartVista, Inc.
 *   @see         SBPropertyListPlugin
 *   @see         SBRFPulseSharedParameters
 *   @see         SBReadoutSharedParameters
 *   @see         SBTrDependentPulseDataProtocol
 */

@interface SBPulsePlugin : SBPropertyListPlugin {
  
@protected
	SBPulseData *pulseData; /**<  @brief Convenience #SBPulseData object with the correct number of axes, allocated in the constructor */
	SBAnchorController *anchor;  /**<  Reference to the associated #SBAnchorController instance, to allow anchoring between pulses */

@private
	BOOL usesAnchoring;
  int axisControlBoxState;
	
	// to automatically setup pulse axes
	NSMutableArray *rfTxChannelControllers;
	NSBox *rfTransformBox;
	NSBox *gradTransformBox;
	
	// attributekeys and observedattributekeys are cached for speed
	NSArray *__attributeKeys;
	NSArray *__observedAttributeKeys;
}

/**
 *    @brief     Returns the TR-independent portions of the pulse data
 *    @details   This returns an #SBPulseData object contaning the TR-independent portions of the output waveforms.  The default implementation of this method simply returns the #pulseData member variable.  Reimplement this method for different functionality.
 *    @result    An #SBPulseData object containing TR-independent pulse data
 *    @note      Subclasses should not need to call this method directly
 */
- (SBPulseData *)trIndependentPulseData;

/**
 *    @brief     Returns the metadata associated with this pulse
 *    @details   This returns an #SBSegmentMetadata object contaning the metadata associated with the pulse for a given TR number. Subclasses should implement this method to add metadata to the pulse, but implementations must call the superclass method in order to preserve the original functionality.  If only tags are changed, consider reimplementing #tagsForTrNum:of: instead.  Users can expect that this function will never be called with out-of-bounds indices.
 *    @param     trNum the number of the requested TR
 *    @param     numTr the total number of TRs expected for this pulse
 *    @result    An #SBSegmentMetadata object containing the requested metadata
 *    @see       tagsForTrNum:of:
 */
- (SBSegmentMetadata *)metadataForTrNum:(int)trNum of:(int)numTr;

/**
 *    @brief     Returns the tags associated with this pulse
 *    @details   This returns an NSMutableArray of #SBTag objects associated with the pulse for a given TR number. Subclasses should implement this method to add tags to the pulse, but implementations must call the superclass method in order to preserve the original functionality.  Users can expect that this function will never be called with out-of-bounds indices.
 *    @param     trNum the number of the requested TR
 *    @param     numTr the total number of TRs expected for this pulse
 *    @result    An NSArray of #SBTag objects containing the requested tags
 *    @see       metadataForTrNum:of:
 */
- (NSMutableArray *)tagsForTrNum:(int)trNum of:(int)numTr;

/**
 *    @brief     Returns the view index keys associated with this pulse
 *    @details   This returns an NSArray of NSStrings indicating the view-index keys associated with this pulse.  Values for view-index keys should be set in #tagsForTrNum:of:, and for example may be used by readout pulses to specify an ordering for readout views.  The default implementation of this function returns an empty array.  Subclasses should override this method to associate additional view index tags with this pulse.
 *    @result    An NSArray of NSString objects containing the view index keys
 *    @see       tagsForTrNum:of:
 */
- (NSArray *)viewIndexKeys;

/**
 *    @brief     For gradient pulses, returns the x component of the zero-index gradient waveform.
 *    @details   This value can also be accessed through SBPulseData#gradTransform [0][0].
 *    @result    The x component of the gradient transform
 *    @see       SBPulseData#gradTransform
 */
- (float)xComponent;
/**
 *    @brief     For gradient pulses, returns the y component of the zero-index gradient waveform.  If not a gradient pulse, returns 0.
 *    @details   This value can also be accessed through SBPulseData#gradTransform [0][1].
 *    @result    The y component of the gradient transform
 *    @see       SBPulseData#gradTransform
 */
- (float)yComponent;
/**
 *    @brief     For gradient pulses, returns the z component of the zero-index gradient waveform.  If not a gradient pulse, returns 0.
 *    @details   This value can also be accessed through SBPulseData#gradTransform [0][2].
 *    @result    The z component of the gradient transform
 *    @see       SBPulseData#gradTransform
 */
- (float)zComponent;

/**
 *    @brief     For gradient pulses, sets the x component of the zero-index gradient waveform.  If not a gradient pulse, returns 0.
 *    @details   This value can also be accessed through SBPulseData#gradTransform [0][0].
 *    @param     val The x component of the gradient transform
 *    @see       SBPulseData#gradTransform
 */
- (void)setXComponent:(float)val;
/**
 *    @brief     For gradient pulses, sets the y component of the zero-index gradient waveform.
 *    @details   This value can also be accessed through SBPulseData#gradTransform [0][1].
 *    @param     val The y component of the gradient transform
 *    @see       SBPulseData#gradTransform
 */
- (void)setYComponent:(float)val;
/**
 *    @brief     For gradient pulses, sets the z component of the zero-index gradient waveform.
 *    @details   This value can also be accessed through SBPulseData#gradTransform [0][2].
 *    @param     val The y component of the gradient transform
 *    @see       SBPulseData#gradTransform
 */
- (void)setZComponent:(float)val;

/**
 *    @brief     Refresh the pulse status indicator.
 *    @details   For gui-based usage, this function updates the pulse status indicator in the Pulse Selector Controller.
 */
- (void)updatePulseStatus;

/**
 *    @brief     Sets the number of axes for each pulse type.
 *    @details   Reimplement this function to indicate the number of axes for #SBPulseTypeEnum values.  Implementations can expect that the function will only be called with pulse types SBRF, SBGrad, and SBOther.  Returning 0 for a specific pulse type indicates that the pulse does not implement that pulse type.
 *    @param     type A #SBPulseTypeEnum value
 *    @result    The number of axes for that pulse type
 */
- (int)numAxesForPulseType:(SBPulseType)type; 

/**
 *    @brief     The maximum gradient amplitudes for a given pulse interval
 *    @details   This function should be used instead of SBParamsObject#gradLimit to determine the maximum allowable gradient amplitude for your particular pulse along its pulse axes.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit and also observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     limits (output) The maximum gradient amplitudes for a given pulse interval, as a float array
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single gradient limit applicable throughout the pulse duration.
 *    @param     rotatable Flag indicating whether the computed values should be freely rotatable to any other orthogonal set
 */
- (void)getGradLimits:(float *)limits forInterval:(NSString *)interval freelyRotatable:(BOOL)rotatable;

/**
 *    @brief     The maximum gradient slew rates for a given pulse interval, provided that gradients do not exceed supplied values
 *    @details   This function should be used instead of SBParamsObject#slewRateLimit to determine the maximum allowable gradient slew rate for your particular pulse along its pulse axes.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  Gradient limits on each axis are explicitly supplied; Reilly curve slew rate limits are applied using these limits, which need not correspond to the gradient hardware limits.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed the supplied gradient limits or the returned slew rate limits and also observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     limits (output) The maximum gradient slew rates for a given pulse interval, as a float array
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single slew limit applicable throughout the pulse duration.
 *    @param     rotatable Flag indicating whether the computed values should be freely rotatable to any other orthogonal set
 *    @param     maxXGrad The maximum gradient amplitude to be allowed on the X gradient axis (G/cm)
 *    @param     maxYGrad The maximum gradient amplitude to be allowed on the Y gradient axis (G/cm)
 *    @param     maxZGrad The maximum gradient amplitude to be allowed on the Z gradient axis (G/cm)
 */
- (void)getSlewRateLimits:(float *)limits forInterval:(NSString *)interval freelyRotatable:(BOOL)rotatable xGradientConstraint:(float)maxXGrad yGradientConstraint:(float)maxYGrad zGradientConstraint:(float)maxZGrad;

/**
 *    @brief     The maximum gradient slew rates for a given pulse interval
 *    @details   This function should be used instead of SBParamsObject#slewRateLimit to determine the maximum allowable gradient slew rate for your particular pulse along its pulse axes.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit and also observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     limits (output) The maximum gradient slew rates for a given pulse interval, as a float array
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single slew limit applicable throughout the pulse duration.
 *    @param     rotatable Flag indicating whether the computed values should be freely rotatable to any other orthogonal set
 */
- (void)getSlewRateLimits:(float *)limits forInterval:(NSString *)interval freelyRotatable:(BOOL)rotatable;

/**
 *    @brief     The maximum gradient amplitude for a given pulse gradient axis
 *    @details   This function should be used instead of SBParamsObject#gradLimit to determine the maximum allowable gradient amplitude for your particular pulse along the specified axis.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit and also observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single gradient limit applicable throughout the pulse duration.
 *    @param     rotatable Flag indicating whether the computed values should be freely rotatable to any other orthogonal set
 *    @result    The maximum gradient amplitude for the given gradient axis, as a float
 */
- (float)gradLimitForInterval:(NSString *)interval gradAxis:(int)axis freelyRotatable:(BOOL)rotatable;

/**
 *    @brief     The maximum gradient slew rate for a given pulse gradient axis
 *    @details   This function should be used instead of SBParamsObject#slewRateLimit to determine the maximum allowable gradient slew rate for your particular pulse along the specified axis.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit and also observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single slew limit applicable throughout the pulse duration.
 *    @param     axis The pulse axis to be returned
 *    @param     rotatable Flag indicating whether the computed values should be freely rotatable to any other orthogonal set
 *    @result    The maximum gradient slew rate for the given gradient axis, as a float
 */
- (float)slewRateLimitForInterval:(NSString *)interval gradAxis:(int)axis freelyRotatable:(BOOL)rotatable;

/**
 *    @brief     The maximum gradient amplitude for a given pulse gradient axis
 *    @details   This function should be used instead of SBParamsObject#gradLimit to determine the maximum allowable gradient amplitude for your particular pulse along the specified axis.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit.  Returned values observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single gradient limit applicable throughout the pulse duration.
 *    @result    The maximum gradient amplitude for the given gradient axis, as a float
 */
- (float)gradLimitForInterval:(NSString *)interval gradAxis:(int)axis;

/**
 *    @brief     The maximum gradient slew rate for a given pulse gradient axis
 *    @details   This function should be used instead of SBParamsObject#slewRateLimit to determine the maximum allowable gradient slew rate for your particular pulse along the specified axis.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit.  Returned values observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single slew limit applicable throughout the pulse duration.
 *    @param     axis The pulse axis to be returned
 *    @result    The maximum gradient slew rate for the given gradient axis, as a float
 */
- (float)slewRateLimitForInterval:(NSString *)interval gradAxis:(int)axis;

/**
 *    @brief     The maximum spanning gradient slew rate for a given pulse interval, provided that gradients do not exceed supplied values
 *    @details   This function returns the largest absolute slew rate allowed in any single direction within the subspace defined by the span of the pulse axes.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  Gradient limits on each axis are explicitly supplied; Reilly curve slew rate limits are applied using these limits, which need not correspond to the gradient hardware limits.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed the supplied gradient limits or the returned slew rate limit and also observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single slew limit applicable throughout the pulse duration.
 *    @param     rotatable Flag indicating whether the computed values should be freely rotatable to any other orthogonal set
 *    @param     maxXGrad The maximum gradient amplitude to be allowed on the X gradient axis (G/cm)
 *    @param     maxYGrad The maximum gradient amplitude to be allowed on the Y gradient axis (G/cm)
 *    @param     maxZGrad The maximum gradient amplitude to be allowed on the Z gradient axis (G/cm)
 *    @result    The maximum gradient slew rate spanning the pulse axes, as a float
 *    @note      Applying this slew rate to all pulse axes simultaneously would result in an overrange
 */
- (float)slewRateLimitMagnitudeForInterval:(NSString *)interval freelyRotatable:(BOOL)rotatable xGradientConstraint:(float)maxXGrad yGradientConstraint:(float)maxYGrad zGradientConstraint:(float)maxZGrad;

/**
 *    @brief     The maximum spanning gradient slew rate for a given pulse interval
 *    @details   This function returns the largest absolute slew rate allowed in any single direction within the subspace defined by the span of the pulse axes.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit and also observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single slew limit applicable throughout the pulse duration.
 *    @param     rotatable Flag indicating whether the computed values should be freely rotatable to any other orthogonal set
 *    @result    The maximum gradient slew rate spanning the pulse axes, as a float
 *    @note      Applying this slew rate to all pulse axes simultaneously would result in an overrange
 */
- (float)slewRateLimitMagnitudeForInterval:(NSString *)interval freelyRotatable:(BOOL)rotatable;

/**
 *    @brief     The maximum spanning gradient magnitude for a given pulse interval
 *    @details   This function returns the largest absolute gradient magnitude allowed in any single direction within the subspace defined by the span of the pulse axes.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit and also observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single gradient limit applicable throughout the pulse duration.
 *    @param     rotatable Flag indicating whether the computed values should be freely rotatable to any other orthogonal set
 *    @result    The maximum gradient magnitude spanning the pulse axes, as a float
 *    @note      Applying this magnitude to all pulse axes simultaneously would result in an overrange
 */
- (float)gradLimitMagnitudeForInterval:(NSString *)interval freelyRotatable:(BOOL)rotatable;

/**
 *    @brief     The maximum spanning gradient slew rate for a given pulse interval
 *    @details   This function returns the largest absolute slew rate allowed in any single direction within the subspace defined by the span of the pulse axes.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit.  Returned values observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single slew limit applicable throughout the pulse duration.
 *    @result    The maximum gradient slew rate spanning the pulse axes, as a float
 *    @note      Applying this slew rate to all pulse axes simultaneously would result in an overrange
 */
- (float)slewRateLimitMagnitudeForInterval:(NSString *)interval;

/**
 *    @brief     The maximum spanning gradient slew rate for a given pulse interval
 *    @details   This function returns the largest absolute slew rate allowed in any single direction within the subspace defined by the span of the pulse axes.  These limits are based on the number of pulse axes requested and the potential overlap from other pulses.  It is up to the pulse plugin designer to ensure that the waveforms do not exceed this limit.  Returned values observe the document-wide SBParamsObject#freelyRotatable setting
 *    @param     interval The pulse interval key of the desired interval.  Set to nil to obtain a single slew limit applicable throughout the pulse duration.
 *    @result    The maximum gradient slew rate spanning the pulse axes, as a float
 *    @note      Applying this slew rate to all pulse axes simultaneously would result in an overrange
 */
- (float)gradLimitMagnitudeForInterval:(NSString *)interval;

/**
 *    @brief     Keys available for use as pulse anchor points
 *    @details   An NSArray of NSStrings specifying the pulse's anchor keys.  The default implementation returns an empty array; subclasses should reimplement this function if anchoring is desired.  If an empty array, then no anchoring points are exported and also no anchoring is used.  If not empty, anchor elements are automatically added to the pulse inspector.  For each anchor key described, corresponding getter and setter functions should be added ( e.g., for key="key", -(float)key and -(void)setKey:(float)val ).  It's usually only necessary to have one anchor key with actual storage; others can be referenced to that one.  Typically, the storage is kept with the "start" key and other keys are referenced to that one.
 *    @note      Reimplementations of this method should not return nil
 *    @result    An NSArray of anchor keys
 */
- (NSArray *)anchorKeys;

/**
 *    @brief     A flag specifying whether the given pulse uses anchoring keys
 *    @note      Subclasses should not reimplement this function
 */
- (BOOL)usesAnchoring;

/**
 *    @brief     Repositions the pulse based upon anchoring information
 *    @details   Call this function in #calculateOutput after all timing information used by anchorKey accessors is updated, but before absolute pulse position information is used.  This sets the pulse position appropriately based upon the currently selected anchor.
 *    @param     anc The anchor controller specifying the currently applicable anchor (typically, this is the protected member variable #anchor)
 */
- (void)setPulsePositionFromAnchor:(SBAnchorController *)anc;

/**
 *    @brief     Initialize a pulse without anchoring
 *    @details   Set this in #initWithPropertyList:params: to prevent anchors from being used.  This sets usesAnchoring=NO.  Anchoring information then will not be included in the pulse inspector.
 */
- (void)initializeWithoutAnchoring;

/**
 *    @brief     A list of keys specifying pulse intervals
 *    @details   An NSArray of NSStrings that indicate the key names for time intervals within the pulse.  Override this function in subclasses to name the available pulse intervals for your pulse.  For each time interval, pulse overlap information can be independently specified.  Then, using one of the *LimitForInterval: functions, maximum gradient parameters can be applied for each interval.  These keys should be added in temporal order within the pulse.
      @result    An NSArray of NSString keys
 */
- (NSArray *)intervalKeys;

/**
 *    @brief     Returns the protected #pulseData member
 *    @details   Users should rarely need this function.  This member should usually be manipulated to create the #trIndependentPulseData and SBTrDependentPulseDataProtocol#trDependentPulseDataForTrNum:of: outputs.  This accessor is used to provide rotation matrix hints and other global information to this plugin.  If an alternative to the #pulseData member is used, then this function may also need to be updated to point toward that object.
 */
- (SBPulseData *)pulseData;

/**
 *    @brief     Private function; not intended for external use
 *    @note      The implementation of this function may change
 */
- (void)processOverlaps:(float *)overlaps;

/**
 *    @brief     Private function; not intended for external use
 *    @note      The implementation of this function may change
 */
- (BOOL)detachStandardInspectors;

/**
 *    @brief     Private function; not intended for external use
 *    @note      The implementation of this function may change
 */
- (BOOL)attachStandardInspectors;

/**
 *    @brief     Private function; not intended for external use
 *    @note      The implementation of this function may change
 */
- (void)initializeAttributeKeys;

@end

/**
 *   @protocol    SBRFPulseSharedParameters 
 *   @brief       Formal protocol for RF Pulses
 *   @details     Pulses which conform to this protocol can relay pertinent information to other classes like #SBSliceSelectGradient.  This allows the slice-selection gradient to associate with the RF pulse to accurately set parameters like thickness.  For examples, see #SBSincRF and #SBSLRExcitation.
 *   @note        RF pulses that include their own gradients (#SBSpectralSpatialRF, for example) should not conform to this protocol
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin
 */
@protocol SBRFPulseSharedParameters <NSObject>

/**
 *   @brief       Returns the total duration of the RF, in milliseconds
 */
- (double)duration;

/**
 *   @brief       Sets the total duration of the RF, in milliseconds
 *   @details     conformant pulses should respond to this setter by making any adjustments necessary to change the pulse duration.  Conformant pulses should also implement validateDuration:error: if certain durations are not acceptable.
 */
- (void)setDuration:(double)val;

/**
 *   @brief       Returns the time-bandwidth product of the pulse
 */
- (double)timeBandwidth;

/**
 *   @brief       Sets the time-bandwidth product of the pulse
 *   @details     conformant pulses should respond to this setter by making any adjustments necessary to change the pulse time-bandwidth product.  Conformant pulses should also implement validateTimeBandwidth:error: if certain time-bandwidth products are not acceptable.
 */
- (void)setTimeBandwidth:(double)val;

/**
 *   @brief       Returns the time in milliseconds between the beginning of the pulse and the pulse peak
 *   @details     This is used to calculate the distribution of rewinders in slice-selection gradients.  For symmetric pulses, this should be set to duration/2.
 */
- (double)peakOffset;

@end

/**
 *   @protocol    SBReadoutSharedParameters 
 *   @brief       Formal protocol for Readout Pulses
 *   @details     Pulses which conform to this protocol can relay readout properties to other classes like #SBReadoutAxis.  For examples, see #SBReadoutInterval and #SBReadoutGradient.
 *   @note        This protocol changed in SpinBench version 1.3.
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin
 */
@protocol SBReadoutSharedParameters <NSObject>

/**
 *   @brief       Returns the readout's resolution in samples
 */
- (int)readoutRes;

@end

/**
 *   @protocol    SBTrDependentPulseDataProtocol 
 *   @brief       Formal protocol for TR-dependent Pulses
 *   @details     Conform to this protocol and implement trDependentPulseDataForTrNum:of: to allow pulse data updating from TR to TR (e.g., phase-encoding gradients).  For efficiency, only return the TR-dependent portions here; return static parts of the pulse in the usual way (see SBPulsePlugin#trIndependentPulseData).
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin
 */
@protocol SBTrDependentPulseDataProtocol

- (SBPulseData *)trDependentPulseDataForTrNum:(int)trNum of:(int)numTr;

@end