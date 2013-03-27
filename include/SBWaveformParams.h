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

@class SBSequencePlugin;
@class SBWaveformSegment;
@class SBKSpaceData;
@class SBSegmentMetadata;
@class SBReadoutTag;
@class SBRotation;

@interface SBWaveformParams : NSObject {
@private
	SBSequencePlugin *parent;

	int *samples;
	int totalSamples;

	int rfTxChannels;
	int gradAxes;
	int otherAxes;
	int allAxes;
	
	int numTr;
	int parentOutputNum;
  
  SBRotation *gradRotation;
	
	BOOL updatedSinceLastUsed;
}

- (id)initWithSamples:(int *)_samples numTr:(int)_numTr rfTxChannels:(int)_rfTxChannels gradAxes:(int)_gradAxes otherAxes:(int)_otherAxes parent:(SBSequencePlugin *)_parent output:(int)_parentOutputNum;

- (void)setSamples:(int *)_samples numTr:(int)_numTr rfTxChannels:(int)_rfTxChannels gradAxes:(int)_gradAxes otherAxes:(int)_otherAxes;
- (void)setEqualTo:(SBWaveformParams *)paramsToCopy;
- (id)initEqualTo:(SBWaveformParams *)paramsToCopy withParent:(SBSequencePlugin *)_parent output:(int)_parentOutputNum;

- (SBWaveformSegment *)waveformSegmentForTrNum:(int)trNum;
- (SBSegmentMetadata *)metadataForTrNum:(int)trNum;
- (SBKSpaceData *)kSpaceWithMoment:(int)moment tag:(SBReadoutTag *)readout trNum:(int)trNum;

- (int *)samples;
- (SBRotation *)gradRotation;
- (int)totalSamples;
- (int)startOfTrNum:(int)trNum;
- (int)samplesForTrNum:(int)trNum;
- (int)samplesForLongestTr;
- (int)trNumForSample:(int)sample;

- (int)rfTxChannels;
- (int)gradAxes;
- (int)otherAxes;
- (int)allAxes;

- (int)numTr;
- (void)printState;

// 'private'; shouldn't be needed by external plugins
- (BOOL)updatedSinceLastUsed;
- (void)setUpdatedSinceLastUsed:(BOOL)val;

@end
