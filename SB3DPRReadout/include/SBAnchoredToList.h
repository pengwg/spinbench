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
@class SBPropertyListPlugin;

/**
 *   @class       SBAnchoredToList 
 *   @brief       Manages a list of anchors and a currently selected anchor
 *   @details     Provides a mechanism for choosing anchors from a list for plugin use.  The currently selected anchor from the list is maintained here, and a supplied NSPopUpButton can be bound to the proper observers.  See #SBReadoutInterval and #SBRFFrequency for examples of usage.
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin
 *   @see         SBSequencePlugin
 */
@interface SBAnchoredToList : NSObject {
@private
	SBPropertyListPlugin *parent;
	BOOL enabled;
	BOOL boxBindingsActive;
	BOOL observersAreActive;
  
	NSString *posKey;
	NSString *selectedAnchor;
	NSString *selectedSBKeyKey;
	id selectedOwner;
	
	NSPopUpButton *popUpButton;
}

/**
 *    @brief     Initializes a new instance
 *    @details   Creates a new #SBAnchoredToList instance.  The parent plugin must be key-value coding compliant for a floating-point positionKey and an NSString anchorKey.  AnchorKey contains the SpinBench key path for an anchor provided by a #SBPulsePlugin; positionKey provides storage for the current position represented by that key path.  This class makes sure those two values stay in sync.
 *    @param     positionKey An NSString representing a floating-point key in the parent plugin
 *    @param     anchorKey An NSString representing an NSString key in the parent plugin
 *    @param     plugin The parent #SBPropertyListPlugin instance implementing these keys
 *    @result    An #SBAnchoredToList object with initialized values
 *    @note      Designated initializer.
 */
- (id)initWithPositionKey:(NSString *)positionKey anchorKey:(NSString *)anchorKey parent:(SBPropertyListPlugin *)plugin;

/**
 *    @brief     The anchorKey name of the selected anchor within the selectedOwner
 *    @details   The currently selected key is: <[selectedOwner editableName]>.selectedAnchor, unless the owner is the top-level #SBParamsObject.
 */
- (NSString *)selectedAnchor;

/**
 *    @brief     The instance owning the currently selected key.
 *    @details   The currently selected key is: <[selectedOwner editableName]>.selectedAnchor, unless the owner is the top-level #SBParamsObject.
 */
- (id)selectedOwner;

/**
 *    @brief     Updates the positionKey within the parent
 *    @details   The key is updated based upon the currently selected anchor.
 */
- (void)updatePosition;

/**
 *    @brief     Sets the currently selected key based upon the provided SBPath
 *    @details   This updates selectedAnchor and selectedOwner appropriately.
 */
- (void)setSelectedKey:(NSString *)selectedAnchorKey;

/**
 *    @brief     Connects observers and bindings to the provided NSPopUpButton
 */
- (BOOL)addObserversWithPopUpButton:(NSPopUpButton *)box;

/**
 *    @brief     Disonnects observers and bindings
 */
- (void)removeObservers;

/**
 *    @brief     Updates plugin dependencies based upon the anchor's attribute settings
 *    @details   If the selectedOwner is a plugin, then this method creates a dependency between the parent pulse and that plugin.  This method is called automatically when #setSelectedKey: is called, but may also be called manually.
 */
- (void)updateDependencies;

@end
