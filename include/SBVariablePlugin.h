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
#import "SBPropertyListPlugin.h"

@class SBKey;


/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBVariablePlugin
 *   @brief       Abstract superclass for all Variable Plugins
 *   @author      HeartVista, Inc.
 */

@interface SBVariablePlugin : SBPropertyListPlugin {

}

- (float)repeatInterval;

@end

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @protocol    SBMultiInputValueVariableProtocol
 *   @brief       Formal protocol for independent variables used in simulations
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin
 */

@protocol SBMultiInputValueVariableProtocol

- (SBKey *)axisSBKey;

// primaryKey is used only for calculation speed; it can be set to nil.  If it is set, then the primary key is:
//-- the first thing set in setSelectedIndex
//-- set for every call to setSelectedIndex
//-- set absolute-ly every time (x=5, not x=x+3).
- (SBKey *)primaryKey;
- (BOOL)isTimeAxis; // set this to return YES if it's a variableaxis that only modifies time variables (timewithintr, absolutetime, trnum)

- (float)min;
- (float)max;
- (int)res;
- (BOOL)averaging;

- (void)setMin:(float)val;
- (void)setMax:(float)val;
- (void)setRes:(int)val;

- (NSInteger)orderedIndex;
- (void)setOrderedIndex:(int)val;

	// this is called whenever the axis' parameter value was changed by an external source.  Override this to do your 
	//  own thing when that happens, but make sure to include a call to [super paramValueDidChange] if implemented in 
	//  a subclass of sbvariableaxis
- (void)paramValueDidChange;

// setSelectedIndex: sets the variable to its value at index idx
- (void)setSelectedIndex:(float)idx;
- (int)selectedIndex;
- (float)unboundedFloatSelectedIndex;

- (float)paramValueForIndex:(float)idx;

- (NSArray *)tickLocs;
- (NSArray *)tickLabels;

// for selecting a range of values, this keeps track of the starting location
- (float)selectionStart;
- (int)selectionStartIndex;
- (float)unboundedFloatSelectionStartIndex;
- (BOOL)selectionStartFollowsSelection;
- (void)setSelectionStartFollowsSelection:(BOOL)val;

@end



/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @protocol    SBOutputProcessingProtocol
 *   @brief       Formal protocol for in-place post-processing of the output in simulations 
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin, SBGaussianNoise
 */

@protocol SBOutputProcessingProtocol

//processOutput:length: not often used, but this is a chance for in-place postprocessing of the output (Mx,My,Mz) float
// array to do things not modifiable at the input.  Most plugins don't implement this.  Processing occurs after averaging, if any.
// This function must be thread-safe.  The input array is of length 3*length, with elements in order:
// {Mx_1, My_1, Mz_1, Mx_2, My_2, Mz_2, ... Mx_length, My_length, Mz_length}.
- (void)processOutput:(float *)magnetization length:(int)len;

@end
