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

#import <Cocoa/Cocoa.h>
#import <Accelerate/Accelerate.h>

@interface SB2DFT : NSObject {
	int xRes,yRes;
	int M,N;
	int log2M, log2N;
	int ftSize;
	float scale;
	FFTSetup ftSetup;
	float *ftReal, *ftImag;
	unsigned char *byteReal, *byteImag, *byteMag, *bytePhase;
}

+ (id)ftWithXRes:(int)_xRes yRes:(int)_yRes;
- (id)initWithXRes:(int)_xRes yRes:(int)_yRes;
- (void)setXRes:(int)_xRes yRes:(int)_yRes;
- (void)setRawDataReal:(float *)rawReal imag:(float *)rawImag stride:(int)stride;
- (void)ftDataRealBytes:(unsigned char **)real imagBytes:(unsigned char **)imag;
- (void)ftDataMagBytes:(unsigned char **)mag phaseBytes:(unsigned char **)phase;
- (float *)realFTFloatData;
- (float *)imagFTFloatData;
- (int)ftXRes;
- (int)ftYRes;
- (float)scale;
- (void)setScale:(float)_scale;

@end
