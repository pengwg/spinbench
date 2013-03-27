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

//    Encapsulates the data for one segment of an entire waveform; usually, this means one TR interval

#import <Cocoa/Cocoa.h>

@class SBWaveformParams;
@class SBWaveform;

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
