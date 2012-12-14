/***************************************************************************
 
 Copyright 2007-2011 HeartVista Inc.  All rights reserved.
 Contact: HeartVista, Inc. <rthawk-info@heartvista.com>
 
 This file is part of the RTHawk system.
 
 $HEARTVISTA_BEGIN_LICENSE$
 
 THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF HEARTVISTA
 The copyright notice above does not evidence any	
 actual or intended publication of such source code.
 
 $HEARTVISTA_END_LICENSE$
 
 ***************************************************************************/

#import <Cocoa/Cocoa.h>
#import <SBGraphView.h>

@interface SBXYGraphView : SBGraphView
{
}

- (void)setNominalRes:(int)_res numCurves:(int)_nCurves;
- (void)setData:(void *)data forGraph:(int)idx res:(int)_res numCurves:(int)_nCurves;

@end
