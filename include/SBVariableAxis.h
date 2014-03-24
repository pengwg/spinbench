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
#import "SBVariablePlugin.h"

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBVariableAxis
 *   @brief       Variable Plugin that permits most attributes to be used as X or Y axes for standard plots
 *   @details     It typically calculate the values of attributes over a specified range. It can also be used for averaging over that range.
 *   @author      HeartVista, Inc.
 *   @see         SBVariablePlugin
 */

@class SBKey;

enum SBAxisTypeEnum { SBXAxis=0x00, SBYAxis=0x01, SBAveragedAxis=0xFF, SBUnassignedAxis=0xFE };

@interface SBVariableAxis : SBVariablePlugin <SBMultiInputValueVariableProtocol> {
	float range;
	float center;
	int res;
	float selectionStart;
  BOOL initializing;

	SBKey *axisKeyObj;
  NSMutableArray *tickLocs;
	NSMutableArray *tickLabels;
  
@private
	NSDictionary *loadedPlist; // to help with loading keys
	int orderedIndex;
	BOOL selectionStartFollowsSelection;
	BOOL unassignedAxisTypeEnabled;
  IBOutlet NSPopUpButton *axisKeyPopup;
}
-(NSMutableArray *)axisKeyList;

//- (float)range;
//- (float)center;
- (NSInteger)orderedIndex;

- (void)setRange:(float)val;
- (void)setCenter:(float)val;
- (void)setOrderedIndex:(int)val;
//- (void)setAveraging:(BOOL)val;
//- (void)setSelectionStartIndex:(float)idx;

- (enum SBAxisTypeEnum)axisTypeListBox;
- (void)setAxisTypeListBox:(enum SBAxisTypeEnum)val;

- (IBAction)clickDelete:(id)sender;

@end
