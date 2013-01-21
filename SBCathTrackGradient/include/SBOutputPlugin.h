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

@class SBParamsObject;
@class SBMainWindowController;
@class SBVariablePlugin;
@protocol SBMultiInputValueVariableProtocol;
@class SBOutputPluginContainer;

enum SBOutputOptionsEnum { SBOutputFullRange = 0x00, SBOutputSelectedRange = 0x01, 
						   SBOutputSelectedPoint = 0x02 };

//wro: this is a viewcontroller to eliminate retain cycles on exit.  May not be necessary if GC enabled
@interface SBOutputPlugin : NSViewController {
	NSNotificationCenter *nc;
	SBParamsObject *params;
	SBMainWindowController *mainWindowController;
	NSString *editableName;
	float magnification;
	
	// pointers to viewports:
	IBOutlet id mainView;
	IBOutlet id optionsView;
	
	// private variables:
	SBOutputPluginContainer *container;
	BOOL nibIsLoaded;
@private
	float viewLeft,viewBottom,viewWidth,viewHeight;
	BOOL observersAreActive;
  BOOL paramsEnabled;
}

+ (NSString *)pluginDescription;
+ (NSString *)humanReadablePluginName;
- (id)initWithPropertyList:(NSDictionary *)plist params:(SBParamsObject *)_params;
- (void)encodePropertyList:(NSMutableDictionary *)dict;
- (NSString *)nibName;
- (NSString *)urlToGetPlugin;
- (int)pluginVersion;
- (NSArray *)attributeKeys;
- (void)setMagnification:(float)val;
- (float)magnification;

/**
 *   @brief      The global plot magnification value
 *   @details    For most output plugins, the total scaling of the data view is the product of this globalMagnification value and the internal magnification provided in #magnification.  To be notified of changes in globalMagnification, register for SBGlobalMagnificationDidChangeNotification with the #SBParamsObject as the sender object.
 */
- (float)globalMagnification;
- (BOOL)usesBlochData;

- (void)setViewBottom:(float)top left:(float)left;
- (void)setViewBottom:(float)loc;
- (void)setViewLeft:(float)loc;
- (void)setViewHeight:(float)height width:(float)width;
- (void)setViewHeight:(float)val;
- (void)setViewWidth:(float)val;
- (void)setEditableName:(NSString *)val;
- (BOOL)validateEditableName:(NSString **)valObj error:(NSError **)outError;

/**
 *   @brief     Flag for enabled state
 *   @details   The master SBParamsObject becomes disabled when certain long-running processes are active and any parameter changes may disrupt the process.  For example, rendering operations use paramsEnabled to prevent changes to the simulation during rendering.  Plugins should check paramsEnabled before allowing any gui changes to parameters.  Often this can be accomplished by binding ui elements' enabled property to the paramsEnabled key of their owning plugin class.
 */
- (BOOL)paramsEnabled;

- (float)viewBottom;
- (float)viewLeft;
- (float)viewHeight;
- (float)viewWidth;
- (NSString *)editableName;

- (NSView *)optionsView;
- (NSView *)mainView;
- (NSMenu *)contextualMenu; // subclasses can use this method to add items to the contextual menu

- (void)setEditingFlag:(BOOL)flag;
- (BOOL)editingFlag;

- (BOOL)viewShouldPrint; // normally returns YES; override to return NO if its view should be invisible for print operations
- (SBOutputPluginContainer *)container;

-(NSSize)minSize;  //this sets the minimum size of the mainView rectangle

// this allocates & fills a copy of the current spin array with 2-dimensional data: 
//    options specify how to use the current selection. First index is Mx,My,Mz; Second index is data point#
//    the existingArray input is required so that it only allocates memory that it needs.  
//      If existingArray=nil, the function allocates & returns one; 
//      if existingArrray is of the wrong size, the function resizes it.
//    currently only works for xVar = [params xAxisVariable] && yVar = [params yAxisVariable].
//    May be extended later.
- (float **)copySpinArrayWithXVariable:(id<SBMultiInputValueVariableProtocol>)xVar
                               options:(enum SBOutputOptionsEnum)xOptions
                             yVariable:(id<SBMultiInputValueVariableProtocol>)yVar
                               options:(enum SBOutputOptionsEnum)yOptions
                         existingArray:(float **)existingArray
                            outputXRes:(int *)xOutRes outputYRes:(int *)yOutRes;
-(void)freeSpinArray:(float **)spinArray;

//this allows the menu item to be grayed out when this plugin is unavailable.
+ (BOOL)enabledForParams:(SBParamsObject *)_params;
- (void)close;
- (void)bringToFront;

/**
 *   @brief      Returns a list of preferred locations for the left edge of the container
 *   @details    Useful for snapping to other objects when resizing or moving.
 */
- (NSArray *)preferredLeftEdgeLocations;
/**
 *   @brief      Returns a list of preferred locations for the right edge of the container
 *   @details    Useful for snapping to other objects when resizing or moving.
 */
- (NSArray *)preferredRightEdgeLocations;
/**
 *   @brief      Returns a list of preferred locations for the top edge of the container
 *   @details    Useful for snapping to other objects when resizing or moving.
 */
- (NSArray *)preferredTopEdgeLocations;
/**
 *   @brief      Returns a list of preferred locations for the bottom edge of the container
 *   @details    Useful for snapping to other objects when resizing or moving.
 */
- (NSArray *)preferredBottomEdgeLocations;

-(BOOL)loadNib;
-(void)destroyNib;
-(void)addObservers;
-(void)removeObservers;

@end

@interface NSMutableDictionary(SBOutputPluginSupport)
- (void)setOutputPlugin:(SBOutputPlugin *)plugin;
@end

@interface NSDictionary(SBOutputPluginSupport)
- (SBOutputPlugin *)outputPluginWithParams:(SBParamsObject *)_params;
@end
