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

#import <Foundation/Foundation.h>

@class SBWaveform;
@class SBWaveformSegment;
@class SBKSpaceData;
@class SBReadoutTag;
@class SBVariablePlugin;
@protocol SBMultiInputValueVariableProtocol;
@class SBPropertyListPlugin;
@class SBSegmentMetadata;
@class SBDocument;

/**
 *   @defgroup    SpinBenchDesignTool SpinBench Library
 *   @brief       Pulse sequence generation library
 *   @details     This library is shared between SpinBench.app and the RTHawk Sequencer to produce high-fidelity MR waveforms.
 *   @author      HeartVista, Inc.
 */

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBParamsObject 
 *   @brief       Base class for document-wide parameters and plugin management
 *   @details     SBParamsObject provides storage for document-wide parameters (fieldStrength, t1, t2, ...) as well as management for plugins (pulses, sequencing, simulations, etc.).  A key-value mechanism is also implemented through #valueForSBKey: and #setValue:forSBKey:, which allows setting and getting of document-wide and pulse-related parameters.
 *   @author      HeartVista, Inc.
 *   @see         SBParamsController
 *   @see         SBSpinModel
 */
@interface SBParamsObject : NSObject {
@protected
  NSNotificationCenter *nc; /**< @brief The global NSNotificationCenter instance */

  BOOL initializing; /**< @brief Indicates when the instance or its associated waveform object is being initialized from a file */
  double fieldStrength; /**< @brief Magnetic field strength (T) */
  double gamma; /**< @brief Gyromagnetic ratio (Hz/G) */
  double maxReadoutRate; /**< @brief Maximum acquisition rate for readout intervals (kSamp/sec) */
  int readoutSampleDivisor; /**< @brief Readout windows must have number of samples divisible by this */
  double rfLimit; /**< @brief Maximum RF amplitude (G) */
  double gradLimit; /**< @brief Maximum gradient amplitude (G/cm) */
  double nominalGradLimitScale; /**< @brief Nominal gradient amplitude scale factor */
  double gradLimitXScale; /**< @brief Scale factor for gradient amplitude limit, X component */
  double gradLimitYScale; /**< @brief Scale factor for gradient amplitude limit, Y component */
  double gradLimitZScale; /**< @brief Scale factor for gradient amplitude limit, Z component */
  double slewRateLimit; /**< @brief Maximum gradient slew rate (G/cm/ms) */
  double slewRateLimitXScale; /**< @brief Scale factor for gradient slew rate limit, X component */
  double slewRateLimitYScale; /**< @brief Scale factor for gradient slew rate limit, Y component */
  double slewRateLimitZScale; /**< @brief Scale factor for gradient slew rate limit, Z component */
  double slewMagLimitXScale; /**< @brief Scale factor for gradient slew rate magnitude limit, X component */
  double slewMagLimitYScale; /**< @brief Scale factor for gradient slew rate magnitude limit, Y component */
  double slewMagLimitZScale; /**< @brief Scale factor for gradient slew rate magnitude limit, Z component */
  double slewRateLimitXSlope; /**< @brief DC R/L for the X gradient coil.  Using the voltage model of a gradient coil, this allows slew rates higher than slewRateLimit depending on Gx */
  double slewRateLimitYSlope; /**< @brief DC R/L for the Y gradient coil.  Using the voltage model of a gradient coil, this allows slew rates higher than slewRateLimit depending on Gy */
  double slewRateLimitZSlope; /**< @brief DC R/L for the Z gradient coil.  Using the voltage model of a gradient coil, this allows slew rates higher than slewRateLimit depending on Gz */
  double chronaxie; /**< @brief Reilly curve Chronaxie factor for dB/dt patient safety (ms) */
  BOOL freelyRotatable; /**< @brief Flag indicating whether gradients should be designed so that they may be safely rotated */
  
  double t1; /**< @brief The T1 value of the simulated tissue (ms) */
  double t2; /**< @brief The T2 value of the simulated tissue (ms) */
  double protonDensity; /**< @brief The proton density of the simulated tissue */
  double receiverPhase; /**< @brief Global phase offset for the RF receiver (deg) */
  double tr; /**< @brief Global repetition time (ms) */
  BOOL beginAtSS; /**< @brief Flag indicating whether the Bloch simulator should initiate the sequence at its steady state magnetization */
  NSString *appVersion; /**< @brief An NSString containing the version key for this instance of SpinBench */
  double waveSamplingRate; /**< @brief The sampling rate for RF and gradient waveforms (ksamp/sec) */
  double absoluteTime; /**< @brief The time location of the simulation, counted from the beginning of TR 0 (ms) */
  double frequency; /**< @brief The relative center frequency of the simulation at time 0, with 0 being on-resonant (Hz) */
  double dFrequency; /**< @brief The time rate of change of center frequency of the simulation (Hz/s) */
  double xPosition; /**< @brief The x position of the simulated spins at time 0 (mm) */
  double yPosition; /**< @brief The y position of the simulated spins at time 0 (mm) */
  double zPosition; /**< @brief The z position of the simulated spins at time 0 (mm) */
  double xVelocity; /**< @brief The x velocity of the simulated spins (mm/s) */
  double yVelocity; /**< @brief The y velocity of the simulated spins (mm/s) */
  double zVelocity; /**< @brief The z velocity of the simulated spins (mm/s) */
  
  int numRfTxChannels; /**< @brief The number of (possibly complex-valued) RF transmitters available to the system */
  int numRfRxChannels; /**< @brief The number of (possibly complex-valued) RF receivers available to the system */

  SBDocument *document; /**< @brief The parent SBDocument object associated with this object, if any */
  SBWaveform *waveforms; /**< @brief The SBWaveform object associated with this object */

  NSMutableArray *viewAxes; /**< @brief An array of SBViewAxis objects indicating any views present in the simulation */
  NSMutableArray *orderedReadoutTags; /**< @brief A time-ordered array of readout tags available to the simulation */
  
  NSMutableArray *variablePluginArray; /**< @brief The array of variable plugins specifying the current simulation */
  NSMutableArray *orderedVariableAxes; /**< @brief A subset of the variablePluginArray consisting of only SBVariableAxis objects */
  NSMutableArray *averagedVariableAxes; /**< @brief A subset of the orderedVariableAxes consisting of only objects with averaging enabled */
  SBVariablePlugin <SBMultiInputValueVariableProtocol>* readoutAxis; /**< @brief If not nil, specifies the current readout axis of the simulation */

@private
  int pendingChanges;
  BOOL paramsEnabled;
}

