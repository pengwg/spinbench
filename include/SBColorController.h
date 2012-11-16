/***************************************************************************
 
 Copyright 2006-2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

#import <Cocoa/Cocoa.h>

@interface SBColorController : NSObject
{
	NSDocumentController *docCon;
	NSUserDefaults *userDefaults;
    IBOutlet id averageSpinWell;
    IBOutlet id backgroundWell;
    IBOutlet id selectedSpinWell;
    IBOutlet id unselectedSpinWell;
    IBOutlet id wallsWell;
    IBOutlet id waveformsWell;
}
- (IBAction)newColor:(id)sender;
- (IBAction)RestoreDefaults:(id)sender;
@end

@interface NSUserDefaults(myColorSupport)
- (void)setColor:(NSColor *)aColor forKey:(NSString *)aKey;
- (NSColor *)colorForKey:(NSString *)aKey;
@end
