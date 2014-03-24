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
#import "SBPulsePlugin.h"

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBCartesianReadout
 *   @brief       Pulse Plugin class that provides phase and frequency encoding gradients
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin
 */

@interface SBCartesianReadout : SBPulsePlugin <SBReadoutSharedParameters> {
@private
	double start;
	double fov;
  float kxCoverage; 
  float kyCoverage;
  float samplingRate;
	int xRes, yRes;
	double netArea;
	double readoutStartOffset,readoutEndOffset,endOffset;
  double duration;
  double samples;
	BOOL hasPhaseEncoding;
	BOOL hasRewinder;
    
  IBOutlet id kxPercentFormatter;
  IBOutlet id kyPercentFormatter;
  
  IBOutlet id kxCoverageSlider;
  IBOutlet id kxCoverageTextValue;
  IBOutlet id kyCoverageSlider;
  IBOutlet id kyCoverageTextValue;
}

- (void)setKxCoverage:(float)val;
- (void)setKyCoverage:(float)val;

/**
 *   @brief   Returns the duration of the pulse
 *   @note    Read-only
 *   @returns The pulse duration in ms
 */
-(double)duration;

@end
