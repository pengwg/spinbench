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
#import "SBPulsePlugin.h"

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBPulseTag
 *   @brief       Describes a pulse
 *   @author      HeartVista, Inc.
 *   @see         SBTag
 */


@interface SBPulseTag : SBTag {
@private
	float start,end;
	enum SBPulseTypeEnum pulseType;
}

- (float)start;
- (float)end;
- (enum SBPulseTypeEnum)pulseType;

- (void)setStart:(float)val;
- (void)setEnd:(float)val;
- (void)setPulseType:(enum SBPulseTypeEnum)val;

@end
