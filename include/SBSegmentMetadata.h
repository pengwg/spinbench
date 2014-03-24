/***************************************************************************
 
 Copyright 2009-2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

#import <Cocoa/Cocoa.h>

@class SBReadoutTag;
@class SBPulseTag;
@class SBRFTag;
@class SBTag;
@class SBPropertyListPlugin;


/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBSegmentMetadata
 *   @brief       Metadata associated with a TR segment of a sequence
 *   @author      HeartVista, Inc.
 */

@interface SBSegmentMetadata : NSObject {
	NSMutableArray *pulseTags;
	NSMutableArray *readoutTags;
	NSMutableArray *rfTags;
	NSMutableArray *otherTags;
	NSMutableDictionary *viewIndexDict;
	NSMutableDictionary *numViewsDict;
	NSMutableArray *hardwareInstructions;
}

// ( designated initializer is -(id)init )

+ (SBSegmentMetadata *)metadata;

- (NSMutableArray *)pulseTags;
- (NSMutableArray *)readoutTags;
- (NSMutableArray *)rfTags;
- (void)addReadoutTag:(SBReadoutTag *)tag;
- (void)addPulseTag:(SBPulseTag *)tag;
- (void)addRFTag:(SBRFTag *)tag;
- (void)addTag:(SBTag *)tag;
- (void)addTags:(NSArray *)tags;
- (NSMutableArray *)hardwareInstructions;
- (void)setViewIndex:(int)idx of:(int)total forKey:(NSString *)key object:(SBPropertyListPlugin *)object;
//- (void)setViewIndex:(int)idx of:(int)total forKeyPath:(NSString *)keyPath;
- (NSMutableDictionary *)viewIndexDictForReadoutTag:(SBReadoutTag *)readout;
- (NSMutableDictionary *)numViewsDictForReadoutTag:(SBReadoutTag *)readout;
- (void)addItemsFromMetadata:(SBSegmentMetadata *)other;

@end
