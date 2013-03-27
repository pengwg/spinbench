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

@class SBWaveform;
@class SBRotation;

/**
 *   @class       SBPulseData 
 *   @brief       Provides storage for floating point pulse data
 *   @details     Pulses are stored in floating-point arrays #rfData, #gradData, and #otherData.  Individual objects may contain any or all of these data types.  Information about the current rotation matrix and pulse overlaps is also available through this class.  Through this class, simple data manipulations and filtering can also be applied.  For example, subdata objects containing only certain pulse axes can be easily extracted.  Data resampled at different sampling rates is also available.
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin
 */
@interface SBPulseData : NSObject {
@private
	float **data;
	float **rfGainTransform;
	float **rfPhaseTransform;
	float **gradTransform;
  SBRotation *rotation;
	float **otherTransform;
  float *gradScaleFactors;
  NSMutableDictionary *overlaps;
	int numPoints;
	int startPosition;

	int numRfTxAxes;
	int numGradAxes;
	int numOtherAxes;
	int numAxes;
	
	SBWaveform *waveform;
}

/**
 *    @brief     Initializes a new instance to the provided specifications
 *    @details   NumPoints is initialized to 0.  To set up a number of points, use #setNumPoints:setToZero: or #setNumPoints:startPosition:setToZero:.
 *    @param     _rf    The number of RF transmit axes needed by the pulse
 *    @param     _grad  The number of Gradient axes needed by the pulse
 *    @param     _other The number of Other axes needed by the pulse
 *    @param     _wfm   A reference to the associated waveform object
 *    @result    An initialized #SBPulseData object
 *    @note      Designated initializer.
 */
- (id)initWithRfAxes:(int)_rf gradAxes:(int)_grad otherAxes:(int)_other waveform:(SBWaveform *)_wfm;

/**
 *    @brief     Initializes an autoreleased instance to the provided specifications
 *    @details   This method is provided for convenience; it returns an autoreleased instance but is otherwise identical to #initWithRfAxes:gradAxes:otherAxes:waveform:.
 *    @param     _rf    The number of RF transmit axes needed by the pulse
 *    @param     _grad  The number of Gradient axes needed by the pulse
 *    @param     _other The number of Other axes needed by the pulse
 *    @param     _wfm   A reference to the associated waveform object
 *    @result    An initialized #SBPulseData object
 */
+ (id)pulseDataWithRfAxes:(int)_rf gradAxes:(int)_grad otherAxes:(int)_other waveform:(SBWaveform *)_wfm;

/**
 *    @brief     Provides an autoreleased instance identical to the passed parameter
 *    @details   Pulse axis parameters are copied as well as any floating-point data contained in the passed object.
 *    @param     copyData The instance to be copied
 *    @result    A copied #SBPulseData object
 */
+ (id)pulseDataEqualTo:(SBPulseData *)copyData;

/**
 *    @brief     The total number of pulse axes stored in this SBPulseData instance
 *    @details   This is the sum of the rf axes, gradient axes, and other axes.
 */
- (int)numAxes;

/**
 *    @brief     The total number of RF transmit axes stored in this SBPulseData instance
 *    @details   Typically, a quadrature channel consists of two axes.  Thus, numRfTxAxes would be twice numRfTxChannels.  This class currently assumes that RF transmit axes are paired into quadrature channels, unless an odd number of axes are present, in which case the last axis is an unpaired channel.
 */
- (int)numRfTxAxes;

/**
 *    @brief     The total number of RF transmit channels stored in this SBPulseData instance
 *    @details   Typically, a quadrature channel consists of two axes.  Thus, numRfTxAxes would be twice numRfTxChannels.  This class currently assumes that RF transmit axes are paired into quadrature channels, unless an odd number of axes are present, in which case the last axis is an unpaired channel.
 */
- (int)numRfTxChannels;

/**
 *    @brief     The total number of gradient axes stored in this SBPulseData instance
 */
- (int)numGradAxes;

/**
 *    @brief     The total number of other (non-rf, non-gradient) axes stored in this SBPulseData instance
 */
- (int)numOtherAxes;

/**
 *    @brief     The number of data points per axis of pulse data
 */
- (int)numPoints;

/**
 *    @brief     The sample number of the starting position of this pulse data within TR
 *    @details   The starting position is passed to the sequencing system to place the pulse correctly within TR.
 */
- (int)startPosition;

