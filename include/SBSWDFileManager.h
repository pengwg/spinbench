/***************************************************************************
 
 Copyright 2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

#import <Foundation/Foundation.h>

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBSWDFileManager
 *   @brief       SpinBench Waveform Data File manager
 *   @author      HeartVista, Inc.
 */


@interface SBSWDFileManager : NSObject {
  @private
  id waveObject;
}

// waveObject can be either a SBWaveform or a SBPulsePlugin; wave data or tr-independent pulse data is appropriately saved for each case
- (id)initForObject:(id)_waveObject;
+ (id)managerForObject:(id)_waveObject;

- (NSData *)readFromFile:(NSURL *)fileName;
- (NSData *)rawData;
- (BOOL)writeToFile:(NSURL *)fileName;
- (BOOL)isApproximatelyEqualToFile:(NSURL *)fileName;

@end
