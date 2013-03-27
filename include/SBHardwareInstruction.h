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

typedef enum {	SBHardwareOpcodeJump=				0x0001,
				SBHardwareOpcodePauseUntilTrigger=	0x0002,
				SBHardwareOpcodeTriggerMask=		0x8000,
				SBHardwareOpcodeJumpOnTrigger=		0x8001
			  } SBHardwareOpcode;

@interface SBHardwareInstruction : NSObject {
	SBHardwareOpcode opcode;
	int value;
}

// convenience method returns autoreleased object
+ (id)instructionWithOpcode:(SBHardwareOpcode)_opcode value:(int)_value;

// designated initializer
- (id)initWithOpcode:(SBHardwareOpcode)_opcode value:(int)_value;

- (SBHardwareOpcode)opcode;
- (int)value;
- (NSString *)opcodeString;

@end