/**
 *    @brief     Initializes a new instance to default values
 *    @param     doc The parent document associated with this object.  If nil, then gui-related controls may be unavailable.
 *    @result    An #SBParamsObject object containing default-initialized parameters
 *    @note      Designated initializer.
 */
-(id)initWithDocument:(SBDocument *)doc;


-(void)setFieldStrength:(double)val; /**< @brief Set the main magnetic field strength (T) */
-(void)setGamma:(double)val; /**< @brief Set the gyromagnetic ratio (Hz/G) */
-(void)setMaxReadoutRate:(double)val; /**< @brief Set the maximum acquisition rate for readout intervals (kSamp/sec) */
-(void)setReadoutSampleDivisor:(int)val; /**< @brief Set the number by which readout window number of samples must be divisible */

/**
 *    @brief     Set the RF limit (G)
 *    @details   The RF limit should be set to the largest RF that can be produced by the hardware under the given conditions.  In general, pulses do not automatically respect this limit.  It is up to the pulse sequence designer to ensure that all RF pulses remain below this limit throughout the user-configurable range.
 */
-(void)setRfLimit:(double)val;
/**
 *    @brief     Set the Gradient magnitude limit (G/cm)
 *    @details   The gradient magnitude limit should be set to the maximum safe value for the hardware system.  If this is not equal for all gradient coils, then scale factors can be set through #setGradLimitXScale:, #setGradLimitYScale:, and #setGradLimitZScale: such that the actual limit on each axis is the product of the scale factor and this gradient limit. If #freelyRotatable is set, then the gradient magnitude will be constrained to the lowest of these three values.
 */
-(void)setGradLimit:(double)val;
/**
 *    @brief     X scale factor for gradient magnitude limit
 *    @details   The actual gradient magnitude limit on each axis is the product of this scale factor and #gradLimit.
 */
-(void)setGradLimitXScale:(double)val;
/**
 *    @brief     Y scale factor for gradient magnitude limit
 *    @details   The actual gradient magnitude limit on each axis is the product of this scale factor and #gradLimit.
 */
-(void)setGradLimitYScale:(double)val;
/**
 *    @brief     Z scale factor for gradient magnitude limit
 *    @details   The actual gradient magnitude limit on each axis is the product of this scale factor and #gradLimit.
 */
-(void)setGradLimitZScale:(double)val;

/**
 *    @brief     Set the Nominal gradient magnitude limit scale factor
 *    @details   This sets the fraction of the maximum gradient amplitude that should be used for pulse waveforms where a specific gradient magnitude is not required.  For example, a specific gradient area may be needed in a pulse sequence without constraint on the waveshape required.  Setting this fraction below 1.0 will instruct SpinBench to create such pulses using a smaller gradient maximum.  This may be helpful to minimize the un-necessary application of dB/dt, and may also be useful for minimization of heating in certain gradient hardware.  This limit is multiplicative with gradLimit, gradLimitXScale, gradLimitYScale, and gradLimitZScale.

      @param val must be positive and less than or equal to 1.0.
*/
-(void)setNominalGradLimitScale:(double)val;

/**
 *    @brief     Set the Gradient slew rate limit (G/cm/ms)
 *    @details   The gradient slew rate limit should be set to the maximum safe value for the hardware system and also based upon patient dB/dt limits.  If the hardware limit is not equal for all gradient coils, then scale factors can be set through #setSlewRateLimitXScale:, #setSlewRateLimitYScale:, and #setSlewRateLimitZScale: such that the actual limit on each axis is the product of the scale factor and this slew limit.  Regulatory limits on dB/dt should be specified for each axis through the product of this value and #slewMagLimitXScale, #slewMagLimitYScale, and #slewMagLimitZScale.  The #chronaxie property may also be set to help speed up some pulses that don't reach full-scale gradient magnitude.  These products specify the radii of an ellipsoid ensuring safety limits are respected in all directions.  For example, if regulatory limits dictate a maximum slew rate of 20 G/cm/ms, and hardware limits dictate a maximum of 15 G/cm/ms on all axes, then setting gradLimit=20, slewMagLimit?Scale=1.0, and slewRateLimit?Scale=0.75 will ensure both hardware and patient safety.  If #freelyRotatable is set, then the gradient slew rate will be constrained to the lowest acceptable value.
 */
-(void)setSlewRateLimit:(double)val;
/**
 *    @brief     X scale factor for gradient slew rate hardware limit
 *    @details   The actual gradient hardware slew rate limit on each axis is the product of this scale factor and #slewRateLimit.
 */
-(void)setSlewRateLimitXScale:(double)val;
/**
 *    @brief     Y scale factor for gradient slew rate hardware limit
 *    @details   The actual gradient hardware slew rate limit on each axis is the product of this scale factor and #slewRateLimit.
 */
-(void)setSlewRateLimitYScale:(double)val;
/**
 *    @brief     Z scale factor for gradient slew rate hardware limit
 *    @details   The actual gradient hardware slew rate limit on each axis is the product of this scale factor and #slewRateLimit.
 */
-(void)setSlewRateLimitZScale:(double)val;
/**
 *    @brief     X scale factor for gradient slew rate patient safety limit
 *    @details   The actual gradient slew rate patient safety limit on each axis is the product of this scale factor and #slewRateLimit.
 */
-(void)setSlewMagLimitXScale:(double)val;
/**
 *    @brief     Y scale factor for gradient slew rate patient safety limit
 *    @details   The actual gradient slew rate patient safety limit on each axis is the product of this scale factor and #slewRateLimit.
 */
-(void)setSlewMagLimitYScale:(double)val;
/**
 *    @brief     Z scale factor for gradient slew rate patient safety limit
 *    @details   The actual gradient slew rate patient safety limit on each axis is the product of this scale factor and #slewRateLimit.
 */
-(void)setSlewMagLimitZScale:(double)val;

/**   @brief     DC R/L for the X gradient coil
 *    @details   Using the voltage model of a gradient coil, this allows slew rates higher than slewRateLimit depending on Gx.
 */
-(void)setSlewRateLimitXSlope:(double)val;

