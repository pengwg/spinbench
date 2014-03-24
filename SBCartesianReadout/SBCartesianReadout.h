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

@interface SBCartesianReadout : SBPulsePlugin <SBReadoutSharedParameters> {
	
	double start;
	double fov;
  float kxCoverage; 
  float kyCoverage;
	int xRes, yRes;
	double netArea;
	double readoutStartOffset,readoutEndOffset,endOffset;
	BOOL hasPhaseEncoding;
	BOOL hasRewinder;
    
  IBOutlet id kxPercentFormatter;
  IBOutlet id kyPercentFormatter;
  
  IBOutlet id kxCoverageSlider;
  IBOutlet id kxCoverageTextValue;
  IBOutlet id kyCoverageSlider;
  IBOutlet id kyCoverageTextValue;
}

- (void)setXRes:(float)val;
- (void)setYRes:(float)val;

@end
