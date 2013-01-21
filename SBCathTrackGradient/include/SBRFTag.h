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

@interface SBRFTag : SBTag {
@private
	float peak;
	float start,end;
}

- (float)start;
- (float)end;
- (float)peak;

- (void)setStart:(float)val;
- (void)setEnd:(float)val;
- (void)setPeak:(float)val;

@end