/**
 *    @brief     Returns the scale factor associated with a specific pulse gradient axis
 *    @details   Scale factors are a convenient way of rescaling gradients without iterating over their contents.  For example, phase-encoding gradients may set the gradient scale factor for each view amplitude rather than re-writing the gradient data at each step.  Prints an error message if idx exceeds numGradAxes.  Scale factors are initialized to 1.0.
 */
- (float)scaleFactorForGradAxis:(int)idx;

/**
 *    @brief     Sets the scale factor associated with a specific pulse gradient axis
 *    @details   Scale factors are a convenient way of rescaling gradients without iterating over their contents.  For example, phase-encoding gradients may set the gradient scale factor for each view amplitude rather than re-writing the gradient data at each step.  Prints an error message if idx exceeds numGradAxes.  Scale factors are initialized to 1.0.
 */
- (void)setScaleFactor:(float)val forGradAxis:(int)idx;

/**
 *    @brief     Modifiable pointer to all data
 *    @details   This returns an array of data that is #numAxes by #numPoints, but the order of gradient, rf, and other axes cannot be guaranteed.  Use this pointer only if that information is not relevant; for example, to set all data to zeros.  To set specific data, use #rfData, #gradData, or #otherData pointers instead.
 */
- (float **)allData;

/**
 *    @brief     Modifiable pointer to rf tx data
 *    @details   This returns an array of data that is #numRfTxAxes by #numPoints.  Channels are paired sequentially.  Upon creation, this data is initialized with zeros.
 */
- (float **)rfData;

/**
 *    @brief     Modifiable pointer to gradient data
 *    @details   This returns an array of data that is #numGradAxes by #numPoints.  Upon creation, this data is initialized with zeros.
 */
- (float **)gradData;

/**
 *    @brief     Modifiable pointer to other (non-rf, non-gradient) data
 *    @details   This returns an array of data that is #numOtherAxes by #numPoints.  Upon creation, this data is initialized with zeros.
 */
- (float **)otherData;

/**
 *    @brief     Returns the current gradient rotation matrix
 *    @details   The current rotation matrix is used to determine the maximum gradient amplitude and slew rates in non-freely-rotatable designs.  If numGradAxes=0, the rotation is not initialized and points to nil.
 */
- (SBRotation *)rotation;

/**
 *    @brief     Sets the current gradient rotation matrix
 *    @details   The current rotation matrix is used to determine the maximum gradient amplitude and slew rates in non-freely-rotatable designs.  This method copies the passed rotation.
 */
- (void)setRotation:(SBRotation *)rot;

/**
 *    @brief     Returns the overlap coefficients for a given pulse interval
 *    @details   Overlaps are [waveform numGradAxes] in length (note that these are waveform, not pulse axes).  An overlap of 1.0 means that the pulse is fully overlapping on that axis; for each interval, overlaps are initialized to 1.0 for all waveform axes.
 */
- (const float *)overlapsForInterval:(NSString *)interval;

/**
 *    @brief     Sets zeros to the overlap coefficients for a given pulse interval
 *    @details   Overlaps are [waveform numGradAxes] in length (note that these are waveform, not pulse axes).  An overlap of 1.0 means that the pulse is fully overlapping on that axis.  This sets all overlaps to 0.0 for the given interval.
 */
- (void)setNoOverlapForInterval:(NSString *)interval;

/**
 *    @brief     Sets the overlap coefficients for a given pulse interval
 *    @details   Overlaps are [waveform numGradAxes] in length (note that these are waveform, not pulse axes).  An overlap of 1.0 means that the pulse is fully overlapping on that axis.  This sets the x, y, and z overlaps for the given interval.  If [waveform numGradAxes] is less than 3, then the other axes are not set.  This method has no effect on axis indices greater than 3.  For greater compatibility with various [waveform numGradAxes], consider using #setOverlapsForInterval:numValues:values:.
 */
- (void)setOverlapsForInterval:(NSString *)interval x:(float)xVal y:(float)yVal z:(float)zVal;

/**
 *    @brief     Sets the overlap coefficients for a given pulse interval
 *    @details   Overlaps are [waveform numGradAxes] in length (note that these are waveform, not pulse axes).  An overlap of 1.0 means that the pulse is fully overlapping on that axis.  This sets numValues overlaps for the given interval.  If [waveform numGradAxes] is less than numValues, then the other values are ignored.    If [waveform numGradAxes] is greater than numValues, then the other values are not set.
 *    @param     interval The interval key.
 *    @param     numValues The number of values to be set.  Do not call this method with numValues < 1.
 *    @param     val1,... A NULL-terminated list of double values, of length numValues.
 */
