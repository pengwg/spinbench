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
@class SBOutputPlugin;

typedef enum { SBOutputPluginNoBackground=0x00, SBOutputPluginUnselectedBackground=0x01, SBOutputPluginSelectedBackground=0x02
} SBOutputPluginBackgroundStyle;

@interface SBOutputPluginContainer : NSBox {
	NSUserDefaults *userDefaults;
	NSArray *pasteboardTypes;
	BOOL cornerControlsAreVisible;
	NSRect botRigRect,botLefRect,topRigRect,topLefRect;
	NSSize minSize;
	SBOutputPlugin *plugin;
}

- (id)initWithFrame:(NSRect)frameRect plugin:(SBOutputPlugin *)_plugin;
- (void)setBackground:(SBOutputPluginBackgroundStyle) bkgSty;
- (void)setCornerControlsAreVisible:(BOOL)flag;
- (BOOL)cornerControlsAreVisible;

@end
