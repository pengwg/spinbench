/***************************************************************************
 
 Copyright 2010-2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/
//
//  Finds a quasi-optimal minimum-time multidimensional waveform that meets a given set of constraints on endpoints, areas, and moments.
//  Currently only supports one or two-dimensional waveforms.


#import <Cocoa/Cocoa.h>

@class SBRotation;
@class SBPulsePlugin;
@class SBRotation;
typedef enum {
  SBNoConstraint        =0x00,
  SBRotatableConstraint =0x01,
  SBScalableConstraint  =0x10
} SBGradientDesignConstraints;

typedef enum {
  SBOptimizeAxisUninitialized=0x000,
  SBOptimizeAxisRamp         =0x001,
  SBOptimizeAxisArea         =0x010,
  SBOptimizeAxisMoment       =0x100
} SBOptimizedGradientAxisType;

@interface SBOptimizedGradient : NSObject {
@private
  int numAxes;
  BOOL needsUpdate;
  double *e;
  double *f;
  double *A;
  double *M;
  
  SBOptimizedGradientAxisType *axisType;
  double Ss;
  float S[3];
  float Smag[3];
  double Gg;
  float G[3];
  double tstep;
  float **gt;
  float *ovr;
  float *rotAxis;
  SBRotation *rotation;
  SBGradientDesignConstraints designConstraints;
  BOOL paramsFreelyRotatable;
  
  double duration;
  double sa;
  double *T0, *T1, *T2, *T3, *T4;
  double *peak;
  double *valley;
  double *moments;
  double *areas;
  // these are for convenience
  double er[2];
  double fr[2];
}

// designated initializer
-(id)initWithAxes:(int)num;
+(id)gradientWithAxes:(int)num;

// startValue and endValue are in G/cm.  area is in G/cm*ms.  Moment is in G/cm*ms^2
-(void)setAxis:(int)axisNum startValue:(double)startVal endValue:(double)endVal;
-(void)setAxis:(int)axisNum startValue:(double)startVal endValue:(double)endVal area:(double)area;
-(void)setAxis:(int)axisNum startValue:(double)startVal endValue:(double)endVal area:(double)area moment:(double)moment;

// this method automatically sets slewLimit, gradLimit, samplingInterval, and etc based on a pulsePlugin object.  This is normally all you need.
-(void)setLimitsForPulse:(SBPulsePlugin *)pulse interval:(NSString *)interval;
// this method allows additional constraints to be specified for the design, including:
//  SBRotatableDesignConstraint designs pulses that can be freely rotated in-plane (for 2-axis design)
//                              or within the volume (for 3-axis)
//                              (which is distinct from the global [params freelyRotatable] setting)
// SBScalableDesignConstraint   designs pulses that can be freely scaled down or amplitude-inverted in any
//                              of the logical axes independently.
//                              For example, you'd want to do this for a pulse to be used for phase-encoding,
//                              where the PE axis will be rescaled each TR.  Another option would be to design two
//                              pulses, one with positive phase-encoding and one with negative phase-encoding,
//                              then take linear combinations of the two to get to each PE step...
-(void)setLimitsForPulse:(SBPulsePlugin *)pulse interval:(NSString *)interval
             constraints:(SBGradientDesignConstraints)constraints;

-(double)amplitudeAtTime:(double)t onAxis:(int)axis;

-(double)duration;
-(const double *)areas;
-(const double *)momentsForStartTime:(double)startTime;
-(const double *)timeReversedMomentsForStartTime:(double)startTime;
-(void)generateWaveformsWithStartTime:(double)startTime dataArray:(float **)data dataArrayLength:(int)dataPts;
-(void)generateTimeReversedWaveformsWithStartTime:(double)startTime dataArray:(float **)data dataArrayLength:(int)dataPts;

@end