- (void)setOverlapsForInterval:(NSString *)interval numValues:(int)numValues values:(double)val1,...;

/**
 *    @brief     Sets the overlap coefficients for a given pulse interval and index
 *    @details   Overlaps are [waveform numGradAxes] in length (note that these are waveform, not pulse axes).  An overlap of 1.0 means that the pulse is fully overlapping on that axis.  This sets the overlap value for the given interval and waveform gradient axis index.
 *    @param     interval The interval key.
 *    @param     idx The number of values to be set.  Do not call this method with idx >= [waveform numGradAxes] or idx<0.
 *    @param     val The value to set.
 */
- (void)setOverlapForInterval:(NSString *)interval index:(int)idx value:(float)val;

/**
 *    @brief     Sets the length and starting position of the pulse
 *    @details   Use this to resize the pulse data object appropriately for the pulse.  Assume that any data already in the object will be corrupted by this call.  If desired, the data can be reset to zeros using the toZero parameter.
 *    @param     pts The requested number of data points per axis
 *    @param     pos The starting sample of the pulse within TR
 *    @param     toZero Flag indicating whether the method should set the data to zero after allocation
 */
- (void)setNumPoints:(int)pts startPosition:(int)pos setToZero:(BOOL)toZero;

/**
 *    @brief     Sets the length of the pulse
 *    @details   Use this to resize the pulse data object appropriately for the pulse.  Assume that any data already in the object will be corrupted by this call.  If desired, the data can be reset to zeros using the toZero parameter.  Start position is unaffected by this method.
 *    @param     pts The requested number of data points per axis
 *    @param     toZero Flag indicating whether the method should set the data to zero after allocation
 */
- (void)setNumPoints:(int)pts setToZero:(BOOL)toZero;

/**
 *    @brief     Sets the starting sample of the pulse
 *    @details   The starting position is passed to the sequencing system to place the pulse correctly within TR.
 *    @param     pos The starting sample of the pulse within TR
 */
- (void)setStartPosition:(int)pos;

/**
 *    @brief     Resets pulse data to zeros
 *    @details   This sets zeros for all points and all axes.
 */
- (void)setToZero;

/**
 *    @brief     Returns a new SBPulseData object containing data copied from one RF channel
 *    @details   The returned object has identical parameters to the receiver, but contains one RF channel and no gradient or other axes.  RF data for the returned channel is copied from the receiver.
 */
- (SBPulseData *)subDataWithRfChannel:(int)ch;

/**
 *    @brief     Returns a new SBPulseData object containing data copied from one gradient axis
 *    @details   The returned object has identical parameters to the receiver, but contains one gradient axis and no RF or other axes.  Gradient data for the returned axis is copied from the receiver.
 */
- (SBPulseData *)subDataWithGradAxis:(int)axis;

/**
 *    @brief     Returns a new SBPulseData object containing data copied from one other axis
 *    @details   The returned object has identical parameters to the receiver, but contains one 'other' axis and no gradient or RF axes.  Other data for the returned axis is copied from the receiver.
 */
- (SBPulseData *)subDataWithOtherAxis:(int)axis;

/**
 *    @brief     Returns a new SBPulseData object containing all but one RF channel of the receiver
 *    @details   The returned object has identical parameters to the receiver, but contains one less RF channel.  Data for the remaining axes is copied from the receiver.
 */
- (SBPulseData *)subDataExcludingRfChannel:(int)ch;

/**
 *    @brief     Returns a new SBPulseData object containing all but one gradient axis of the receiver
 *    @details   The returned object has identical parameters to the receiver, but contains one less gradient axis.  Data for the remaining axes is copied from the receiver.
 */
- (SBPulseData *)subDataExcludingGradAxis:(int)axis;

/**
 *    @brief     Returns a new SBPulseData object containing all but one other axis of the receiver
 *    @details   The returned object has identical parameters to the receiver, but contains one less 'other' axis.  Data for the remaining axes is copied from the receiver.
 */
- (SBPulseData *)subDataExcludingOtherAxis:(int)axis;

/**
 *    @brief     Returns a new SBPulseData object containing data copied from a range of RF channels
 *    @details   The returned object has identical parameters to the receiver, but contains only the specified RF channels and no gradient or other axes.  RF data for the returned channels is copied from the receiver.
 */
