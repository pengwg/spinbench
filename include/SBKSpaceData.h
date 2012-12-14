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


@interface SBKSpaceData : NSObject {
	float **traj;
	int length;
	int numAxes;
}

+ (SBKSpaceData *)kSpaceDataWithLength:(int)_length numAxes:(int)_numAxes;

- (float **)trajectory;  // in units of cyc/mm.  Multiply by desired resolution (in mm) to get scaling in (-0.5,0.5)
- (int)length;
- (int)numAxes;

@end