/**   @brief     DC R/L for the Y gradient coil
 *    @details   Using the voltage model of a gradient coil, this allows slew rates higher than slewRateLimit depending on Gy.
 */
-(void)setSlewRateLimitYSlope:(double)val;

/**   @brief     DC R/L for the Z gradient coil
 *    @details   Using the voltage model of a gradient coil, this allows slew rates higher than slewRateLimit depending on Gz.
 */
-(void)setSlewRateLimitZSlope:(double)val;

/**
 *    @brief     Reilly curve Chronaxie for gradient slew rate patient safety limit
 *    @details   The chronaxie value (in ms) sets the amount of gradient speedup that is possible for gradient ramps of a finite length.  See Reilly equation.
 */
-(void)setChronaxie:(double)val;

/**
 *    @brief     Set flag indicating whether gradients should be designed so that they may be safely rotated
 *    @details   Freely rotatable gradients can be safely rotated after their design without fear of the rotated waveform exceeding patient or hardware safety limits.  However, this design constraint results in significantly slower waveforms in most cases.
 */
-(void)setFreelyRotatable:(BOOL)val;
-(void)setT1:(double)val; /**< @brief Set the T1 value of the simulated tissue (ms) */
-(void)setT2:(double)val; /**< @brief Set the T2 value of the simulated tissue (ms) */
-(void)setProtonDensity:(double)val; /**< @brief Set the proton density of the simulated tissue */
-(void)setReceiverPhase:(double)val; /**< @brief Set the global phase offset for the RF receiver relative to the transmitter (deg) */
/**
 *    @brief     Set the relative center frequency of the simulation at time 0 (Hz)
 *    @details   The absolute center frequency of the simulation can be determined by the following formula: \f$ F = gamma * fieldStrength * 10000 + frequency + absoluteTime*dFrequency \f$.  Thus, this frequency variable represents an offset from the center frequency of the magnet at \f$ absoluteTime = 0 \f$.  In most simulations, a time-varying field is not desired and \f$ dFrequency = 0 \f$.
 */
-(void)setFrequency:(double)val;
/**
 *    @brief     Set the time rate of change of center frequency of the simulation (Hz/s)
 *    @details   The absolute center frequency of the simulation can be determined by the following formula: \f$ F = gamma * fieldStrength * 10000 + frequency + absoluteTime*dFrequency \f$.  Thus, this frequency variable represents the time rate of change of center frequency.  In most simulations, a time-varying field is not desired and \f$ dFrequency = 0 \f$.
 */
-(void)setDFrequency:(double)val;
/**
 *    @brief     Set the X position of the simulated spins at time 0 (mm)
 *    @details   The X position of the spins to be simulated, relative to isocenter.  If #xVelocity \f$ \neq 0 \f$, then the time-varying position is given by \f$X = xPosition + absoluteTime*xVelocity \f$.
 */
-(void)setXPosition:(double)value;
/**
 *    @brief     Set the Y position of the simulated spins at time 0 (mm)
 *    @details   The Y position of the spins to be simulated, relative to isocenter.  If #yVelocity \f$ \neq 0 \f$, then the time-varying position is given by \f$Y = yPosition + absoluteTime*yVelocity \f$.
 */
-(void)setYPosition:(double)value;
/**
 *    @brief     Set the Z position of the simulated spins at time 0 (mm)
 *    @details   The Z position of the spins to be simulated, relative to isocenter.  If #zVelocity \f$ \neq 0 \f$, then the time-varying position is given by \f$Z = zPosition + absoluteTime*zVelocity \f$.
 */
-(void)setZPosition:(double)value;

/**
 *    @brief     Set the spin velocity in the x direction (mm/s)
 *    @details   See #setXVelocity: for more details on simulated spin motion.
 */
-(void)setXVelocity:(double)value;

/**
 *    @brief     Set the spin velocity in the y direction (mm/s)
 *    @details   See #setYVelocity: for more details on simulated spin motion.
 */
-(void)setYVelocity:(double)value;

/**
 *    @brief     Set the spin velocity in the z direction (mm/s)
 *    @details   See #setZVelocity: for more details on simulated spin motion.
 */
-(void)setZVelocity:(double)value;

/**
 *    @brief     Set the global repetition time (ms)
 *    @details   The global repetition time is used when new intervals are created for sequencing.  This may or may not correspond to the TR of the resulting sequence, depending upon the sequencing choices made by the sequence designer.  For example, a different TR value may be specified through creating a #SBNewTRPlugin instance.
 */
-(void)setTr:(double)val;

/**
 *    @brief     Set flag indicating whether the Bloch simulator should initiate the sequence at its steady state magnetization
 *    @details   This allows simpler simulation of SSFP sequences by setting this flag and creating only one TR with an RF pulse.
 */
-(void)setBeginAtSS:(BOOL)val;

/**
 *    @brief     Set the index of the currently simulated TR interval
 *    @details   This index starts at zero and goes through numTr-1.  This should not be called with a value outside of that range.  If the current value of #timeWithinTr is larger than the length of the desired TR index, then the behavior is currently undefined.
 */
-(void)setTrNum:(int)val;

/**
 *    @brief     Sets the time location of the simulation, counted from the beginning of TR 0 (ms)
 *    @details   The values for #trNum and #timeWithinTr can be derived from this value.
 */
-(void)setAbsoluteTime:(double)val;

/**
 *    @brief     Set the time interval from the beginning of the current TR to the insertion point
 *    @details   This value should be non-negative and bounded by the length of the current TR.  If the function is called with a value outside that range, then the behavior is currently undefined.
 */
-(void)setTimeWithinTr:(double)val;

/**
 *    @brief     Set the index and time interval of the currently simulated TR interval
 *    @details   This convenience method simultaneously sets the TR index, which should be between zero and numTr-1, and the time within TR, which should be between zero and the duration of the desired TR interval.  This should not be called with values outside of these ranges, as the behavior in those cases is currently undefined.
 */
-(void)setTimeWithinTr:(double)timeWithin trNum:(int)num;

/**
 *    @brief     Set the sampling rate for RF and gradient waveforms (ksamp/sec)
 *    @details   This determines the number of samples used in creating the waveforms for both RF and gradient.  Currently RF and gradients must share the same sampling rate.  Lower values can make simulations faster but also can lead to aliasing or simulation inaccuracies.
 */
-(void)setWaveSamplingRate:(double)value;

