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
#import <SBGenericPlotView.h>

@interface SBGraphView : SBGenericPlotView
{
	BOOL isBarGraph;
	BOOL horizRangeIsSelectable;
	int lastCount;
	float lineWidth;
	NSUserDefaults *userDefaults;
}

- (void)setNumSecondary:(int)ns forGraph:(int)idx;
- (void)setData:(void *)data forGraph:(int)idx xRes:(int)_xres yRes:(int)_yres;
- (void)reloadDataForGraph:(int)idx;
//- (void)setAttributeMask:(unsigned int)mask forGraph:(int)gr;
//- (void)linkMagnitudesFromGraph:(int)nFirst to:(int)nLast;
- (void)setHorizRangeIsSelectable:(BOOL)value;
- (void)setIsBarGraph:(BOOL)value;
- (BOOL)isBarGraph;

@end
