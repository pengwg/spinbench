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
@class SBPulsePlugin;

/**
 *    @enum  SBAnchorTypeEnum
 *    @brief Identifies whether the pulse is anchored absolutely to a position within TR, or relative to another pulse's anchor.
 *    @note  Typedef'd to SBAnchorType
 */
typedef enum SBAnchorTypeEnum {
  SBAbsoluteAnchor=0x00,     /**<  Absolute Anchor: Pulse anchor is located at a fixed position within TR */
  SBRelativeAnchor=0x01      /**<  Relative Anchor: Pulse anchor is located relative to an anchor of another pulse */
} SBAnchorType;

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBAnchorController 
 *   @brief       Manages the timing relationships between pulses
 *   @details     Instances of #SBPulsePlugin can request to use anchoring, which provides an automatic mechanism for specific time points within the pulse to be placed within the TR interval.  Placement can be absolute; e.g., by specifying that the starting point of a pulse should happen at time=1.0ms within TR.  Alternatively, placement can be relative; e.g., by specifying that the peak of an RF waveform should correspond with the center of a slice-selection gradient plateau.  By specifying anchors in this way, relative timings within the pulse sequence are maintained as sequence parameters change.
 *   @note        Typically, this class is not instantiated directly.  Use the protected member variable SBPulsePlugin#anchor to access the instance of SBAnchorController associated with a particular #SBPulsePlugin.
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin
 */
@interface SBAnchorController : NSObject {
@private
	SBPulsePlugin *pulse;
	SBAnchorType anchorType;
	float position;
	
	NSString *parentKey;
	
	NSString *anchoredToSBKey;
	NSString *anchoredToAnchor;
	id anchoredToOwner;
	BOOL ownerIsPulsePlugin;
	NSString *activeAnchor;
	
	BOOL observersAreActive;
  BOOL nibIsLoaded;
	
	IBOutlet NSView *selectorView;
	IBOutlet NSPopUpButton *relativeAnchorPopup;
	IBOutlet NSPopUpButton *relativeAnchorToPopup;
	IBOutlet NSPopUpButton *absoluteAnchorPopup;
	IBOutlet NSSlider *positionSlider;
	IBOutlet NSTextField *positionTextBox;
}

/**
 *    @brief     Initializes a new instance
 *    @details   Creates a new #SBAnchorController instance.  Anchor attributes ('anchorType', 'activeAnchor', 'anchoredToSBKey', and 'position') are initialized to values specified in the plist.  If the plist is nil, then these parameters are set to default values (below).  If the plist is not nil, then it must include all four of these keys.
 *
 *   Default values for anchor attributes are:
 *      * anchorType      = SBRelativeAnchor
 *      * activeAnchor    = (the pulse plugin's first anchorKey)
 *      * anchoredToSBKey = 'startOfTR'
 *      * position = 0.0
 *   Note that position is unused for relative anchors and anchoredToSBKey is unused for absolute anchors.
 *    @param     plist An NSDictionary of attribute keys and desired values.  May be nil, in which case defaults are used.
 *    @param     _pulse the #SBPulsePlugin instance associated with this anchor
 *    @param     _parentKey The key name for this SBAnchorController instance within the parent pulse
 *    @result    An #SBAnchorController object with initialized values
 *    @note      Designated initializer.
 */
- (id)initWithPropertyList:(NSDictionary *)plist pulse:(SBPulsePlugin *)_pulse parentKey:(NSString *)_parentKey;

/**
 *    @brief     Returns the anchor attributes
 *    @details   Currently, these attributes are 'anchorType', 'activeAnchor', 'anchoredToSBKey', and 'position'.
 */
- (NSArray *)attributeKeys;

/**
 *    @brief     Serializes the anchor's attributes into a property list dictionary
 *    @details   Adds the attribute keys and their values to the passed dictionary.  If those keys already exist in the dictionary, they are overwritten.
 */
- (void)encodePropertyList:(NSMutableDictionary *)dict;

/**
 *    @brief     Sets the position of the associated pulse based upon the anchor's attribute settings
 *    @details   The pulse is notified by setting its value for the activeAnchor key.  Therefore, the pulse must be key-calue coding compliant for activeAnchor (and all of its anchorKeys).
 */
