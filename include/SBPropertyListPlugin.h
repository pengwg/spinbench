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

#ifdef __cplusplus
extern "C" {
#endif
#import <Cocoa/Cocoa.h>
#ifdef __cplusplus
}
#endif

@class SBParamsObject;

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @class       SBPropertyListPlugin 
 *   @brief       Abstract superclass for SpinBench Pulse, Sequence, and Variable plugins. 
 *   @details     All plugins of type #SBPulsePlugin, #SBSequencePlugin, or #SBVariablePlugin descend from SBPropertyListPlugin.  Many of the methods of this class should be reimplemented in subclasses.
 *   @note        This class should not be instantiated directly.  Use subclasses (#SBSincRF, #SBRepeatPlugin, #SBVariableAxis, ...) instead.
 *   @author      HeartVista, Inc.
 *   @see         SBPulsePlugin
 *   @see         SBSequencePlugin
 *   @see         SBVariablePlugin
 */
@interface SBPropertyListPlugin : NSObject {
@protected
	SBParamsObject *params; /**< @brief Convenience pointer to the parent #SBParamsObject instance */
	NSNotificationCenter *nc; /**< @brief Convenience pointer to the shared NSNotificationCenter instance */
	IBOutlet NSView *inspectorPane; /**< @brief Holds the NSView instance of the inspector, if any.  Usually defined in the nib file and connected to this outlet */
	BOOL nibIsLoaded; /**< @brief Flag indicating that the nib file is loaded */
	BOOL observersAreActive; /**< @brief Flag indicating that the observers are connected */
	BOOL autoCalculateOnAttributeChange; /**< @brief flag indicating whether the plugin automatically recalcuates when an attributeKey changes */

@private
	BOOL needsUpdate;
	NSString *editableName;
  NSString *mySBPath;
	BOOL deletable;
  BOOL paramsEnabled;

	NSMutableArray *dependents;
	NSMutableArray *dependsOn;
}

/**
 *    @brief     Initializes a new instance
 *    @details   Creates a new plugin instance and performs any necessary initializations.  Subclasses should reimplement this and include a call to super.  Reimplementations should check fpr plist==nil and if true, set attributeKeys to default values.  The returned instance is owned by the caller.
 *    @param     plist An NSDictionary of values for attributeKeys.  May be nil, in which case default values are set
 *    @param     _params The parent #SBParamsObject associated with this instance
 *    @result    An #SBPropertyListPlugin object containing initialized attributeKeys
 *    @note      Designated initializer.
 */
- (id)initWithPropertyList:(NSDictionary *)plist params:(SBParamsObject *)_params;

/**
 *    @brief     Returns a copy of the instance
 *    @details   Copies the instance and its attributeKey values, and returns the copy.  A different parent SBParamsObject may be supplied as a parameter.  The returned instance is owned by the caller.
 *    @param     _params The parent #SBParamsObject associated with the new instance
 */
- (id)copyWithParams:(SBParamsObject *)_params;

/**
 *    @brief     Calback function invoked upon plugin intialization after other plugins are available
 *    @details   During file loading, some plugins may require that all other plugins be available before certain initialization steps may be completed.  In these cases, move this initialization code from the constructor (#initWithPropertyList:params:) to finalSetup.  finalSetup is called during initialization, but after all plugins have been constructed.  This is also called whenever a new plugin is created.
 */
- (void)finalSetup;

/**
 *    @brief     Loads the associated nib resource for this plugin
 *    @details   SBPropertyListPlugin instances that require a user interface will call loadNib to set their interface outlets (including #inspectorPane) and connect interface-related observers.
 */
- (BOOL)loadNib;

/**
 *    @brief     Unloads the associated nib resource for this plugin
 *    @details   destroyNib may be called by any plugin, whether it has a loaded nib or not.  Test whether the nib is loaded using the #nibIsLoaded member variable.  The default implementation releases the #inspectorPane and should be called in any reimplementations.
 */
- (void)destroyNib;

/**
 *    @brief     Connects attributeKey observers and observers for any other notifications
 *    @details   May be reimplemented in subclasses with a call to super.  AttributeKeys are connected here for the auto-update mechanism, as well as any plugin-specific observers.  This method is automatically called during plugin intialization, but may be disabled and re-enabled through direct calls to this and #removeObservers.
 */
- (void)addObservers;

/**
 *    @brief     Disonnects attributeKey observers and observers for any other notifications
 *    @details   May be reimplemented in subclasses with a call to super.  AttributeKeys are disconnected from the auto-update mechanism, as well as any plugin-specific observers.  This method should be manually called before deallocating the plugin instance.
 */
- (void)removeObservers;

/**
 *    @brief     Serializes the plugin's contents into an existing NSMutableDictionary.
 *    @details   All attributeKeys are added to the dictionary.  Existing values with the same key name are overwritten.
 */
- (void)encodePropertyList:(NSMutableDictionary *)plist;

/**
 *    @brief     Returns an NSArray of NSString keys representing the plugin's state.
 *    @details   This method provides a complete list of state variables required to specify the plugin's state.  Subclasses should reimplement attributeKeys, including keys returned from a call to super.  Subclasses should be key-value coding compliant for all attributeKeys.  These keys are used in file loading and saving, and determining whether #calculateOutput should be called upon state changes.
 */
- (NSArray *)attributeKeys;

/**
 *    @brief     Returns an NSArray of NSString keys representing derived informtation.
 *    @details   Derived keys are not used as primary sources for creating the output of the plugin, and are not saved to or read from files.  The values of these keys can be completely deduced given the current state of the attributeKeys.  Nevertheless, these keys provide useful derived information that can be read by other plugins.
 */
- (NSArray *)derivedAttributeKeys;

/**
 *    @brief     Detailed description of the plugin
 *    @details   Subclasses should override this method to provide details about the function and proper use of the plugin.
 */
+ (NSString *)pluginDescription;

/**
 *    @brief     Returns the plugin's name
 *    @details   Subclasses should override this method to provide a concise name for the plugin.  This name will be used as the default editableName for the plugin and will be used to describe the plugin in pull-down menus.
 */
+ (NSString *)humanReadablePluginName;

/**
 *    @brief     Returns a suggested URL where the plugin may be downloaded
 *    @details   Subclasses should override this method to provide users with informatiom about where the plugin may be acquired.  When a user loads a .spv file that uses an unavailable plugin, the user will be provided with this information as a suggestion for where the plugin may be obtained.  The default implementation of this function returns nil, meaning that url assistance is not provided.
 */
- (NSString *)urlToGetPlugin;

/**
 *    @brief     Returns the name of the plugin's nib file
 *    @details   The default implementation of this method returns the NSMainNibFile key from the plugin's info.plist file.  If a different value is desired, theis method may be overridden.
 */
- (NSString *)nibName;

/**
 *    @brief     Returns the plugin's integer version number
 *    @details   The default implementation of this method returns 1.  When files are loaded, the version of loaded plugins is compared with the available plugin version.  The user is notified of any out-of-date plugins.  Reimplement this function in subclasses to indicate a major version change.
 */
- (int)pluginVersion;

/**
 *    @brief     Returns an NSString identifying the units of a given parameter key
 *    @details   Reimplement this function to provide units strings for all attribute keys that are to be usable as variable axes.  If the plugin does not implement the specified key, or if the specified key is not usable as a variable axis, then this function should return nil.
 */
- (NSString *)unitsOf:(NSString *)paramName;

/**
 *    @brief     Returns a flag indicating whether the given key represents an angle
 *    @details   Reimplement this function to indicate if a particular attribute key represents an angle.  The default implementation returns NO.
 */
- (BOOL)isAngle:(NSString *)paramName;

/**
 *    @brief     Returns the parent #SBParamsObject associated with this plugin
 */
- (SBParamsObject *)params;

/**
 *    @brief     Returns the global attribute keys observed by this plugin
 *    @details   Reimplement this function to return the global parameters used by the subclass during its calculation.  For example, a gradient pulse might need to re-calculate whenever 'slewRateLimit' changes, so it would add 'slewRateLimit' to the array of attribute keys.  Reimplementations should include the array obtained by calling [super observedAttributeKeys] to ensure dependencies in the parent are also respected.
 *    @note      For #SBPulsePlugin (only): it is not necessary to specify waveSamplingRate or tr; all pulses are automatically recalculated when these change.
 */
- (NSArray *)observedAttributeKeys;

/**
 *    @brief     Returns an NSBox with gradient-transform component text boxes
 *    @details   This is used, for example, by SBEddyGradient to provide an interface for changing per-gradient parameters.
 */
- (NSBox *)gradTransformBoxWithRows:(int)rows keys:(NSArray *)keys labels:(BOOL)labels overlaps:(NSArray *)overlaps expandable:(BOOL)expandable;

/**
 *    @brief     Returns the inspector pane associated with this plugin
 *    @details   The inspector pane is specified in the plugin's nib file, and the nib must explicitly connect its inspector pane view to the inspectorPane outlet of the file's owner.  This is a convenience accessor for that outlet.
 */
- (NSView *)inspectorPane;

/**
 *    @brief     Returns the user-editable unique name of the instance
 *    @details   This is the instance name that is shown in the selector pane and is used in SBKeys to refer to a plugin instance.  All plugins associated with a specific document must be uniquely named.
 */
- (NSString *)editableName;

/**
 *    @brief     Sets the user-editable unique name of the instance
 *    @details   This is the instance name that is shown in the selector pane and is used in SBKeys to refer to a plugin instance.  All plugins associated with a specific document must be uniquely named.  To ensure a unique name, users should call #validateEditableName:error: to obtain a unique editableName before calling setEditableName.  Upon completion, this method posts SBPluginEditableNameDidChangeNotification with the parent #SBParamsObject as the sender object.
 */
- (void)setEditableName:(NSString *)name;

/**
 *    @brief     Validates a candidate user-editable unique name for the instance
 *    @details   This validates a candidate NSString to be used as an editableNamefor this instance.  If another plugin shares the editableName sent in valObj, this function modifies valObj (by adding an incrementing number at the end) to distinguish it from the other plugin.  EditableNames must be unique because the SBKey mechanism uses the editableName to identify objects.  The error parameter is not currently used and may be nil.
 */
- (BOOL)validateEditableName:(NSString **)valObj error:(NSError **)outError;

/**
 *    @brief     Returns a short status identifier for the plugin
 *    @details   This provides a 4-character-or-less indicator of the status or type of the plugin.  Selector panes use this information to provide the second column of information in the plugin list.  For example, #SBHardRF sets this value to "RF", while #SBAreaTrapezoid sets this value to "Gz" by default.  Override this function in subclasses to provide different information.
 */
- (NSString *)status;

/**
 *    @brief     Performs an undo operation using a dictionary
 *    @details   The dictionary is assumed to contain a set of this plugin's attributeKeys and associated values.  The plugin's state is set to the values specified in the dictionary.  An exception will occur if keys are provided which are not attributeKeys.
 */
- (void)undoWithDictionary:(NSDictionary *)dict;

/**
 *    @brief     Calculates the intended output of the plugin
 *    @details   Subclasses should reimplement this method to provide the intended output of the plugin.  The operation of this method is different for each plugin class.  This method is called whenever an attributeKey is changed within this instance, or whenever an observedAttributeKey is changed external to this instance.  For #SBPulsePlugin, this method calculates SBPulsePlugin#pulseData.  For #SBSequencePlugin, this method calculates the #SBWaveformParams data and caches any sequencing information needed.  For #SBVariablePlugin, this method calculates the intended result.  Subclasses should call [super calculateOutput] at the end of processing so that dependent plugins can be notified of changes.
 */
- (void)calculateOutput;

/**
 *    @brief     Flag indicating whether #calculateOutput is automatically called when attributeKeys change
 *    @details   This flag is normally YES, but it may be disabled by the user if desired.  For example, if an attributeKey is modified during #calculateOutput itself, an infinite loop may result unless autoCalculateOnAttributeChange is disabled first.
 */
- (BOOL)autoCalculateOnAttributeChange;

/**
 *    @brief     Flag indicating whether #calculateOutput is automatically called when attributeKeys change
 *    @details   This flag is normally YES, but it may be disabled by the user if desired.  For example, if an attributeKey is modified during #calculateOutput itself, an infinite loop may result unless autoCalculateOnAttributeChange is disabled first.
 *    @note      If the value of this key is changed, then it should also be set back to its prior value.  Failure to restore this value to its initial value would result in an unresponsive user interface.
 */
- (void)setAutoCalculateOnAttributeChange:(BOOL)val;

/**
 *    @brief     Flag indicating whether a call to #calculateOutput is enqueued
 *    @details   This flag is normally not used by plugin programmers.  The core system enqueues updates in certain instances for performance, and this flag co-ordinates which plugins require processing.
 */
-(BOOL)needsUpdate;

/**
 *    @brief     Sets the flag indicating whether a call to #calculateOutput is enqueued
 *    @details   This flag is normally not used by plugin programmers.  The core system enqueues updates in certain instances for performance, and this flag co-ordinates which plugins require processing.
 */
-(void)setNeedsUpdate:(BOOL)val;

/**
 *    @brief     Returns the SBPath of this plugin.
 *    @details   The SBPath of a plugin is currently created by enclosing its editableName in brackets.  Rather than manually creating that structure, plugin users should call this function in case the structure of an SBPath changes in the future.  SBKeys are constructed as SBPath.key.
 *    @see       SBParamsObject#SBKeyForKey:withOwner:
 */
- (NSString *)mySBPath;

/**
 *    @brief     Flag indicating whether the plugin is deletable
 *    @details   Under certain circumstances, plugins may declare themselves un-deletable.  This should be used rarely, but for example, the interface requires that at least one #SBVariableAxis is available at all times.  This mechanism facilitates that.
 */
- (BOOL)deletable;

/**
 *    @brief     Sets the flag indicating whether the plugin is deletable
 *    @details   Under certain circumstances, plugins may declare themselves un-deletable.  This should be used rarely, but for example, the interface requires that at least one #SBVariableAxis is available at all times.  This mechanism facilitates that.
 */
- (void)setDeletable:(BOOL)val;

/**
 *   @brief     Flag for enabled state
 *   @details   The master SBParamsObject becomes disabled when certain long-running processes are active and any parameter changes may disrupt the process.  For example, rendering operations use paramsEnabled to prevent changes to the simulation during rendering.  Plugins should check paramsEnabled before allowing any gui changes to parameters.  Often this can be accomplished by binding ui elements' enabled property to the paramsEnabled key of their owning plugin class.
 */
- (BOOL)paramsEnabled;

/**
 *    @brief     Prints the current state of the plugin to the message log
 *    @details   Provided information includes dependencies on other plugins, values of all attributeKeys, and hex identifiers for self and params.
 */
- (void)printState;

/**
 *    @brief     Adds an explicit dependency to another plugin instance
 *    @details   Dependencies indicate that this plugin gets some information from another plugin and should update itself (using #updateDependentValues) whenever that plugin updates.  Dependencies are automatically removed when pointed-to objects are deleted, so implementations must be robust to lost dependencies.
 */
- (BOOL)addSBDependency:(SBPropertyListPlugin *)obj;

/**
 *    @brief     Removes a dependency to another plugin instance
 *    @details   If the dependency is successfully removed, returns YES.  If the dependency did not exist, returns NO.
 */
- (BOOL)removeSBDependency:(SBPropertyListPlugin *)obj;

/**
 *    @brief     Clears all dependencies on other plugins
 *    @details   Returns YES upon success.
 */
- (BOOL)removeAllDependencies;

/**
 *    @brief     Returns an array of #SBPropertyListPlugin instances that are dependent on this instance.
 *    @details   Dependencies indicate that this plugin gets some information from another plugin and should update itself (using #updateDependentValues) whenever that plugin updates.  Dependencies are automatically removed when pointed-to objects are deleted, so implementations must be robust to lost dependencies.
 */
- (NSArray *)allDependentPlugins;

// override in sublcasses if needed; ony for when you need to explicitly set up dependencies b/t objects in your implementation

/**
 *    @brief     Callback function called when dependencies may need updating.
 *    @details   Override this method to set up dependencies within plugins.  This may be called upon initialization or whenever dependencies may have changed.  If the plugin adds no explicit dependencies, the default implementation is sufficient.  If overridden, make sure to call [super updateDependencies] to ensure all dependencies are maintained.
 */
- (void)updateDependencies;

/**
 *    @brief     Callback function called when a dependent plugin has changed.
 *    @details   Override this method to perform processing related to a changed plugin that is a dependency.  For example, this function may do processing to determine whether the changed value in the dependency would affect this plugin.  If so, then [self setNeedsUpdate:YES] could enqueue a call to #calculateOutput.
 */
- (void)updateDependentValues;

/**
 *    @brief     Private function; not intended for external use
 *    @note      The implementation of this function may change
 */
- (NSMutableArray *)dependents;

/**
 *    @brief     Private function; not intended for external use
 *    @note      The implementation of this function may change
 */
- (void)processChangedKey;

@end

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @category    NSMutableDictionary(SBPropertyListPluginSupport) 
 *   @brief       Extends NSMutableDictionary to provide plugin serialization methods
 *   @author      HeartVista, Inc.
 *   @see         NSMutableDictionary
 */
@interface NSMutableDictionary(SBPropertyListPluginSupport)

/**
 *    @brief     Serializes an array of #SBPropertyListPlugin into the receiver with key aKey.
 */
- (void)setPropertyListPluginArray:(NSArray *)plugins forKey:(NSString *)aKey;
@end

/**
 *   @ingroup     SpinBenchDesignTool Sequencer
 *   @category    NSDictionary(SBPropertyListPluginSupport) 
 *   @brief       Extends NSDictionary to provide plugin deserialization methods
 *   @author      HeartVista, Inc.
 *   @see         NSDictionary
 */
@interface NSDictionary(SBPropertyListPluginSupport)

/**
 *    @brief     Returns a deserialized array of #SBPropertyListPlugin from the receiver value for aKey.  Returned plugins are filtered such that all are of type _class or its subclasses.
 */
- (NSMutableArray *)propertyListPluginArrayForKey:(NSString *)aKey params:(SBParamsObject *)_params verifySuperclass:(Class)_class;
@end