/**
 *    @brief     Set the sampling rate for the RF receiver (ksamp/sec)
 *    @details   This determines the sampling rate for all RF receivers.
 *    @deprecated  Use samplingRate attributes associated with individual readout pulses instead
 */
-(void)setReadSamplingRate:(double)value;

/**
 *    @brief     Set the number of (possibly complex-valued) RF transmitters available to the system 
 *    @details   Whenever this value is set, SBNumRfTxChannelsDidChangeNotification is sent with this SBParamsObject as the sender object.
 */
-(void)setNumRfTxChannels:(int)value;

/**
 *    @brief     Set the number of (possibly complex-valued) RF receivers available to the system 
 *    @details   Whenever this value is set, SBNumRfRxChannelsDidChangeNotification is sent with this SBParamsObject as the sender object.
 */
-(void)setNumRfRxChannels:(int)value;

-(double)fieldStrength; /**< @brief The main magnetic field strength (T) */
-(double)gamma; /**< @brief Gyromagnetic ratio (Hz/G) */
-(double)maxReadoutRate; /**< @brief The maximum acquisition rate for readout intervals (kSamp/sec) */
-(int)readoutSampleDivisor; /**< @brief Readout windows must have number of samples divisible by this */

/**
 *    @brief     The maximum RF amplitude (G)
 *    @details   The RF limit should be set to the largest RF that can be produced by the hardware under the given conditions.  In general, pulses do not automatically respect this limit.  It is up to the pulse sequence designer to ensure that all RF pulses remain below this limit throughout the user-configurable range.
 */
-(double)rfLimit;

/**
 *    @brief     The Gradient magnitude limit (G/cm)
 *    @details   The maximum gradient amplitude should be set to the maximum safe value for the hardware system.  If this is not equal for all gradient coils, then scale factors can be set through #setGradLimitXScale:, #setGradLimitYScale:, and #setGradLimitZScale: such that the actual limit on each axis is the product of the scale factor and this gradient limit. If #freelyRotatable is set, then the gradient magnitude will be constrained to the lowest of these three values.
 */
-(double)gradLimit;
-(double)gradLimitXScale; /**< @brief Scale factor for gradient amplitude limit, X component */
-(double)gradLimitYScale; /**< @brief Scale factor for gradient amplitude limit, Y component */
-(double)gradLimitZScale; /**< @brief Scale factor for gradient amplitude limit, Z component */

/**
 *    @brief     The Nominal gradient magnitude limit scale factor
 *    @details   The fraction of the maximum gradient amplitude that should be used for pulse waveforms where a specific gradient magnitude is not required.  For example, a specific gradient area may be needed in a pulse sequence without constraint on the waveshape required.  Setting this fraction below 1.0 will instruct SpinBench to create such pulses using a smaller gradient maximum.  This may be helpful to minimize the un-necessary application of dB/dt, and may also be useful for minimization of heating in certain gradient hardware.  This limit is multiplicative with gradLimit, gradLimitXScale, gradLimitYScale, and gradLimitZScale.

 This value must be positive and less than or equal to 1.0.
 */
-(double)nominalGradLimitScale;

/**
 *    @brief     The gradient slew rate limit (G/cm/ms)
 *    @details   The gradient slew rate limit should be set to the maximum safe value for the hardware system and also based upon patient dB/dt limits.  If the hardware limit is not equal for all gradient coils, then scale factors can be set through #setSlewRateLimitXScale:, #setSlewRateLimitYScale:, and #setSlewRateLimitZScale: such that the actual limit on each axis is the product of the scale factor and this slew limit.  For the voltage model of slew rate, set #slewRateLimitXSlope, #slewRateLimitYSlope and #slewRateLimitZSlope appropriately to allow slew rates higher than the fixed limit depending on gradient magnitude.  Regulatory limits on dB/dt should be specified for each axis through the product of this value and #slewMagLimitXScale, #slewMagLimitYScale, and #slewMagLimitZScale.  The #chronaxie property may also be set to help speed up some pulses that don't reach full-scale gradient magnitude.  These products specify the radii of an ellipsoid ensuring safety limits are respected in all directions.  For example, if regulatory limits dictate a maximum slew rate of 20 G/cm/ms, and hardware limits dictate a maximum of 15 G/cm/ms on all axes, then setting gradLimit=20, slewMagLimit?Scale=1.0, and slewRateLimit?Scale=0.75 will ensure both hardware and patient safety.  If #freelyRotatable is set, then the gradient slew rate will be constrained to the lowest acceptable value.
 */
-(double)slewRateLimit;
-(double)slewRateLimitXScale; /**< @brief Scale factor for gradient slew rate limit, X component */
-(double)slewRateLimitYScale; /**< @brief Scale factor for gradient slew rate limit, Y component */
-(double)slewRateLimitZScale; /**< @brief Scale factor for gradient slew rate limit, Z component */
-(double)slewMagLimitXScale; /**< @brief Scale factor for gradient slew rate magnitude limit, X component */
-(double)slewMagLimitYScale; /**< @brief Scale factor for gradient slew rate magnitude limit, Y component */
-(double)slewMagLimitZScale; /**< @brief Scale factor for gradient slew rate magnitude limit, Z component */
-(double)slewRateLimitXSlope; /**< @brief DC R/L for the X gradient coil.  Using the voltage model of a gradient coil, this allows slew rates higher than slewRateLimit depending on Gx */
-(double)slewRateLimitYSlope; /**< @brief DC R/L for the Y gradient coil.  Using the voltage model of a gradient coil, this allows slew rates higher than slewRateLimit depending on Gy */
-(double)slewRateLimitZSlope; /**< @brief DC R/L for the Z gradient coil.  Using the voltage model of a gradient coil, this allows slew rates higher than slewRateLimit depending on Gz */
-(double)chronaxie; /**< @brief Reilly curve Chronaxie for gradient slew rate patient safety limit (ms) */

/**
 *    @brief     Flag indicating whether gradients should be designed so that they may be safely rotated
 *    @details   Freely rotatable gradients can be safely rotated after their design without fear of the rotated waveform exceeding patient or hardware safety limits.  However, this design constraint results in significantly slower waveforms in most cases.
 */
-(BOOL)freelyRotatable;

