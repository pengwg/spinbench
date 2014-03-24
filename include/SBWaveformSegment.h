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

@class SBWaveformParams;
@class SBWaveform;


/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBWaveformSegment
 *   @brief       Stores the data for one segment (usually, one TR) of an entire waveform
 *   @author      HeartVista, Inc.
 */


@interface SBWaveformSegment : NSObject {
@private
	int samples;
  int allocatedSamples;
	float **allSegments;
	float **rfSegment,**gradSegment,**otherSegment;

	SBWaveform *waveforms;
}

+(SBWaveformSegment *)segmentWithLength:(int)len waveforms:(SBWaveform *)wfm;
+(SBWaveformSegment *)segmentEqualTo:(SBWaveformSegment *)segment;

-(int)length;
-(void)setLength:(int)val;      // this can be used to shorten an existing segment; usually fails if val > current length

-(float **)rfSegment;
-(float **)gradSegment;
-(float **)otherSegment;

-(void)overlayWaveformFromSegment:(SBWaveformSegment *)segment;
-(void)printState;

@end
