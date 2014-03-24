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
#import "SBTag.h"
#import "SBConstants.h"

@class SBPropertyListPlugin;



/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBReadoutTag
 *   @brief       Describes a readout window
 *   @author      HeartVista, Inc.
 *   @see         SBTag
 */
@interface SBReadoutTag : SBTag {

@private
	NSString *pluginName;
	float readoutStart,readoutEnd;
	float readoutReferencePoint;
	float frequencyShift;  // in Hz; expressed as an offset from the center frequency.
	float phase;   // in degrees; try to keep it between 0 and 360
//	long view, slice;
	float resolution[MAX_IMAGE_DIMENSIONS]; // resolution along each image dimension (mm)
	float fov[MAX_IMAGE_DIMENSIONS]; //fov along each image dimension (cm)
	float** kSpace;
	int kSpaceLength;
  float samplingRate;
	BOOL flipped;
  BOOL discard;
	
	NSMutableDictionary *internalViewIndexDict;
	NSMutableDictionary *internalViewTotalDict;
}

-(NSString *)pluginName;
-(float)readoutStart;
-(float)readoutEnd;
-(float)readoutReferencePoint;
-(float)frequencyShift;
-(float)phase;
-(BOOL)isFlipped;
-(BOOL)discard;
//-(long)view;
//-(long)slice;

-(float *)resolution;
-(float *)fov;
-(float **)allocateKSpaceWithLength:(int)val;
-(float**)kSpace;
-(int)kSpaceLength;
-(float*)kSpaceDensity;
-(float)samplingRate;

-(void)setPluginName:(NSString *)name;
-(void)setReadoutStart:(float)val;
-(void)setReadoutEnd:(float)val;
-(void)setReadoutReferencePoint:(float)val;
-(void)setFrequencyShift:(float)val;
-(void)setPhase:(float)val;
-(void)setSamplingRate:(float)val;
-(void)setFlipped:(BOOL)val;
-(void)setDiscard:(BOOL)val;
//-(void)setView:(long)val;
//-(void)setSlice:(long)val;

-(void)setResolution:(float *)_res;
-(void)setFov:(float *)_fov;

- (void)setViewIndex:(int)idx of:(int)total forKey:(NSString *)key object:(SBPropertyListPlugin *)object;
//- (void)setViewIndex:(int)idx of:(int)total forKey:(NSString *)key; // DEPRECATED
- (int)viewIndexForKey:(NSString *)key;
- (int)numViewsForKey:(NSString *)key;

// PRIVATE: do not access these methods as their implementation may change
-(NSMutableDictionary *)internalViewIndexDict;
-(NSMutableDictionary *)internalViewTotalDict;

@end