-(double)t1; /**< @brief The T1 value of the simulated tissue (ms) */
-(double)t2; /**< @brief The T2 value of the simulated tissue (ms) */
-(double)protonDensity; /**< @brief The proton density of the simulated tissue */
-(double)receiverPhase; /**< @brief Global phase offset for the RF receiver (deg) */
-(double)tr; /**< @brief Global repetition time (ms) */
-(BOOL)beginAtSS; /**< @brief Flag indicating whether the Bloch simulator should initiate the sequence at its steady state magnetization */
-(NSString *)appVersion; /**< @brief An NSString containing the version key for this instance of SpinBench */

/**
 *    @brief     The relative center frequency of the simulation at time 0 (Hz)
 *    @details   The absolute center frequency of the simulation can be determined by the following formula: \f$ F = gamma * fieldStrength * 10000 + frequency + absoluteTime*dFrequency \f$.  Thus, this frequency variable represents an offset from the center frequency of the magnet at \f$ absoluteTime = 0 \f$.  In most simulations, a time-varying field is not desired and \f$ dFrequency = 0 \f$.
 */
-(double)frequency;

/**
 *    @brief     The time rate of change of center frequency of the simulation (Hz/s)
 *    @details   The absolute center frequency of the simulation can be determined by the following formula: \f$ F = gamma * fieldStrength * 10000 + frequency + absoluteTime*dFrequency \f$.  Thus, this frequency variable represents the time rate of change of center frequency.  In most simulations, a time-varying field is not desired and \f$ dFrequency = 0 \f$.
 */
-(double)dFrequency;

/**
 *    @brief     The X position of the simulated spins at time 0 (mm)
 *    @details   The X position of the spins to be simulated, relative to isocenter.  If #xVelocity \f$ \neq 0 \f$, then the time-varying position is given by \f$X = xPosition + absoluteTime*xVelocity \f$.
 */
-(double)xPosition;

/**
 *    @brief     The Y position of the simulated spins at time 0 (mm)
 *    @details   The Y position of the spins to be simulated, relative to isocenter.  If #yVelocity \f$ \neq 0 \f$, then the time-varying position is given by \f$Y = yPosition + absoluteTime*yVelocity \f$.
 */
-(double)yPosition;

/**
 *    @brief     The Z position of the simulated spins at time 0 (mm)
 *    @details   The Z position of the spins to be simulated, relative to isocenter.  If #zVelocity \f$ \neq 0 \f$, then the time-varying position is given by \f$Z = zPosition + absoluteTime*zVelocity \f$.
 */
-(double)zPosition;
-(double)xVelocity; /**< @brief     The X velocity of the simulated spins (mm/s) */
-(double)yVelocity; /**< @brief     The Y velocity of the simulated spins (mm/s) */
-(double)zVelocity; /**< @brief     The Z velocity of the simulated spins (mm/s) */

/**
 *    @brief     The sampling rate for RF and gradient waveforms (ksamp/sec)
 *    @details   This determines the number of samples used in creating the waveforms for both RF and gradient.  Currently RF and gradients must share the same sampling rate.  Lower values can make simulations faster but also can lead to aliasing or simulation inaccuracies.
 */
-(double)waveSamplingRate;

/**
 *    @brief     The sampling rate for the RF receiver (ksamp/sec)
 *    @details   This determines the sampling rate for all RF receivers.
 *    @deprecated  Use samplingRate attributes associated with individual readout pulses instead
 */
-(double)readSamplingRate;

-(double)absoluteTime; /**< @brief The time location of the simulation, counted from the beginning of TR 0 (ms) */
-(double)timeWithinTr; /**< @brief The time interval from the beginning of the current TR to the insertion point */
-(int)trNum; /**< @brief The index of the currently simulated TR interval */

-(int)numRfTxChannels; /**< @brief The number of (possibly complex-valued) RF transmitters available to the system */
-(int)numRfRxChannels; /**< @brief The number of (possibly complex-valued) RF receivers available to the system */
-(int)numGradAxes; /**< @brief The number of gradient axes available to the system */
-(int)numOtherAxes; /**< @brief The number of non-gradient, non-RF waveforms associated with the system */

-(int)waveSamplesForTrNum:(int)trNum; /**< @brief     Number of samples for a given TR interval */
-(double)durationForTrNum:(int)trNum; /**< @brief     Duration for a given TR interval (ms) */
-(int)startOfTrNum:(int)trNum; /**< @brief     Starting sample number for a given TR interval */
-(int)waveSamples; /**< @brief     Total number of samples for the waveform */
-(int)numTr; /**< @brief     Total number of TR intervals */
-(double)sequenceDuration; /**< @brief     Total duration for the waveform (ms) */

/**
 *    @brief     Returns an NSString identifying the units of a given parameter key
 *    @details   If the params object does not implement the specified key, or if the specified key is not usable as a variable axis, then this function returns nil.
 */
- (NSString *)unitsOf:(NSString *)key;

/**
 *    @brief     Returns an array of the read/write SB Keys that are available for use by the calling plugin
 *    @details   Attribute keys which do not have units (i.e., return nil for unitsOf: calls) are not included in this list.  This function is equivalent to calling availabieSBKeysForPlugin:includeReadOnly:NO.
 *    @note      This function should not be called during computations, but can be used to populate user-interface elements.
 *    @param     callingPlugin Generally, the plugin being used to call the function.  Keys associated with the calling function will not be included.  This value may be nil.
 */
- (NSMutableArray *)availableSBKeysForPlugin:(id)callingPlugin;

/**
 *    @brief     Returns an array of the SB Keys that are available for use by the calling plugin
 *    @details   Attribute keys which do not have units (i.e., return nil for unitsOf: calls) are not included in this list.  Read-only keys (including derived and anchor keys) can be included using the includeReadOnly flag.
 *    @note      This function should not be called during computations, but can be used to populate user-interface elements.
 *    @param     callingPlugin Generally, the plugin being used to call the function.  Keys associated with the calling function will not be included.  This value may be nil.
 *    @param     inclReadOnly If YES, then read-only keys are included.
 */
- (NSMutableArray *)availableSBKeysForPlugin:(id)callingPlugin includeReadOnly:(BOOL)inclReadOnly;

/**
 *    @brief     Returns an array of the read-only SB keys that are available for use by the calling plugin
 *    @note      This function should not be called during computations, but can be used to populate user-interface elements.
 *    @param     callingPlugin Generally, the plugin being used to call the function.  Keys associated with the calling function will not be included.  This value may be nil.
 */
