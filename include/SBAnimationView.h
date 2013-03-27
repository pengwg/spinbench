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
#import "AppKit/NSStringDrawing.h"

enum SBAnimationStyleEnum { SBAnimateDots = 0x00, SBAnimateArrows = 0x01, SBAnimateConnectingLine = 0x02};

@interface SBAnimationView : NSView {

	float width,height,diag;
	float *Avg;
	float edgepad;
	NSRect xyRect, xzRect, yzRect;
  NSRect auxRect;
	enum SBAnimationStyleEnum animationStyle;
	
	unsigned int selectedSpin;
	NSUserDefaults *userDefaults;
	NSBezierPath *axes;
	NSRect lastFrame;
	id _delegate;
	int xres,yres;
	float scaleFac;
	NSMutableArray *xySpins, *xzSpins, *yzSpins;
	float **Locs;
	NSAffineTransform *xyCenter, *xzCenter, *yzCenter;
	
	NSPoint xyXLabelPos,xyYLabelPos;
	NSPoint xzXLabelPos,xzZLabelPos;
	NSPoint yzYLabelPos,yzZLabelPos;
	NSBezierPath *xyAvg, *xzAvg, *yzAvg;
	NSBezierPath *xyBoundary, *xzBoundary, *yzBoundary;
	float shRight;
	float magnify;
	NSString *xLabel;
	NSString *yLabel;
	NSString *zLabel;
	NSMutableDictionary *labelAttrs;
	NSBezierPath *dot;
	NSSize labelSize;
	NSColor *spinColor;
	NSColor *unselectedColor;
	NSColor *avgColor;
	NSMutableArray *pasteBoardTypes;
  NSString *auxText;
}
- (enum SBAnimationStyleEnum)animationStyle;
- (void)setAnimationStyle:(enum SBAnimationStyleEnum)style;

- (void)setLocs:(float **)_locs xRes:(int)_xres yRes:(int)_yres;
- (void)setAuxText:(NSString *)text;
- (void)setAvg:(float *)avg;
- (void)setMagnify:(float)mag;
- (void)setSelectedSpin:(int)sp;
- (void)setXyBoundary:(NSBezierPath *)xyb xzBoundary:(NSBezierPath *)xzb  yzBoundary:(NSBezierPath *)yzb;

- (id)delegate;
- (void)setDelegate:(id)new_delegate;

@end

@interface NSObject (SBAnimationViewDelegate)

- (void)ClickGraph:(id)sender xIndex:(int)xIdx yIndex:(int)yIdx event:(NSEvent *)theEvent;

@end