- (SBPulseData *)subDataWithRfChannelRange:(NSRange)range;

/**
 *    @brief     Returns a new SBPulseData object containing data copied from a range of gradient axes
 *    @details   The returned object has identical parameters to the receiver, but contains only the specified gradient axes and no RF or other axes.  Gradient data for the returned axes is copied from the receiver.
 */
- (SBPulseData *)subDataWithGradAxisRange:(NSRange)range;

/**
 *    @brief     Returns a new SBPulseData object containing data copied from a range of other axes
 *    @details   The returned object has identical parameters to the receiver, but contains only the specified 'other' axes and no gradient or RF axes.  Other data for the returned axes is copied from the receiver.
 */
- (SBPulseData *)subDataWithOtherAxisRange:(NSRange)range;

/**
 *    @brief     Returns a new SBPulseData object omitting a range of RF channels from the receiver
 *    @details   The returned object has identical parameters to the receiver, but removes the specified RF channels.  Data for the remaining axes is copied from the receiver.
 */
- (SBPulseData *)subDataExcludingRfChannelRange:(NSRange)range;

/**
 *    @brief     Returns a new SBPulseData object omitting a range of gradient axes from the receiver
 *    @details   The returned object has identical parameters to the receiver, but removes the specified gradient axes.  Data for the remaining axes is copied from the receiver.
 */
- (SBPulseData *)subDataExcludingGradAxisRange:(NSRange)range;

/**
 *    @brief     Returns a new SBPulseData object omitting a range of other axes from the receiver
 *    @details   The returned object has identical parameters to the receiver, but removes the specified 'other' axes.  Data for the remaining axes is copied from the receiver.
 */
- (SBPulseData *)subDataExcludingOtherAxisRange:(NSRange)range;

/**
 *    @brief     Returns a modifiable reference to the M-by-N gain transform between RF pulse channels and RF waveform channels
 *    @details   M is the number of pulse RF channels, and N is the number of waveform RF channels.
 */
- (float **)rfGainTransform;

/**
 *    @brief     Returns a modifiable reference to the M-by-N phase transform between RF pulse channels and RF waveform channels
 *    @details   M is the number of pulse RF channels, and N is the number of waveform RF channels.
 */
- (float **)rfPhaseTransform;

/**
 *    @brief     Returns a const reference to the M-by-N transform between gradient pulse axes and gradient waveform axes
 *    @details   M is the number of gradient pulse axes, and N is the number of gradient waveform axes.
 */
- (float const * const * const)constGradTransform;

/**
 *    @brief     Returns a modifiable reference to the M-by-N transform between gradient pulse axes and gradient waveform axes
 *    @details   M is the number of gradient pulse axes, and N is the number of gradient waveform axes.
 */
- (float **)gradTransform;

/**
 *    @brief     Returns a modifiable reference to the M-by-N transform between 'other' pulse axes and 'other' waveform axes
 *    @details   M is the number of 'other' pulse axes, and N is the number of 'other' waveform axes.
 */
- (float **)otherTransform;

/**
 *    @brief     Shifts pulse data to the right by a specified number of pixels
 *    @details   Subshift can be fractional, positive, or negative.  Data are modified in-place.  Any extra data rolls off the end.
 */
- (void)shiftBySubPixelAmount:(float)subShift;

/**
 *    @brief     Resamples and shifts pulse data to the right by a specified number of pixels
 *    @details   Subshift is in units of the resampled waveform, and should be positive.  Subshifts are accomplished through a combination of modifying startPosition and shifting the resampled data by a subpixel amount.
 *    @param     dataToResample A 2D array of size [pulseData numAxes] x samples containing the data to resample
 *    @param     samples the number of samples in the original data
 *    @param     sampleShift Number of samples to shift the resampled waveform; should be positive.  May be non-integer
 *    @param     newDuration Duration of the resampled waveform, in samples.  This excludes any sampleShift.  May be non-integer.
 */
- (void)resample:(float *[])dataToResample length:(int)samples sampleShift:(float)sampleShift newDuration:(float)newDuration;

/**
 *    @brief     Sets the receiver equal to another SBPulseData object
 *    @details   All data values are copied from the receiver.  The receiver must have the same number of pulse axes as the passed object.  If this is not the case, consider using #pulseDataEqualTo:.
 */
- (void)setEqualTo:(SBPulseData *)other;

@end