- (NSMutableArray *)readOnlySBKeysForPlugin:(id)callingPlugin;

/**
 *    @brief     The parent SBDocument object associated with this object, if any
 *    @details   This value should only be nil if no related SBDocument exists.  Thus, when document=nil, the SBParamsObject instance can be assumed to be the top of the object hierarchy.
 *    @deprecated This should be moved to SBParamsController in a future version.
 */
- (SBDocument *)document;

/**
 *    @brief     The #SBWaveform object owned by this object
 *    @details   Each params object has a #SBWaveform object associated with it, which maintains and calculates the sequence waveforms.  This waveform object may be reallocated or relocated, so dependent objects should not cache its address.
 */
- (SBWaveform *)waveforms;

/**
 *    @brief     Change the #SBWaveform object owned by this object
 *    @note      This function should be called in the #SBWaveform constructor and nowhere else.  To update the waveform object associated with a #SBParamsObject, consider using SBWaveform#setFromPropertyList: or SBWaveform#setStateEqualTo:.
 */
- (void)setWaveformObject:(SBWaveform *)obj;

/**
 *    @brief     Update the state of the params object from a property list serialization
 *    @details   This method is typically used during file reading as part of instantiating a file from a SPV file.  Attribute keys are updated, as well as all plugins.  The  #SBWaveform member is updated with new pulses and sequence plugins.  If the property list is nil, the method returns NO.  Params attributeKeys that are not set are left at their existing values, but all plugins are removed whether replacements are specified or not.  Returns YES upon success, and sends SBPulsePluginArrayDidChangeNotification and SBSequencePluginArrayDidChangeNotification.
 */
- (BOOL)setFromPropertyList:(NSDictionary *)plist;

/**
 *    @brief     Serializes the params object into a property list dictionary
 *    @details   This method is typically used during file writing to a SPV file.  All attribute keys are encoded, as well as all plugins related to the #SBParamsObject instance.  The  #SBWaveform member is also exported.  Values are written to the passed plist object, and any duplicate keys in the passed object are overwritten.
 */
- (void)encodePropertyList:(NSMutableDictionary *)plist;

/**
 *    @brief     Returns the plugin's waveform data for a given trNum
 *    @details   This method returns a #SBWaveformSegment object containing the waveform data for a given tr number.
 *    @param     trNum The tr number for the requested waveform data
 *    @result    A #SBSegmentMetadata object containing all the metadata and tags for the requested tr number and output number.
 */
- (SBWaveformSegment *)waveformSegmentForTrNum:(int)trNum;

/**
 *    @brief     Returns the plugin's metadata for a given trNum
 *    @details   This method returns a #SBSegmentMetadata object containing the metadata for a given tr number.
 *    @param     trNum The tr number for the requested metadata
 *    @result    A #SBSegmentMetadata object containing all the metadata and tags for the requested tr number and output number.
 */
- (SBSegmentMetadata *)metadataForTrNum:(int)trNum;

/**
 *    @brief     Returns auto-generated k-space data for a given interval
 *    @details   This method returns a #SBKSpaceData object containing automatically generated k-space data with a specified moment for a given #SBReadoutTag object and tr number.  This function creates the k-space data numerically based on the gradient output of #waveformSegmentForTrNum:.  In many cases, readout-specific k-space data is more useful; this can be found in #SBReadoutTag.
 *    @param     moment A non-negative integer specifying the moment of the desired k-space data.  Set to zero for conventional k-space data.
 *    @param     readout A #SBReadoutTag object specifying the start and end timings for the desired readout.  If nil, then the resulting k-space data spans the entire TR interval
 *    @param     trNum The tr number for the requested k-space data
 *    @result    A #SBKSpaceData object containing all the k-space information for the requested tr number and output number.
 *    @see       SBReadoutTag#kSpace, SBReadoutTag#kSpaceDensity
 */
- (SBKSpaceData *)kSpaceWithMoment:(int)moment tag:(SBReadoutTag *)readout trNum:(int)trNum;

/**
 *    @brief     Forces all pulse plugins to update their dependencies.
 *    @see       SBPulsePlugin#updateDependencies
 */
- (void)updatePulseDependencies;

- (NSArray *)pulsePluginArray; /**< @brief Returns an ordered array of all instantiated pulse plugins */
- (NSArray *)sequencePluginArray; /**< @brief Returns an ordered array of all instantiated sequence plugins */

/**
 *   @brief     Returns an array of all instantiated output plugins, ordered by their position display z position
 *   @details   The first output plugin on the list is positioned below the second, which is below the third, and so on.
 *   @note      This method should not be used by pulse plugins, sequence plugins, or variable plugins.
 */
- (NSArray *)outputPluginArray;

/**
 *   @brief     Returns an array of all instantiated variable plugins, ordered by their position in the plugin list
 *   @details   Variable plugins are executed in the order of their instantiation in this list.  Thus, if more than one variable axis is present, then the first axis is the outermost loop in the evaluation, and the last axis in the list is the innermost loop.
 */
- (NSArray *)variablePluginArray;

/**
 *   @brief     Returns an array of all unaveraged variable axes, ordered by their variable axis position
 *   @details   Variable axes are variable plugins that conform to #SBMultiInputValueVariableProtocol.  For the plugin to appear in this array, it must also not be averaged.  If more than one axis is associated with this plugin, then this array is ordered by their orderedIndex, as defined by #SBMultiInputValueVariableProtocol.  In general, this ordering dictates the order in which the axes are drawn onto x, y, z ... axes.  Classes that modify this array should announce their changes using SBVariablePluginArrayDidChangeNotification with this SBParamsObject as the sender object.
 */
- (NSMutableArray *)orderedVariableAxes;

/**
 *   @brief     Returns an array of all averaged variable axes, in an unspecified order
 *   @details   Variable axes are variable plugins that conform to #SBMultiInputValueVariableProtocol.  For the plugin to appear in this array, it must also be averaged.  The order of plugins in this array is not specified.  Classes that modify this array should announce their changes using SBVariablePluginArrayDidChangeNotification with this SBParamsObject as the sender object.
 */
- (NSMutableArray *)averagedVariableAxes;

/**
 *   @brief     Convenience method to access the x axis variable
 *   @details   This method is the same as calling [orderedVariableAxes objectAtIndex:0].  If there is not an ordered variable axis, then this function returns nil.
 *   @note      The user interface prevents removal of the x variable axis; therefore, this function should not return nil under normal graphical operation.
 */