- (void)setPulsePosition;

/**
 *    @brief     Connects attributeKey and notification observers
 *    @details   AttributeKeys are connected here for the auto-update mechanism, as well as any observers for specific SpinBench notifications.
 */
- (void)addObservers;

/**
 *    @brief     Disonnects attributeKey and notification observers
 *    @details   AttributeKeys are disconnected from the auto-update mechanism, as well as any observers for specific SpinBench notifications.
 */
- (void)removeObservers;

/**
 *    @brief     Loads the associated nib resource for this anchor
 *    @details   SBAnchorController instances that require a user interface will call loadNib to set up the anchoring interface (including #mainView) and connect interface-related observers.
 */
- (BOOL)loadNib;

/**
 *    @brief     Unloads the associated nib resource for this anchor
 *    @details   destroyNib may be called on any anchor controller, whether it has a loaded nib or not.  Releases the #mainView and other nib resources and removes the associated bindings.
 */
- (void)destroyNib;

/**
 *    @brief     Returns the anchoring view for this anchorController
 *    @details   This is only defined if the nib is loaded.  Before loadNib, this value is nil.  After a destroyNib, the view is invalid but not necessarily nil.
 */
- (NSView *)mainView;

/**
 *    @brief     Updates pulse dependencies and observers based upon the anchor's attribute settings
 *    @details   If the anchor is relative, then the anchor creates a dependency between the parent pulse and another pulse.  Calling this function ensures that those dependencies are maintained, so that the anchored pulse is notified when the anchoring pulse moves.
 */
- (void)updateDependencies;

/**
 *    @brief     Returns the SBPath of this anchor controller
 *    @details   This path provides a unique identifier for this anchor controller's attribute keys.
 */
- (NSString *)mySBPath;

/**
 *    @brief     Prints the current state of the anchor to the message log
 *    @details   Provided information includes values of all attributeKeys and hex identifiers for self and pulse.
 */
- (void)printState;

/**
 *    @brief     Returns the current value of the position attribute
 *    @details   Position is specified in ms from the beginning of the TR interval.
 */
- (NSNumber *)position;

/**
 *    @brief     Sets the position attribute and updates the position of the associated pulse appropriately
 *    @details   Position is specified in ms from the beginning of the TR interval.  This setter sets the position absolutely regardless of the current value for anchorType.
 */
- (void)setPosition:(NSNumber *)val;

/**
 *    @brief     Returns the current value of the activeAnchor attribute
 *    @details   The activeAnchor specifies the anchorKey within the parent pulse which is to be used for anchoring.
 */
- (NSString *)activeAnchor;

/**
 *    @brief     Sets the activeAnchor attribute and updates the position of the associated pulse appropriately
 *    @details   The activeAnchor specifies the anchorKey within the parent pulse which is to be used for anchoring.
 */
- (void)setActiveAnchor:(NSString *)val;

/**
 *    @brief     Returns the current value of the anchoredToSBKey attribute
 *    @details   For relative anchoring, this attribute specifies an anchorKey in another pulse (or global anchorKey) to which this pulse is anchored.
 */
- (NSString *)anchoredToSBKey;

/**
 *    @brief     Sets the anchoredToSBKey attribute and updates the position of the associated pulse appropriately
 *    @details   For relative anchoring, this attribute specifies an anchorKey in another pulse (or global anchorKey) to which this pulse is anchored.
 */
- (void)setAnchoredToSBKey:(NSString *)val;

/**
 *    @brief     Returns the current value of the anchorType attribute
 *    @details   AnchorType dentifies whether the pulse is anchored absolutely to a position within TR, or relative to another pulse's anchor.
 */
- (SBAnchorType)anchorType;

/**
 *    @brief     Sets the anchorType attribute and updates the position of the associated pulse appropriately
 *    @details   AnchorType dentifies whether the pulse is anchored absolutely to a position within TR, or relative to another pulse's anchor.
 */
- (void)setAnchorType:(SBAnchorType)val;

@end