- (SBVariablePlugin <SBMultiInputValueVariableProtocol> *)xAxisVariable;

/**
 *   @brief     Convenience method to access the y axis variable
 *   @details   This method is the same as calling [orderedVariableAxes objectAtIndex:1].  If there is not a second ordered variable axis, then this function returns nil.
 */
- (SBVariablePlugin <SBMultiInputValueVariableProtocol> *)yAxisVariable;

/**
 *   @brief     Replaces the existing set of variable plugins with this one
 *   @details   The array is retained but not copied, and this class may modify the array object as necessary after it is set.
 */
- (void)setVariablePlugins:(NSMutableArray *)vars;

/**
 *   @brief     Returns an array of the anchor keys associated with the sequence
 *   @details   At least three anchor keys are provided:
 *              - "startOfTR", which represents the start of each TR interval; 
 *              - "midTR", which represents the center of the global TR interval; and
 *              - "endOfTR", which represents the end of the global TR interval.  
 *              A similar method is provided for each #SBPulsePlugin.
 */
- (NSArray *)anchorKeys;

/**
 *   @brief     The document-wide read/write attribute keys associated with this params object
 *   @details   Attribute keys are saved with the SPV file, provide undo/redo capabilities, and may be modified through the SBKey mechanism.  This array of NSStrings indicates the attribute keys that are provided by SBParamsObject.  A similar method is provided for each #SBPropertyListPlugin.
 */
- (NSArray *)attributeKeys;

/**
 *   @brief     The document-wide derived attribute keys associated with this params object
 *   @details   Derived attribute keys cannot be set, but can be read and used for information or other calculations.  Their value is automatically derived from other sequence information.  These keys are not saved with the SPV file, do not provide undo/redo capabilities, and may only be read through the SBKey mechanism.  This array of NSStrings indicates the read-only attribute keys that are provided by SBParamsObject.  An example of such a key is 'numTr'.
 *   @note      Similar methods are provided for each #SBPropertyListPlugin.
 */
- (NSArray *)derivedAttributeKeys;

/**
 *   @brief     Array of all view index keys provided by currently instantiated plugins
 *   @details   View index keys provide the mechanism for uniquely identifying the order of readouts.  Many sequence plugins and readout pulses define view index keys.  Use this method to get a complete list of usable view index keys provided by the current sequence.  These keys are associated with segment metadata and readout tags.  To query the value of one of these keys, see #SBSegmentMetadata and #SBReadoutTag.
 */
- (NSArray *)viewIndexKeys;

/**
 *   @brief      Increment parameter change grouping status
 *   @deprecated This call has been superseded by SBParamsObject#beginParameterChangeGroup.
 */
- (void)incrementPendingChanges;

/**
 *   @brief      Decrement parameter change grouping status
 *   @deprecated This call has been superseded by SBParamsObject#endParameterChangeGroup.
 */
- (void)decrementPendingChanges;

/**
 *   @brief     Begin a grouping of parameter changes
 *   @details   Parameter change groups provide a mechanism for collecting multiple changes into a single unit.  During a parameter change grouping, simulator recalculation events are not sent and dependent parameters are not updated.  These events are delayed until the end of the parameter change group.  Such groupings may make calculations faster, and may also help to collect a group of operations into a single unit for the undo mechanism.  As an example, changing a nine-element rotation matrix from axial to sagittal co-ordinates requires many floating-point parameter updates but should be considered as a single functional change.
 *   @note      Be sure to balance all incrementPendingChanges calls with a corresponding decrementPendingChanges, or the auto-update mechanism may fail.
 *   @note      Increment/decrement pairs may be nested.  Parameter evaluation is deferred until the last decrementPendingChanges is called.
 */
- (void)beginParameterChangeGroup;

/**
 *   @brief     End a grouping of parameter changes
 *   @details   Parameter change groups provide a mechanism for collecting multiple changes into a single unit.  During a parameter change grouping, simulator recalculation events are not sent, and dependent parameters are not updated.  These events are delayed until the end of the parameter change group.  Such groupings may make calculations faster, and may also help to collect a group of operations into a single unit for the undo mechanism.  As an example, changing a nine-element rotation matrix from axial to sagittal co-ordinates requires many floating-point parameter updates but should be considered as a single functional change.
 *   @note      Be sure to balance all incrementPendingChanges calls with a corresponding decrementPendingChanges, or the auto-update mechanism may fail.
 *   @note      Increment/decrement pairs may be nested.  Parameter evaluation is deferred until the last decrementPendingChanges is called.
 */
- (void)endParameterChangeGroup;

/**
 *   @brief     Returns the number of active parameter change groups
 *   @details   This method is provided as a convenience for debugging.  It should not be relied upon for any production purpose as the status of other parameter change groups may change depending upon the calling environment and other factors.
 */
- (int)pendingChanges;

/**
 *   @brief     Computing flag
 *   @details   When the simulator is in process of computation using this instance, this flag will be true.  Note that non-computing instances of SBParamsObject may be available even during Bloch simulator computation.  To determine whether any computations are ongoing, see SBDocument#working.
 */
- (BOOL)isComputingMagnetization;

/**
 *   @brief     Master instance flag
 *   @details   Returns YES if the receiver is the master instance of #SBParamsObject.  The master instance is responsible for the values shown in the UI, and it implements additional methods found in SBParamsController.  This function is rarely used.
 */
- (BOOL)isParamsController;

/**
 *   @brief     Set initializing flag
 *   @details   Indicates when the instance or its associated waveform object is being initialized from a file.  API users should not need to set this.
 */
- (void)setInitializing:(BOOL)val;

/**
 *   @brief     Initializing flag
 *   @details   Indicates when the instance or its associated waveform object is being initialized from a file.  True when starting up and when copying state.
 */
- (BOOL)initializing;

/**
 *   @brief     Print state variables to the console
 *   @details   Useful for debugging, prints the current values of attributeKeys and other pertinent information.  To obtain this information in an NSString, use -description.
 */
- (void)printState;

/**
 *   @brief     Flag for enabled state
 *   @details   The master SBParamsObject becomes disabled when certain long-running processes are active and any parameter changes may disrupt the process.  For example, rendering operations use paramsEnabled to prevent changes to the simulation during rendering.  Plugins should check paramsEnabled before allowing any gui changes to parameters.  Often this can be accomplished by binding ui elements' enabled property to the paramsEnabled key of their owning plugin class.
 */
- (BOOL)paramsEnabled;

/**
 *   @brief     Temporarily disable the undo mechanism
 *   @details   When disabled, parameter changes are not logged into the undo queue.  This should be used very carefully, as the interface can easily get into an undesirable state if all actions are not properly registered for undo.  Care should also be taken to ensure that the undo mechanism is always re-enabled.
 */
- (void)disableUndo;

/**
 *   @brief     Re-enable the undo mechanism
 *   @details   When disabled, parameter changes are not logged into the undo queue.  This should be used very carefully, as the interface can easily get into an undesirable state if all actions are not properly registered for undo.  Care should also be taken to ensure that the undo mechanism is always re-enabled.
 */
- (void)enableUndo;

/**
 *   @brief     Flag indicating whether the current operation is part of an undo command
 *   @details   If YES, then the current context was triggered through an undo operation.
 */
- (BOOL)isUndoing;

/**
 *   @brief     The closest readout tag
 *   @details   Returns the #SBReadoutTag that is within the current TR interval and is closest to the current insertion point, if any.  May return nil if no readout tags exist in the current TR interval.
 */
- (SBReadoutTag *)closestReadoutTag;

/**
 *   @brief      Sets the time bar to match the currently selected view axis
 *   @deprecated This call may disappear in future versions for performance reasons
 */
- (void)updateTimeBarFromViewAxisSelections;

/**
 *   @brief      Returns a plugin with the given name
 *   @details    The plugin's editable name uniquely identifies it among all instances of subclasses of #SBPropertyListPlugin.  If no plugin is found with the given name, nil is returned.
 */
- (SBPropertyListPlugin *)propertyListPluginWithEditableName:(NSString *)ename;

/**
 *   @brief      Copy internal state from the supplied params object
 *   @details    This call may be used to perform a deep copy of one params object into another.  This does only the minimum required to ensure that the state variables are set.  Follow-up with #configureInternalState to finish the copy operation
 */
- (void)setStateEqualTo:(SBParamsObject *)obj;

/**
 *   @brief      Complete a copy operation by updating derived state variables
 *   @details    This call should only be necessary after calls to #setStateEqualTo:.
 */
- (void)configureInternalState;

/**
 *   @brief      Indicates whether the given SB Key exists
 *   @details    SB Keys are of the form '\<PluginEditableName\>.key' for plugin keys, or simply 'key' for #SBParamsObject keys.  This method queries whether a given key exists.  The key may or may not be writable (e.g., it may be a derived key).
 */
- (BOOL)sbKeyExists:(NSString *)sbKey;

/**
 *   @brief      Set the given SB Key to the supplied value
 *   @details    SB Keys are of the form '\<PluginEditableName\>.key' for plugin keys, or simply 'key' for #SBParamsObject keys.  This method first validates the value for the key, then updates the key with the validated value.  Note that this behavior is different from the standard accessors, which should set the key without validation.  If the key does not exist, nothing is set and an error message is printed.
 */
- (void)setValue:(NSNumber *)val forSBKey:(NSString *)sbKey;

/**
 *   @brief      Returns the current value of the given SB Key
 *   @details    SB Keys are of the form '\<PluginEditableName\>.key' for plugin keys, or simply 'key' for #SBParamsObject keys.  This method returns the current value of the SB Key.  If the key does not exist, a default-initialized NSNumber is returned and an error message is printed.
 */
- (NSNumber *)valueForSBKey:(NSString *)sbKey;

/**
 *   @brief      Returns the owning object of the given SB Key
 *   @details    SB Keys are of the form '\<PluginEditableName\>.key' for plugin keys, or simply 'key' for #SBParamsObject keys.  For #SBParamsObject keys, this method returns the params object instance.  For plugin keys, it returns the plugin instance.  This method does not check to ensure that the specified key exists.
 */
- (id)ownerForSBKey:(NSString *)sbKey;

/**
 *   @brief      Returns the key portion of the given SB Key
 *   @details    SB Keys are of the form '\<PluginEditableName\>.key' for plugin keys, or simply 'key' for #SBParamsObject keys.  This method strips the plugin identifier, if any, and returns simply the key name.  This method does not check to ensure that the specified key or owner exists.
 */
- (NSString *)keyForSBKey:(NSString *)sbKey;

/**
 *   @brief      Composes a conforming SB Key from a given owner and key name
 *   @details    SB Keys are of the form '\<PluginEditableName\>.key' for plugin keys, or simply 'key' for #SBParamsObject keys.  This method creates the SB key given the key's name and the plugin object.  Returns nil if the specified owner can't be found or isn't an instance of an appropriate class.
 */
- (NSString *)sbKeyForKey:(NSString *)key withOwner:(id)owner;

/**
 *   @brief      Updates the internal model and all dependent plugins
 *   @details    Calling this function triggers a recalculation of all computation units affected by the previous parameter change grouping.  
 *   @note       This method should not be called outside of #SBParamsObject.  Sublcasses of #SBParamsObject may reimplement the method to provide additional functionality at the time of parameter updates.
 */
-(void)update:(NSString *)lastSBKey;

/**
 *   @brief      Updates acquisition properties to permit reconstruction of view axes
 *   @note       This function should only be called by subclasses of #SBParamsObject.
 *   @deprecated This call may disappear in future versions for performance reasons
 */
- (void)updateAcquisitionProperties;

@end

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @category    NSString(SBCapSupport) 
 *   @brief       Extends NSString to provide capitalization methods
 *   @author      HeartVista, Inc.
 *   @see         NSString
 */
@interface NSString(SBCapSupport)
/**
 *   @brief      Capitalizes the first letter of the supplied string
 *   @note       If the first letter of the receiver is a lower-case letter a-z, then this method returns a new string with the first letter capitalized.  Otherwise, it returns a copy of the receiver.
 */
-(NSString *)capitalizeFirst;

/**
 *   @brief      Lowercases the first letter of the supplied string
 *   @note       If the first letter of the receiver is an upper-case letter A-Z, then this method returns a new string with the first letter in lower-case.  Otherwise, it returns a copy of the receiver.
 */
-(NSString *)lowercaseFirst;
@end

